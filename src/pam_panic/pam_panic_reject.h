/*
FILENAME :     pam_panic_reject.h
AUTHOR :       Bandie
DATE :         2018-03-27T02:34:08+02:00
LICENSE :      GNU-GPLv3
*/

#ifndef PAM_PANIC_REJECT_H
#define PAM_PANIC_REJECT_H


#include "../config.h"
#include <stdint.h>

int reject(char *serious_dev, int8_t bSerious, int8_t bReboot, int8_t bPoweroff);


#endif
