#include <gtk/gtk.h>
#include "backend.h"
#include "login.h"

// Global widgets
GtkWidget *window;
GtkWidget *calendar;
GtkWidget *entry_cycle_length;
GtkWidget *label_next_period;
GtkWidget *label_fertile_window;
GtkWidget *text_view_history;
GtkWidget *username_entry;
GtkWidget *password_entry;


// Function to handle "Calculate" button
void on_Log_period_clicked(GtkWidget *widget, gpointer data) {
    guint year, month, day;
    gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
    month += 1; // Adjust month (GTK Calendar month is 0-based)
    char last_period[11];
    snprintf(last_period, sizeof(last_period), "%04d-%02d-%02d", year, month, day);

    int cycle_length = atoi(gtk_entry_get_text(GTK_ENTRY(entry_cycle_length)));
    if (cycle_length < 21 || cycle_length > 35) {
        gtk_label_set_text(GTK_LABEL(label_next_period), "Invalid cycle length!");
        return;
    }

    char next_period[20], fertile_start[20], fertile_end[20];
    calculate_next_period(last_period, cycle_length, next_period);
    calculate_fertile_window(next_period, fertile_start, fertile_end);

    gtk_label_set_text(GTK_LABEL(label_next_period), next_period);
    gtk_label_set_text(GTK_LABEL(label_fertile_window), fertile_start);

    char cycle_data[100];
    snprintf(cycle_data, sizeof(cycle_data), "Last Period: %s, Cycle Length: %d, Next Period: %s", last_period, cycle_length, next_period);
    save_cycle_data("cycle_data.txt", cycle_data);
}

// Function to handle "View History" button
void on_view_history_clicked(GtkWidget *widget, gpointer data) {
    char buffer[1024] = {0};
    load_cycle_data("cycle_data.txt", buffer, sizeof(buffer));
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_history));
    gtk_text_buffer_set_text(text_buffer, buffer, -1);
}

// Function to create UI
void create_ui() 
{
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Period Tracker");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    
    GtkWidget *label1 = gtk_label_new("Select Last Period:");
    calendar = gtk_calendar_new();
    
    GtkWidget *label2 = gtk_label_new("Cycle Length (days):");
    entry_cycle_length = gtk_entry_new();
    
    GtkWidget *button_Log_period = gtk_button_new_with_label("Log period");
    g_signal_connect(button_Log_period, "clicked", G_CALLBACK(on_Log_period_clicked), NULL);
    
    GtkWidget *button_history = gtk_button_new_with_label("View History");
    g_signal_connect(button_history, "clicked", G_CALLBACK(on_view_history_clicked), NULL);
    
    label_next_period = gtk_label_new("Next period date: ");
    label_fertile_window = gtk_label_new("Fertile window: ");
    
    text_view_history = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view_history), FALSE);
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), text_view_history);
    gtk_widget_set_size_request(scroll, 350, 100);
    
    // Adding widgets to grid
    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), calendar, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_cycle_length, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_Log_period, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), label_next_period, 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), label_fertile_window, 0, 4, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_history, 0, 5, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), scroll, 0, 6, 2, 1);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
}

//log in ko lagi
void on_login_clicked(GtkWidget *widget, gpointer data) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    if (authenticate((char *)username, (char *)password)) {
        gtk_label_set_text(GTK_LABEL(data), "Login Successful! Welcome to TrackHer.");
        create_ui(); // Launch UI

    } else {
        gtk_label_set_text(GTK_LABEL(data), "Invalid username or password.");
    }
}

void on_signup_clicked(GtkWidget *widget, gpointer data) {
    signup(); // Calls the signup function from login.c
    gtk_label_set_text(GTK_LABEL(data), "Signup Completed! Please login.");
}

void show_login_window() 
{
    // Create window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "TrackHer Login");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a grid layout
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Username label & entry
    GtkWidget *username_label = gtk_label_new("Username:");
    username_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), username_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), username_entry, 1, 0, 1, 1);

    // Password label & entry
    GtkWidget *password_label = gtk_label_new("Password:");
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); // Hide password
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 1, 1, 1);

    // Status label
    GtkWidget *status_label = gtk_label_new("");

    // Login button
    GtkWidget *login_button = gtk_button_new_with_label("Login");
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), status_label);
    gtk_grid_attach(GTK_GRID(grid), login_button, 0, 2, 2, 1);

    // Signup button
    GtkWidget *signup_button = gtk_button_new_with_label("Signup");
    g_signal_connect(signup_button, "clicked", G_CALLBACK(on_signup_clicked), status_label);
    gtk_grid_attach(GTK_GRID(grid), signup_button, 0, 3, 2, 1);

    // Attach status label
    gtk_grid_attach(GTK_GRID(grid), status_label, 0, 4, 2, 1);

    // Show everything
    gtk_widget_show_all(window);
}


