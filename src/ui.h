#ifndef UI_H
#define UI_H

#include "raylib.h"

// Define the maximum length for text input
#define MAX_TEXT_INPUT_LENGTH 100

// Structure for holding a simple text box (used for cycle length input)
typedef struct TextBox {
    Rectangle bounds;
    char text[MAX_TEXT_INPUT_LENGTH];
    bool is_active;
} TextBox;

// Function declarations for UI components
void DrawTextBox(TextBox *textbox);
void HandleTextBoxInput(TextBox *textbox);
void DrawButton(Rectangle rect, const char *text);
void back_DrawButton(Rectangle rect, const char *text);

// Function declarations for Calendar
void DrawCalendar(int year, int month);

// Function declarations for Period Tracker UI
void DrawPeriodTrackerUI(void);

#endif // UI_H
