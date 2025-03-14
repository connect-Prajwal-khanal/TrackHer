//backend.h

#ifndef BACKEND_H
#define BACKEND_H

#include <time.h>

typedef struct {
    int percentage;
    const char *label;
} FertilityStatus;

// Function declarations
void calculate_next_period(const char *last_period, int cycle_length, char *next_period);
void calculate_fertile_window(const char *next_period, char *fertile_start, char *fertile_end);
void load_cycle_data(const char *filename, char *buffer, size_t buffer_size);
int load_last_period(const char *filename, char *buffer, size_t buffer_size);
int average_cycle_length(const char *filename);
int days_until_next_period(const char *today_str, const char *next_period_str);
FertilityStatus calculate_fertility_status(const char *today_str, const char *next_period_str);

#endif