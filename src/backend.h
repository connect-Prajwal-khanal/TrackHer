#ifndef BACKEND_H
#define BACKEND_H

#include <time.h>

// Function declarations
void calculate_next_period(const char *last_period, int cycle_length, char *next_period);
void calculate_fertile_window(const char *next_period, char *fertile_start, char *fertile_end);
int save_cycle_data(const char *filename, const char *data);
int load_cycle_data(const char *filename, char *buffer, size_t buffer_size);
void encrypt_data(const char *input, char *output, const char *key);
void decrypt_data(const char *input, char *output, const char *key);
int load_last_period(const char *filename, char *buffer, size_t buffer_size);
int average_cycle_length(const char *filename);

#endif // BACKEND_H