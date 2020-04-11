/*
 * bmp280.h
 *
 * Created: 4/10/2020 7:34:17 PM
 *  Author: Jake
 */ 


#ifndef BMP280_H_
#define BMP280_H_

/*!
 * \brief I2C bus address for Bosch BMP280 pressure/temperature sensor.
 */
#define BMP280_I2C_ADDR  (0x76) // 0x76 if SDO connected to GND, 0x77 if not

/*!
 * \brief Register map for Bosch BMP280.
 */
#define BMP280_REG_TEMP_XLSB    (0xFC)
#define BMP280_REG_TEMP_LSB     (0xFB)
#define BMP280_REG_TEMP_MSB     (0xFA)
#define BMP280_REG_PRESS_XLSB   (0xF9)
#define BMP280_REG_PRESS_LSB    (0xF8)
#define BMP280_REG_PRESS_MSB    (0xF7)
#define BMP280_REG_CONFIG       (0xF5)
#define BMP280_REG_CTRL_MEAS    (0xF4)
#define BMP280_REG_STATUS       (0xF3)
#define BMP280_REG_RESET        (0xE0)
#define BMP280_REG_ID           (0xD0)

/*!
 * \brief Bosch BMP280 ID register value.
 */
#define BMP280_CHIP_ID          (0x58)

/*!
 * \brief Barometer standby time setting (normal mode only).
 */
typedef enum {
    BMP280_T_SB_500us     = 0,
    BMP280_T_SB_62500us   = 1,
    BMP280_T_SB_125ms     = 2,
    BMP280_T_SB_250ms     = 3,
    BMP280_T_SB_500ms     = 4,
    BMP280_T_SB_1000ms    = 5,
    BMP280_T_SB_2000ms    = 6,
    BMP280_T_SB_4000ms    = 7,
} bmp280_t_sb_t;

/*!
 * \brief Barometer filter coefficient settings.
 */
typedef enum {
    BMP280_FILTER_OFF         = 0,
    BMP280_FILTER_COEFF_2     = 1,
    BMP280_FILTER_COEFF_4     = 2,
    BMP280_FILTER_COEFF_8     = 3,
    BMP280_FILTER_COEFF_16    = 4,
} bmp280_filter_coeff_t;

/*!
 * \brief Barometer SPI settings.
 */
typedef enum {
    BMP280_SPI_4_WIRE         = 0,    // Default setting, I2C compatible.
    BMP280_SPI_3_WIRE         = 1,
} bmp280_spi_t;

/*!
 * \brief Barometer temperature oversampling setting.
 */
typedef enum {
    BMP280_TEMP_OSRS_SKIPPED   = 0,
    BMP280_TEMP_OSRS_x1        = 1,
    BMP280_TEMP_OSRS_x2        = 2,
    BMP280_TEMP_OSRS_x4        = 3,
    BMP280_TEMP_OSRS_x8        = 4,
    BMP280_TEMP_OSRS_x16       = 5,
} bmp280_temp_osrs_t;

/*!
 * \brief Barometer pressure oversampling setting.
 */
typedef enum {
    BMP280_PRES_OSRS_SKIPPED   = 0,
    BMP280_PRES_OSRS_x1        = 1,
    BMP280_PRES_OSRS_x2        = 2,
    BMP280_PRES_OSRS_x4        = 3,
    BMP280_PRES_OSRS_x8        = 4,
    BMP280_PRES_OSRS_x16       = 5,
} bmp280_pressure_osrs_t;

/*!
 * \brief Barometer mode setting.
 */
typedef enum {
    BMP280_MODE_SLEEP          = 0,
    BMP280_MODE_FORCED         = 1,
    BMP280_MODE_NORMAL         = 3,
} bmp280_mode_t;

/*!
 * \brief Barometer configuration type.
 */
typedef struct {
    bmp280_t_sb_t             t_sb        : 3;
    bmp280_filter_coeff_t     filter      : 3;
    bmp280_spi_t              spi3w_en    : 1;
    bmp280_temp_osrs_t        osrs_t      : 3;
    bmp280_pressure_osrs_t    osrs_p      : 3;
    bmp280_mode_t             mode        : 2;
} bmp280_config_t;

/*!
 * \brief Barometer measurement type in raw integer form.
 */
typedef struct {
	uint32_t P;		// divide by 256 for Pa
	int32_t T;		// divide by 100 for degrees C
} bmp280_raw_data_t;

/*!
 * \brief Barometer measurement type converted to SI units.
 */
typedef struct {
	float pressure;
	float temp;
} bmp280_meas_t;

void init_bmp280(void);
void read_bmp280(bmp280_meas_t * meas);
void read_bmp280_integer(bmp280_raw_data_t * meas);

#endif /* BMP280_H_ */