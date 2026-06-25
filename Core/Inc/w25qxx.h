/*
 * w25qxx.h
 *
 *  Created on: Jun 24, 2026
 *      Author: cch11
 */

#ifndef INC_W25QXX_H_
#define INC_W25QXX_H_

#include "main.h"

#define W25Q_CS_LOW() \
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET)

#define W25Q_CS_HIGH() \
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET)


/* Export SPI Handle */
extern SPI_HandleTypeDef hspi2;

/* Function Prototypes */

void W25Q_WriteEnable(void);

uint8_t W25Q_ReadStatus(void);

void W25Q_WaitBusy(void);

void W25Q_SectorErase(uint32_t addr);

void W25Q_Read(uint32_t addr,
               uint8_t *buf,
               uint16_t len);

void W25Q_Write(uint32_t addr,
                uint8_t *buf,
                uint16_t len);

#endif /* INC_W25QXX_H_ */
