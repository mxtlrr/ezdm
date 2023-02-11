#include <security/pam_appl.h>
#include <security/pam_misc.h>

#include <pwd.h>
#include <paths.h>

#include "pam.h"

const char* serivce = "EZDM";
static pam_handle_t *pam_handle;

void err(char* fmt, int result){
  do {
    fprintf(stderr, "%s: %s\n", fmt,
      pam_strerror(pam_handle, result));
    end(result);
  } while(1);
}

bool login(char* username, char* password, pid_t* child_pid){
  char* data[2] = { username, password };
  struct pam_conv pam_conv = {
    conv, data
  };

  // initiate PAM authentication
  int result = pam_start(serivce, username, &pam_conv,
          &pam_handle);
  if(result != 0) err("pam_start", result);


  result = pam_authenticate(pam_handle, 0);
  if (result != PAM_SUCCESS) err("pam_authenticate", result);

  result = pam_acct_mgmt(pam_handle, 0);
  if (result != PAM_SUCCESS) err("pam_acct_mgmt", result);

  result = pam_setcred(pam_handle, PAM_ESTABLISH_CRED);
  if (result != PAM_SUCCESS) err("pam_setcred", result);

  // Attempt to open a session.
  // If failure, delete credentials
  result = pam_open_session(pam_handle, 0);
  if (result != PAM_SUCCESS) {
      pam_setcred(pam_handle, PAM_DELETE_CRED);
      err("pam_open_session");
  }
}