/*
FILENAME :     pam_panic_authdevice.c
DESCRIPTION :  Authenticates against the removable media
AUTHOR :       Bandie
DATE :         2018-03-27T02:34:08+02:00
LICENSE :      GNU-GPLv3
*/


#include <stdint.h>
#include <unistd.h>

#include "../config.h"

#include "../../lib/gettext.h"
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <syslog.h>

#include "pam_panic_reject.h"
#include "pam_panic_authdevice.h"

#define _(String) gettext(String)


int authDevice(pam_handle_t *pamh, char *allowed, char *rejected, char *serious_dev, int8_t bSerious, int8_t bReboot, int8_t bPoweroff){

  #ifndef TEST

    // gettext
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    int8_t counter = 0;
    while(access(allowed, F_OK) == -1 && access(rejected, F_OK) == -1){
      pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, NULL, _("Key? "));
      if(++counter >= 3){
        pam_syslog(pamh, LOG_NOTICE, _("Couldn't identify any key after 3 tries."));
        return (PAM_MAXTRIES);
      }
    }
  #endif

  if(access(allowed, F_OK) != -1)
    #ifndef TEST
      return (PAM_SUCCESS);
    #else
      return 0;
    #endif
  if(access(rejected, F_OK) != -1)
    #ifndef TEST
      return reject(serious_dev, bSerious, bReboot, bPoweroff);
    #else
      return 99;
    #endif

  #ifndef TEST
    return (PAM_MAXTRIES);
  #else
    return 1;
  #endif

}
