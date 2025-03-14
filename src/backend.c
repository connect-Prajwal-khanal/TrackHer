//backend.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "backend.h"
#include "ui.h"


char *strptime(const char *s, const char *format, struct tm *tm) {
    sscanf(s, "%d-%d-%d", &tm->tm_year, &tm->tm_mon, &tm->tm_mday);
    tm->tm_year -= 1900;
    tm->tm_mon -= 1;
    return (char *)(s + strlen(s)); // Return end of the parsed string
}

// Function to calculate the next period date
void calculate_next_period(const char *last_period, int cycle_length, char *next_period) {
    struct tm tm = {0};

    // Parse the last period date (format: YYYY-MM-DD)
    if (strptime(last_period, "%Y-%m-%d", &tm) == NULL) {
        printf("Error: Invalid date format for last period.\n");
        return;
    }

    // Add cycle length to day field
    tm.tm_mday += cycle_length;

    // Normalize the date (adjust month/year if needed)
    time_t result_time = mktime(&tm);
    if (result_time == -1) {
        printf("Error: Failed to calculate next period date.\n");
        return;
    }

    // Get normalized tm structure after mktime
    struct tm *adjusted_tm = localtime(&result_time);

    // Format final next period date as "YYYY-MM-DD"
    strftime(next_period, 11, "%Y-%m-%d", adjusted_tm);  // 11 = 10 chars + null terminator
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



// Function to load the last cycle data entry from a file
int load_last_period (const char *filename, char *buffer, size_t buffer_size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    char temp[256];  // Temporary buffer
    char last_line[256] = ""; 

    // Read through each line to find the last entry
    while (fgets(temp, sizeof(temp), file) != NULL) {
        strncpy(last_line, temp, sizeof(last_line) - 1);  // Update last_line
        last_line[sizeof(last_line) - 1] = '\0';  
    }

    fclose(file);

    // Copy the last line to buffer
    strncpy(buffer, last_line, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';  //null terminates

    return 1;
}

// Function to calculate the average cycle length
int average_cycle_length(const char *filename)
{
    FILE *fp = fopen (filename,"r");
    if (fp == NULL)
    {
        perror("Error opening file");
        return 2;
    }

    char line[256];
    int cycle_length;
    int count=0;
    int sum = 0;

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        char *ptr = strstr(line , "Cycle Length: ");
        if(ptr != NULL)
        {
            ptr += strlen("Cycle Length: ");
            if(sscanf(ptr, "%d", &cycle_length) == 1)
            {
                sum += cycle_length;
                count++;
            }
        }
    }
    fclose(fp);

    return count > 0 ? sum / count : 0;
}

// Function to load cycle data from a file
void load_cycle_data(const char *filename, char *buffer, size_t buffer_size) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fread(buffer, sizeof(char), buffer_size, file);
        fclose(file);
    } else {
        perror("Error opening file");
    }
}


// Function to calculate fertility percentage (simplified model)
int calculate_fertility_percentage(const char *today_str, const char *next_period_str) {
    struct tm today = {0}, next_period = {0};
    strptime(today_str, "%Y-%m-%d", &today);
    strptime(next_period_str, "%Y-%m-%d", &next_period);

    time_t t_today = mktime(&today);
    time_t t_next = mktime(&next_period);

    int days_to_next_period = (int)((t_next - t_today) / (60 * 60 * 24));
    int ovulation_day = days_to_next_period - 14;

    int distance_from_ovulation = abs(ovulation_day);
    int fertility = 100 - (distance_from_ovulation * 10); // crude estimation

    if (fertility < 0) fertility = 0;
    if (fertility > 100) fertility = 100;

    return fertility;
}

// Function to calculate days left until next period
int days_until_next_period(const char *today_str, const char *next_period_str) {
    struct tm today = {0}, next_period = {0};
    strptime(today_str, "%Y-%m-%d", &today);
    strptime(next_period_str, "%Y-%m-%d", &next_period);

    time_t t_today = mktime(&today);
    time_t t_next = mktime(&next_period);

    return (int)((t_next - t_today) / (60 * 60 * 24));
}
