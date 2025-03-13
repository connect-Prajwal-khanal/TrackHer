#include <gtk/gtk.h>

// Callback function for logging a cycle
static void on_log_cycle_clicked(GtkWidget *widget, gpointer data) {
    g_print("Log Cycle button clicked.\n");
}

// Callback function for viewing predictions
static void on_view_predictions_clicked(GtkWidget *widget, gpointer data) {
    g_print("View Predictions button clicked.\n");
}

// Main function to create the TrackHer UI
int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *header_label;
    GtkWidget *calendar;
    GtkWidget *log_cycle_button;
    GtkWidget *view_predictions_button;

    gtk_init(&argc, &argv);

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "TrackHer - Menstrual Cycle Tracker");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a grid layout
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Add a header label
    header_label = gtk_label_new("Welcome to TrackHer");
    gtk_widget_set_halign(header_label, GTK_ALIGN_CENTER);  // Center-align the header
    gtk_grid_attach(GTK_GRID(grid), header_label, 0, 0, 2, 1);  // Span 2 columns

    // Add a calendar widget
    calendar = gtk_calendar_new();
    gtk_grid_attach(GTK_GRID(grid), calendar, 0, 1, 2, 1);

    // Add a "Log Cycle" button
    log_cycle_button = gtk_button_new_with_label("Log Cycle");
    gtk_grid_attach(GTK_GRID(grid), log_cycle_button, 0, 2, 1, 1);
    g_signal_connect(log_cycle_button, "clicked", G_CALLBACK(on_log_cycle_clicked), NULL);

    // Add a "View Predictions" button
    view_predictions_button = gtk_button_new_with_label("View Predictions");
    gtk_grid_attach(GTK_GRID(grid), view_predictions_button, 1, 2, 1, 1);
    g_signal_connect(view_predictions_button, "clicked", G_CALLBACK(on_view_predictions_clicked), NULL);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
