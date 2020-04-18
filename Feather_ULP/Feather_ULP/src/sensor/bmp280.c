/*
 * bmp280.c
 *
 * Created: 4/10/2020 7:34:05 PM
 *  Author: Jake
 */ 

 #include "asf.h"

 #include "i2c_interface.h"

 #include "bmp280.h"

 /*!
 * \brief Barometer calibration data struct.
 */
static struct {
	uint16_t dig_T1;
	int16_t  dig_T2;
	int16_t  dig_T3;
	uint16_t dig_P1;
	int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
} bmp280_cal_data;

static int32_t t_fine;

static volatile bmp280_raw_data_t current_bmp280_data;

/*!
 * \brief Configure Bosch BMP280 registers.
 *
 * \param config Pointer to BMP280 configuration object
 */
static void bmp280_write_configuration(const bmp280_config_t * config)
{
    // Sensor configuration register.
    uint8_t temp = ((config->t_sb & 0x07) << 5) |
                   ((config->filter & 0x07) << 2) |
                   (config->spi3w_en & 0x01);

	i2c_write_bytes(&temp, 1, BMP280_I2C_ADDR, BMP280_REG_CONFIG);

    // Measurement control configuration register.
    temp = ((config->osrs_t & 0x07) << 5) |
           ((config->osrs_p & 0x07) << 2) |
           (config->mode & 0x03);

	i2c_write_bytes(&temp, 1, BMP280_I2C_ADDR, BMP280_REG_CTRL_MEAS);
}

static uint16_t unpack_uint16(uint8_t * data)
{
	uint16_t val = 0;
	val |= (*data | ((uint16_t) *(data+1) << 8));
	return val;
}

static int16_t unpack_int16(uint8_t * data)
{
	int16_t val = 0;
	val |= (*data | ((uint16_t) *(data+1) << 8));
	return val;
}

/*!
 * \brief Basic initialization function for Bosch BMP280 pressure/temperature sensor.
 */
void init_bmp280(void)
{
	uint8_t me = 0;
	i2c_read_bytes(&me, 1, BMP280_I2C_ADDR, BMP280_REG_ID);
	if (me != BMP280_CHIP_ID)
    {
        return;
    }

    // Read calibration data if not done already.
	if(bmp280_cal_data.dig_T1 == 0)
	{
		uint8_t temp[24] = {0};
		i2c_read_bytes(temp, 24, BMP280_I2C_ADDR, 0x88);

		// Parse calibration data.
		bmp280_cal_data.dig_T1 = unpack_uint16(&temp[0]);
		bmp280_cal_data.dig_T2 = unpack_int16(&temp[2]);
		bmp280_cal_data.dig_T3 = unpack_int16(&temp[4]);
		bmp280_cal_data.dig_P1 = unpack_uint16(&temp[6]);
		bmp280_cal_data.dig_P2 = unpack_int16(&temp[8]);
		bmp280_cal_data.dig_P3 = unpack_int16(&temp[10]);
		bmp280_cal_data.dig_P4 = unpack_int16(&temp[12]);
		bmp280_cal_data.dig_P5 = unpack_int16(&temp[14]);
		bmp280_cal_data.dig_P6 = unpack_int16(&temp[16]);
		bmp280_cal_data.dig_P7 = unpack_int16(&temp[18]);
		bmp280_cal_data.dig_P8 = unpack_int16(&temp[20]);
		bmp280_cal_data.dig_P9 = unpack_int16(&temp[22]);
	}

	// Configure barometer in forced mode (1 sample then sleep), 1x oversample on both for <6.4ms measurement time,
	// no filter
    bmp280_config_t config = {
        .t_sb       = BMP280_T_SB_1000ms,
        .filter     = BMP280_FILTER_OFF,
        .spi3w_en   = BMP280_SPI_4_WIRE,
        .osrs_t     = BMP280_TEMP_OSRS_x1,
        .osrs_p     = BMP280_PRES_OSRS_x1,
        .mode       = BMP280_MODE_FORCED,
    };
    bmp280_write_configuration(&config);
}

void sleep_bmp280(void)
{
	bmp280_config_t config = {
		.t_sb       = BMP280_T_SB_1000ms,
		.filter     = BMP280_FILTER_OFF,
		.spi3w_en   = BMP280_SPI_4_WIRE,
		.osrs_t     = BMP280_TEMP_OSRS_x1,
		.osrs_p     = BMP280_PRES_OSRS_x1,
		.mode       = BMP280_MODE_SLEEP,
	};
	bmp280_write_configuration(&config);
}
	
/*!
 * \brief Temperature compensation according to Bosch BMP280 datasheet (Pg. 21-23).
 */
static int32_t bmp280_compensate_T_int32(int32_t adc_T)
{
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t)bmp280_cal_data.dig_T1 << 1))) *
            ((int32_t)bmp280_cal_data.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)bmp280_cal_data.dig_T1)) *
            ((adc_T >> 4) - ((int32_t)bmp280_cal_data.dig_T1))) >> 12) *
            ((int32_t)bmp280_cal_data.dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

/*!
 * \brief Pressure compensation according to Bosch BMP280 datasheet (Pg. 21-23).
 */
static uint32_t bmp280_compensate_P_int32(int32_t adc_P)
{
    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)bmp280_cal_data.dig_P6;
    var2 = var2 + ((var1 * (int64_t)bmp280_cal_data.dig_P5) << 17);
    var2 = var2 + (((int64_t)bmp280_cal_data.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)bmp280_cal_data.dig_P3) >> 8) +
            ((var1 * (int64_t)bmp280_cal_data.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)bmp280_cal_data.dig_P1) >> 33;
    if (var1 == 0)
    {
        return 0; // Avoid exception caused by division by zero.
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)bmp280_cal_data.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)bmp280_cal_data.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)bmp280_cal_data.dig_P7) << 4);
    return (uint32_t)p;
}

/*!
 * \brief Gets BMP280 readings in Pa and deg C
 */
void read_bmp280(bmp280_meas_t * meas)
{
	bmp280_raw_data_t raw_data;
	read_bmp280_integer(&raw_data);
	
	// Convert to Pascal and degrees C
	meas->pressure = raw_data.P / 256.0;
	meas->temp = raw_data.T * 0.01;
}

/*!
 * \brief Gets BMP280 readings in integer units for faster speed
 */
void read_bmp280_integer(bmp280_raw_data_t * meas)
{
	uint8_t buffer[6] = {0};
	i2c_read_bytes(buffer, 6, BMP280_I2C_ADDR, BMP280_REG_PRESS_MSB);
	
	// Create union type to easily transform uncompensated measurements.
	union {
		uint32_t i;
		uint8_t b[4];
	} UT, UP;

	// Parse uncompensated pressure measurement.
	UP.b[0] = *(buffer+2);
	UP.b[1] = *(buffer+1);
	UP.b[2] = *buffer;
	UP.i = (UP.i >> 4) & 0xFFFFFul;

	// Parse uncompensated temperature measurement.
	UT.b[0] = *(buffer+5);
	UT.b[1] = *(buffer+4);
	UT.b[2] = *(buffer+3);
	UT.i = (UT.i >> 4) & 0xFFFFFul;

	// Perform pressure/temperature compensation.
	meas->T = bmp280_compensate_T_int32(UT.i);
	meas->P = (int32_t)bmp280_compensate_P_int32(UP.i);
}
