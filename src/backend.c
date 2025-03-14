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



int load_last_period(const char *filename, char *buffer, size_t buffer_size) {
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

    // Extract just the date after "Last Period: "
    char *start = strstr(last_line, "Last Period: ");
    if (start) {
        start += strlen("Last Period: "); // Move pointer after "Last Period: "
        char *end = strchr(start, ',');   // Stop at comma (before "Cycle Length")

        if (end && (end - start < buffer_size)) {
            strncpy(buffer, start, end - start);
            buffer[end - start] = '\0';  // Null terminate
        } else {
            strncpy(buffer, start, buffer_size - 1);
            buffer[buffer_size - 1] = '\0';
        }
    } else {
        // Fallback if format doesn't match
        strncpy(buffer, "1970-01-01", buffer_size);
        buffer[buffer_size - 1] = '\0';
        return 0;
    }

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

FertilityStatus calculate_fertility_status(const char *today_str, const char *next_period_str) {
    struct tm today_tm = {0}, next_tm = {0};
    strptime(today_str, "%Y-%m-%d", &today_tm);
    strptime(next_period_str, "%Y-%m-%d", &next_tm);

    time_t t_today = mktime(&today_tm);
    time_t t_next = mktime(&next_tm);

    // Calculate ovulation day and fertile window
    time_t ovulation = t_next - (14 * 24 * 60 * 60);
    time_t fertile_start = ovulation - (4 * 24 * 60 * 60); // 5 days before ovulation
    time_t fertile_end = ovulation + (1 * 24 * 60 * 60);   // 1 day after ovulation

    FertilityStatus status = {0, "Not Fertile"};

    if (t_today >= fertile_start && t_today <= fertile_end) {
        int days_from_start = (int)((t_today - fertile_start) / (60 * 60 * 24));

        // Fertility curve across fertile window days (index 0-5)
        int fertility_curve[] = {40, 60, 80, 100, 70, 50};
        int percentage = fertility_curve[days_from_start];
        const char *label;

        // Assign label based on percentage
        if (percentage >= 80)
            label = "High";
        else if (percentage >= 60)
            label = "Moderate";
        else if (percentage >= 40)
            label = "Low";
        else
            label = "Very Low";

        status.percentage = percentage;
        status.label = label;
    }

    return status;
}

// int calculate_fertility_percentage(const char *today_str, const char *next_period_str) {
//     struct tm today_tm = {0}, next_tm = {0};
//     strptime(today_str, "%Y-%m-%d", &today_tm);
//     strptime(next_period_str, "%Y-%m-%d", &next_tm);

//     time_t t_today = mktime(&today_tm);
//     time_t t_next = mktime(&next_tm);

//     // Calculate fertile window
//     time_t ovulation = t_next - (14 * 24 * 60 * 60); // Ovulation day
//     time_t fertile_start = ovulation - (4 * 24 * 60 * 60); // 5 days before ovulation
//     time_t fertile_end = ovulation + (1 * 24 * 60 * 60);   // 1 day after ovulation

//     // Assign fertility percentage
//     if (t_today >= fertile_start && t_today <= fertile_end) {
//         // During fertile window
//         int days_from_start = (t_today - fertile_start) / (60 * 60 * 24);
//         // Peak fertility on ovulation day (4th day of window), use a simple curve
//         int fertility[] = {40, 60, 80, 100, 70, 50}; // example curve
//         return fertility[days_from_start];
//     } else {
//         // Outside fertile window
//         return 0;
//     }
// }

int days_until_next_period(const char *today_str, const char *next_period_str) {
    struct tm today = {0}, next_period = {0};
    strptime(today_str, "%Y-%m-%d", &today);
    strptime(next_period_str, "%Y-%m-%d", &next_period);

    time_t t_today = mktime(&today);
    time_t t_next = mktime(&next_period);

    int days = (int)((t_next - t_today) / (60 * 60 * 24));

    // Prevent negative values, return 0 at minimum
    return days < 0 ? 0 : days;
}
