/*
FILENAME :     pam_panic_reject.c
DESCRIPTION :  Panic function
AUTHOR :       Bandie
DATE :         2018-03-27T02:34:08+02:00
LICENSE :      GNU-GPLv3
*/

#include <stdint.h>
#include <unistd.h>
#include <security/pam_ext.h>
#include <sys/wait.h>
#include "pam_panic_reject.h"

int reject(char *serious_dev, int8_t bSerious, int8_t bReboot, int8_t bPoweroff){
  if(bSerious){
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

  if(bReboot)
    execlp(REBOOT, REBOOT, NULL);
  if(bPoweroff)
    execlp(POWEROFF, POWEROFF, NULL);

  return (PAM_MAXTRIES);
}

