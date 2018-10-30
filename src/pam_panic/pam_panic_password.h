/*
FILENAME :     pam_panic_password.h
AUTHOR :       Bandie
DATE :         2018-03-27T02:34:08+02:00
LICENSE :      GNU-GPLv3
*/

#ifndef PAM_PANIC_PASSWORD_H
#define PAM_PANIC_PASSWORD_H

#include "../config.h"
#include <stdint.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>

#ifndef PPASSFILE
  #error PPASSFILE must be declared!
#endif

int authPassword(pam_handle_t *pamh, char *serious_dev, int8_t bSerious, int8_t bReboot, int8_t bPoweroff);

#endif
