#ifndef LCDLIB_H_
#define LCDLIB_H_

int check_busy();

void send_command(unsigned char command, PORT_t* data_port, PORT_t* control_port);

void send_character(unsigned char character, PORT_t* data_port, PORT_t* control_port);

void write_string(char* string, PORT_t* data_port, PORT_t* control_port);

void clear_screen(PORT_t* data_port, PORT_t* control_port);

void init_lcd(PORT_t* data_port, PORT_t* control_port);

#endif //LCDLIB_H_