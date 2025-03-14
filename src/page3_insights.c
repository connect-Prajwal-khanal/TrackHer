// insight.c
#include "raylib.h"
#include "backend.h"
#include <stdio.h>
#include "ui.h"
#include <string.h>
#include "page2_choice.h"

extern ScreenType currentScreen;

void Draw_Insight(int days_left, int fertility_percentage, const char *fertility_label) {
    ClearBackground((Color){252, 243, 207, 255}); // Soft background

    // Main card
    DrawRectangleRounded((Rectangle){100, 200, 650, 200}, 0.2f, 10, (Color){239, 140, 104, 255});

    // Headings
    DrawText("Today", 120, 220, 20, DARKBROWN);

    // Period Info
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "PERIODS IN %d DAY%s", days_left, days_left == 1 ? "" : "S");
    DrawText(buffer, 120, 250, 24, DARKBROWN);

    // Dynamic Fertility Message
    const char *fertility_message;
    const char *cycle_phase;

    if (strcmp(fertility_label, "High") == 0) {
        fertility_message = "HIGH CHANCES OF GETTING PREGNANT";
        cycle_phase = "You're in your OVULATION phase";
    } else if (strcmp(fertility_label, "Moderate") == 0) {
        fertility_message = "MODERATE CHANCES OF GETTING PREGNANT";
        cycle_phase = "You're in your FERTILE window";
    } else if (strcmp(fertility_label, "Low") == 0) {
        fertility_message = "LOW CHANCES OF GETTING PREGNANT";
        if (days_left <= 2) {
            cycle_phase = "You're in your LUTEAL phase";
        } else if (days_left <= 6) {
            cycle_phase = "You're in your POST-OVULATORY phase";
        } else {
            cycle_phase = "You're in your FOLLICULAR phase";
        }
    } else { // Not Fertile
        fertility_message = "NOT IN YOUR FERTILE WINDOW";
        cycle_phase = "You're in your SAFE phase";
    }

    // Draw the messages
    DrawText(fertility_message, 120, 280, 20, DARKBROWN);
    DrawText(cycle_phase, 150, 330, 16, DARKBROWN);

    // Fertility Percentage Circle
    DrawCircle(650, 275, 40, BEIGE);
    DrawText("Fertility", 625, 260, 14, DARKBROWN);
    snprintf(buffer, sizeof(buffer), "%d%%", fertility_percentage);
    DrawText(buffer, 635, 290, 14, DARKBROWN);

     // Back Button
     Rectangle back_button = {20, 20, 100, 40};
     back_DrawButton(back_button, "< Back");
 
     if (CheckCollisionPointRec(GetMousePosition(), back_button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
         currentScreen = PAGE2;  // Switch screen back to choice page
     }
    
}
