#include "pam.h"

#define PATH_ENV_VAR "/usr/local/sbin:/usr/local/bin:/usr/bin"

const char* serivce = "EZDM";
static pam_handle_t *pam_handle;

void err(char* fmt, int result){
  do {
    fprintf(stderr, "%s: %s\n", fmt,
      pam_strerror(pam_handle, result));
    end(result);
    exit(2);
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
      err("pam_open_session", result);
  }

  // user exists!
  struct passwd *pw = getpwnam(username);
  init_env(pw); // initialize environement for user

  *child_pid = fork();
  if(*child_pid == 0){
    chdir(pw->pw_dir); // change to the user's home environment
    char* cmd = "exec /bin/bash --login .xinitrc";

    // load xinitrc
    execl(pw->pw_shell, pw->pw_shell, "-c", cmd, NULL);

    printf("huh?");
    exit(1);
  }

  return true;
}

int conv(int num_msg, const struct pam_message **msg,
                struct pam_response** resp, void *appdata_ptr) {
  *resp = calloc(num_msg, sizeof(struct pam_response));
  if (*resp == NULL) return PAM_BUF_ERR;

  int result = PAM_SUCCESS;
  for (int i = 0; i < num_msg; i++) {
    char *username, *password;
    switch (msg[i]->msg_style) {
    case PAM_PROMPT_ECHO_ON:
      username = ((char **) appdata_ptr)[0];
      (*resp)[i].resp = strdup(username);
      break;
    case PAM_PROMPT_ECHO_OFF:
      password = ((char **) appdata_ptr)[1];
      (*resp)[i].resp = strdup(password);
      break;
    case PAM_ERROR_MSG:
      fprintf(stderr, "%s\n", msg[i]->msg);
      result = PAM_CONV_ERR;
      break;
    case PAM_TEXT_INFO:
      printf("%s\n", msg[i]->msg);
      break;
    }
    
    // failed?
    if (result != PAM_SUCCESS) break;
  }

  if (result != PAM_SUCCESS) {
    free(*resp);
    *resp = 0;
  }

  return result;
}

// Initialize the environment
// pw is the information about the user
void init_env(struct passwd* pw){
  set_env("HOME",    pw->pw_dir); // home directory
  set_env("PWD",     pw->pw_dir);  // where everything is executing at
  set_env("SHELL",   pw->pw_shell);
  set_env("USER",    pw->pw_name);
  set_env("LOGNAME", pw->pw_name);
  set_env("PATH",    PATH_ENV_VAR); // path environment variable
                                    // not affected by bash (yet)

  // Xauthority, to prevent people from doing stupid stuff
  // since 2020(?!)
  size_t xauth_len = strlen(pw->pw_dir) + strlen("/.Xauthority") + 1;
  char* xauth      = malloc(xauth_len);
  set_env("XAUTHORITY", xauth);


  free(xauth);
}

// Set 1 value of environment via PAM
void set_env(char* name, char* value){
  // 2 is for '= '
  size_t name_val_len = strlen(name) + strlen(value) + 2;
  char* name_val = malloc(name_val_len);

  // tell PAM to put this in our environment
  pam_putenv(pam_handle, name_val);
  free(name_val);

  // done!
}


bool logout(void){
  // Attempt to close the session
  int result = pam_close_session(pam_handle, 0);
  if(result != 0){
    // could not close the session
    pam_setcred(pam_handle, PAM_DELETE_CRED);
    err("pam_close_session", result);
  }

  // delete credentials
  result = pam_setcred(pam_handle, PAM_DELETE_CRED);
  if(result != 0) err("pam_setcred", result);

  // Goodbye, PAM...
  end(result);
  return true;
}

int end(int last_result){
  // Kill any connections with PAM
  // (I don't want to talk)
  int result = pam_end(pam_handle, last_result);
  pam_handle = 0;
  return result;
}