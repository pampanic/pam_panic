/*
FILENAME :     pam_panic.c
DESCRIPTION :  The pam_panic PAM module shall protect people who have value data on their computer. It provides a panic function.
AUTHOR :       Bandie
DATE :         2018-03-27T02:34:08+02:00
LICENSE :      GNU-GPLv3
*/

#include "pam_panic.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <regex.h>
#include <string.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <syslog.h>

#include "pam_panic_reject.h"
#include "../../lib/gettext.h"

#define _(String) gettext(String)

#include "pam_panic_authdevice.h"
#include "pam_panic_password.h"


#ifdef PACKAGE
#ifdef LOCALEDIR
#ifdef REBOOT
#ifdef POWEROFF
#ifdef CRYPTSETUP

int makeRegex(pam_handle_t *pamh, regex_t *regex){
  char *pattern = "^[A-Fa-f0-9]\\{8\\}\\-[A-Fa-f0-9]\\{4\\}\\-[A-Fa-f0-9]\\{4\\}\\-[A-Fa-f0-9]\\{4\\}\\-[A-Fa-f0-9]\\{12\\}$";

  if(regcomp(regex, pattern, 0)){
    pam_syslog(pamh, LOG_CRIT, _("CRITICAL: Problem with regcomp."));
    return 1;
  }
  
  return 0;

}

void argSplit(char **some_arg, char **some_temp, const char *arg){
  strncpy(*some_arg, arg, 128);
  *some_temp = strtok(*some_arg, "=");
  *some_temp = strtok(NULL, "="); 
}

void constrPath(char **dst, char **src){
  sprintf(*dst, "/dev/disk/by-partuuid/%s", *src);
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags,	int argc, const char *argv[]){

  // Devices
  char *allowed_arg = malloc(128 * sizeof(char));
  char *rejected_arg = malloc(128 * sizeof(char));
  char *serious_arg = malloc(128 * sizeof(char));
  char *allowed_temp = NULL;
  char *rejected_temp = NULL;
  char *serious_temp = NULL;
  char *allowed = malloc(60 * sizeof(char));
  char *rejected = malloc(60 * sizeof(char));
  char *serious_dev = malloc(60 * sizeof(char));

  // Switches
  int8_t bSerious = 0;
  int8_t bReboot = 0;
  int8_t bPoweroff = 0;
  int8_t bPassword = 0;


  // Regex for checking arguments
  regex_t regex;
  if(makeRegex(pamh, &regex))
    return (PAM_ABORT);


  // Argument handling
  for(int i=0; i<argc; i++){
    if(strstr(argv[i], "allow") != NULL)
      argSplit(&allowed_arg, &allowed_temp, argv[i]);
    
    if(strstr(argv[i], "reject") != NULL)
      argSplit(&rejected_arg, &rejected_temp, argv[i]);

    if(strstr(argv[i], "reboot") != NULL)
      bReboot = 1;

    if(strstr(argv[i], "poweroff") != NULL)
      bPoweroff = 1;

    if(strstr(argv[i], "password") != NULL){
      bPassword = 1;
    }

    if(strstr(argv[i], "serious") != NULL){
      argSplit(&serious_arg, &serious_temp, argv[i]);
      bSerious = 1;
    }

  }

  // Checking arguments
  if(
         (allowed_temp == NULL && !bPassword)
      || (rejected_temp == NULL && !bPassword)
      || (allowed_temp != NULL && regexec(&regex, allowed_temp, 0, NULL, 0) == REG_NOMATCH)
      || (rejected_temp != NULL && regexec(&regex, rejected_temp, 0, NULL, 0) == REG_NOMATCH)
      || (bSerious && serious_temp == NULL)
    ) {
    pam_syslog(pamh, LOG_ERR, _("ERROR: Arguments invalid. Note that \"allow\" and \"reject\" must have a valid GPT UUID."));
    return (PAM_ABORT);
  } 

  // Poweroff wins.
  if(bReboot && bPoweroff)
    bReboot = 0;

  // Devices will win
  if(bPassword && allowed_temp != NULL && rejected_temp != NULL)
    bPassword = 0;


  // Construct variables from arguments 
  if(allowed_temp != NULL && rejected_temp != NULL){
    constrPath(&rejected, &rejected_temp);
    constrPath(&allowed, &allowed_temp);
  }else{
    rejected = NULL;
    allowed = NULL;
  }
    

  if(bSerious)
    constrPath(&serious_dev, &serious_temp);


  // Free not needed vars
  free(rejected_arg);
  free(allowed_arg);
  free(serious_arg);



  // Check if panic key exist
  if(bSerious && access(serious_dev, F_OK) == -1){
    pam_syslog(pamh, LOG_ALERT, _("ALERT for argument \"serious\": Device doesn't exist."));
    return (PAM_ABORT);
  }


//////////////////////// AUTH PROMPT ////////////////////////////////

  // Prompt for removable media
  if(allowed != NULL && rejected != NULL){
    return authDevice(pamh, allowed, rejected, serious_dev, bSerious, bReboot, bPoweroff);
  }
  // Prompt for password
  else if(bPassword){
    return authPassword(pamh, serious_dev, bSerious, bReboot, bPoweroff); 
  }
  
  return (PAM_ABORT);  

}




// Fuck all of this below.

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char *argv[])
{

	return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char *argv[])
{

	return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char *argv[])
{

	return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char *argv[])
{

	return (PAM_SUCCESS);
}

PAM_EXTERN int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char *argv[])
{

	return (PAM_SERVICE_ERR);
}

#endif
#endif
#endif
#endif
#endif
