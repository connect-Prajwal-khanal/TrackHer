// main program to run the period tracker

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backend.h"
#include "authenticate.h"

#define FILENAME "cycle_data.txt"
#define KEY "secret" // key for XOR encryption
#define MAX 456


int main() 
{
    char last_period[11], next_period[11], fertile_start[11], fertile_end[11];
    int cycle_length;
    char buffer[MAX];
    char saved_last_period[11];
    int saved_cycle_length;

    while (1) 
    {
        printf("Welcome to TrackHer - Period Tracker\n");
        printf("1. Log Period\n");
        printf("2. Predict Next Period\n");
        printf("3. View Cycle Records\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        
        int choice;
        scanf(" %d", &choice);
        getchar();  

        switch (choice) 
        {
            case 1:
                do
                {
                    printf("Enter the last period date (YYYY-MM-DD): ");
                    fgets(last_period, sizeof(last_period), stdin);
                    last_period[strcspn(last_period, "\n")] = 0;
                } while (!validate_date(last_period));  // Repeat the loop until the date is valid

                
                do {
                    printf("Enter your cycle length (in days): ");
                    scanf(" %d", &cycle_length);  

                    // Validate cycle length
                    if (cycle_length < 21 || cycle_length > 35) {
                        printf("Invalid cycle length. Cycle length should be between 21 and 35 days.\n");
                    }
                } while (cycle_length < 21 || cycle_length > 35);  // Repeat the loop until the length is valid
                
                
                // confirm the input
                printf("You entered: Last Period: %s, Cycle Length: %d. Is this correct? (y/n): ", last_period, cycle_length);
                char confirm;
                scanf(" %c", &confirm);
                if (confirm != 'y' && confirm != 'Y') 
                {
                    printf("Entry canceled. Please re-enter your details.\n");
                    continue;
                }


                // Get the current timestamp
                time_t now = time(NULL);
                struct tm *current_time = localtime(&now);
                char timestamp[20];
                strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", current_time);  // Format the timestamp

                // Save the last period date, cycle length, and timestamp
                char log_entry[100];  // Increased buffer size to accommodate the timestamp
                snprintf(log_entry, sizeof(log_entry), "Last Period: %s, Cycle Length: %d, Logged at: %s", last_period, cycle_length, timestamp);

                if (!save_cycle_data(FILENAME, log_entry)) 
                {  // Modify `save_cycle_data` to return success/failure
                    printf("Error: Could not save period data. Please try again.\n");
                    return 2;
                } 
                else 
                {
                    printf("Period logged successfully!\n");
                }
                break;

            case 2:

                // Load the cycle data from the file
                load_last_period(FILENAME, buffer, sizeof(buffer));
            
                // Extract the last period date and cycle length from the saved data
                if (sscanf(buffer, "Last Period: %10s, Cycle Length: %d", saved_last_period, &saved_cycle_length) != 2) {
                    printf("Error reading saved cycle data. Please log your period again.\n");
                    break;
                }

                int average_length = average_cycle_length(FILENAME);
                printf("Average cycle length: %d days\n", average_length);

                // Calculate the next period and fertile window
                calculate_next_period(saved_last_period, average_length, next_period);
                calculate_fertile_window(next_period, fertile_start, fertile_end);
            
                // Display the predictions
                printf("\nPrediction Results:\n");
                printf("--------------------------------------------------\n");
                printf("Last Period Date: %s\n", saved_last_period);
                printf("Cycle Length: %d days\n", saved_cycle_length);
                printf("Next Period Date: %s\n", next_period);
                printf("Fertile Window: %s to %s\n", fertile_start, fertile_end);
                printf("--------------------------------------------------\n");
                break;
            
            case 3:
                printf("Cycle Records:\n");
                load_cycle_data(FILENAME, buffer, sizeof(buffer));
                printf("%s\n", buffer);
                break;

            case 4:
                printf("Exiting the program.....!\n");
                printf("Thank you for using TrackHer! Goodbye!\n");
                printf("Developed by: Prajwal Khanal , Shakshi Gyawali , Ranjana K. Jha , Shaunak Baniya\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
        printf("\n--------------------------------------------------------------------------------------------\n\n");
    }
    return 0;
}