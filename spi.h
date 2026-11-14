#ifndef SPI_H
#define SPI_H

#include <stdint.h>

// Включает тактирование GPIOA и SPI1, настраивает PA5 (SCK), PA7 (MOSI) и сам SPI1
void SPI1_Init(void);

// Отправляет один байт
void SPI1_Write(uint8_t data);

// Запускает обмен и возвращает принятый байт
uint8_t SPI1_Read(void);

// Ждёт, пока SPI1 закончит передачу последнего бита
void SPI1_WaitIdle(void);

#endif
