# L476-FRTOS7-W25QFlash
# STM32 FreeRTOS W25Q128 SPI Flash Demo

## Overview

This project demonstrates a **FreeRTOS-based embedded application** running on the **STM32 NUCLEO-L476RG**. The project integrates a **W25Q128 SPI NOR Flash Driver** with multiple FreeRTOS tasks to demonstrate concurrent peripheral access and resource synchronization.

Three independent tasks execute simultaneously:

- **LED Task** – Blinks the onboard LED every 400 ms.
- **UART Task** – Periodically outputs system status messages.
- **Flash Task** – Performs SPI Flash operations including JEDEC ID reading, sector erase, page write, data read, and verification.

Since both the UART Task and Flash Task access the same UART peripheral, a **FreeRTOS Mutex** is used to guarantee thread-safe UART transmission and prevent mixed console output.

---

## Features

- STM32 HAL Driver
- FreeRTOS (CMSIS-RTOS v2)
- Multi-task Scheduling
- SPI Communication
- UART Communication
- W25Q128 SPI Flash Driver
- Read JEDEC ID
- Sector Erase
- Flash Programming
- Flash Readback
- Data Verification
- UART Mutex Protection
- Concurrent Embedded Firmware Design

---

## Hardware

| Item | Description |
|------|-------------|
| MCU | STM32 NUCLEO-L476RG |
| Flash Memory | Winbond W25Q128 SPI NOR Flash |
| IDE | STM32CubeIDE |
| Debugger | ST-Link |
| RTOS | FreeRTOS (CMSIS-RTOS v2) |

---

## Software Architecture

```
                 +----------------+
                 |   FreeRTOS     |
                 +----------------+
                    |     |     |
                    |     |     |
            LED Task UART Task Flash Task
               |         |         |
               |         |         |
             GPIO      UART      SPI
                           \      /
                            Mutex
```

---

## FreeRTOS Tasks

### LED Task

Runs every **400 ms**

Responsibilities

- Toggle onboard LED (PA5)
- Demonstrate periodic task scheduling

```
LED ON
400 ms
LED OFF
400 ms
```

---

### UART Task

Runs every **5 seconds**

Responsibilities

- Output system heartbeat message

Example

```
UART Task Running...
```

---

### Flash Task

Runs every **10 seconds**

Responsibilities

1. Read Flash JEDEC ID
2. Erase Sector
3. Program Flash
4. Read Flash
5. Verify Data

Execution Flow

```
Flash Task Start
        │
        ▼
Read JEDEC ID
        │
        ▼
Erase Sector
        │
        ▼
Write Data
        │
        ▼
Read Data
        │
        ▼
Compare
        │
        ▼
PASS / FAIL
```

---

## UART Mutex Protection

Both **UART Task** and **Flash Task** access USART2 simultaneously.

To prevent data corruption, every UART transmission is protected using a FreeRTOS Mutex.

```c
void UART_SEND(UART_HandleTypeDef *huart, char buffer[])
{
    osMutexAcquire(myMutex01Handle, osWaitForever);

    HAL_UART_Transmit(huart,
                      (uint8_t*)buffer,
                      strlen(buffer),
                      HAL_MAX_DELAY);

    osMutexRelease(myMutex01Handle);
}
```

Without Mutex

```
UART TaFlash Task Start...
sk Running...
```

Output becomes corrupted because two tasks transmit at the same time.

With Mutex

```
Flash Task Start

JEDEC ID = EF 40 18

Erase OK

Write OK

Read = Hello W25Qxx

PASS

Flash Task Finish

UART Task Running...
```

Every UART message is transmitted atomically without interleaving.

---

## Flash Driver Functions

### Read JEDEC ID

```
Command : 0x9F

Manufacturer ID : EF
Memory Type     : 40
Capacity         : 18
```

---

### Sector Erase

```
Address : 0x000000
```

Erase one 4 KB sector before programming.

---

### Write Data

```
Hello W25Qxx
```

---

### Read Data

The programmed data is read back from Flash.

---

### Data Verification

```c
if(strcmp(writeData, readData)==0)
    PASS;
else
    FAIL;
```

The verification step confirms that the data stored in Flash matches the original write buffer.

---

## Example UART Output

```
Flash Task Start

JEDEC ID = EF 40 18

Erase OK

Write OK

Read = Hello W25Qxx

PASS

Flash Task Finish

UART Task Running...

UART Task Running...

Flash Task Start

JEDEC ID = EF 40 18
...
```

---

## Project Structure

```
Project
│
├── Core
│   ├── Inc
│   └── Src
│       └── main.c
│
├── Drivers
│
├── Middlewares
│   └── FreeRTOS
│
├── w25qxx.c
├── w25qxx.h
│
└── README.md
```

---

## Technologies Used

- C Programming
- STM32 HAL
- FreeRTOS
- CMSIS-RTOS v2
- SPI Driver Development
- UART Driver
- Mutex Synchronization
- Embedded Systems Programming

---

## Learning Outcomes

Through this project, I learned how to:

- Develop concurrent embedded firmware using FreeRTOS.
- Create multiple independent tasks with different execution periods.
- Synchronize shared hardware resources using Mutex.
- Implement a W25Q128 SPI Flash driver.
- Read JEDEC IDs and communicate over SPI.
- Perform Flash erase, write, read, and verification operations.
- Debug embedded firmware using UART logging and ST-Link.
- Design scalable multitasking firmware based on the STM32 HAL framework.

---

## Future Improvements

- DMA-based SPI Communication
- DMA UART Logging
- Queue-based Logger
- Binary Semaphore
- Event Flags
- Software Timers
- Multi-page Flash Write
- Page Boundary Handling
- Block Erase
- Chip Erase
- LittleFS Integration
- FATFS Integration
- Command Line Interface (CLI)
- Flash Driver State Machine

