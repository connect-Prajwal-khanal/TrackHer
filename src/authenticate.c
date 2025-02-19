#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backend.h"

int is_leap_year(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

int validate_date(const char *date) {
    int year, month, day;
    
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') {
        printf("Invalid format......!\n");
        return 0;  // Invalid format
    }
    
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) {
        printf("Invalid...... seperate the date with -\n");
        return 0;  
    }

    if (month < 1 || month > 12) {
        printf("Invalid month......\n");
        return 0;  
    }

    int days_in_month[] = {31, (is_leap_year(year) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (day < 1 || day > days_in_month[month - 1]) {
        printf("Invalid day......\n");
        return 0;  
    }

    return 1;  
}

