/*
 * lis3dh.h
 *
 * Created: 4/10/2020 7:33:52 PM
 *  Author: Jake
 */ 


#ifndef LIS3DH_H_
#define LIS3DH_H_

#define LIS3DH_ADDRESS                  0x19

#define REG_STATUS_REG_AUX              0x07
#define REG_WHO_AM_I                    0x0F
#define WHO_AM_I_VAL                            0x33
#define REG_CTRL_REG0                   0x1E
#define REG_TEMP_CFG_REG                0x1F
#define REG_CTRL_REG1                   0x20
#define REG_CTRL_REG2                   0x21
#define REG_CTRL_REG3                   0x22
#define REG_CTRL_REG4                   0x23
#define REG_CTRL_REG5                   0x24
#define REG_CTRL_REG6                   0x25
#define REG_REFERENCE                   0x26
#define REG_STATUS_REG                  0x27
#define REG_OUT_X_L                     0x28
#define REG_OUT_X_H                     0x29
#define REG_OUT_Y_L                     0x2A
#define REG_OUT_Y_H                     0x2B
#define REG_OUT_Z_L                     0x2C
#define REG_OUT_Z_H                     0x2D
#define REG_INT1_CFG                    0x30
#define REG_INT1_SRC                    0x31
#define REG_INT1_THS                    0x32
#define REG_INT1_DUR                    0x33

typedef struct
{
	int16_t x;
	int16_t y;
	int16_t z;
} lis3dh_data_raw_t;

typedef struct
{
	float x;
	float y;
	float z;
} lis3dh_data_g_t;

void init_lis3dh(void);
void read_lis3dh_raw(lis3dh_data_raw_t * data);
void read_lis3dh_g(lis3dh_data_g_t * data);


#endif /* LIS3DH_H_ */