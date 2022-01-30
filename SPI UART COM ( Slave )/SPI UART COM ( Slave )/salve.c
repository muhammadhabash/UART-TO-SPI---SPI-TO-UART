#include "MCAL/UART/UART_interface.h"
#include "MCAL/SPI/SPI_interface.h"

int main(){
	
	uint8_t u8_buffer[20];
	UART_init();
	SPI_init();
	
	
	while(1){
		SPI_slaveReceiveString(u8_buffer);
		UART_sendString(u8_buffer);
	}
}