/*
FILENAME :     pam_panic_reject.c
DESCRIPTION :  Panic function
AUTHOR :       Bandie
DATE :         2018-03-27T02:34:08+02:00
LICENSE :      GNU-GPLv3
*/

#include "pam_panic_reject.h"
#include <unistd.h>
#ifdef TEST
  #include <stdio.h>
#else
  #include <security/pam_ext.h>
#endif
#include <sys/wait.h>

int reject(char *serious_dev, int8_t bSerious, int8_t bReboot, int8_t bPoweroff){
  if(bSerious){
    #ifndef TEST
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
    #else
      return 0;
    #endif
  }

  if(bReboot)
    #ifndef TEST
      execlp(REBOOT, REBOOT, NULL);
    #else
      return 1;
    #endif
  if(bPoweroff)
    #ifndef TEST
      execlp(POWEROFF, POWEROFF, NULL);
    #else
      return 2;
    #endif

  #ifndef TEST
    return (PAM_MAXTRIES);
  #else
    return 3;
  #endif
}

