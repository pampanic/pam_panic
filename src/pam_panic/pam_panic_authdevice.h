/*
FILENAME :     pam_panic_authdevice.h
AUTHOR :       Bandie
DATE :         2018-03-27T02:34:08+02:00
LICENSE :      GNU-GPLv3
*/


#define ASK "Please enter your secret key to decrypt the firewall and access the mainframe. "

int authDevice(pam_handle_t *pamh, char *allowed, char *rejected, char *serious_dev, int8_t bSerious, int8_t bReboot, int8_t bPoweroff);
