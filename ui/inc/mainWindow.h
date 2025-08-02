#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// BIBLIOTECAS
#include <gtk/gtk.h>
#include "utils.h" 
#include "leftbar.h"
#include "paths.h"
#include "dashboard.h"
#include "ui_clients.h"
#include "ui_doctors.h"
#include "ui_appointments.h"
#include "structs.h"
#include "settings.h"

// PROTÓTIPOS DAS FUNÇÕES MENUS
void createMainWindow(GtkApplication *app, gpointer data);

#endif
