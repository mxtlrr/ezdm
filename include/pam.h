#pragma once

#include <stdbool.h>

bool login(char* username, char* password, pid_t* child_pid);
bool logout(void); // TODO


static void init_env(struct passwd *pw);
static void set_env(char *name, char *value);
static int end(int last_result);

static int conv(int num_msg, const struct pam_message **msg,
                struct pam_response **resp, void *appdata_ptr);