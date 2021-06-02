# uart_fingerprint_sensor

This is a library for the Waveshare Round Capacitive UART Fingerprint sensor.

This device communicates at a baud rate of 19200, with 8N1 UART config.

As is, communication with the device does not function as intended.  I'm not
quite sure what the problem is.  I have a feeling it is either the baud
register calculation, or with the actual sending/receiving of data from the 
fingerprint sensor.
