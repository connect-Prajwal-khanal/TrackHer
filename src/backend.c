//backend.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "backend.h"
#include "calender.h"

char *strptime(const char *s, const char *format, struct tm *tm) {
    sscanf(s, "%d-%d-%d", &tm->tm_year, &tm->tm_mon, &tm->tm_mday);
    tm->tm_year -= 1900;
    tm->tm_mon -= 1;
    return (char *)(s + strlen(s)); // Return end of the parsed string
}

// Function to calculate the next period date
void calculate_next_period(const char *last_period, int cycle_length, char *next_period) {
    struct tm tm = {0};

    // Parse the last period date (YYYY-MM-DD)
    if (strptime(last_period, "%Y-%m-%d", &tm) == NULL) {
        printf("Error: Invalid date format for last period.\n");
        return;
    }

    // Add cycle length 
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

// to load last period date in format (YYYY-M-DD)
int load_last_period(const char *filename, char *buffer, size_t buffer_size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    char temp[256];  // Temp
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

FertilityStatus calculate_fertility_status(const char *today_str, const char *last_period_str, int avg_cycle_length) {
    struct tm today_tm = {0}, last_period_tm = {0};
    strptime(today_str, "%Y-%m-%d", &today_tm);
    strptime(last_period_str, "%Y-%m-%d", &last_period_tm);

    time_t t_today = mktime(&today_tm);
    time_t t_last = mktime(&last_period_tm);
    
    int cycle_day = (int)((t_today - t_last) / (60 * 60 * 24)) + 1;
    if (cycle_day > avg_cycle_length || cycle_day < 1)
        cycle_day = 1; // Reset to start if out of bounds

    int fertility_percentage = 0;
    const char *label = "Not Fertile";

    // Map day to fertility percentage based on biological curve
    if (cycle_day >= 1 && cycle_day <= 5)
        fertility_percentage = 0;  // Menstrual Phase
    else if (cycle_day >= 6 && cycle_day <= 9)
        fertility_percentage = 10 + (cycle_day - 6) * 7;  // Slowly rising
    else if (cycle_day == 10)
        fertility_percentage = 40;
    else if (cycle_day == 11)
        fertility_percentage = 55;
    else if (cycle_day == 12)
        fertility_percentage = 70;
    else if (cycle_day == 13)
        fertility_percentage = 85;
    else if (cycle_day == 14)
        fertility_percentage = 100; // Ovulation Peak
    else if (cycle_day == 15)
        fertility_percentage = 80;
    else if (cycle_day == 16)
        fertility_percentage = 60;
    else if (cycle_day == 17)
        fertility_percentage = 40;
    else if (cycle_day == 18)
        fertility_percentage = 20;
    else if (cycle_day >= 19 && cycle_day <= avg_cycle_length)
        fertility_percentage = 10; // Luteal Phase — low fertility

    // Assign label
    if (fertility_percentage >= 80)
        label = "High";
    else if (fertility_percentage >= 50)
        label = "Moderate";
    else if (fertility_percentage >= 20)
        label = "Low";
    else
        label = "Very Low";

    FertilityStatus status = { fertility_percentage, label };
    return status;
}

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
