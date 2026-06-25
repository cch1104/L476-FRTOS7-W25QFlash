/*
 * w25qxx.c
 *
 *  Created on: Jun 22, 2026
 *      Author: cch11
 */

#include "w25qxx.h"

/* Export SPI Handle */
extern SPI_HandleTypeDef hspi2;

/***********************************************
*Write enable function
************************************************/
void W25Q_WriteEnable(void)
{
    uint8_t cmd = 0x06;

    W25Q_CS_LOW();

    HAL_SPI_Transmit(&hspi2,
                     &cmd,
                     1,
                     HAL_MAX_DELAY);

    W25Q_CS_HIGH();
}

/*************************************************
 * *ReadStatus function
 * ***********************************************/
uint8_t W25Q_ReadStatus(void)
{
    uint8_t cmd = 0x05;
    uint8_t status = 0;

    W25Q_CS_LOW();

    HAL_SPI_Transmit(&hspi2,
                     &cmd,
                     1,
                     HAL_MAX_DELAY);

    HAL_SPI_Receive(&hspi2,
                    &status,
                    1,
                    HAL_MAX_DELAY);

    W25Q_CS_HIGH();

    return status;
}

/****************************************************
 * Sector Erase function-Sector Erase-W25Q_WaitBusy function
 * *************************************************/
void W25Q_WaitBusy(void)
{
    while(W25Q_ReadStatus() & 0x01);
}


void W25Q_SectorErase(uint32_t addr)
{
    uint8_t cmd[4];

    W25Q_WriteEnable();

    cmd[0] = 0x20;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;

    W25Q_CS_LOW();
    HAL_SPI_Transmit(&hspi2, cmd, 4, HAL_MAX_DELAY);
    W25Q_CS_HIGH();

    W25Q_WaitBusy();
}

/****************************************************
 * Read Function
 * *************************************************/
void W25Q_Read(uint32_t addr,
               uint8_t *buf,
               uint16_t len)
{
    uint8_t cmd[4];

    cmd[0] = 0x03;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;

    W25Q_CS_LOW();

    HAL_SPI_Transmit(&hspi2,
                     cmd,
                     4,
                     HAL_MAX_DELAY);

    HAL_SPI_Receive(&hspi2,
                    buf,
                    len,
                    HAL_MAX_DELAY);

    W25Q_CS_HIGH();
}

/****************************************************
*Write Function
****************************************************/
void W25Q_Write(uint32_t addr,
                uint8_t *buf,
                uint16_t len)
{
    uint8_t cmd[4];

    W25Q_WriteEnable();

    cmd[0] = 0x02;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;

    W25Q_CS_LOW();

    HAL_SPI_Transmit(&hspi2,
                     cmd,
                     4,
                     HAL_MAX_DELAY);

    HAL_SPI_Transmit(&hspi2,
                     buf,
                     len,
                     HAL_MAX_DELAY);

    W25Q_CS_HIGH();

    W25Q_WaitBusy();
}

