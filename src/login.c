#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "login.h"

#define ENTER 13
#define TAB 9
#define BCKSPC 8

// Function to take password input
void takepassword(char pwd[50]) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == ENTER || ch == TAB) {
            pwd[i] = '\0';
            break;
        } else if (ch == BCKSPC) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            pwd[i++] = ch;
            printf("*");
        }
    }
}

// Function to take input (full name, email, etc.)
void takeinput(char ch[50]) {
    fgets(ch, 50, stdin);
    ch[strlen(ch) - 1] = 0;
}

// Function to generate username based on email (before '@' symbol)
void generateUsername(char email[50], char username[50]) {
    for (int i = 0; i < strlen(email); i++) {
        if (email[i] == '@') break;
        else username[i] = email[i];
    }
}

// Signup function
void signup() {
    FILE *fp;
    struct user user;
    char password2[50];

    system("cls");
    printf("\nTrackHer - Signup\n");

    printf("Enter your full name: ");
    takeinput(user.fullName);
    printf("Enter your email: ");
    takeinput(user.email);

    printf("Enter your password: ");
    takepassword(user.password);

    if (strlen(user.password) < 9) {
        printf("\nPassword must be at least 8 characters long.\n");
        return;
    }

    printf("\nConfirm your password: ");
    takepassword(password2);

    if (strcmp(user.password, password2) == 0) {
        generateUsername(user.email, user.username);
        fp = fopen("../data/Users.dat", "a+");
        fwrite(&user, sizeof(struct user), 1, fp);
        printf("\nUser registration successful! Your username is %s\n", user.username);
        fclose(fp);
    } else {
        printf("\nPasswords do not match!\n");
    }
}

// Authentication function (to log in)
int authenticate(char username[50], char password[50]) {
    // FILE *fp;
    // struct user usr;

    // fp = fopen("../data/Users.dat", "r");
    // if (!fp) {
    //     printf("\nError: Unable to open user database.\n");
    //     return 0;
    // }

    // while (fread(&usr, sizeof(struct user), 1, fp)) {
    //     if (strcmp(usr.username, username) == 0 && strcmp(usr.password, password) == 0) {
    //         fclose(fp);
    //         return 1; // Authentication successful
    //     }
    // }

    // fclose(fp);
    // return 0; // Authentication failed
    return 1;
}