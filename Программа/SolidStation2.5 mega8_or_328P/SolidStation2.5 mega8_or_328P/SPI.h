/*
 * SPI.h
 *
 * Created: 09.09.2018 12:43:33
 *  Author: Artur
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "main.h"

#define SPI_Cloc_Rate_2 0
#define SPI_Cloc_Rate_8 1
#define SPI_Cloc_Rate_32 2
#define SPI_Cloc_Rate_64 3

#define SPI_Int_Ena SPCR |= (1<<SPIE)
#define SPI_Int_Dis SPCR &= ~(1<<SPIE)
#define SPI_Ena SPCR |= (1<<SPE)
#define SPI_Dis SPCR &= ~(1<<SPE)
#define SPI_First_bit_LOW SPCR |= (1<<DORD)
#define SPI_First_bit_HIGH SPCR &= ~(1<<DORD)
#define SPI_Ena_Master SPCR |= ((1<<SPE)|(1<<MSTR))
#define SPI_Ena_Slave SPCR |= (1<<SPE); SPCR &= ~(1<<MSTR)
#define SPI_Idle_Rising SPCR |= (1<<CPOL)//≈сли данный бит в 1, то режим ожидани€ будет у нас при восход€щем фронте
//When this bit is written to one, SCK is high when idle. When CPOL is written to zero, SCK is low when idle.
#define SPI_Idle_Falling SPCR &= ~(1<<CPOL)// если в 0, то при нисход€щем
#define SPI_Cloc_Phase_Sample SPCR |= (1<<CPHA)
#define SPI_Cloc_Phase_Setup SPCR &= ~(1<<CPHA)
#define SPI_Cloc_Rate(val) 	SPCR &= ~((1<<SPR0)|(1<<SPR1)); SPSR |= (1<<SPI2X); SPCR |= val
#define SPI_Cloc_Rate_Not2x SPSR |= (1<<SPI2X)
#define SPI_Cloc_Rate_2x SPSR &= ~(1<<SPI2X)
//#define SPI_Transmitted !(SPSR&(1<<SPIF))
#define SPI_Transmitted !BitTest(SPSR,SPIF)
#define SPI_Data_Bufer SPDR
#endif /* SPI_H_ */