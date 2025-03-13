#include <gtk/gtk.h>
#include "ui.h"
#include "backend.h"
#include "authenticate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "cycle_data.txt"


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv); // Initialize GTK

    signuptrial(argc, argv);
    gtk_main(); // Start GTK event loop
    
    return 0;
}
