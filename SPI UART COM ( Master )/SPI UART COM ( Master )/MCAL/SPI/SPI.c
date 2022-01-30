#include "SPI_interface.h"
#include "../GPIO/GPIO.h"

#define PORTB_DIR  *((volatile uint8_t*) 0x37)
#define PORTB_DATA  *((volatile uint8_t*) 0x38)
void SPI_init(void){
	
	#if SPI_MODE == MASTER_MODE
	    GPIO_dir(B,SCK,OUTPUT);
	    GPIO_dir(B,MOSI,OUTPUT);
		GPIO_dir(B,SS,OUTPUT);
		PORTB_DATA |= (1 << SS);
	    SPC_R |= (1 << SPE) | (1 << MSTR);
	
	    #if SCK_FREQ_PRESCALE == PRESCALER_2
	        SPC_R |= (1 << SPI2X);
	    #elif SCK_FREQ_PRESCALE == PRESCALER_4
	        SPC_R &= ~(1 << SPR0) & ~(1 << SPR1) & ~(1 << SPI2X);
	    #elif SCK_FREQ_PRESCALE == PRESCALER_8
	        SPC_R |= (1 << SPR0) | (1 << SPI2X);
	    #elif SCK_FREQ_PRESCALE == PRESCALER_16
	        SPC_R |= (1 << SPR0);
	    #elif SCK_FREQ_PRESCALE == PRESCALER_32
	        SPC_R |= (1 << SPR1) | (1 << SPI2X);
	    #elif SCK_FREQ_PRESCALE == PRESCALER_64
	        SPC_R |= (1 << SPR1);
	    #elif SCK_FREQ_PRESCALE == PRESCALER_128
	        SPC_R |= (1 << SPR1) | (1 << SPR0);
	    #else
	        #error "Invalid Clock Frequency Setting"
	    #endif
	
	    #if CLOCK_POLARITY == HIGH_WHEN_IDLE
	        SPC_R |= (1 << CPOL);
	    #elif CLOCK_POLARITY == LOW_WHEN_IDLE
	        SPC_R &= ~(1 << CPOL);
	    #else
	        #error "Invalid Clock Polarity Setting"
	    #endif
	
	    #if DATA_SAMPLING == LEADING_EDGE
	        SPC_R &= ~(1 << CPHA);
	    #elif DATA_SAMPLING == TRAILING_EDGE
	        SPC_R |= (1 << CPHA);
	    #else
	        #error "Invalid Data Sampling Setting"
	    #endif
	
	#elif SPI_MODE == SLAVE_MODE
	    GPIO_dir(B,MISO,OUTPUT);
	    SPC_R |= (1 << SPE);
	#else
	    #error "Invalid Mode Setting"
	#endif
		
	#if SPI_INTERRUPT == ENABLE
		SPC_R    |= (1 << SPIE);
		STATUS_R |= (1 << GLOBAL_INTERRUPT);
	#elif SPI_INTERRUPT == DISABLE
	    SPC_R    &=	~(1 << SPIE);
		STATUS_R &= ~(1 << GLOBAL_INTERRUPT);
	#else 
	    #error "Invalid Interrupt Setting"
	#endif
	
	#if DATA_ORDER == MSB_FIRST
	    SPC_R &= ~(1 << DORD);
	#elif DATA_ORDER == LSB_FIRST
	    SPC_R |= (1 << DORD);
	#else 
	    #error "Invalid Data Order Setting"
	#endif
}

void SPI_masterTransmitByte(uint8_t u8_data){
	
	PORTB_DATA &= 0xEF;
	SPD_R = u8_data;
	while (!(SPS_R & (1 << SPIF)));
	PORTB_DATA |= 0x10;
}

uint8_t SPI_slaveReceive(){
	
	while (!(SPS_R & (1 << SPIF)));
	return SPD_R;
}

void SPI_masterTransmitString(uint8_t* u8_buffer){
	
	uint8_t u8_index = 0;
	
	do{
		SPI_masterTransmitByte(u8_buffer[u8_index]);
		u8_index++;
	}while (u8_buffer[u8_index - 1] != '\0');
}