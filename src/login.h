#ifndef LOGIN_H
#define LOGIN_H

struct user {
    char fullName[50];
    char email[50];
    char password[50];
    char username[50];
};

void takepassword(char pwd[50]);
void takeinput(char ch[50]);
char generateUsername(char email[50], char username[50]);
void signup();
int authenticate(char username[50], char password[50]);

#endif
