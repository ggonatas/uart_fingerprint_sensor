/*
 * IncFile1.h
 *
 * Created: 5/28/2021 11:21:53 AM
 *  Author: gonat
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#include "fs_config.h"

#define BAUD_RATE 19200

//Sends one byte of data via UART
void send_byte(uint8_t data);

/*
Takes in an array of n bytes and sends each through UART
*/
void send_command_nbyte(uint8_t *bytes, int n);

//Receives one bye of data via UART
uint8_t receive_byte();
/*
This function will return an array of data received via UART comm
with the fingerprint sensor.  The array will contain 8 bytes of
data.  See the data sheet for info on the ack responses for
each command.
*/
void receive_ack_8bytes(uint8_t *ack_response);

/*
This function adds a fingerprint to the database.  It will do so
three times to ensure proper adding.  First the print is sent.  Then,
the ack response is checked to make sure the DB is not full and it did
not fail.
*/
uint8_t add_fingerprint(uint8_t perm);

/*
This function will disable the ability of the sensor to store
duplicate fingerprints.  This is the default mode on power on.
Returns whether or not the action was successful.
*/
uint8_t disable_duplicate_mode();

/*
This function will enable the ability of the sensor to store
duplicate fingerprints.  This is not the default mode.
Returns whether or not the action was successful.
*/
uint8_t enable_duplicate_mode();

/*
This function will query the fingerprint sensor and determine if
duplication mode is enabled or not. Returns the duplication mode
upon success, returns 2 for data transfer failure.
*/
uint8_t query_duplicate_mode();

/*
This function will delete all users stored.  The parameter entered
will determine if all users, or only users of a certain permission
will be removed.
*/
uint8_t delete_all_users(uint8_t perm);

/*
This function will read the number of registered users on the 
fingerprint sensor chip. Returns the number of users on success,
or MAX_USERS + 1 upon failure.
*/
uint16_t get_user_count();

/*
This function will capture a fingerprint and get the eigenvalues
from the sensor chip.
*/
uint8_t get_eigenvalues();

/*
This function will compare a UserID 1-to-n with the registered
users in the database.  Will return the user ID if found, or 
MAX_USERS + 1 if not found.
*/
uint16_t compare_1_to_n();

/*
This function sets the query timeout for the sensor.
*/
void set_capture_timeout(uint8_t timeout);

#endif /* INCFILE1_H_ */