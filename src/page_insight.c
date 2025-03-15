// insight.c
#include "raylib.h"
#include "backend.h"
#include <stdio.h>
#include "calender.h"
#include <string.h>
#include "page_choice.h"

#define SCREEN_WIDTH 850

extern ScreenType currentScreen;


int selectedTab = 0; // 0: Overview, 1: Fertility, 2: Cycle Chart
void Draw_CycleLengthChart() {
    int cycle_lengths[6] = {26, 28, 30, 27, 29, average_cycle_length("../data/data.txt")}; 
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

    DrawText("Last 6 Cycle Length (Days)", chart_x, chart_y + 40, 18, BROWN);
}

void Draw_Insight(int days_left, int fertility_percentage, const char *fertility_label) {
    ClearBackground((Color){252, 243, 207, 255});

    // Tabs
    Rectangle tab1 = {120, 150, 150, 40};
    Rectangle tab2 = {280, 150, 150, 40};
    Rectangle tab3 = {440, 150, 150, 40};

    DrawRectangleRec(tab1, selectedTab == 0 ? DARKBROWN : LIGHTGRAY);
    DrawText("Overview", tab1.x + 20, tab1.y + 10, 18, selectedTab == 0 ? WHITE : DARKBROWN);

    DrawRectangleRec(tab2, selectedTab == 1 ? DARKBROWN : LIGHTGRAY);
    DrawText("Fertility", tab2.x + 30, tab2.y + 10, 18, selectedTab == 1 ? WHITE : DARKBROWN);

    DrawRectangleRec(tab3, selectedTab == 2 ? DARKBROWN : LIGHTGRAY);
    DrawText("Cycle Chart", tab3.x + 10, tab3.y + 10, 18, selectedTab == 2 ? WHITE : DARKBROWN);

    Vector2 mouse = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, tab1)) selectedTab = 0;
        else if (CheckCollisionPointRec(mouse, tab2)) selectedTab = 1;
        else if (CheckCollisionPointRec(mouse, tab3)) selectedTab = 2;
    }

    if (selectedTab == 0) {
        DrawRectangleRounded((Rectangle){100, 210, 650, 180}, 0.2f, 10, (Color){239, 140, 104, 255});
        DrawText("Today", 120, 230, 20, DARKBROWN);

        char buffer[128];
        snprintf(buffer, sizeof(buffer), "PERIODS IN %d DAY%s", days_left, days_left == 1 ? "" : "S");
        DrawText(buffer, 120, 260, 24, DARKBROWN);
        DrawText("Stay Prepared and take care!", 120, 300, 18, DARKBROWN);
    }
    else if (selectedTab == 1) {
            DrawRectangleRounded((Rectangle){100, 210, 650, 200}, 0.2f, 10, (Color){239, 140, 104, 255});

            const char *fertility_message;
            const char *cycle_phase;

            if (strcmp(fertility_label, "High") == 0) {
                fertility_message = "You have HIGH chances of getting pregnant";
                cycle_phase = "You're in your OVULATION phase";
            } else if (strcmp(fertility_label, "Moderate") == 0) {
                fertility_message = "You have MODERATE chances of getting pregnant";
                cycle_phase = "You're in your FERTILE window";
            } else if (strcmp(fertility_label, "Low") == 0) {
                fertility_message = "You have LOW chances of getting pregnant";

                // More accurate biological phase suggestion (optional: use cycle_day if available)
                if (days_left <= 2)
                    cycle_phase = "You're in your LUTEAL phase";
                else if (days_left <= 6)
                    cycle_phase = "You're in your POST-OVULATORY phase";
                else
                    cycle_phase = "You're in your FOLLICULAR phase";
            } else {
                fertility_message = "You are NOT in your fertile window";
                cycle_phase = "You're in your SAFE phase";
            }

            DrawText(fertility_message, 120, 240, 20, DARKBROWN);
            DrawText(cycle_phase, 150, 280, 18, DARKBROWN);

            // Fertility color mapping based on scale
            Color fertilityColor;
            if (fertility_percentage >= 80)
                fertilityColor = (Color){34, 139, 34, 255}; // dark green
            else if (fertility_percentage >= 60)
                fertilityColor = (Color){154, 205, 50, 255}; // yellow-green
            else if (fertility_percentage >= 40)
                fertilityColor = (Color){255, 165, 0, 255}; // orange
            else
                fertilityColor = (Color){220, 20, 60, 255}; // red

            DrawCircle(650, 275, 40, fertilityColor);
            DrawText("Fertility", 625, 260, 14, DARKBROWN);

            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%d%%", fertility_percentage);
            DrawText(buffer, 635, 290, 14, DARKBROWN);
        }

        else if (selectedTab == 2) {
            Draw_CycleLengthChart();
        
            // Extract Data Button
            Rectangle extractButton = {SCREEN_WIDTH / 2 - 80, 540, 160, 40};
            DrawRectangleRounded(extractButton, 0.3f, 10, DARKBROWN);
            DrawText("Extract Data", extractButton.x + 25, extractButton.y + 10, 20, WHITE);
        
            if (CheckCollisionPointRec(GetMousePosition(), extractButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                FILE *sourceFile = fopen("../data/data.txt", "r");
                FILE *exportFile = fopen("../data/Exported_cycle_data.txt", "w");
        
                if (sourceFile != NULL && exportFile != NULL) {
                    char buffer[256];
                    while (fgets(buffer, sizeof(buffer), sourceFile)) {
                        fputs(buffer, exportFile);
                    }
                    fclose(sourceFile);
                    fclose(exportFile);
                    // Optional: Show success message if needed
                } else {
                    if (sourceFile) fclose(sourceFile);
                    if (exportFile) fclose(exportFile);
                    // Optional: Handle error or display failure message
                }
            }
        }
        

    Rectangle back_button = {20, 20, 100, 40};
    back_DrawButton(back_button, "< Back");
    if (CheckCollisionPointRec(GetMousePosition(), back_button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentScreen = PAGE2;
    }
}
