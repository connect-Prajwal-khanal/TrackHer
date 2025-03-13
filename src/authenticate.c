#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "authenticate.h"

#define MAX_USERS 100
#define FILENAME "users.txt"
#define KEY "secret"

// Simple XOR encryption
void encrypt_data(const char *input, char *output, const char *key) {
    size_t key_length = strlen(key);
    for (size_t i = 0; i < strlen(input); i++) {
        output[i] = input[i] ^ key[i % key_length];
    }
    output[strlen(input)] = '\0'; // Null-terminate the output
}

// Decryption is same as encryption
void decrypt_data(const char *input, char *output, const char *key) {
    encrypt_data(input, output, key);
}

// Function to register a new user
int register_user(const char *username, const char *password) {
    FILE *file = fopen(FILENAME, "a");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    char encrypted_pass[100];
    encrypt_data(password, encrypted_pass, KEY);

    fprintf(file, "%s %s\n", username, encrypted_pass);
    fclose(file);
    return 1;
}

// Function to authenticate user login
int authenticate_user(const char *username, const char *password) {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    char stored_user[100], stored_pass[100], decrypted_pass[100];
    char encrypted_pass[100];
    encrypt_data(password, encrypted_pass, KEY);

    while (fscanf(file, "%s %s", stored_user, stored_pass) == 2) {
        decrypt_data(stored_pass, decrypted_pass, KEY);
        if (strcmp(stored_user, username) == 0 && strcmp(decrypted_pass, password) == 0) {
            fclose(file);
            return 1; // Authentication successful
        }
    }
    fclose(file);
    return 0; // Authentication failed
}
