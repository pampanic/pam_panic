/*
FILENAME :     pam_panic.c
DESCRIPTION :  The pam_panic PAM module shall protect people who have value data on their computer. It provides a panic function.
AUTHOR :       Bandie
DATE :         2018-03-27T02:34:08+02:00
LICENSE :      GNU-GPLv3
*/


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

#ifdef REBOOT
#ifdef POWEROFF
#ifdef CRYPTSETUP

#define ASK "Please enter your secret key to decrypt the firewall and access the mainframe. "


PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags,	int argc, const char *argv[])
{

  char *allowed_arg = malloc(128 * sizeof(char));
  char *rejected_arg = malloc(128 * sizeof(char));
  char *serious_arg = malloc(128 * sizeof(char));
  char *allowed_temp = NULL;
  char *rejected_temp = NULL;
  char *serious_temp = NULL;
  char *allowed = malloc(60 * sizeof(char));
  char *rejected = malloc(60 * sizeof(char));
  char *serious_dev = malloc(60 * sizeof(char));
  int8_t serious = 0;
  int8_t reboot = 0;
  int8_t poweroff = 0;


  // Regex for checking arguments
  char *resp;
  char *pattern = "^[A-Fa-f0-9]\\{8\\}\\-[A-Fa-f0-9]\\{4\\}\\-[A-Fa-f0-9]\\{4\\}\\-[A-Fa-f0-9]\\{4\\}\\-[A-Fa-f0-9]\\{12\\}$";
  regex_t regex;

  if(regcomp(&regex, pattern, 0)){
    pam_syslog(pamh, LOG_CRIT, "ERROR: Problem with regcomp.");
    return (PAM_IGNORE);
  }



  // Check number of arguments
  if(argc<2){
    pam_syslog(pamh, LOG_ERR, "Missing arguments.");
    return (PAM_IGNORE);
  }


  
  // Argument handling
  for(int i=0; i<argc; i++){
    if(strstr(argv[i], "allow") != NULL){
      strcpy(allowed_arg, argv[i]);
      allowed_temp = strtok(allowed_arg, "=");
      allowed_temp = strtok(NULL, "=");
    }

    if(strstr(argv[i], "reject") != NULL){
      strcpy(rejected_arg, argv[i]);
      rejected_temp = strtok(rejected_arg, "=");
      rejected_temp = strtok(NULL, "=");
    }

    if(strstr(argv[i], "reboot") != NULL){
      reboot = 1;
    }

    if(strstr(argv[i], "poweroff") != NULL){
      poweroff = 1;
    }

    if(strstr(argv[i], "serious") != NULL){
      strcpy(serious_arg, argv[i]);
      serious_temp = strtok(serious_arg, "=");
      serious_temp = strtok(NULL, "=");
      serious = 1;
    }
  }



  // Poweroff wins.
  if(reboot && poweroff)
    reboot = 0;




  // Checking arguments
  if(
      allowed_temp == NULL 
      || rejected_temp == NULL 
      || regexec(&regex, allowed_temp, 0, NULL, 0) == REG_NOMATCH 
      || regexec(&regex, rejected_temp, 0, NULL, 0) == REG_NOMATCH
      || (serious && serious_temp == NULL)
  ) {
    pam_syslog(pamh, LOG_ERR, "Argument invalid. Note that allow and reject must have a valid GPT UUID.");
    return (PAM_IGNORE);
  } 


  // Construct variables from arguments
  sprintf(rejected, "/dev/disk/by-partuuid/%s", rejected_temp);
  sprintf(allowed, "/dev/disk/by-partuuid/%s", allowed_temp);
  if(serious)
    sprintf(serious_dev, "/dev/disk/by-partuuid/%s", serious_temp);


  // Free not needed vars
  free(rejected_arg);
  free(allowed_arg);
  free(serious_arg);


  // Check if panic key exist
  if(serious && access(serious_dev, F_OK) == -1){
    pam_syslog(pamh, LOG_ALERT, "ALERT for argument \"serious\": Device doesn't exist.\n");
    return (PAM_IGNORE);
  }


  // Prompt for (auth|panic) key
  int8_t counter = 0;
  while(access(allowed, F_OK) == -1 && access(rejected, F_OK) == -1){
    pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, &resp, ASK);
    if(++counter >= 3){
      pam_syslog(pamh, LOG_NOTICE, "Couldn't identify any keys. 3 tries.");
      return (PAM_MAXTRIES);
    }
  }

  // Auth key? OK!
  if(access(allowed, F_OK) != -1)
    return (PAM_SUCCESS);

  // Panic key? PANIC!!1
  if(access(rejected, F_OK) != -1){

    if(serious){
      int ser_stat;
      int yes[2];
      pipe(yes);
      if(fork() == 0){
        close(yes[1]);
        dup2(yes[0], 0);

        execlp(CRYPTSETUP, CRYPTSETUP, "luksErase", serious_dev, NULL);
      }else {
        close(yes[0]);
        write(yes[1], "YES\n", 4);
        close(yes[1]);

        wait(&ser_stat);
      }
    }

    if(reboot)
      execlp(REBOOT, REBOOT, NULL);
    if(poweroff)
      execlp(POWEROFF, POWEROFF, NULL);

    return (PAM_MAXTRIES);
  }

  return (PAM_MAXTRIES);
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
