/*
 * fingerprint_sensor_uart.c
 *
 * Created: 5/28/2021 10:51:46 AM
 * Author : gonat
 */ 

#include <avr/io.h>
#include "fs_lib/fs_config.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "fs_lib/cap_fs_uart.h"
#include "lcd_lib/lcd_lib.h"


#define UART_PORT PORTA
#define LCD_PORT PORTD
#define SWITCH_PORT PORTF
#define LCD_DATA_PORT PORTD
#define LCD_CONTROL_PORT PORTF
#define X_PIN 1
#define Y_PIN 2
#define S_PIN 3

ISR(PORTF_PORT_vect)
{
	cli();
	//Wait 10ms
	_delay_ms(10);
	if (!(SWITCH_PORT.IN & (1<<SWITCH_bp)))
	{
		//Force RST high
		UART_PORT.OUT |= (1<<RST_bp);
		//Wait for module to start
		clear_screen(&LCD_DATA_PORT, &LCD_CONTROL_PORT);
		_delay_ms(25);
		write_string("Place Finger...", &LCD_DATA_PORT, &LCD_CONTROL_PORT);
		while(!(UART_PORT.IN & (1<<WAKE_bp)));
		clear_screen(&LCD_DATA_PORT, &LCD_CONTROL_PORT);
		_delay_ms(25);
		write_string("Reading...", &LCD_DATA_PORT, &LCD_CONTROL_PORT);
		uint16_t result = add_fingerprint(0x02);
		_delay_ms(1000);
		_delay_ms(1000);
		clear_screen(&LCD_DATA_PORT, &LCD_CONTROL_PORT);
		if (result != ACK_SUCCESS)
		{
			write_string("Could not add.", &LCD_DATA_PORT, &LCD_CONTROL_PORT);
		}
		else
		{
			write_string("Add Successful!", &LCD_DATA_PORT, &LCD_CONTROL_PORT);
		}
	}
	
	_delay_ms(1000);
	_delay_ms(1000);
	_delay_ms(1000);
	clear_screen(&LCD_DATA_PORT, &LCD_CONTROL_PORT);
	//Write sleep message to LCD
	write_string("Sleeping...  aaa", &LCD_DATA_PORT, &LCD_CONTROL_PORT);
	//Force RST low
	UART_PORT.OUT &= ~(1<<RST_bp);
	PORTF.INTFLAGS = 0x00;
	sei();
}

ISR(PORTA_PORT_vect)
{
	cli();
	//Wait 10ms
	_delay_ms(10);
	if ((UART_PORT.IN & (1<<WAKE_bp)))
	{
		//Force RST high
		UART_PORT.OUT |= (1<<RST_bp);
		//Wait for module to start
		clear_screen(&LCD_DATA_PORT, &LCD_CONTROL_PORT);
		_delay_ms(25);
		write_string("Place Finger...", &LCD_DATA_PORT, &LCD_CONTROL_PORT);
		_delay_ms(1000);
		_delay_ms(1000);
		uint16_t result = compare_1_to_n();
		clear_screen(&LCD_DATA_PORT, &LCD_CONTROL_PORT);
		if (result > MAX_USERS)
		{
			write_string("User Not Found", &LCD_DATA_PORT, &LCD_CONTROL_PORT);
		}
		else
		{
			write_string("Welcome!", &LCD_DATA_PORT, &LCD_CONTROL_PORT);
			send_command(0xC0, &LCD_DATA_PORT, &LCD_CONTROL_PORT); //Next Line
			write_string("ID: ", &LCD_DATA_PORT, &LCD_CONTROL_PORT);
			char id[5];
			itoa(result, id, 10);
			write_string(id, &LCD_DATA_PORT, &LCD_CONTROL_PORT);
		}
	}
	
	_delay_ms(1000);
	_delay_ms(1000);
	_delay_ms(1000);
	clear_screen(&LCD_DATA_PORT, &LCD_CONTROL_PORT);
	//Write sleep message to LCD
	write_string("Sleeping... bbb", &LCD_DATA_PORT, &LCD_CONTROL_PORT);
	//Force RST low
	UART_PORT.OUT &= ~(1<<RST_bp);
	PORTA.INTFLAGS = 0x00;
	sei();
}


void init_uart()
{
	//Set the baud rate for the device
	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(BAUD_RATE);
	//Enable the TX and RX pins
	USART0.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
	//Set USART to normal mode
	USART0.CTRLB |= USART_RXMODE_NORMAL_gc;
	//Set USART to async, no parity, one stop bit
	USART0.CTRLC |= USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc;
	//Set USART to 8-bit transmission
	USART0.CTRLC |= USART_CHSIZE_8BIT_gc;
}

void init_ports()
{
	//Set TX pin as output
	UART_PORT.DIR |= (1<<TX_bp);
	//Set RX pin as input
	UART_PORT.DIR &= ~(1<<RX_bp);
	//Set PA2 as input for wake
	UART_PORT.DIR &= ~(1<<WAKE_bp);
	//Enable interrupt and pull up on PA2
	UART_PORT.PIN2CTRL|= 0x08 | 0x02;
	//Set PA3 as output, set to 0
	UART_PORT.DIR |= (1<<RST_bp);
	UART_PORT.OUT &= ~(1<<RST_bp);
	
	LCD_DATA_PORT.OUT = 0x00; //Set all PORTD outputs to low
	LCD_DATA_PORT.DIR |= 0xFF; //Set PORTD pins as output
	LCD_CONTROL_PORT.OUT = 0x00; //Set all PortF outputs to low
	LCD_CONTROL_PORT.DIR |= 0x1F; //Set PortB pins 0-2 as output
	
	//Set PF6 as input, enable pull-up, enable falling edge interrupts
	SWITCH_PORT.DIR &= ~(1<<SWITCH_bp);
	SWITCH_PORT.PIN6CTRL |= 0x08 | 0x03;
}

int main(void)
{
	init_ports();
	init_uart();
	//Force RST high
	UART_PORT.OUT |= (1<<RST_bp);
	_delay_ms(25);
	set_capture_timeout(TIMEOUT_MAX);
	UART_PORT.OUT &= ~(1<<RST_bp);
	init_lcd(&LCD_DATA_PORT, &LCD_CONTROL_PORT);
	_delay_ms(1);
	
	//Display initial sleep message
	clear_screen(&LCD_DATA_PORT, &LCD_CONTROL_PORT);
	write_string("Sleeping...", &LCD_DATA_PORT, &LCD_CONTROL_PORT);
	
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

