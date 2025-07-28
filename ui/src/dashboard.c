#include "dashboard.h"

void initializeDashboard(GtkWidget *stack, ST_APPLICATION *application) {
  GtkWidget *rigth_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_stack_add_named(GTK_STACK(stack), rigth_box, "dashboard");
  gtk_widget_add_css_class(rigth_box, "box");
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "dashboard");

  GtkWidget *rigth_top_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_css_class(rigth_top_box, "rigth_top_box");
  gtk_widget_set_size_request(rigth_top_box, -1, 60);
  gtk_box_append(GTK_BOX(rigth_box), rigth_top_box);
  
  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_hexpand(spacer, true);
  gtk_box_append(GTK_BOX(rigth_top_box), spacer);
  
  initializeUserMenu(rigth_top_box, application, "dashboard");
 
  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
  gtk_widget_set_margin_bottom(grid, 30);
  gtk_box_append(GTK_BOX(rigth_box), grid);
  
  addCardsToGrid(grid, application);
  addPieChartToGrid(grid, application);
  addBarChartToGrid(grid, application);
}

void addCardsToGrid(GtkWidget *grid, ST_APPLICATION *application) {
  
  char total_doctors[16];
  snprintf(total_doctors, sizeof(total_doctors), "%d", numberOf(application->doctors, TYPE_DOCTORS));

  ST_MEDICO *doctors_active = NULL;
  int counter = obterListaMedicosAtivos(application->doctors, &doctors_active);
  char available_doctors[30];
  snprintf(available_doctors, sizeof(available_doctors), "%d currently available", counter);

  char total_clients[16];
  snprintf(total_clients, sizeof(total_clients), "%d", numberOf(application->clients, TYPE_CLIENTS));

  ST_CLIENTE *clients_active = NULL;
  counter = obterListaClientesAtivos(application->clients, &clients_active);
  char available_clients[30];
  snprintf(available_clients, sizeof(available_clients), "%d currently active", counter);
  
  ST_CONSULTA *appointments_scheduled = NULL;
  counter = obterListaConsultasAgendadas(application->appointments, &appointments_scheduled);
  char scheduled_appointments[16];
  snprintf(scheduled_appointments, sizeof(scheduled_appointments), "%d", counter);
  
  ST_DATA date;
  dataAtual(&date);
  char data[11];
  snprintf(data, sizeof(data), "%02u-%02u-%04u", date.dia, date.mes, date.ano);

  ST_CONSULTA *appointments_today = NULL;
  counter = procurarConsultasData(appointments_scheduled, &appointments_today, data);
  char today_appointments[16];
  snprintf(today_appointments, sizeof(today_appointments), "%d today", counter);
  
  GtkStringList *list = loadSpecialty();
  char specialties[16];
  snprintf(specialties, sizeof(specialties), "%d", g_list_model_get_n_items(G_LIST_MODEL(list)));

  const char *image[] = {
    DOCTOR_CARD_PATH,
    CLIENT_CARD_PATH,
    APPOINTMENT_CARD_PATH,
    SPECIALTY_CARD_PATH
  };

  const char *title[] = {
    "Doctors Registered",
    "Clients",
    "Scheduled Appointments",
    "Active Specialties"
  };

  const char *info[] = {
    total_doctors,
    total_clients,
    scheduled_appointments,
    specialties
  };

  const char *subtitle[] = {
    available_doctors,
    available_clients,
    today_appointments,
    NULL
  };

  for (int i = 0; i < 4; i++) {
    GtkWidget *card_box = createAnalyticCard(i, image[i], title[i], info[i], subtitle[i]);
    gtk_grid_attach(GTK_GRID(grid), card_box, i, 0, 1, 1);
  }
}


void addPieChartToGrid(GtkWidget *grid, ST_APPLICATION *application) {
  ST_CONSULTA *appointments = application->appointments;
  
  GtkChart *chart; 
  chart = GTK_CHART(gtk_chart_new());
  gtk_chart_set_type(chart, GTK_CHART_TYPE_PIE);
  gtk_chart_set_font(GTK_CHART(chart), "Sans");
  gtk_chart_set_font_size(GTK_CHART(chart), 14);
  
  int counter = 0;
  for(int i = 0; i < numberOf(appointments, TYPE_APPOINTMENTS); i++) {
    if(appointments[i].estado == Agendado) {
      counter++;
    }
  }

  char scheduled[30];
  snprintf(scheduled, sizeof(scheduled), "Scheduled: %d", counter);
  gtk_chart_add_slice(GTK_CHART(chart), (double)counter, scheduled, "#F1C40F");
  
  counter = 0;
  for(int i = 0; i < numberOf(appointments, TYPE_APPOINTMENTS); i++) {
    if(appointments[i].estado == Realizado) {
      counter++;
    }
  }

  char completed[30];
  snprintf(completed, sizeof(completed), "Completed: %d", counter);
  gtk_chart_add_slice(GTK_CHART(chart), (double)counter, completed, "#2ECC71");

  counter = 0;
  for(int i = 0; i < numberOf(appointments, TYPE_APPOINTMENTS); i++) {
    if(appointments[i].estado == Cancelado) {
      counter++;
    }
  }

  char canceled[30];
  snprintf(canceled, sizeof(canceled), "Canceled: %d", counter);
  gtk_chart_add_slice(GTK_CHART(chart), (double)counter, canceled, "#E74C3C");
  
  gtk_widget_set_margin_top(GTK_WIDGET(chart), 40);
  gtk_widget_set_size_request(GTK_WIDGET(chart), 180, 240);
 
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_size_request(box, 150, 350);
  gtk_widget_add_css_class(box, "box-white");
  gtk_grid_attach(GTK_GRID(grid), box, 0, 1, 2, 4);
  
  GtkWidget *box_horizontal = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_box_append(GTK_BOX(box), box_horizontal);
  
  GtkWidget *image_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_halign(image_box, GTK_ALIGN_START);
  gtk_widget_set_hexpand(image_box, false);
  gtk_widget_set_size_request(image_box, 40, 40);
  gtk_widget_add_css_class(image_box, "box-image-cyan");
  gtk_box_append(GTK_BOX(box_horizontal), image_box);
  
  GtkWidget *image = gtk_image_new_from_file(PIE_CHART_PATH);
  gtk_widget_set_size_request(image, 30, 30);
  gtk_widget_set_margin_top(image, 5);
  gtk_widget_set_valign(image, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(image_box), image);

  GtkWidget *label = gtk_label_new("Appointments Status");
  gtk_widget_add_css_class(label, "box-card-title");
  gtk_box_append(GTK_BOX(box_horizontal), label);
 
  gtk_box_append(GTK_BOX(box), GTK_WIDGET(chart));
}

void addBarChartToGrid(GtkWidget *grid, ST_APPLICATION *application) {
  ST_CONSULTA *appointments = application->appointments;
  
  GtkChart *chart = GTK_CHART(gtk_chart_new());
  gtk_chart_set_type(chart, GTK_CHART_TYPE_COLUMN);
  gtk_chart_set_column_ticks(chart, 1);
  gtk_chart_set_font(GTK_CHART(chart), "Sans");
  gtk_chart_set_font_size(GTK_CHART(chart), 14);
  
  ST_DATA date;
  dataAtual(&date);
  
  struct tm tm = {0};
  tm.tm_mday = date.dia;
  tm.tm_mon = date.mes - 1;
  tm.tm_year = date.ano - 1900;
  mktime(&tm);
  
  // Get the first day of the week
  while(tm.tm_wday != 0) {
    tm.tm_mday -= 1;
    mktime(&tm); // For example the date: 01-01-2025 will turn into 31-12-2024.
  }
  
  const char *colors[] = {
    "#3498DB",
    "#2ECC71",
    "#F1C40F",
    "#E67E22",
    "#E74C3C",
    "#9B59B6",
    "#95A5A6"
  };

  const char *week[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
  };

  for(int i = 0; i < 7; i++) {
    mktime(&tm);
    
    char data[11];
    snprintf(data, sizeof(data), "%02u-%02u-%04u", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    
    ST_CONSULTA *appointments_scheduled = NULL;
    int counter = procurarConsultasData(appointments, &appointments_scheduled, data);
    gtk_chart_add_column(GTK_CHART(chart), (double)counter, week[i], colors[i]);

    tm.tm_mday += 1;
  }
  
  double max_value = gtk_chart_get_column_max_value(GTK_CHART(chart));
  if(((int)max_value % 2) == 0) {
    if (max_value >= 4) {
      gtk_chart_set_column_ticks(GTK_CHART(chart), 4);
    }
  }else {
    if(max_value >= 4) {
      gtk_chart_set_column_ticks(GTK_CHART(chart), 5);
    }
  }
  
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_add_css_class(box, "box-white");
  gtk_grid_attach(GTK_GRID(grid), box, 2, 1, 2, 4);
  
  GtkWidget *box_horizontal = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_box_append(GTK_BOX(box), box_horizontal);

  GtkWidget *image_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_halign(image_box, GTK_ALIGN_START);
  gtk_widget_set_hexpand(image_box, false);
  gtk_widget_set_size_request(image_box, 40, 40);
  gtk_widget_add_css_class(image_box, "box-image-purple");
  gtk_box_append(GTK_BOX(box_horizontal), image_box);
  
  GtkWidget *image = gtk_image_new_from_file(BAR_CHART_PATH);
  gtk_widget_set_size_request(image, 30, 30);
  gtk_widget_set_margin_top(image, 5);
  gtk_widget_set_valign(image, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(image_box), image);
   
  GtkWidget *label = gtk_label_new("Appointments This Week");
  gtk_widget_add_css_class(label, "box-card-title");
  gtk_box_append(GTK_BOX(box_horizontal), label);
  
  gtk_widget_set_vexpand(GTK_WIDGET(chart), true);
  gtk_widget_set_size_request(GTK_WIDGET(chart), 200, 200);
  gtk_widget_set_margin_bottom(GTK_WIDGET(chart), 50);
  gtk_box_append(GTK_BOX(box), GTK_WIDGET(chart));
}
