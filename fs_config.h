/*
 * fs_config.h
 *
 * Created: 5/28/2021 12:12:40 PM
 *  Author: gonat
 */ 


#ifndef FS_CONFIG_H_
#define FS_CONFIG_H_

#define F_CPU 3333333
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU / (16 * (float)BAUD_RATE)))
#define TX_bp 0
#define RX_bp 1
#define WAKE_bp 2
#define RST_bp 3
#define SWITCH_bp 6

#define LOW_BYTE_bm 0xFF
#define HIGH_BYTE_bm 0xFF00

#define RES_BYTE 4
#define MODE_BYTE 3
#define LEN_EIGENVALUE 193

#define ACK_SUCCESS 0x00 //Success
#define ACK_FAIL 0x01 //Failed
#define ACK_FULL 0x04 //The database is full
#define ACK_NOUSER 0x05 //The user does not exist
#define ACK_USER_OCCUPIED 0x06 //The user exists
#define ACK_FINGER_OCCUPIED 0x07 //The fingerprint exists
#define ACK_TIMEOUT 0x08 //Time out

#define PERM_0 0x00
#define PERM_1 0x01
#define PERM_2 0x02
#define PERM_3 0x03

#define ZERO 0x00

#define EN_DUP 0x00
#define DIS_DUP 0x01
#define NEW_MODE 0x00
#define READ_MODE 0x01

#define BYTEL 3
#define BYTEH 2
#define MAX_USERS 1000
#define QUERY_COUNT 0x00
#define QUERY_AMOUNT 0xFF

#define SET_TIMEOUT 0x00
#define QUERY_TIMEOUT 0x00
#define TIMEOUT_NONE 0x00
#define TIMEOUT_LOW 0x33
#define TIMEOUT_MID 0x7F
#define TIMEOUT_HIGH 0xBF
#define TIMEOUT_MAX 0xFF

#define HEAD 0xF5
#define TAIL 0xF5
#define MODIFY_SN 0x08
#define QUERY_SN 0x2A
#define SLEEP 0x2C
#define SET_READ_MODE 0x2D
#define ADD_PRINT1 0x01
#define ADD_PRINT2 0x02
#define ADD_PRINT3 0x03
#define ADD_USER 0x06
#define DELETE_USER 0x04
#define DELETE_ALL 0x05
#define USER_COUNT 0x09
#define COMPARE_1to1 0x0B
#define COMPARE_1toN 0x0C
#define QUERY_PERM 0x0A
#define SET_COMPARE_LEVEL 0x28
#define ACQUIRE_IMG_UPL 0x24
#define ACQUIRE_IMG_UPL_EIG 0x23
#define CAPTURE_TIMEOUT 0x2E

#endif /* FS_CONFIG_H_ */
