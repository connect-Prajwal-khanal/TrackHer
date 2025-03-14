#include <stdio.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "ui.h"
#include "backend.h"
#include "page2_choice.h"
#include "page3_insights.h"  // Include the header file where Draw_Insight() is declared

#define SCREEN_WIDTH 800 
#define SCREEN_HEIGHT 600

// Global state
int days_left = 0;
int fertility_percentage = 0;
char fertility_label[16] = "";
bool insightsCalculated = false;


ScreenType currentScreen = PAGE2;

// Function declaration
void work_choice(void);

void work_choice(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Choice AVATAR");

    Texture2D insightIcon = LoadTexture("insight.png");
    Texture2D avatarIcon = LoadTexture("avatar.png");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){ 224, 135, 102, 255 }); // Light peach

        if (currentScreen == PAGE2) {
            // Layout
            int iconSpacing = 20;
            int textOffset = 60;

            DrawText("What do you want to explore today?", 
                     SCREEN_WIDTH / 2 - MeasureText("What do you want to explore today?", 15) / 2,
                     SCREEN_HEIGHT / 2 + 160, 15, DARKBROWN);

            // Insight icon
            int insightX = SCREEN_WIDTH / 4 - insightIcon.width / 2;
            int insightY = SCREEN_HEIGHT / 2 - 50;
            DrawTexture(insightIcon, insightX, insightY, WHITE);
            DrawText("insight", insightX + insightIcon.width / 2 - 20, 
                     insightY + insightIcon.height + iconSpacing + textOffset, 25, DARKBROWN);

            // Avatar icon
            int avatarX = (SCREEN_WIDTH * 3) / 4 - avatarIcon.width / 2;
            int avatarY = SCREEN_HEIGHT / 2 - 50;
            DrawTexture(avatarIcon, avatarX, avatarY, WHITE);
            DrawText("me", avatarX + avatarIcon.width / 2 - 10,
                     avatarY + avatarIcon.height + iconSpacing + textOffset, 25, DARKBROWN);

            // Click handlers
            Rectangle avatarButtonRect = { avatarX, avatarY, avatarIcon.width, avatarIcon.height };
            if (CheckCollisionPointRec(GetMousePosition(), avatarButtonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = AVATAR;
            }

            Rectangle insightButtonRect = { insightX, insightY, insightIcon.width, insightIcon.height };
            if (CheckCollisionPointRec(GetMousePosition(), insightButtonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = INSIGHT;
            }
        }

        else if (currentScreen == AVATAR) {
            DrawPeriodTrackerUI();  // Draw from AVATAR.c
        }

        else if (currentScreen == INSIGHT) {
            if (!insightsCalculated) {
                char last_period[20];
                char today_str[20];
                char next_period_str[20];
                int cycle_length = 0;

                // Get today's date
                time_t t = time(NULL);
                struct tm today = *localtime(&t);
                strftime(today_str, sizeof(today_str), "%Y-%m-%d", &today);

                // Load last period
                if (!load_last_period("data.txt", last_period, sizeof(last_period))) {
                    printf("Error loading last period\n");
                    strcpy(last_period, today_str);  // fallback
                }

                // Get average cycle and next period
                cycle_length = average_cycle_length("data.txt");
                calculate_next_period(last_period, cycle_length, next_period_str);

                // Get insights
                days_left = days_until_next_period(today_str, next_period_str);

                FertilityStatus status = calculate_fertility_status(today_str, next_period_str);
                fertility_percentage = status.percentage;
                strcpy(fertility_label, status.label);


                // Debug print
                printf("Last Period: %s\n", last_period);
                printf("Cycle Length: %d\n", cycle_length);
                printf("Next Period: %s\n", next_period_str);
                printf("Today: %s\n", today_str);
                printf("Days Left: %d\n", days_left);
                printf("Fertility: %d%%, Label: %s\n", fertility_percentage, fertility_label);

                insightsCalculated = true;
            }

            Draw_Insight(days_left, fertility_percentage, fertility_label);
        }

        EndDrawing();
    }

    UnloadTexture(insightIcon);
    UnloadTexture(avatarIcon);
    CloseWindow();
}
