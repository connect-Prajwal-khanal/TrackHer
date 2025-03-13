#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    gtk_init(&argc,&argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"TrackHer");
    gtk_window_set_default_size(GTK_WINDOW(window),1200,900);
    g_signal_connect(window)
}
