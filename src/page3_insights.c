// insight.c
#include "raylib.h"
#include "backend.h"
#include <stdio.h>

void Draw_Insight(int days_left, int fertility_percentage) {
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

    if (fertility_percentage >= 75) {
        fertility_message = "HIGH CHANCES OF GETTING PREGNANT";
        cycle_phase = "You're in your OVULATION phase";
    } else if (fertility_percentage >= 40) {
        fertility_message = "MODERATE CHANCES OF GETTING PREGNANT";
        cycle_phase = "You're in your FERTILE window";
    } else {
        fertility_message = "LOW CHANCES OF GETTING PREGNANT";
        if (days_left <= 2) {
            cycle_phase = "You're in your LUTEAL phase";
        } else if (days_left <= 6) {
            cycle_phase = "You're in your POST-OVULATORY phase";
        } else {
            cycle_phase = "You're in your FOLLICULAR phase";
        }
    }

    // Draw the messages
    DrawText(fertility_message, 120, 280, 20, DARKBROWN);
    DrawText(cycle_phase, 150, 330, 16, DARKBROWN);

    // Fertility Percentage Circle
    DrawCircle(650, 275, 40, BEIGE);
    DrawText("Fertility", 625, 260, 14, DARKBROWN);
    snprintf(buffer, sizeof(buffer), "%d%%", fertility_percentage);
    DrawText(buffer, 635, 290, 14, DARKBROWN);
}
