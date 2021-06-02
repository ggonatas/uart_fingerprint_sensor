/*
 * cap_fs_uart.c
 *
 * Created: 6/1/2021 9:20:09 AM
 *  Author: gonat
 */ 

#include <avr/io.h>
#include "fs_config.h"
#include "cap_fs_uart.h"

//Sends one byte of data via UART
void send_byte(uint8_t data)
{
	//Wait until any previous transmission is done
	while (!(USART0.STATUS & USART_DREIF_bm));
	//Put byte data in TX Low
	USART0.TXDATAL = data;
}

/*
Takes in an array of n bytes and sends each through UART
*/
void send_command_nbyte(uint8_t *bytes, int n)
{
	for (int i = 0; i < n; i++)
	{
		send_byte(bytes[i]);
	}
}

//Receives one bye of data via UART
uint8_t receive_byte()
{
	while (!(USART0.STATUS & USART_RXCIF_bm));
	return USART0.RXDATAL;
}
/*
This function will put the ack received via UART comm with 
the fingerprint sensor into the array passed.  The array will
contain 8 bytes of data.  See the data sheet for info on the
ack responses for each command.
*/
void receive_ack_8bytes(uint8_t *ack_response)
{
	for (int i = 0; i < 8; i++)
	{
		uint8_t received = receive_byte();
		ack_response[i] = received;
	}
}

/*
This function will receive the eigenvalue data packet
from the fingerprint sensor.  The data of this packet
is 193 bytes, plus the remaining 6 bytes of the std
packet form.
*/

void receive_eigenvalues(uint8_t *eigenvalues)
{
	//Receive first 4 bytes of packet
	receive_byte();
	receive_byte();
	receive_byte();
	receive_byte();
	//Get data and enter into array
	
	for (int i = 0; i < LEN_EIGENVALUE; i++)
	{
		eigenvalues[i] = receive_byte();
	}
}

/*
This function adds a fingerprint to the database.  It will do so
three times to ensure proper adding.  First the print is sent.  Then,
the ack response is checked to make sure the DB is not full and it did
not fail.
*/
uint8_t add_fingerprint(uint8_t perm)
{
	//Generate next user ID, split into low/high
	uint16_t user_id = get_user_count() + 1;
	if (user_id >= MAX_USERS) return ACK_FULL;
	uint8_t user_idL = user_id & LOW_BYTE_bm;
	uint8_t user_idH = (user_id >> 8) & LOW_BYTE_bm;
	//Create checksum and message
	uint8_t b1_chk = HEAD^ADD_PRINT1^user_idL^user_idH^perm;
	uint8_t add_bytes1[] = {HEAD, ADD_PRINT1, user_idH, user_idL, perm, ZERO, b1_chk, TAIL};
	//Send the command
	send_command_nbyte(add_bytes1, 8);
	//Get response from sensor
	uint8_t ack1[8];
	receive_ack_8bytes(ack1);
	//Validate response, return if anything but success
	if (ack1[RES_BYTE] != ACK_SUCCESS) return ack1[RES_BYTE];
	
	//Send a second time
	
	//Create checksum and message
	b1_chk = HEAD^ADD_PRINT2^user_idL^user_idH^perm;
	uint8_t add_bytes2[] = {HEAD, ADD_PRINT2, user_idH, user_idL, perm, ZERO, b1_chk, TAIL};
	//Send the command
	send_command_nbyte(add_bytes2, 8);
	//Get response from sensor
	uint8_t ack2[8];
	receive_ack_8bytes(ack2);
	//Validate response, return if anything but success
	if (ack2[RES_BYTE] != ACK_SUCCESS) return ack2[RES_BYTE];
	
	//Create checksum and message
	b1_chk = HEAD^ADD_PRINT3^user_idL^user_idH^perm;
	uint8_t add_bytes3[] = {HEAD, ADD_PRINT3, user_idH, user_idL, perm, ZERO, b1_chk, TAIL};
	//Send the command
	send_command_nbyte(add_bytes3, 8);
	//Get response from sensor
	uint8_t ack3[8];
	receive_ack_8bytes(ack3);
	//Validate response, return if anything but success
	if (ack3[RES_BYTE] != ACK_SUCCESS) return ack3[RES_BYTE];
	
	return ACK_SUCCESS;
}

uint8_t disable_duplicate_mode()
{
	//Create checksum and message
	uint8_t message_chk = HEAD^NEW_MODE^DIS_DUP^NEW_MODE;
	uint8_t message[] = {HEAD, NEW_MODE, ZERO, DIS_DUP, NEW_MODE, ZERO, message_chk, TAIL};
	//Send the command
	send_command_nbyte(message, 8);
	//Wait for ack and return outcome
	uint8_t ack[8];
	receive_ack_8bytes(ack);
	return ack[RES_BYTE];
}

uint8_t enable_duplicate_mode()
{
	//Create checksum and message
	uint8_t message_chk = HEAD^SET_READ_MODE^EN_DUP^NEW_MODE;
	uint8_t message[] = {HEAD, SET_READ_MODE, ZERO, EN_DUP, NEW_MODE, ZERO, message_chk, TAIL};
	//Send the command
	send_command_nbyte(message, 8);
	//Wait for ack and return outcome
	uint8_t ack[8];
	receive_ack_8bytes(ack);
	return ack[RES_BYTE];
}

uint8_t query_duplicate_mode()
{
	//Create checksum and message
	uint8_t message_chk = HEAD^SET_READ_MODE^READ_MODE;
	uint8_t message[] = {HEAD, SET_READ_MODE, ZERO, ZERO, READ_MODE, ZERO, message_chk, TAIL};
	//Send the command
	send_command_nbyte(message, 8);
	//Wait for ack and return outcome
	uint8_t ack[8];
	receive_ack_8bytes(ack);
	if (ack[RES_BYTE] == ACK_FAIL) return 2;
	return ack[MODE_BYTE];
}

uint8_t delete_all_users(uint8_t perm)
{
	//Check if perm number is valid
	if (perm > PERM_3) return ACK_FAIL;
	//Create checksum and message
	uint8_t message_chk = HEAD^DELETE_ALL^perm;
	uint8_t message[] = {HEAD, DELETE_ALL, ZERO, ZERO, perm, ZERO, message_chk, TAIL};
	//Send the command
	send_command_nbyte(message, 8);
	//Wait for ack and return outcome
	uint8_t ack[8];
	receive_ack_8bytes(ack);
	return ack[RES_BYTE];
}

uint16_t get_user_count()
{
	//Create checksum and message
	uint8_t message_chk = HEAD^USER_COUNT^QUERY_COUNT;
	uint8_t message[] = {HEAD, USER_COUNT, ZERO, ZERO, QUERY_COUNT, ZERO, message_chk, TAIL};
	//Send the command
	send_command_nbyte(message, 8);
	//Wait for ack and return outcome
	uint8_t ack[8];
	receive_ack_8bytes(ack);
	if (ack[RES_BYTE] == ACK_SUCCESS)
	{
		uint16_t count = (ack[BYTEH] << 8) | ack[BYTEL];
		return count;
	}
	else
	{
		return MAX_USERS + 1;
	}
}

uint8_t get_eigenvalues(uint8_t *eigenvalues1)
{
	//Acquire image and upload eigenvalues
	uint8_t b2_chk = HEAD^ACQUIRE_IMG_UPL_EIG;
	uint8_t upload1[] = {HEAD, ACQUIRE_IMG_UPL_EIG, ZERO, ZERO, ZERO, ZERO, b2_chk, TAIL};
	send_command_nbyte(upload1, 8);
	//Receive ack head and eigenvalues
	uint8_t ack_up1[8];
	receive_ack_8bytes(ack_up1);
	if (ack_up1[RES_BYTE] != ACK_SUCCESS) return ACK_FAIL;
	receive_eigenvalues(eigenvalues1);
	return ACK_SUCCESS;
}

uint16_t compare_1_to_n()
{
	//Create checksum and message
	uint8_t message_chk = HEAD^COMPARE_1toN;
	uint8_t message[] = {HEAD, USER_COUNT, ZERO, ZERO, ZERO, ZERO, message_chk, TAIL};
	//Send the command
	send_command_nbyte(message, 8);
	//Wait for ack and return outcome
	uint8_t ack[8];
	receive_ack_8bytes(ack);
	if (ack[RES_BYTE] == ACK_NOUSER || ack[RES_BYTE] == ACK_TIMEOUT) return MAX_USERS + 1;
	uint16_t user_id = (ack[BYTEH] << 8) | ack[BYTEL];
	return user_id;
}

void set_capture_timeout(uint8_t timeout)
{
	//Create checksum and message
	uint8_t message_chk = HEAD^CAPTURE_TIMEOUT^timeout;
	uint8_t message[] = {HEAD, CAPTURE_TIMEOUT, ZERO, timeout, QUERY_TIMEOUT, ZERO, message_chk, TAIL};
	//Send the command
	send_command_nbyte(message, 8);
	//Wait for ack and return outcome
	uint8_t ack[8];
	receive_ack_8bytes(ack);
	return;
}