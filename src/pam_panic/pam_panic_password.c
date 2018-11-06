/*
FILENAME :     pam_panic_password.c
DESCRIPTION :  Reads password from PPASSFILE
AUTHOR :       Bandie
DATE :         2018-03-27T02:34:08+02:00
LICENSE :      GNU-GPLv3
*/

#include "pam_panic_password.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "../../lib/gettext.h"

#include <syslog.h>
#include <crypt.h>
#include "pam_panic_reject.h"

#define _(String) gettext(String)

int readPassword(pam_handle_t *pamh, char pw[2][99]){

  // Open file
  if(access(PPASSFILE, F_OK) == -1){
    pam_syslog(pamh, LOG_ALERT, _("ALERT for password option: No password file detected."));
    return 2;
  }
  FILE *f = fopen(PPASSFILE, "r");
  if(f == NULL){
    pam_syslog(pamh, LOG_ALERT, _("ERROR: Couldn't open password file."));
    return 1;
  }

  // Get file contents
  size_t nread;
  char filecontent[198];

  nread = fread(filecontent, sizeof(char), 198, f);
  fclose(f);

  if(nread != 198){
    pam_syslog(pamh, LOG_CRIT, _("CRITICAL: Password file is corrupt!"));
    return 3;
  }

  /* Split file content
  *   pw[0] := key
  *   pw[1] := panic key
  */
  strcpy(pw[0], strtok(filecontent, "\n"));
  strcpy(pw[1], strtok(NULL, "\n"));

  if(pw[0] == NULL || pw[1] == NULL)
    return 1;

  return 0;

}


int authPassword(pam_handle_t *pamh, char *serious_dev, int8_t bSerious, int8_t bReboot, int8_t bPoweroff){

  // gettext
  setlocale (LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);

  // PAM password response
  char resp[256];
  char *response = NULL;

  // Not so panic password
  char *pwkey_tmp;
  char pwkey[99];

  // Panic password
  char *pwpanic_tmp;
  char pwpanic[99];

  // Read passwords from file
  char pw[2][99];
  if(readPassword(pamh, pw))
    return(PAM_ABORT);


  for(int i=0; i<3; i++){
    pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, &response, _("Password: "));

    // Abort on null response. xscreensaver is known for passing null instead of aborting.
    // refer to https://bandie.org/programming/2018/04/24/pam_panic-Security-fix.html
    if(!response)
      return(PAM_ABORT);

    strcpy(resp, response);
 

  
    pwkey_tmp = crypt(resp, pw[0]);
    strcpy(pwkey, pwkey_tmp);

    pwpanic_tmp = crypt(resp, pw[1]);
    strcpy(pwpanic, pwpanic_tmp);

    // Key?

    if(!strcmp(pwkey, pw[0]))
      return (PAM_SUCCESS);
    if(!strcmp(pwpanic, pw[1])){
      return reject(serious_dev, bSerious, bReboot, bPoweroff);
    }
  }

  sleep(5);
  return (PAM_AUTH_ERR);
}
