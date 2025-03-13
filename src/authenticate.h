#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

#include <time.h>

// Function declarations

int is_leap_year(int year);
int validate_date(const char *date);
void encrypt_data(const char *input, char *output, const char *key);
void decrypt_data(const char *input, char *output, const char *key);
int register_user(const char *username, const char *password);
int authenticate_user(const char *username, const char *password);

#endif 