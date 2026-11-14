#include "stm32f10x.h"
#include "spi.h"

void SPI1_Init(void)
{
    // Тактирование GPIOA и SPI1 (оба на шине APB2)
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_SPI1EN;

    // Выводы SPI: PA5 = SCK, PA7 = MOSI
    GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5 |
                    GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
    GPIOA->CRL |=  (GPIO_CRL_MODE5 | GPIO_CRL_CNF5_1 |
                    GPIO_CRL_MODE7 | GPIO_CRL_CNF7_1);

    // Настройка SPI1 
    SPI1->CR1 = SPI_CR1_CPOL    
              | SPI_CR1_CPHA    
              | SPI_CR1_MSTR    
              | SPI_CR1_BR      
              | SPI_CR1_SSM     
              | SPI_CR1_SSI;    

    // Включаем SPI1
    SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI1_Write(uint8_t data)
{
    while (!(SPI1->SR & SPI_SR_TXE)) { }
    SPI1->DR = data;
}

uint8_t SPI1_Read(void)
{
    SPI1->DR = 0;
    while (!(SPI1->SR & SPI_SR_RXNE)) { }
    return (uint8_t)SPI1->DR;
}

void SPI1_WaitIdle(void)
{
    while (SPI1->SR & SPI_SR_BSY) { }
}
