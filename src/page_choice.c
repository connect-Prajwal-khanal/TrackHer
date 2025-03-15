#include <stdio.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "calender.h"
#include "backend.h"
#include "page_choice.h"
#include "page_insight.h"  // Include the header file where Draw_Insight() is declared

#define SCREEN_WIDTH 800 
#define SCREEN_HEIGHT 600


bool insightsCalculated = false;

ScreenType currentScreen = PAGE2;

// Function declaration
void work_choice(void);

void work_choice(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Choice AVATAR");

    Texture2D insightIcon = LoadTexture("../images/insight.png");
    Texture2D avatarIcon = LoadTexture("../images/avatar.png");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){ 224, 135, 102, 255 }); // Light peach

        if (currentScreen == PAGE2) {
            // Layout
            int iconSpacing = 20;
            int textOffset = 40;
            int iconY = SCREEN_HEIGHT / 2 - 80;  // Move icons slightly upward

            DrawText("What do you want to explore today?", 
                     SCREEN_WIDTH / 2 - MeasureText("What do you want to explore today?", 20) / 2,
                     SCREEN_HEIGHT / 2 + 120, 20, DARKBROWN);

            // Insight icon
            int insightX = SCREEN_WIDTH / 4 - insightIcon.width / 2;
            DrawTexture(insightIcon, insightX, iconY, WHITE);
            DrawText("insight", insightX + insightIcon.width / 2 - 20, 
                     iconY + insightIcon.height + iconSpacing + textOffset, 25, DARKBROWN);

            // Avatar icon
            int avatarX = (SCREEN_WIDTH * 3) / 4 - avatarIcon.width / 2;
            DrawTexture(avatarIcon, avatarX, iconY, WHITE);
            DrawText("me", avatarX + avatarIcon.width / 2 - 10,
                     iconY + avatarIcon.height + iconSpacing + textOffset, 25, DARKBROWN);

            // Click handlers
            Rectangle avatarButtonRect = { avatarX, iconY, avatarIcon.width, avatarIcon.height };
            if (CheckCollisionPointRec(GetMousePosition(), avatarButtonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = AVATAR;
            }

            Rectangle insightButtonRect = { insightX, iconY, insightIcon.width, insightIcon.height };
            if (CheckCollisionPointRec(GetMousePosition(), insightButtonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = INSIGHT;
            }
        }

        else if (currentScreen == AVATAR) {
            DrawPeriodTrackerUI();  // Draw calender
        }

        else if (currentScreen == INSIGHT) {
            Draw_Insight();  //draw insight
        }

        EndDrawing();
    }

    UnloadTexture(insightIcon);
    UnloadTexture(avatarIcon);
    CloseWindow();
}