#pragma once

#include <stdbool.h>

bool login(char* username, char* password, pid_t* child_pid);
bool logout(void);


void init_env(struct passwd *pw);
void set_env(char *name, char *value);
int end(int last_result);

int conv(int num_msg, const struct pam_message **msg,
                struct pam_response **resp, void *appdata_ptr);