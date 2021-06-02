
#include <avr/io.h>
#include <util/delay.h>
#include "lcd_lib.h"


void send_command(unsigned char command, PORT_t* data_port, PORT_t* control_port)
{
	
	data_port->OUT = command;	//Write command for LCD
	data_port->DIR |= 0xFF; //Set Port A as output
	control_port->OUT &= ~(1<<3);	//Set LCD to write
	control_port->OUT &= ~(1<<4);	//Set LCD to command
	
	control_port->OUT |= (1<<2);	//Enable LCD
	_delay_us(1);
	control_port->OUT &= ~(1<<2);	//Disable LCD
	_delay_ms(3);
}

void send_character(unsigned char character, PORT_t* data_port, PORT_t* control_port)
{
	
	data_port->OUT = character;	//Write command for LCD
	
	data_port->DIR = 0xFF; //Set Port A as output
	control_port->OUT &= ~(1<<3);	//Set LCD to write
	control_port->OUT |= (1<<4);	//Set LCD to data reg
	
	control_port->OUT |= (1<<2);	//Enable LCD
	_delay_us(1); //Delay for 37 us
	control_port->OUT &= ~(1<<2);	//Disable LCD
	_delay_ms(1);
}

void write_string(char* string, PORT_t* data_port, PORT_t* control_port)
{
	while (*string != 0)
	{
		send_character(*string, data_port, control_port);
		string++;
	}

}

void clear_screen(PORT_t* data_port, PORT_t* control_port)
{
	//Clear the display
	send_command(0x01, data_port, control_port);
	
	//Set cursor to home
	send_command(0x80, data_port, control_port);
}

void init_lcd(PORT_t* data_port, PORT_t* control_port)
{
	//Set to 8-bit bus mode, 2-line display, small font
	send_command(0x38, data_port, control_port);
	
	//Turn on display and don't show/blink cursor
	send_command(0x0C, data_port, control_port);
	
	//Set entry mode to increment and no shift
	send_command(0x06, data_port, control_port);
	
	//Clear the display
	send_command(0x01, data_port, control_port);
	
	//Set cursor to home
	send_command(0x80, data_port, control_port);
	
}