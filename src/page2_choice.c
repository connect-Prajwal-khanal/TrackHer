#include <stdio.h>
#include "raylib.h"
#include "ui.h"
#include "backend.h"
#include "page3_insights.h"  // Include the header file where Draw_Insight() is declared

// Declare the screen dimensions
#define SCREEN_WIDTH 800 
#define SCREEN_HEIGHT 600

// Add globally accessible variables
int days_left = 0;
int fertility_percentage = 0;
bool insightsCalculated = false;

// Function to handle transition to different screens
void work_choice(void);

// Global variable to manage which screen to display
typedef enum {PAGE2, AVATAR, INSIGHT} ScreenType;
ScreenType currentScreen = PAGE2;

void work_choice(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Choice AVATAR");

    // Load icons (Ensure these files exist in the working directory)
    Texture2D insightIcon = LoadTexture("insight.png");
    Texture2D avatarIcon = LoadTexture("avatar.png");

    SetTargetFPS(60);

    // Main loop to handle screen drawing and input
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){ 224, 135, 102, 255}); // Light peach background

        // Handle AVATAR for page 2
        if (currentScreen == PAGE2) {
            // Define positions
            int iconSpacing = 20; // Increased spacing between icon and text
            int textOffset = 60;   // Adjusted vertical text position
            
            // Draw "make a choice" text
            DrawText("What do you want to explore today?", SCREEN_WIDTH / 2 - MeasureText("What do you want to explore today?", 15) / 2, SCREEN_HEIGHT / 2 + 160, 15, DARKBROWN);

            // Draw "INSIGHT" icon
            int insightX = SCREEN_WIDTH / 4 - insightIcon.width / 2;
            int insightY = SCREEN_HEIGHT / 2 - 50;
            DrawTexture(insightIcon, insightX, insightY, WHITE);
            DrawText("insight", insightX + insightIcon.width / 2 - 20, insightY + insightIcon.height + iconSpacing + textOffset, 25, DARKBROWN);

            // Draw female avatar icon
            int avatarX = (SCREEN_WIDTH * 3) / 4 - avatarIcon.width / 2;
            int avatarY = SCREEN_HEIGHT / 2 - 50;
            DrawTexture(avatarIcon, avatarX, avatarY, WHITE);
            DrawText("me", avatarX + avatarIcon.width / 2 - 10, avatarY + avatarIcon.height + iconSpacing + textOffset, 25, DARKBROWN);

            // Check for clicks on the avatar icon (Switch to Period Tracker)
            Rectangle avatarButtonRect = {avatarX, avatarY, avatarIcon.width, avatarIcon.height};
            if (CheckCollisionPointRec(GetMousePosition(), avatarButtonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = AVATAR;
            }

            // Check for clicks on the insight icon (Switch to Insight screen)
            Rectangle insightButtonRect = {insightX, insightY, insightIcon.width, insightIcon.height};
            if (CheckCollisionPointRec(GetMousePosition(), insightButtonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = INSIGHT;
            }
        }

        // Handle Period Tracker AVATAR screen (from AVATAR.c)
        if (currentScreen == AVATAR) {
            DrawPeriodTrackerUI();  // This function is defined in AVATAR.c
        }

        if (currentScreen == INSIGHT) {
            if (!insightsCalculated) {

                // 👉 Declare all required variables here
                char last_period[20];             // Holds last period date from file
                int cycle_length = 0;             // Holds average cycle length
                char next_period_str[20];         // Will be filled by calculate_next_period()

                char today_str[20];
                // Get today's date
                time_t t = time(NULL);
                struct tm today = *localtime(&t);
                strftime(today_str, sizeof(today_str), "%Y-%m-%d", &today);

                // 👉 Load last period date from file
                if (!load_last_period("cycle_data.txt", last_period, sizeof(last_period))) {
                    printf("Error loading last period\n");
                    return;
                }

                // 👉 Get average cycle length from file
                cycle_length = average_cycle_length("cycle_data.txt");
        
                calculate_next_period(last_period, cycle_length, next_period_str); 

                days_left = days_until_next_period(today_str, next_period_str);
                fertility_percentage = calculate_fertility_percentage(today_str, next_period_str);
                insightsCalculated = true;
            }
        
            Draw_Insight(days_left, fertility_percentage);
        }

        EndDrawing();
    }

    // Unload textures
    UnloadTexture(insightIcon);
    UnloadTexture(avatarIcon);
    CloseWindow();
}
