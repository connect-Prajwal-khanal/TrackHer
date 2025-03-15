// insight.c
#include "raylib.h"
#include "backend.h"
#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "page2_choice.h"

extern ScreenType currentScreen;

void Draw_CycleLengthChart() {
    int cycle_lengths[6] = {26, 28, 30, 27, 29, average_cycle_length("data.txt")}; 
    int num_cycles = 6;
    int chart_x = 120, chart_y = 460;
    int bar_width = 45;
    int max_height = 120;

    DrawLine(chart_x, chart_y, chart_x + num_cycles * bar_width + 20, chart_y, DARKGRAY);  // X-axis
    DrawLine(chart_x, chart_y, chart_x, chart_y - max_height - 20, DARKGRAY);             // Y-axis

    for (int i = 0; i < num_cycles; i++) {
        int bar_height = (cycle_lengths[i] - 20) * 3;
        DrawRectangle(chart_x + (i * bar_width), chart_y - bar_height, bar_width - 8, bar_height, (Color){255, 178, 102, 255});
        
        char label[8];
        snprintf(label, sizeof(label), "%d", cycle_lengths[i]);
        DrawText(label, chart_x + (i * bar_width) + 10, chart_y - bar_height - 20, 14, BROWN);

        char month_label[8];
        snprintf(month_label, sizeof(month_label), "C%d", i + 1); // Cycle index
        DrawText(month_label, chart_x + (i * bar_width) + 5, chart_y + 5, 14, GRAY);
    }

    DrawText("Cycle Length (Days)", chart_x, chart_y + 40, 18, BROWN);
}

void Draw_Insight(int days_left, int fertility_percentage, const char *fertility_label) {
    ClearBackground((Color){252, 243, 207, 255}); // Soft pastel background

    // Main Info Card
    DrawRectangleRounded((Rectangle){80, 120, 700, 220}, 0.2f, 12, (Color){255, 178, 102, 255}); // Light orange

    // Title & Period Info
    DrawText("Today's Insight", 100, 140, 26, DARKBROWN);

    char buffer[128];
    snprintf(buffer, sizeof(buffer), "Period in %d Day%s", days_left, (days_left == 1 ? "" : "s"));
    DrawText(buffer, 100, 180, 22, DARKBROWN);

    // Fertility Message & Phase
    const char *fertility_message;
    const char *cycle_phase;

    if (strcmp(fertility_label, "High") == 0) {
        fertility_message = "High chances of getting pregnant";
        cycle_phase = "You're in the OVULATION phase.";
    } else if (strcmp(fertility_label, "Moderate") == 0) {
        fertility_message = "Moderate chances of getting pregnant";
        cycle_phase = "You're in the FERTILE window.";
    } else if (strcmp(fertility_label, "Low") == 0) {
        fertility_message = "Low chances of getting pregnant";
        if (days_left <= 2) {
            cycle_phase = "You're in the LUTEAL phase.";
        } else if (days_left <= 6) {
            cycle_phase = "You're in the POST-OVULATORY phase.";
        } else {
            cycle_phase = "You're in the FOLLICULAR phase.";
        }
    } else {
        fertility_message = "Not in fertile window";
        cycle_phase = "You're in a SAFE phase.";
    }

    DrawText(fertility_message, 100, 220, 20, DARKBROWN);
    DrawText(cycle_phase, 100, 250, 18, DARKBROWN);

    // Fertility Percentage Circle
    Color fertilityColor = fertility_percentage > 50 ? (Color){50, 205, 50, 255} : (Color){255, 99, 71, 255}; // Tomato red or green
    DrawCircle(680, 220, 45, fertilityColor);
    DrawText("Fertility", 655, 200, 16, DARKBROWN);
    snprintf(buffer, sizeof(buffer), "%d%%", fertility_percentage);
    DrawText(buffer, 665, 235, 20, DARKBROWN);

    // Draw Chart
    Draw_CycleLengthChart();

    // Back Button
    Rectangle back_button = {20, 20, 100, 40};
    back_DrawButton(back_button, "< Back");

    if (CheckCollisionPointRec(GetMousePosition(), back_button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentScreen = PAGE2;
    }
}
