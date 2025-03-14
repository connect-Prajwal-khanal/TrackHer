#include "raylib.h"
#include "backend.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIGHTBLUE (Color){135, 206, 250, 255}  
#define LIGHTGREEN (Color){144, 238, 144, 255} 
#define MAX_TEXT_INPUT_LENGTH 100

typedef struct TextBox {
    Rectangle bounds;
    char text[MAX_TEXT_INPUT_LENGTH];
    bool is_active;
} TextBox;

TextBox cycle_length_textbox = {0};
char cycle_data[100] = "Cycle Data: ";
int selected_day = -1;

// Calendar month/year (dynamic)
int currentMonth = 3;  // Default start month
int currentYear = 2025; // Default start year

void DrawTextBox(TextBox *textbox) {
    DrawRectangleRec(textbox->bounds, textbox->is_active ? LIGHTGRAY : DARKGRAY);
    DrawText(textbox->text, textbox->bounds.x + 5, textbox->bounds.y + 5, 20, textbox->is_active ? BLACK : WHITE);
}

void HandleTextBoxInput(TextBox *textbox) {
    if (textbox->is_active) {
        int key = GetCharPressed();
        while (key > 0) {
            int len = strlen(textbox->text);
            if (len < MAX_TEXT_INPUT_LENGTH - 1) {
                textbox->text[len] = (char)key;
                textbox->text[len + 1] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && strlen(textbox->text) > 0) {
            textbox->text[strlen(textbox->text) - 1] = '\0';
        }
        if (IsKeyPressed(KEY_ENTER)) {
            textbox->is_active = false;
        }
    }
}

void DrawButton(Rectangle rect, const char *text) {
    Color buttonColor = CheckCollisionPointRec(GetMousePosition(), rect) ? LIGHTBLUE : DARKBLUE;
    DrawRectangleRec(rect, buttonColor);
    DrawText(text, rect.x + 10, rect.y + 10, 20, WHITE);
}

void DrawCalendar(int year, int month, int x_offset, int y_offset) {
    struct tm time_info = {0};
    time_info.tm_year = year - 1900;
    time_info.tm_mon = month - 1;
    time_info.tm_mday = 1;
    mktime(&time_info);

    int first_day_of_week = time_info.tm_wday;
    int days_in_month = (month == 2) ? ((year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28) : 
                         (month == 4 || month == 6 || month == 9 || month == 11) ? 30 : 31;

    // Month Title
    const char *month_names[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    DrawText(TextFormat("%s %d", month_names[month - 1], year), x_offset + 100, y_offset - 40, 25, BLACK);

    const char *days_of_week[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    for (int i = 0; i < 7; i++) {
        DrawText(days_of_week[i], x_offset + i * 50 + 10, y_offset, 20, BLACK);
    }

    for (int day = 1; day <= days_in_month; day++) {
        int row = (first_day_of_week + day - 1) / 7;
        int col = (first_day_of_week + day - 1) % 7;
        Rectangle day_rect = {x_offset + col * 50, y_offset + 30 + row * 50, 50, 50};

        if (selected_day == day) {
            DrawRectangleRec(day_rect, LIGHTGREEN);
        }
        DrawRectangleLines(day_rect.x, day_rect.y, day_rect.width, day_rect.height, BLACK);
        DrawText(TextFormat("%d", day), day_rect.x + 15, day_rect.y + 15, 20, BLACK);

        if (CheckCollisionPointRec(GetMousePosition(), day_rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selected_day = day;
        }
    }
}

void SavePeriodData() {
    FILE *file = fopen("data.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char logged_at[30];
    strftime(logged_at, sizeof(logged_at), "%Y-%m-%d %H:%M:%S", t);

    char last_period_date[15];
    snprintf(last_period_date, sizeof(last_period_date), "%04d-%02d-%02d", currentYear, currentMonth, selected_day);

    if (selected_day != -1 && strlen(cycle_length_textbox.text) > 0) {
        fprintf(file, "Last Period: %s, Cycle Length: %s, Logged at: %s\n", 
                last_period_date, cycle_length_textbox.text, logged_at);
        printf("Data Saved: Last Period: %s, Cycle Length: %s, Logged at: %s\n", 
                last_period_date, cycle_length_textbox.text, logged_at);
    } else {
        printf("No day selected or cycle length empty.\n");
    }

    fclose(file);
}

void DrawPeriodTrackerUI() {
    int calendar_x = 50, calendar_y = 70;
    int calendar_width = 7 * 50;
    int textbox_width = 200;
    int button_width = 200;

    // Month Navigation Buttons
    Rectangle prev_button = {calendar_x, calendar_y - 40, 120, 30};
    Rectangle next_button = {calendar_x + calendar_width - 120, calendar_y - 40, 120, 30};
    DrawButton(prev_button, "< Prev Month");
    DrawButton(next_button, "Next Month >");

    if (CheckCollisionPointRec(GetMousePosition(), prev_button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentMonth--;
        if (currentMonth < 1) {
            currentMonth = 12;
            currentYear--;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), next_button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentMonth++;
        if (currentMonth > 12) {
            currentMonth = 1;
            currentYear++;
        }
    }

    // Draw Calendar
    DrawCalendar(currentYear, currentMonth, calendar_x, calendar_y);

    // Textbox
    int textbox_x = calendar_x + (calendar_width - textbox_width) / 2;
    int textbox_y = calendar_y + 280;
    int button_x = calendar_x + (calendar_width - button_width) / 2;
    int button_y = textbox_y + 50;

    cycle_length_textbox.bounds = (Rectangle){textbox_x, textbox_y, textbox_width, 30};
    DrawTextBox(&cycle_length_textbox);

    if (CheckCollisionPointRec(GetMousePosition(), cycle_length_textbox.bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        cycle_length_textbox.is_active = true;
    }

    HandleTextBoxInput(&cycle_length_textbox);

    // Log Period Button
    Rectangle log_period_button = {button_x, button_y, button_width, 40};
    DrawButton(log_period_button, "Log Period");

    if (CheckCollisionPointRec(GetMousePosition(), log_period_button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        SavePeriodData();
    }
}
