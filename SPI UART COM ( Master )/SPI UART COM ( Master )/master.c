#include "MCAL/UART/UART_interface.h"
#include "MCAL/SPI/SPI_interface.h"

int main(){
	
	uint8_t u8_buffer[20];
	UART_init();
	SPI_init(); 
	
	
	while(1){
		
		UART_receiveString('\r',u8_buffer,20);
		SPI_masterTransmitString(u8_buffer);
	}
	
}