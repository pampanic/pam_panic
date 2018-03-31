

int readPassword(pam_handle_t *pamh, char pw[2][99]){

  // Open file
  if(access(PPASSFILE, F_OK) == -1){
    pam_syslog(pamh, LOG_ALERT, "ALERT for password option: No password file detected.");
    return 2;
  }
  FILE *f = fopen(PPASSFILE, "r");
  if(f == NULL){
    pam_syslog(pamh, LOG_ALERT, "ERROR: Couldn't open file.");
    return 1;
  }

  // Get file contents
  size_t nread;
  char filecontent[198];
  char chr;

  nread = fread(filecontent, sizeof(char), 198, f);
  fclose(f);

  if(nread != 198){
    pam_syslog(pamh, LOG_CRIT, "CRITICAL: Password file is corrupt!");
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
    return(PAM_IGNORE);



  pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, &response, "Password:: ");
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
  return (PAM_AUTH_ERR);
}
