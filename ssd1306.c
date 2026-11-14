#include "stm32f10x.h"
#include "spi.h"
#include "ssd1306.h"

#define CS_LOW()    (GPIOA->ODR &= ~GPIO_ODR_ODR4)
#define CS_HIGH()   (GPIOA->ODR |=  GPIO_ODR_ODR4)
#define DC_LOW()    (GPIOA->ODR &= ~GPIO_ODR_ODR1)   
#define DC_HIGH()   (GPIOA->ODR |=  GPIO_ODR_ODR1)  
#define RES_LOW()   (GPIOA->ODR &= ~GPIO_ODR_ODR2)
#define RES_HIGH()  (GPIOA->ODR |=  GPIO_ODR_ODR2)

static void delay(volatile uint32_t n)
{
    while (n--) { }
}

void display_cmd(uint8_t cmd)
{
    CS_LOW();
    DC_LOW();
    delay(1000);
    SPI1_Write(cmd);
    SPI1_WaitIdle();      
    CS_HIGH();
}

void display_data(uint8_t data)
{
    CS_LOW();
    DC_HIGH();
    delay(1000);
    SPI1_Write(data);
    SPI1_WaitIdle();
    CS_HIGH();
}

void display_init(void)
{
    // PA1 (DC), PA2 (/RES), PA4 (/CS)
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1 |
                    GPIO_CRL_MODE2 | GPIO_CRL_CNF2 |
                    GPIO_CRL_MODE4 | GPIO_CRL_CNF4);
    GPIOA->CRL |=  (GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE4);

    CS_HIGH();           

    // Аппаратный сброс
    RES_LOW();
    delay(100000);
    RES_HIGH();
    delay(100000);

    static const uint8_t init_seq[] = {
        0xAE,             
        0x20, 0x00,     
        0x40,             
        0x81, 0x7F,      
        0xA1,            
        0xC8,          
        0xA6,             
        0xA8, 0x3F,      
        0xA4,             
        0xD3, 0x00,       
        0xD5, 0x80,      
        0xD9, 0xF1,      
        0xDA, 0x12,     
        0xDB, 0x40,       
        0x8D, 0x14        
    };
    for (uint32_t i = 0; i < sizeof(init_seq); i++)
        display_cmd(init_seq[i]);

    display_cmd(0x21); display_cmd(0); display_cmd(127);  
    display_cmd(0x22); display_cmd(0); display_cmd(7);   
    display_cmd(0xAF);                                    
}

// Шахматная доска 8x8 клеток по 8x8 пикселей (64x64), по центру экрана.
void display_chessboard(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        for (uint8_t col = 0; col < 128; col++)
        {
            uint8_t b = 0x00;                      
            if (col >= 32 && col < 96)
            {
                uint8_t square = (uint8_t)((col - 32) >> 3);  
                b = ((page + square) & 1) ? 0xFF : 0x00;      
            }
            display_data(b);
        }
    }
}

// Мишень (не успел доделать)
"""
void display_target(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        for (uint8_t col = 0; col < 128; col++)
        {
            uint8_t b = 0;
            for (uint8_t bit = 0; bit < 8; bit++)
            {
                int32_t x = (int32_t)col - 64;
                int32_t y = (int32_t)(page * 8 + bit) - 32;
                int32_t d2 = x * x + y * y;

                if (d2 > 34 * 34)          // за пределами мишени — чёрный фон
                    continue;

                // Определяем номер кольца: |r| = 4*k  →  d2 = 16*k*k
                uint8_t ring = 0;
                while ((ring + 1) * (ring + 1) * 16 <= d2)
                    ring++;

                if (ring & 1)              // нечётные кольца — белые
                    b |= (uint8_t)(1u << bit);
            }
            display_data(b);
        }
    }
}
"""