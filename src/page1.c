#include <stdio.h>
#include "raylib.h"
#include <string.h>
#include "login.h"  // Include login-related functions
#include "backend.h" // Include backend-related functions
#include "page2_choice.h" // Include page2-related functions

#define MAX_INPUT_CHARS 50


char today_str[11];
char last_period[20];
char next_period[20];
char fertile_start[20], fertile_end[20];
int cycle_length;


void draw_login_screen() {
    // Initialize input handling variables
    char username[MAX_INPUT_CHARS] = "\0";
    char password[MAX_INPUT_CHARS] = "\0";
    int letterCountUsername = 0;
    int letterCountPassword = 0;
    bool usernameSelected = false;
    bool passwordSelected = false;

    float cursorTimer = 0.0f;  // Timer for cursor blinking
    bool showCursor = false;

    // Window and UI element positions
    const int screenWidth = 1200;
    const int screenHeight = 700;
    const int boxWidth = 350;
    const int boxHeight = 50;
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2 - 100;

    // Define colors
    Color backgroundColor = (Color){224, 135, 102, 255};
    Color textColor = (Color){245, 230, 196, 255};
    Color inputBoxColor = (Color){250, 230, 180, 255};
    Color placeholderColor = (Color){150, 75, 50, 255};
    Color buttonColor = (Color){200, 90, 60, 255};

    Font customFont = GetFontDefault();  // Use default font

    // Initialize window
    InitWindow(screenWidth, screenHeight, "TrackHer - Login");
    SetTargetFPS(60);  // Set the target frames per second

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition(); // Get mouse position

        // Define input box and button rectangles
        Rectangle usernameBox = {centerX - boxWidth / 2, centerY, boxWidth, boxHeight};
        Rectangle passwordBox = {centerX - boxWidth / 2, centerY + 70, boxWidth, boxHeight};
        Rectangle loginButton = {centerX - 100, centerY + 150, 200, 50};
        Rectangle signupButton = {centerX - 100, centerY + 210, 200, 50};

        // Handle input selection
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            usernameSelected = CheckCollisionPointRec(mousePoint, usernameBox);
            passwordSelected = CheckCollisionPointRec(mousePoint, passwordBox);
        }

        // Reset selection if clicked outside the boxes
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            !CheckCollisionPointRec(mousePoint, usernameBox) &&
            !CheckCollisionPointRec(mousePoint, passwordBox)) {
            usernameSelected = false;
            passwordSelected = false;
        }

        // Handle keyboard input
        int key = GetCharPressed();
        while (key > 0) {
            if (usernameSelected && letterCountUsername < MAX_INPUT_CHARS) {
                username[letterCountUsername] = (char)key;
                letterCountUsername++;
                username[letterCountUsername] = '\0';
            }
            if (passwordSelected && letterCountPassword < MAX_INPUT_CHARS) {
                password[letterCountPassword] = (char)key;
                letterCountPassword++;
                password[letterCountPassword] = '\0';
            }
            key = GetCharPressed();  // Check for more key inputs
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (usernameSelected && letterCountUsername > 0) {
                username[--letterCountUsername] = '\0';
            }
            if (passwordSelected && letterCountPassword > 0) {
                password[--letterCountPassword] = '\0';
            }
        }

        // Mask password input
        char maskedPassword[MAX_INPUT_CHARS + 1];
        memset(maskedPassword, '*', letterCountPassword);
        maskedPassword[letterCountPassword] = '\0';

        // Cursor blinking logic
        cursorTimer += GetFrameTime();
        if (cursorTimer >= 0.5f) {
            showCursor = !showCursor; // Toggle cursor visibility
            cursorTimer = 0.0f;
        }

        // Draw UI
        BeginDrawing();
        ClearBackground(backgroundColor);

        // Draw "TrackHer" text centered
        DrawTextEx(customFont, "Track", (Vector2){centerX - 140, centerY - 150}, 60, 2, textColor);
        DrawTextEx(customFont, "Her", (Vector2){centerX + 30, centerY - 150}, 60, 2, placeholderColor);

        // Draw "login" text
        DrawTextEx(customFont, "login", (Vector2){centerX - 30, centerY - 40}, 24, 1, placeholderColor);

        // Draw input boxes
        DrawRectangleRec(usernameBox, inputBoxColor);
        DrawRectangleRec(passwordBox, inputBoxColor);

        // Highlight selected box
        if (usernameSelected) DrawRectangleLinesEx(usernameBox, 2, placeholderColor);
        if (passwordSelected) DrawRectangleLinesEx(passwordBox, 2, placeholderColor);

        // Draw text inside input boxes (centered)
        int usernameTextWidth = MeasureText(usernameSelected || letterCountUsername > 0 ? username : "enter your username", 20);
        int passwordTextWidth = MeasureText(passwordSelected || letterCountPassword > 0 ? maskedPassword : "enter your password", 20);

        DrawText(usernameSelected || letterCountUsername > 0 ? username : "enter your username",
                 centerX - usernameTextWidth / 2, centerY + 15, 20, placeholderColor);

        DrawText(passwordSelected || letterCountPassword > 0 ? maskedPassword : "enter your password",
                 centerX - passwordTextWidth / 2, centerY + 85, 20, placeholderColor);

        // Draw blinking cursor in username field
        if (usernameSelected && showCursor) {
            int cursorX = centerX - usernameTextWidth / 2 + MeasureText(username, 20);
            DrawText("|", cursorX, centerY + 15, 20, placeholderColor);
        }

        // Draw blinking cursor in password field
        if (passwordSelected && showCursor) {
            int cursorX = centerX - passwordTextWidth / 2 + MeasureText(maskedPassword, 20);
            DrawText("|", cursorX, centerY + 85, 20, placeholderColor);
        }

        // Draw Login Button
        DrawRectangleRec(loginButton, buttonColor);
        DrawText("Login", centerX - MeasureText("Login", 24) / 2, centerY + 165, 24, WHITE);

        // Draw Signup Button
        DrawRectangleRec(signupButton, buttonColor);
        DrawText("Sign Up", centerX - MeasureText("Sign Up", 24) / 2, centerY + 225, 24, WHITE);

        EndDrawing();

        // Handle login button click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePoint, loginButton)) {
            // Authenticate the user
            if (authenticate(username, password)) {
                // If authentication is successful, load period data and other backend logic
                work_choice();
            } else {
                printf("Invalid credentials.\n");
            }
        }

        // Handle signup button click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePoint, signupButton)) {
            // Call the signup function
            signup();
        }
    }

    CloseWindow();  // Close window when the loop exits
}

int main() {

    // Get today's date
    time_t t = time(NULL);
    struct tm today = *localtime(&t);
    strftime(today_str, sizeof(today_str), "%Y-%m-%d", &today);

    // Load from file
    load_last_period("data.txt", last_period, sizeof(last_period));
    cycle_length = average_cycle_length("data.txt");

    // Calculate next period
    calculate_next_period(last_period, cycle_length, next_period);

    // Fertile Window
    calculate_fertile_window(next_period, fertile_start, fertile_end);

    draw_login_screen();  // Call the function to handle login/signup screen drawing and interactions
    return 0;
}