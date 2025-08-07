#ifndef SETTINGS_H
#define SETTINGS_H 

#include <gtk/gtk.h>
#include "structs.h"
#include "userMenu.h"

void initializeSettings(GtkWidget *stack, ST_APPLICATION *application);
void addGeneralToGrid(GtkWidget *grid, ST_APPLICATION *application);
void addAppearanceToGrid(GtkWidget *grid);

#endif
