#ifndef _BSP_I2C_H
#define _BSP_I2C_H

#include <inttypes.h>

#define I2C_WR	0		/* Ð´¿ØÖÆbit */
#define I2C_RD	1		/* ¶Á¿ØÖÆbit */

void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(uint8_t ack);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);
void i2c_GPIO_Config(void);
u8 i2c_WriteLen(u8 addr,u8 reg,u8 len,u8 *buf);
u8 i2c_ReadLen(u8 addr,u8 reg,u8 len,u8 *buf);
void WriteCmd(u8 command);
void WriteDat(u8 data);

#endif
