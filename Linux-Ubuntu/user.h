#ifndef USER_H
#define USER_H

#ifdef __cplusplus
extern "C" {
#endif

int create_account();
int login(char* username, int* user_id);

#ifdef __cplusplus
}
#endif

#endif
