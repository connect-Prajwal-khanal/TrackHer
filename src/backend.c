// backend.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "backend.h"


char *strptime(const char *s, const char *format, struct tm *tm) {
    sscanf(s, "%d-%d-%d", &tm->tm_year, &tm->tm_mon, &tm->tm_mday);
    tm->tm_year -= 1900;
    tm->tm_mon -= 1;
    return (char *)(s + strlen(s)); // Return end of the parsed string
}


// Function to calculate the next period date
void calculate_next_period(const char *last_period, int cycle_length, char *next_period) {
    struct tm tm;
    strptime(last_period, "%Y-%m-%d", &tm); // Parse the last period date
    tm.tm_mday += cycle_length; // Add cycle length
    mktime(&tm); // Normalize the date
    strftime(next_period, 11, "%Y-%m-%d", &tm); // Format the next period date
}

// Function to calculate the fertile window
void calculate_fertile_window(const char *next_period, char *fertile_start, char *fertile_end) {
    struct tm tm;
    strptime(next_period, "%Y-%m-%d", &tm);
    tm.tm_mday -= 14; // Fertile window starts 14 days before the next period
    mktime(&tm);
    strftime(fertile_start, 11, "%Y-%m-%d", &tm); // Format fertile start date

    tm.tm_mday += 6; // Fertile window ends 6 days after the start
    mktime(&tm);
    strftime(fertile_end, 11, "%Y-%m-%d", &tm); // Format fertile end date
}

// Function to save cycle data to a file
int save_cycle_data(const char *filename, const char *data) {
    FILE *file = fopen(filename, "a");
    if (file) {
        fprintf(file, "%s\n", data);
        fclose(file);
        return 1; 
    } else {
        perror("Error opening file");
        return 0; 
    }
}

// Function to load cycle data from a file
int load_cycle_data(const char *filename, char *buffer, size_t buffer_size) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fread(buffer, sizeof(char), buffer_size, file);
        fclose(file);
    } else {
        perror("Error opening file");
    }
}

// Simple XOR encryption
void encrypt_data(const char *input, char *output, const char *key) {
    size_t key_length = strlen(key);
    for (size_t i = 0; i < strlen(input); i++) {
        output[i] = input[i] ^ key[i % key_length];
    }
    output[strlen(input)] = '\0'; // Null-terminate the output
}

// Simple XOR decryption (same as encryption)
void decrypt_data(const char *input, char *output, const char *key) {
    encrypt_data(input, output, key); // XOR encryption and decryption are the same
}