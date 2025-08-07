#include "ui_appointments.h"

// CALLBACKS
static void clickedButtonAdd(GtkButton *button, gpointer data);
static void clickedButtonEdit(GtkButton *button, gpointer data);
static void clickedButtonToggle(GtkButton *button, gpointer data);
static void clickedButtonView(GtkButton *button, gpointer data);

static void clickedButtonBack(GtkButton *button, gpointer data);
static void clickedButtonSubmitAdd(GtkButton *button, gpointer data);
static void clickedButtonSubmitEdit(GtkButton *button, gpointer data);
static void clickedButtonSubmitToggle(GtkButton *button, gpointer data);

static void changedSearchAppointment(GtkSearchEntry *search_entry, gpointer data);
static void changedSearchViewAppointment(GtkSearchEntry *search_entry, gpointer data); 

static void activateSearchEditAppointment(GtkSearchEntry *search_entry, gpointer data);
static void activateSearchToggleAppointment(GtkSearchEntry *search_entry, gpointer data);

static void toggledButton(GtkToggleButton *toggle, gpointer data);

static void changedEntryClientID(GtkEntry *entry, gpointer data);
static void changedEntryClientNIF(GtkEntry *entry, gpointer data);
static void changedEntryDoctorID(GtkEntry *entry, gpointer data);
static void changedEntryDoctorLicense(GtkEntry *entry, gpointer data);
static void changedEntryStartDate(GtkEntry *entry, gpointer data);
static void changedDropdownStartHour(GtkDropDown *dropdown, GParamSpec *pspec, gpointer data);

/** 
 *  @brief Initializes the interface for the appointments. 
 *
 *  @param stack      A pointer to the stack widget used to manage different UI pages.
 *  @param doctors    Pointer to the ST_CONSULTA struct.
 *
 */
void initializeUIAppointments(GtkWidget *stack, ST_APPLICATION *application) {
  GtkWidget *rigth_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
  gtk_widget_add_css_class(rigth_box, "box");
  gtk_stack_add_named(GTK_STACK(stack), rigth_box, "appointments");
  
  GtkWidget *rigth_top_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_css_class(rigth_top_box, "rigth_top_box");
  gtk_widget_set_size_request(rigth_top_box, -1, 60);
  gtk_box_append(GTK_BOX(rigth_box), rigth_top_box);
  
  GtkWidget *search_entry = gtk_search_entry_new();
  gtk_search_entry_set_placeholder_text(GTK_SEARCH_ENTRY(search_entry), "Search for appointments");
  gtk_widget_add_css_class(search_entry, "search-entry");
  gtk_widget_set_hexpand(search_entry, true);
  gtk_box_append(GTK_BOX(rigth_top_box), search_entry);
  g_signal_connect(search_entry, "search-changed", G_CALLBACK(changedSearchAppointment), application);
  
  initializeUserMenu(rigth_top_box, application, "appointments");
  
  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(spacer, -1, 10);
  gtk_box_append(GTK_BOX(rigth_box), spacer);
  
  GtkWidget *label = gtk_label_new("");
  gtk_widget_add_css_class(label, "label-error");
  gtk_widget_set_visible(label, false);
  g_object_set_data(G_OBJECT(rigth_box), "label-error", label);
  gtk_box_append(GTK_BOX(spacer), label);
  
  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(rigth_box), grid);

  addAppointmentButtonsToGrid(grid, application);

  GtkWidget *scrolled = gtk_scrolled_window_new();
  gtk_widget_set_vexpand(scrolled, true);
  g_object_set_data(G_OBJECT(rigth_box), "Scrolled", scrolled);
  gtk_box_append(GTK_BOX(rigth_box), scrolled);

  ST_CONSULTA *appointments_scheduled = NULL;
  int counter = obterListaConsultasAgendadas(application->appointments, &appointments_scheduled);
  if(!appointments_scheduled) return;
  grid = createAppointmentTable(appointments_scheduled, counter);
  g_object_set_data(G_OBJECT(rigth_box), "AppointmentTable", grid);
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), grid);
}

/** 
 * @brief Creates and attaches the buttons to a specific position within a GtkGrid.
 *
 * This function helps reduce code duplication and encapsulates the creation and configuration of buttons.
 *
 * @param GtkWidget      GtkGrid widget where the buttons will be placed.
 * @param appointments   Pointer to the ST_CONSULTA struct. 
 *
 */
void addAppointmentButtonsToGrid(GtkWidget *grid, ST_APPLICATION *application) {
  ST_BUTTON button;
  
  const char *labels[] = {
    "Add", "Edit", "Toggle", "View"
  };

  const char *paths[] = {
    ADD_APPOINTMENT_PATH, 
    EDIT_APPOINTMENT_PATH, 
    TOGGLE_APPOINTMENT_PATH, 
    VIEW_APPOINTMENT_PATH
  };
  
  for (int i = 0; i < 4; i++) {
    createButtonWithImageLabel(&button, paths[i], labels[i], BUTTON_ORIENTATION_VERTICAL, BUTTON_POSITION_FIRST_IMAGE);
    gtk_widget_set_size_request(button.button, 96, 96);
    gtk_widget_set_size_request(button.image, 48, 48);
    gtk_widget_add_css_class(button.button, "button");
    gtk_widget_add_css_class(button.label, "button-label");
    gtk_grid_attach(GTK_GRID(grid), button.button, i, 0, 1, 1);

    switch(i) {
      case 0:
        g_signal_connect(button.button, "clicked", G_CALLBACK(clickedButtonAdd), application);
        break;
      case 1:
        g_signal_connect(button.button, "clicked", G_CALLBACK(clickedButtonEdit), application);
        break;
      case 2:
        g_signal_connect(button.button, "clicked", G_CALLBACK(clickedButtonToggle), application);
        break;
      case 3:
        g_signal_connect(button.button, "clicked", G_CALLBACK(clickedButtonView), application);
        break;
    }
  }
}

/** 
  * @brief Creates a table to display the information for all the appointments.
  *
  * @param appointments     Pointer to the ST_CONSULTA struct. 
  * @param n_appointments   Total number of appointments.
  *
  * @return GtkWidget   GtkGrid widget which represents the table.
  *
  */
GtkWidget *createAppointmentTable(ST_CONSULTA *appointments, int n_appointments) {
  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 14);

  const char *headers[] = {"ID", "Client", "Doctor", "Specialty", "Start Date", "End Date", "Status" };
  for (int i = 0; i < 7; i++) {
    GtkWidget *label = gtk_label_new(headers[i]);
    gtk_widget_add_css_class(label, "header-label");
    gtk_grid_attach(GTK_GRID(grid), label, i, 0, 1, 1);
  }

  for (int i = 0; i < n_appointments; i++) {
    ST_CONSULTA j = appointments[i];
    
    char id[5];
    snprintf(id, sizeof(id), "%u", j.ID);

    char start_date[20];
    snprintf(start_date, sizeof(start_date), "%02u-%02u-%04u %02u:00", j.data_inicial.dia, j.data_inicial.mes, j.data_inicial.ano, j.data_inicial.hora);

    char end_date[20];
    snprintf(end_date, sizeof(end_date), "%02u-%02u-%04u %02u:00", j.data_final.dia, j.data_final.mes, j.data_final.ano, j.data_final.hora);
    

    char status[10];
    if (j.estado == Cancelado) {
      strcpy(status,  "🔴");
    } else if (j.estado == Agendado) {
      strcpy(status, "🟡");
    } else if(j.estado == Realizado) {
      strcpy(status, "🟢");
    }
    
    GtkWidget *labels[] = {
      gtk_label_new(id),
      gtk_label_new(j.cliente->nome),
      gtk_label_new(j.medico->nome),
      gtk_label_new(j.medico->especialidade),
      gtk_label_new(start_date),
      gtk_label_new(end_date),
      gtk_label_new(status)
    };

    for (int z = 0; z < 7; z++) {
      gtk_widget_add_css_class(labels[z], "label");
      gtk_grid_attach(GTK_GRID(grid), labels[z], z, i + 1, 1, 1);
      if(z < 5) gtk_label_set_selectable(GTK_LABEL(labels[z]), true);  // get selectable all the labels except the status (emoji).
    }
  }
  
  return grid;
}

static void clickedButtonAdd(GtkButton *button, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;
  
  ST_CONSULTA *appointments = application->appointments;

  if(numberOf(appointments, TYPE_APPOINTMENTS) >= MAX_CONSULTAS) return; 
  
  GtkWidget *stack = gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_STACK);
  if(!stack) {
    return;
  }

  GtkWidget *rigth_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_add_css_class(rigth_box, "box");
  gtk_stack_add_named(GTK_STACK(stack), rigth_box, "AddAppointments");
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "AddAppointments");

  GtkWidget *rigth_top_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_css_class(rigth_top_box, "rigth_top_box");
  gtk_widget_set_size_request(rigth_top_box, -1, 60);
  gtk_box_append(GTK_BOX(rigth_box), rigth_top_box);
 
  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_hexpand(spacer, true);
  gtk_box_append(GTK_BOX(rigth_top_box), spacer);
  
  initializeUserMenu(rigth_top_box, application, NULL);

  spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(spacer, -1, 30);
  gtk_box_append(GTK_BOX(rigth_box), spacer);

  ST_BUTTON btn; 
  createButtonWithImageLabel(&btn, BACK_ICON_PATH,"BACK", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_IMAGE);  
  gtk_widget_add_css_class(btn.button, "back-button");
  gtk_widget_add_css_class(btn.label, "back-button-label");
  gtk_widget_set_size_request(btn.button, 25, 25);
  gtk_widget_set_halign(btn.button, GTK_ALIGN_START);
  gtk_widget_set_hexpand(btn.button, false);
  gtk_box_append(GTK_BOX(rigth_box), btn.button);
  g_signal_connect(btn.button, "clicked", G_CALLBACK(clickedButtonBack), stack);
  
  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 14);
  gtk_widget_set_hexpand(grid, true);
  gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(rigth_box), grid);
  
  GtkWidget *label = gtk_label_new("ID");
  gtk_widget_set_hexpand(label, true);
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

  GtkWidget *entry = gtk_entry_new();
  gtk_widget_set_hexpand(entry, true);
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  g_object_set_data(G_OBJECT(rigth_box), "ID", entry);
  gtk_widget_add_css_class(entry, "form-entry-disabled");

  char id[15];
  snprintf(id, sizeof(id), "%d", numberOf(application->appointments, TYPE_APPOINTMENTS) + 1);

  GtkEntryBuffer *buffer = gtk_entry_buffer_new(id, -1);
  gtk_entry_set_buffer(GTK_ENTRY(entry), buffer);
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 0, 1, 1);
  
  label = gtk_label_new("Client ID");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
  
  entry = gtk_entry_new();
  gtk_entry_set_input_purpose(GTK_ENTRY(entry), GTK_INPUT_PURPOSE_DIGITS);
  gtk_widget_set_hexpand(entry, true);
  g_object_set_data(G_OBJECT(rigth_box), "ClientID", entry);
  gtk_widget_add_css_class(entry, "form-entry");
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 1, 1, 1);
  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryClientID), application->clients);
    
  label = gtk_label_new("NIF");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 2, 1, 1, 1);
  
  entry = gtk_entry_new();
  gtk_entry_set_input_purpose(GTK_ENTRY(entry), GTK_INPUT_PURPOSE_DIGITS);
  gtk_entry_set_max_length(GTK_ENTRY(entry), 9);
  gtk_widget_add_css_class(entry, "form-entry");
  g_object_set_data(G_OBJECT(rigth_box), "ClientNIF", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 3, 1, 1, 1);
  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryClientNIF), application->clients);
  
  label = gtk_label_new("Name");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
  
  entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Name"); 
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "ClientName", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 2, 2, 1);  
  
  label = gtk_label_new("Doctor ID");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);
 
  entry = gtk_entry_new();
  gtk_entry_set_input_purpose(GTK_ENTRY(entry), GTK_INPUT_PURPOSE_DIGITS);
  gtk_widget_add_css_class(entry, "form-entry");
  g_object_set_data(G_OBJECT(rigth_box), "DoctorID", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 3, 1, 1);
  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryDoctorID), application->doctors);

  label = gtk_label_new("License Number");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 2, 3, 1, 1);
 
  entry = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(entry), 6);
  gtk_widget_add_css_class(entry, "form-entry");
  g_object_set_data(G_OBJECT(rigth_box), "LicenseNumber", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 3, 3, 1, 1);
  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryDoctorLicense), application->doctors);
  
  label = gtk_label_new("Name");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);
  
  entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Name");  
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "DoctorName", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 4, 2, 1);

  label = gtk_label_new("Specialty");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 3, 4, 1, 1);
  
  entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Pediatria");  
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "DoctorSpecialty", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 4, 4, 1, 1);
  
  label = gtk_label_new("Start Date");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 6, 1, 1);

  entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "01-01-2025");
  gtk_entry_set_max_length(GTK_ENTRY(entry), 10);
  gtk_widget_add_css_class(entry, "form-entry");
  g_object_set_data(G_OBJECT(rigth_box), "StartDate", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 6, 1, 1);  
  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryStartDate), application);
  
  const char *start_hour[] = {"10h00", NULL};
  GtkWidget *dropdown = gtk_drop_down_new_from_strings(start_hour);  
  gtk_widget_set_sensitive(dropdown, false);
  gtk_widget_set_hexpand(dropdown, true);
  gtk_widget_add_css_class(dropdown, "form-dropdown");
  g_object_set_data(G_OBJECT(rigth_box), "StartHour", dropdown);
  gtk_grid_attach(GTK_GRID(grid), dropdown, 2, 6, 1, 1);
  g_signal_connect(dropdown, "notify::selected", G_CALLBACK(changedDropdownStartHour), NULL);
  
  label = gtk_label_new("End Date");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 3, 6, 1, 1);

  entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "01-01-2025");
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_entry_set_max_length(GTK_ENTRY(entry), 10);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "EndDate", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 4, 6, 1, 1);  
 
  const char *end_hour[] = {"11h00", NULL};
  dropdown = gtk_drop_down_new_from_strings(end_hour);  
  gtk_widget_set_sensitive(dropdown, false);
  gtk_widget_set_hexpand(dropdown, true);
  gtk_widget_add_css_class(dropdown, "form-dropdown");
  g_object_set_data(G_OBJECT(rigth_box), "EndHour", dropdown);
  gtk_grid_attach(GTK_GRID(grid), dropdown, 5, 6, 1, 1);

  spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(spacer, -1, 60);
  gtk_box_append(GTK_BOX(rigth_box), spacer);
 
  label = gtk_label_new("");
  gtk_widget_add_css_class(label, "label-error");
  g_object_set_data(G_OBJECT(rigth_box), "label-error", label);
  gtk_widget_set_visible(label, false);
  gtk_box_append(GTK_BOX(rigth_box), label);
  
  createButtonWithImageLabel(&btn, SUBMIT_PATH,"SUBMIT", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_LABEL);  
  gtk_widget_add_css_class(btn.button, "submit-button");
  gtk_widget_add_css_class(btn.label, "submit-button-label");
  gtk_widget_set_size_request(btn.button, 40, 40);
  gtk_widget_set_halign(btn.button, GTK_ALIGN_CENTER);
  gtk_widget_set_hexpand(btn.button, false);
  gtk_box_append(GTK_BOX(rigth_box), btn.button);
  g_signal_connect(btn.button, "clicked", G_CALLBACK(clickedButtonSubmitAdd), application);
}

static void clickedButtonEdit(GtkButton *button, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;
  
  GtkWidget *stack = gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_STACK);
  if(!stack) {
    return;
  }

  GtkWidget *rigth_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_add_css_class(rigth_box, "box");
  gtk_stack_add_named(GTK_STACK(stack), rigth_box, "EditAppointments");
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "EditAppointments");

  GtkWidget *rigth_top_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_css_class(rigth_top_box, "rigth_top_box");
  gtk_widget_set_size_request(rigth_top_box, -1, 60);
  gtk_box_append(GTK_BOX(rigth_box), rigth_top_box);
 
  GtkWidget *search_entry = gtk_search_entry_new();
  gtk_search_entry_set_placeholder_text(GTK_SEARCH_ENTRY(search_entry), "Search by ID");
  gtk_widget_add_css_class(search_entry, "search-entry");
  gtk_widget_set_hexpand(search_entry, true);
  gtk_box_append(GTK_BOX(rigth_top_box), search_entry);
  g_signal_connect(search_entry, "activate", G_CALLBACK(activateSearchEditAppointment), application);

  initializeUserMenu(rigth_top_box, application, NULL);
  
  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(spacer, -1, 30);
  gtk_box_append(GTK_BOX(rigth_box), spacer);
  
  GtkWidget *label = gtk_label_new("");
  gtk_widget_add_css_class(label, "label-error");
  gtk_widget_set_visible(label, false);
  g_object_set_data(G_OBJECT(rigth_box), "label-error", label);
  gtk_box_append(GTK_BOX(spacer), label);
  
  ST_BUTTON btn; 
  createButtonWithImageLabel(&btn, BACK_ICON_PATH,"BACK", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_IMAGE);  
  gtk_widget_add_css_class(btn.button, "back-button");
  gtk_widget_add_css_class(btn.label, "back-button-label");
  gtk_widget_set_size_request(btn.button, 25, 25);
  gtk_widget_set_halign(btn.button, GTK_ALIGN_START);
  gtk_widget_set_hexpand(btn.button, false);
  gtk_box_append(GTK_BOX(rigth_box), btn.button);
  g_signal_connect(btn.button, "clicked", G_CALLBACK(clickedButtonBack), stack);
  
  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 14);
  gtk_widget_set_hexpand(grid, true);
  gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(rigth_box), grid);
  
  label = gtk_label_new("ID");
  gtk_widget_set_hexpand(label, true);
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

  GtkWidget *entry = gtk_entry_new();
  gtk_widget_set_hexpand(entry, true);
  gtk_widget_set_sensitive(entry, false);
  g_object_set_data(G_OBJECT(rigth_box), "ID", entry);
  gtk_widget_add_css_class(entry, "form-entry-disabled");  
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 0, 1, 1);

  label = gtk_label_new("Client ID");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
  
  entry = gtk_entry_new();
  gtk_entry_set_input_purpose(GTK_ENTRY(entry), GTK_INPUT_PURPOSE_DIGITS);
  gtk_widget_set_hexpand(entry, true);
  g_object_set_data(G_OBJECT(rigth_box), "ClientID", entry);
  gtk_widget_add_css_class(entry, "form-entry");
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 1, 1, 1);
  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryClientID), application->clients);
    
  label = gtk_label_new("NIF");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 2, 1, 1, 1);
  
  entry = gtk_entry_new();
  gtk_entry_set_input_purpose(GTK_ENTRY(entry), GTK_INPUT_PURPOSE_DIGITS);
  gtk_entry_set_max_length(GTK_ENTRY(entry), 9);
  gtk_widget_add_css_class(entry, "form-entry");
  g_object_set_data(G_OBJECT(rigth_box), "ClientNIF", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 3, 1, 1, 1);
  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryClientNIF), application->clients);
  
  label = gtk_label_new("Name");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
  
  entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Name"); 
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "ClientName", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 2, 2, 1);  
  
  label = gtk_label_new("Doctor ID");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);
 
  entry = gtk_entry_new();
  gtk_entry_set_input_purpose(GTK_ENTRY(entry), GTK_INPUT_PURPOSE_DIGITS);
  gtk_widget_add_css_class(entry, "form-entry");
  g_object_set_data(G_OBJECT(rigth_box), "DoctorID", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 3, 1, 1);
  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryDoctorID), application->doctors);

  label = gtk_label_new("License Number");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 2, 3, 1, 1);
 
  entry = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(entry), 6);
  gtk_widget_add_css_class(entry, "form-entry");
  g_object_set_data(G_OBJECT(rigth_box), "LicenseNumber", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 3, 3, 1, 1);
  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryDoctorLicense), application->doctors);
  
  label = gtk_label_new("Name");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);
  
  entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Name");  
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "DoctorName", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 4, 2, 1);

  label = gtk_label_new("Specialty");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 3, 4, 1, 1);
  
  entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Pediatria");  
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "DoctorSpecialty", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 4, 4, 1, 1);
  
  label = gtk_label_new("Start Date");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 6, 1, 1);

  entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "01-01-2025");
  gtk_entry_set_max_length(GTK_ENTRY(entry), 10);
  gtk_widget_add_css_class(entry, "form-entry");
  g_object_set_data(G_OBJECT(rigth_box), "StartDate", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 6, 1, 1);  
  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryStartDate), application);
  
  const char *start_hour[] = {"10h00", NULL};
  GtkWidget *dropdown = gtk_drop_down_new_from_strings(start_hour);  
  gtk_widget_set_sensitive(dropdown, false);
  gtk_widget_set_hexpand(dropdown, true);
  gtk_widget_add_css_class(dropdown, "form-dropdown");
  g_object_set_data(G_OBJECT(rigth_box), "StartHour", dropdown);
  gtk_grid_attach(GTK_GRID(grid), dropdown, 2, 6, 1, 1);
  g_signal_connect(dropdown, "notify::selected", G_CALLBACK(changedDropdownStartHour), NULL);
  
  label = gtk_label_new("End Date");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 3, 6, 1, 1);

  entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "01-01-2025");
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_entry_set_max_length(GTK_ENTRY(entry), 10);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "EndDate", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 4, 6, 1, 1);  
 
  const char *end_hour[] = {"11h00", NULL};
  dropdown = gtk_drop_down_new_from_strings(end_hour);  
  gtk_widget_set_sensitive(dropdown, false);
  gtk_widget_set_hexpand(dropdown, true);
  gtk_widget_add_css_class(dropdown, "form-dropdown");
  g_object_set_data(G_OBJECT(rigth_box), "EndHour", dropdown);
  gtk_grid_attach(GTK_GRID(grid), dropdown, 5, 6, 1, 1);

  spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(spacer, -1, 60);
  gtk_box_append(GTK_BOX(rigth_box), spacer);
 
  label = gtk_label_new("");
  gtk_widget_add_css_class(label, "label-error");
  g_object_set_data(G_OBJECT(rigth_box), "label-error", label);
  gtk_widget_set_visible(label, false);
  gtk_box_append(GTK_BOX(rigth_box), label);
  
  createButtonWithImageLabel(&btn, SUBMIT_PATH,"SUBMIT", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_LABEL);  
  gtk_widget_add_css_class(btn.button, "submit-button");
  gtk_widget_add_css_class(btn.label, "submit-button-label");
  gtk_widget_set_size_request(btn.button, 40, 40);
  gtk_widget_set_halign(btn.button, GTK_ALIGN_CENTER);
  gtk_widget_set_hexpand(btn.button, false);
  gtk_box_append(GTK_BOX(rigth_box), btn.button);
  g_signal_connect(btn.button, "clicked", G_CALLBACK(clickedButtonSubmitEdit), application);
}

static void clickedButtonToggle(GtkButton *button, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;

  ST_CONSULTA *appointments = application->appointments;

  GtkWidget *stack = gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_STACK);
  if(!stack) {
    return;
  }

  GtkWidget *rigth_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_add_css_class(rigth_box, "box");
  gtk_stack_add_named(GTK_STACK(stack), rigth_box, "ToggleAppointments");
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "ToggleAppointments");
 
  GtkWidget *rigth_top_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_css_class(rigth_top_box, "rigth_top_box");
  gtk_widget_set_size_request(rigth_top_box, -1, 60);
  gtk_box_append(GTK_BOX(rigth_box), rigth_top_box);
  
  GtkWidget *search_entry = gtk_search_entry_new();
  gtk_search_entry_set_placeholder_text(GTK_SEARCH_ENTRY(search_entry), "Search by ID.");
  gtk_widget_add_css_class(search_entry, "search-entry");
  gtk_widget_set_hexpand(search_entry, true);
  gtk_box_append(GTK_BOX(rigth_top_box), search_entry);
  g_signal_connect(search_entry, "activate", G_CALLBACK(activateSearchToggleAppointment), appointments);
  
  initializeUserMenu(rigth_top_box, application, NULL);
  
  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(spacer, -1, 30);
  gtk_box_append(GTK_BOX(rigth_box), spacer);

  GtkWidget *label = gtk_label_new("");
  gtk_widget_add_css_class(label, "label-error");
  gtk_widget_set_visible(label, false);
  g_object_set_data(G_OBJECT(rigth_box), "label-error", label);
  gtk_box_append(GTK_BOX(spacer), label);
 
  ST_BUTTON btn; 
  createButtonWithImageLabel(&btn, BACK_ICON_PATH,"BACK", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_IMAGE);  
  gtk_widget_add_css_class(btn.button, "back-button");
  gtk_widget_add_css_class(btn.label, "back-button-label");
  gtk_widget_set_size_request(btn.button, 25, 25);
  gtk_widget_set_halign(btn.button, GTK_ALIGN_START);
  gtk_widget_set_hexpand(btn.button, false);
  gtk_box_append(GTK_BOX(rigth_box), btn.button);
  g_signal_connect(btn.button, "clicked", G_CALLBACK(clickedButtonBack), stack);
  
  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 14);
  gtk_widget_set_halign(grid, GTK_ALIGN_START);
  gtk_widget_set_hexpand(grid, true);
  gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(rigth_box), grid);
  
  label = gtk_label_new("ID");
  gtk_widget_set_hexpand(label, true);
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

  GtkWidget *entry = gtk_entry_new();
  gtk_widget_set_hexpand(entry, true);
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  g_object_set_data(G_OBJECT(rigth_box), "ID", entry);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 0, 1, 1);
  
  label = gtk_label_new("Client ID");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);

  entry = gtk_entry_new();
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "ClientID", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 1, 1, 1);
  
  label = gtk_label_new("Name");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

  entry = gtk_entry_new();
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_entry_set_max_length(GTK_ENTRY(entry), 100);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "ClientName", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 2, 1, 1);
  
  label = gtk_label_new("NIF");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 2, 2, 1, 1);
 
  entry = gtk_entry_new();
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_entry_set_max_length(GTK_ENTRY(entry), 9);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "NIF", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 3, 2, 1, 1);
  
  label = gtk_label_new("Doctor ID");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);
 
  entry = gtk_entry_new();
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "DoctorID", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 3, 6, 1);
  
  label = gtk_label_new("Name");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);

  entry = gtk_entry_new();
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_entry_set_max_length(GTK_ENTRY(entry), 100);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "DoctorName", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 1, 4, 1, 1);
  
  label = gtk_label_new("License Number");
  gtk_widget_add_css_class(label, "form-label");
  gtk_grid_attach(GTK_GRID(grid), label, 2, 4, 1, 1);
 
  entry = gtk_entry_new();
  gtk_widget_set_sensitive(entry, false);
  gtk_editable_set_editable(GTK_EDITABLE(entry), false);
  gtk_entry_set_max_length(GTK_ENTRY(entry), 9);
  gtk_widget_add_css_class(entry, "form-entry-disabled");
  g_object_set_data(G_OBJECT(rigth_box), "LicenseNumber", entry);
  gtk_grid_attach(GTK_GRID(grid), entry, 4, 4, 1, 1);

  spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(spacer, -1, 80);
  gtk_box_append(GTK_BOX(rigth_box), spacer);

  GtkWidget *toggle_button = gtk_toggle_button_new_with_label("Scheduled");
  gtk_widget_add_css_class(toggle_button, "form-toggle");
  g_object_set_data(G_OBJECT(rigth_box), "status", toggle_button);
  gtk_widget_set_halign(toggle_button, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(spacer), toggle_button);
  g_signal_connect(toggle_button, "toggled", G_CALLBACK(toggledButton), NULL);

  createButtonWithImageLabel(&btn, SUBMIT_PATH,"SUBMIT", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_LABEL);  
  gtk_widget_add_css_class(btn.button, "submit-button");
  gtk_widget_add_css_class(btn.label, "submit-button-label");
  gtk_widget_set_size_request(btn.button, 40, 40);
  gtk_widget_set_halign(btn.button, GTK_ALIGN_CENTER);
  gtk_widget_set_hexpand(btn.button, false);
  gtk_box_append(GTK_BOX(rigth_box), btn.button);
  g_signal_connect(btn.button, "clicked", G_CALLBACK(clickedButtonSubmitToggle), application);
}

static void clickedButtonView(GtkButton *button, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;

  ST_CONSULTA *appointments = application->appointments;

  GtkWidget *stack = gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_STACK);
  if(!stack) {
    return;
  }

  GtkWidget *rigth_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_add_css_class(rigth_box, "box");
  gtk_stack_add_named(GTK_STACK(stack), rigth_box, "ViewAppointments");
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "ViewAppointments");
 
  GtkWidget *rigth_top_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_css_class(rigth_top_box, "rigth_top_box");
  gtk_widget_set_size_request(rigth_top_box, -1, 60);
  gtk_box_append(GTK_BOX(rigth_box), rigth_top_box);
  
  GtkWidget *search_entry = gtk_search_entry_new();
  gtk_search_entry_set_placeholder_text(GTK_SEARCH_ENTRY(search_entry), "Search for appointments");
  gtk_widget_add_css_class(search_entry, "search-entry");
  gtk_widget_set_hexpand(search_entry, true);
  gtk_box_append(GTK_BOX(rigth_top_box), search_entry);
  g_signal_connect(search_entry, "search-changed", G_CALLBACK(changedSearchViewAppointment), application);
  
  initializeUserMenu(rigth_top_box, application, NULL);
  
  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request(spacer, -1, 15);
  gtk_widget_set_halign(spacer, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(rigth_box), spacer);

  GtkWidget *label = gtk_label_new("");
  gtk_widget_add_css_class(label, "label-error");
  gtk_widget_set_visible(label, false);
  g_object_set_data(G_OBJECT(rigth_box), "label-error", label);
  gtk_box_append(GTK_BOX(spacer), label);

  ST_BUTTON btn; 
  createButtonWithImageLabel(&btn, BACK_ICON_PATH,"BACK", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_IMAGE);  
  gtk_widget_add_css_class(btn.button, "back-button");
  gtk_widget_add_css_class(btn.label, "back-button-label");
  gtk_widget_set_size_request(btn.button, 25, 25);
  gtk_widget_set_halign(btn.button, GTK_ALIGN_START);
  gtk_widget_set_hexpand(btn.button, false);
  gtk_box_append(GTK_BOX(rigth_box), btn.button);
  g_signal_connect(btn.button, "clicked", G_CALLBACK(clickedButtonBack), stack);
  
  GtkWidget *scrolled = gtk_scrolled_window_new();
  g_object_set_data(G_OBJECT(rigth_box), "Scrolled", scrolled);
  gtk_widget_set_vexpand(scrolled, true);
  gtk_box_append(GTK_BOX(rigth_box), scrolled);
  
  GtkWidget *grid = createAppointmentTable(appointments, numberOf(appointments, TYPE_APPOINTMENTS));
  g_object_set_data(G_OBJECT(rigth_box), "AppointmentTable", grid);
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), grid);
}

static void clickedButtonBack(GtkButton *button, gpointer data) {
  (void)button; // unused 

  GtkWidget *stack = (GtkWidget *)data;
  
  
  GtkWidget *child = gtk_stack_get_child_by_name(GTK_STACK(stack), "AddAppointments");
  if(child) {
    gtk_stack_remove(GTK_STACK(stack), child);
  }
   
  child = gtk_stack_get_child_by_name(GTK_STACK(stack), "EditAppointments");
  if(child) {
    gtk_stack_remove(GTK_STACK(stack), child);
  }

  child = gtk_stack_get_child_by_name(GTK_STACK(stack), "ToggleAppointments");
  if(child) {
    gtk_stack_remove(GTK_STACK(stack), child);
  }
 
  child = gtk_stack_get_child_by_name(GTK_STACK(stack), "ViewAppointments");
  if(child) {
    gtk_stack_remove(GTK_STACK(stack), child);
  }

  gtk_stack_set_visible_child_name(GTK_STACK(stack), "appointments");
}

static void changedSearchAppointment(GtkSearchEntry *search_entry, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;
  
  ST_CLIENTE *clients = application->clients;
  ST_MEDICO *doctors = application->doctors;
  ST_CONSULTA *appointments = application->appointments;

  ST_CONSULTA *appointments_scheduled = NULL;
  int counter = obterListaConsultasAgendadas(appointments, &appointments_scheduled);
  appointments_scheduled[counter].ID = 0; // Adds a terminator to mark the end of array.

  ST_CONSULTA *appointments_found = NULL;
  ST_CLIENTE *clients_found = NULL;
  ST_MEDICO *doctors_found = NULL;
  
  GtkWidget *rigth_top_box = gtk_widget_get_parent(GTK_WIDGET(search_entry));
  GtkWidget *rigth_box = gtk_widget_get_parent(GTK_WIDGET(rigth_top_box));
  
  GtkWidget *label = g_object_get_data(G_OBJECT(rigth_box), "label-error");
  gtk_label_set_text(GTK_LABEL(label), "");
  gtk_widget_set_visible(label, false);
  
  const char *input = gtk_editable_get_text(GTK_EDITABLE(search_entry));
  if(strlen(input) == 0) {
    GtkWidget *grid = g_object_get_data(G_OBJECT(rigth_box), "AppointmentTable");
    GtkWidget *scrolled = g_object_get_data(G_OBJECT(rigth_box), "Scrolled");
    gtk_box_remove(GTK_BOX(rigth_box), scrolled);
    
    scrolled = gtk_scrolled_window_new();
    g_object_set_data(G_OBJECT(rigth_box), "Scrolled", scrolled);
    gtk_widget_set_vexpand(scrolled, true);
    gtk_box_append(GTK_BOX(rigth_box), scrolled);
  
    grid = createAppointmentTable(appointments_scheduled, counter);
    g_object_set_data(G_OBJECT(rigth_box), "AppointmentTable", grid);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), grid);
    return;
  }

  SEARCH_TYPE type = detectSearchType(input);
  
  switch(type) {
    case SEARCH_BY_ID:
      appointments_found = procurarConsultasID(appointments_scheduled, atoi(input));
      counter = 1; // unique identifier
      break;
    case SEARCH_BY_EMAIL:
      clients_found = procurarClientesEmail(clients, input);
      if(clients_found) {
        counter = procurarConsultasCliente(appointments_scheduled, &appointments_found, clients_found, 1);
      }else {
        doctors_found = procurarMedicosEmail(doctors, input);
        if(doctors_found) {
          counter = procurarConsultasMedico(appointments_scheduled, &appointments_found, doctors_found, 1);
        }
      }
      break;
    case SEARCH_BY_NIF:
      clients_found = procurarClientesNIF(clients, atoi(input));
      if(clients_found) {
        counter = procurarConsultasCliente(appointments_scheduled, &appointments_found, clients_found, 1);
      }
      break;
    case SEARCH_BY_SNS:
      clients_found = procurarClientesSNS(clients, atoi(input));
      if(clients_found) {
        counter = procurarConsultasCliente(appointments_scheduled, &appointments_found, clients_found, 1);
      }
      break;
    case SEARCH_BY_POSTAL_CODE:
      counter = procurarClientesCodigoPostal(clients, &clients_found, input);
      if(clients_found) {
        counter = procurarConsultasCliente(appointments_scheduled, &appointments_found, clients_found, numberOf(clients_found, TYPE_CLIENTS));
      }
      break;
    case SEARCH_BY_LICENSE_NUMBER:
      doctors_found = procurarMedicosLicenseNumber(doctors, atoi(input));
      if(doctors_found) {
        counter = procurarConsultasMedico(appointments_scheduled, &appointments_found, doctors_found, 1);
      }
      break;
    case SEARCH_BY_SPECIALITY:
      counter = procurarMedicosEspecialidade(doctors, &doctors_found, input);
      if(doctors_found) {
        counter = procurarConsultasMedico(appointments_scheduled, &appointments_found, doctors_found, numberOf(doctors_found, TYPE_DOCTORS));
      }
      break;
    case SEARCH_BY_DATE:
      counter = procurarConsultasData(appointments_scheduled, &appointments_found, input); 
      break;
    default:
      break;
  }

  if(appointments_found) {
    GtkWidget *grid = g_object_get_data(G_OBJECT(rigth_box), "AppointmentTable");
    GtkWidget *scrolled = g_object_get_data(G_OBJECT(rigth_box), "Scrolled");
    gtk_box_remove(GTK_BOX(rigth_box), scrolled);
    
    scrolled = gtk_scrolled_window_new();
    g_object_set_data(G_OBJECT(rigth_box), "Scrolled", scrolled);
    gtk_widget_set_vexpand(scrolled, true);
    gtk_box_append(GTK_BOX(rigth_box), scrolled);
   
    grid = createAppointmentTable(appointments_found, counter);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), grid); 
    g_object_set_data(G_OBJECT(rigth_box), "AppointmentTable", grid);
  }else {
    counter = procurarConsultasNome(appointments_scheduled, &appointments_found, input);
    if(counter > 0) {
      GtkWidget *grid = g_object_get_data(G_OBJECT(rigth_box), "AppointmentTable");
      GtkWidget *scrolled = g_object_get_data(G_OBJECT(rigth_box), "Scrolled");
      gtk_box_remove(GTK_BOX(rigth_box), scrolled);

      scrolled = gtk_scrolled_window_new();
      g_object_set_data(G_OBJECT(rigth_box), "Scrolled", scrolled);
      gtk_widget_set_vexpand(scrolled, true);
      gtk_box_append(GTK_BOX(rigth_box), scrolled);
   
      grid = createAppointmentTable(appointments_found, counter);
      gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
      gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), grid); 
      g_object_set_data(G_OBJECT(rigth_box), "AppointmentTable", grid);
    }else {
        label = g_object_get_data(G_OBJECT(rigth_box), "label-error");
        gtk_label_set_text(GTK_LABEL(label), "No appointment found with the provided input. Please check and try again.");
        gtk_widget_set_visible(label, true);
    }
  } 
}

static void clickedButtonSubmitAdd(GtkButton *button, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;
  
  ST_CLIENTE *clients = application->clients;
  ST_MEDICO *doctors = application->doctors;
  ST_CONSULTA *appointments = application->appointments;
  
  GtkWidget *rigth_box = gtk_widget_get_parent(GTK_WIDGET(button));

  ST_CLIENTE *clients_active = NULL;
  obterListaClientesAtivos(clients, &clients_active);
  
  GtkWidget *entry = g_object_get_data(G_OBJECT(rigth_box), "ClientID");
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *client_id = gtk_entry_buffer_get_text(buffer);
  if(detectSearchType(client_id) != SEARCH_BY_ID) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  ST_CLIENTE *client_found = procurarClientesID(clients_active, atoi(client_id));
  if(!client_found) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  entry = g_object_get_data(G_OBJECT(rigth_box), "ClientNIF");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *client_nif = gtk_entry_buffer_get_text(buffer);
  if(detectSearchType(client_nif) != SEARCH_BY_NIF) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");    
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  client_found = procurarClientesNIF(clients_active, atoi(client_nif));
  if(!client_found) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  ST_MEDICO *doctors_active = NULL;
  obterListaMedicosAtivos(doctors, &doctors_active);
  
  entry = g_object_get_data(G_OBJECT(rigth_box), "DoctorID");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *doctor_id = gtk_entry_buffer_get_text(buffer);
  if(detectSearchType(doctor_id) != SEARCH_BY_ID) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  ST_MEDICO *doctor_found = procurarMedicosID(doctors_active, atoi(doctor_id));
  if(!doctor_found) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }
 
  entry = g_object_get_data(G_OBJECT(rigth_box), "LicenseNumber");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *doctor_license = gtk_entry_buffer_get_text(buffer);
  if(detectSearchType(doctor_license) != SEARCH_BY_LICENSE_NUMBER) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");    
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  doctor_found = procurarMedicosLicenseNumber(doctors_active, atoi(doctor_license));
  if(!doctor_found) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  if(client_found->ID != strtoul(client_id, NULL, 10) || doctor_found->ID != strtoul(doctor_id, NULL, 10)) {
    return;
  }
  
  entry = g_object_get_data(G_OBJECT(rigth_box), "StartDate");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *start_date = gtk_entry_buffer_get_text(buffer);
  if(!validarFormatoData(start_date) || validarData(start_date)) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }
  
  GtkWidget *dropdown = g_object_get_data(G_OBJECT(rigth_box), "StartHour");
  GObject *item = gtk_drop_down_get_selected_item(GTK_DROP_DOWN(dropdown));
  if(!item) {
    return;
  }
  const char *start_hour = gtk_string_object_get_string(GTK_STRING_OBJECT(item));
  char *cpy = strdup(start_hour);
  
  ST_CONSULTA new_appointment = {
    .ID = numberOf(appointments, TYPE_APPOINTMENTS) + 1,
    .cliente = client_found,
    .medico = doctor_found,
    .estado = Agendado,
  };
  
  sscanf(start_date, "%02u-%02u-%04u", &new_appointment.data_inicial.dia, &new_appointment.data_inicial.mes, &new_appointment.data_inicial.ano);

  char *token = strtok(cpy, "h");
  int hour_int = atoi(token);
  new_appointment.data_inicial.hora = hour_int;
  
  free(cpy);
  cpy = NULL;
  
  // Thats function it's almost useless, since function 'obterHorario' provides the free hours.
  if(!verificarDisponibilidade(appointments, &new_appointment)) {
    GtkWidget *label = g_object_get_data(G_OBJECT(rigth_box), "label-error");
    gtk_label_set_text(GTK_LABEL(label), "Unavailable time. Please check and try again.");
    gtk_widget_set_visible(label, true);
    return;
  }

  GtkWidget *stack = gtk_widget_get_parent(rigth_box);
  
  GtkWidget *child = gtk_stack_get_child_by_name(GTK_STACK(stack), "AddAppointments");
  if(child) {
    // Confirm the new appointments by adding it to the appointments list.
    confirmarConsultas(appointments, new_appointment);

    // Save the new appointments to the file.
    inserirFicheiroConsulta(new_appointment);
    
    // Remove the "AddAppointments" page from the stack.
    gtk_stack_remove(GTK_STACK(stack), child);
  }
  
  // Remove the "appointments" page from the stack to update the table with new information.
  child = gtk_stack_get_child_by_name(GTK_STACK(stack), "appointments");
  gtk_stack_remove(GTK_STACK(stack), child);
  
  // Reinitialize with the updated appointments list.
  initializeUIAppointments(stack, application);
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "appointments");
}

static void activateSearchEditAppointment(GtkSearchEntry *search_entry, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;
  
  ST_CLIENTE *clients = application->clients;
  ST_MEDICO *doctors = application->doctors;
  ST_CONSULTA *appointments = application->appointments;
  
  GtkWidget *rigth_top_box = gtk_widget_get_parent(GTK_WIDGET(search_entry));
  GtkWidget *rigth_box = gtk_widget_get_parent(GTK_WIDGET(rigth_top_box));

  GtkWidget *label = g_object_get_data(G_OBJECT(rigth_box), "label-error");
  gtk_label_set_text(GTK_LABEL(label), "");
  gtk_widget_set_visible(label, false);

  const char *input = gtk_editable_get_text(GTK_EDITABLE(search_entry));
 
  ST_CONSULTA *edit_appointment = procurarConsultasID(appointments, atoi(input));
  if(!edit_appointment) {
    label = g_object_get_data(G_OBJECT(rigth_box), "label-error");
    gtk_label_set_text(GTK_LABEL(label), "No appointments found with the provided ID. Please check and try again.");
    gtk_widget_set_visible(label, true);
    return;
  }
   
  GtkWidget *entry = g_object_get_data(G_OBJECT(rigth_box), "ID");
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  char id[10];
  snprintf(id, sizeof(id), "%u", edit_appointment->ID);
  gtk_entry_buffer_set_text(buffer, id, -1);
  
  entry = g_object_get_data(G_OBJECT(rigth_box), "ClientID");
  g_signal_handlers_disconnect_by_func(entry, G_CALLBACK(changedEntryClientID), clients);  // Temporarily disconnect the signal to set the text for ClientID GtkEntry. 

  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  snprintf(id, sizeof(id), "%u", edit_appointment->cliente->ID);
  gtk_entry_buffer_set_text(buffer, id, -1);

  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryClientID), clients); 

  entry = g_object_get_data(G_OBJECT(rigth_box), "ClientNIF");
  g_signal_handlers_disconnect_by_func(entry, G_CALLBACK(changedEntryClientNIF), clients);  // Temporarily disconnect the signal to set the text for ClientNIF GtkEntry. 

  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  char nif[10];
  snprintf(nif, sizeof(nif), "%lu", edit_appointment->cliente->NIF);
  gtk_entry_buffer_set_text(buffer, nif, -1);

  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryClientNIF), clients); 
  
  entry = g_object_get_data(G_OBJECT(rigth_box), "ClientName");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  gtk_entry_buffer_set_text(buffer, edit_appointment->cliente->nome, -1);

  entry = g_object_get_data(G_OBJECT(rigth_box), "DoctorID");
  g_signal_handlers_disconnect_by_func(entry, G_CALLBACK(changedEntryDoctorID), doctors);  // Temporarily disconnect the signal to set the text for DoctorID GtkEntry. 

  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  snprintf(id, sizeof(id), "%u", edit_appointment->medico->ID);
  gtk_entry_buffer_set_text(buffer, id, -1);

  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryDoctorID), doctors); 
  
  entry = g_object_get_data(G_OBJECT(rigth_box), "LicenseNumber");
  g_signal_handlers_disconnect_by_func(entry, G_CALLBACK(changedEntryDoctorLicense), doctors);  // Temporarily disconnect the signal to set the text for LicenseNumber GtkEntry. 

  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  char license_number[10];
  snprintf(license_number, sizeof(license_number), "%u", edit_appointment->medico->cedula);
  gtk_entry_buffer_set_text(buffer, license_number, -1);

  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryDoctorLicense), doctors);
  
  entry = g_object_get_data(G_OBJECT(rigth_box), "DoctorName");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  gtk_entry_buffer_set_text(buffer, edit_appointment->medico->nome, -1);

  entry = g_object_get_data(G_OBJECT(rigth_box), "DoctorSpecialty");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  gtk_entry_buffer_set_text(buffer, edit_appointment->medico->especialidade, -1);

  entry = g_object_get_data(G_OBJECT(rigth_box), "StartDate");
  g_signal_handlers_disconnect_by_func(entry, G_CALLBACK(changedEntryStartDate), application);  // Temporarily disconnect the signal to set the text for ClientID GtkEntry. 

  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  char start_date[11];
  snprintf(start_date, sizeof(start_date), "%02u-%02u-%04u", edit_appointment->data_inicial.dia, edit_appointment->data_inicial.mes, edit_appointment->data_inicial.ano);
  gtk_entry_buffer_set_text(buffer, start_date, -1);
  
  unsigned int ano, mes, dia;
  sscanf(start_date, "%02u-%02u-%04u", &dia, &mes, &ano);
  
  // Reset the date to allow editing the appointment at the same hour again.
  edit_appointment->data_inicial.ano = 0;
  edit_appointment->data_inicial.mes = 0;
  edit_appointment->data_inicial.dia = 0;

  GtkWidget *dropdown = g_object_get_data(G_OBJECT(rigth_box), "StartHour");
  
  // Now get the available hours plus the hour from the edit appointment.
  char **start_hour = obterHorario(appointments, clients, doctors, start_date);
  if(start_hour[0] == NULL) {
    gtk_widget_set_sensitive(dropdown, false);
    return;
  }
  
  GtkStringList *list = gtk_string_list_new(NULL);

  for(int i = 0; start_hour[i] != NULL; i++) {
    gtk_string_list_append(list, start_hour[i]);  
  }

  gtk_drop_down_set_model(GTK_DROP_DOWN(dropdown), G_LIST_MODEL(list));  
  gtk_widget_set_sensitive(dropdown, true);
  
  // Restore the old values of the date.
  edit_appointment->data_inicial.ano = ano;
  edit_appointment->data_inicial.mes = mes;
  edit_appointment->data_inicial.dia = dia;

  g_signal_connect(entry, "changed", G_CALLBACK(changedEntryStartDate), application); 
}

static void clickedButtonSubmitEdit(GtkButton *button, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;
  
  ST_CLIENTE *clients = application->clients;
  ST_MEDICO *doctors = application->doctors;
  ST_CONSULTA *appointments = application->appointments;
  
  GtkWidget *rigth_box = gtk_widget_get_parent(GTK_WIDGET(button));
  
  GtkEntry *entry = g_object_get_data(G_OBJECT(rigth_box), "ID");
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));

  ST_CONSULTA *edit_appointment = procurarConsultasID(appointments, atoi(gtk_entry_buffer_get_text(buffer)));
  if(!edit_appointment) {
    return;
  }

  ST_CLIENTE *clients_active = NULL;
  obterListaClientesAtivos(clients, &clients_active);
  
  entry = g_object_get_data(G_OBJECT(rigth_box), "ClientID");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *client_id = gtk_entry_buffer_get_text(buffer);
  if(detectSearchType(client_id) != SEARCH_BY_ID) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  ST_CLIENTE *client_found = procurarClientesID(clients_active, atoi(client_id));
  if(!client_found) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  entry = g_object_get_data(G_OBJECT(rigth_box), "ClientNIF");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *client_nif = gtk_entry_buffer_get_text(buffer);
  if(detectSearchType(client_nif) != SEARCH_BY_NIF) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");    
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  client_found = procurarClientesNIF(clients_active, atoi(client_nif));
  if(!client_found) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  ST_MEDICO *doctors_active = NULL;
  obterListaMedicosAtivos(doctors, &doctors_active);
  
  entry = g_object_get_data(G_OBJECT(rigth_box), "DoctorID");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *doctor_id = gtk_entry_buffer_get_text(buffer);
  if(detectSearchType(doctor_id) != SEARCH_BY_ID) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  ST_MEDICO *doctor_found = procurarMedicosID(doctors_active, atoi(doctor_id));
  if(!doctor_found) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }
 
  entry = g_object_get_data(G_OBJECT(rigth_box), "LicenseNumber");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *doctor_license = gtk_entry_buffer_get_text(buffer);
  if(detectSearchType(doctor_license) != SEARCH_BY_LICENSE_NUMBER) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");    
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  doctor_found = procurarMedicosLicenseNumber(doctors_active, atoi(doctor_license));
  if(!doctor_found) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  if(client_found->ID != strtoul(client_id, NULL, 10) || doctor_found->ID != strtoul(doctor_id, NULL, 10)) {
    return;
  }

  entry = g_object_get_data(G_OBJECT(rigth_box), "StartDate");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *start_date = gtk_entry_buffer_get_text(buffer);
  if(!validarFormatoData(start_date) || validarData(start_date)) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }
  
  GtkWidget *dropdown = g_object_get_data(G_OBJECT(rigth_box), "StartHour");
  GObject *item = gtk_drop_down_get_selected_item(GTK_DROP_DOWN(dropdown));
  if(!item) {
    return;
  }
  const char *start_hour = gtk_string_object_get_string(GTK_STRING_OBJECT(item));
  char *cpy = strdup(start_hour);
  char *token = strtok(cpy, "h");
  int hour_int = atoi(token);
  
  free(cpy);
  cpy = NULL;
  
  sscanf(start_date, "%02u-%02u-%04u", &edit_appointment->data_inicial.dia, &edit_appointment->data_inicial.mes, &edit_appointment->data_inicial.ano);
  
  edit_appointment->cliente = client_found;
  edit_appointment->medico = doctor_found;
  edit_appointment->data_inicial.hora = hour_int;
  edit_appointment->data_final.dia = edit_appointment->data_inicial.dia;
  edit_appointment->data_final.mes = edit_appointment->data_inicial.mes;
  edit_appointment->data_final.ano = edit_appointment->data_inicial.ano;
  edit_appointment->data_final.hora = hour_int + 1;
  
  GtkWidget *stack = gtk_widget_get_parent(rigth_box);
  
  GtkWidget *child = gtk_stack_get_child_by_name(GTK_STACK(stack), "EditAppointments");
  if(child) {
    // Update the appointments file.
    atualizarFicheiroConsulta(application->appointments);

    gtk_stack_remove(GTK_STACK(stack), child);
  }
  
  // Remove the "appointments" page from the stack to update the table with new information.
  child = gtk_stack_get_child_by_name(GTK_STACK(stack), "appointments");
  gtk_stack_remove(GTK_STACK(stack), child);
  
  // Reinitialize with the updated appointments list.
  initializeUIAppointments(stack, application);
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "appointments");
}

static void activateSearchToggleAppointment(GtkSearchEntry *search_entry, gpointer data) {
  ST_CONSULTA *appointments = (ST_CONSULTA *)data;

  GtkWidget *rigth_top_box = gtk_widget_get_parent(GTK_WIDGET(search_entry));
  GtkWidget *rigth_box = gtk_widget_get_parent(GTK_WIDGET(rigth_top_box));
  
  GtkWidget *label = g_object_get_data(G_OBJECT(rigth_box), "label-error");
  gtk_label_set_text(GTK_LABEL(label), "");
  gtk_widget_set_visible(label, false);
  
  const char *input = gtk_editable_get_text(GTK_EDITABLE(search_entry));
  
  ST_CONSULTA *appointments_scheduled = NULL;
  int counter = obterListaConsultasAgendadas(appointments, &appointments_scheduled);
  if(counter == 0) return;

  ST_CONSULTA *toggle_appointment = procurarConsultasID(appointments_scheduled, atoi(input));
  if(!toggle_appointment) {
    label = g_object_get_data(G_OBJECT(rigth_box), "label-error");
    gtk_label_set_text(GTK_LABEL(label), "No appointments found with the provided ID. Please check and try again.");
    gtk_widget_set_visible(label, true);
    return;
  }
  
  if(toggle_appointment) {
    GtkWidget *entry = g_object_get_data(G_OBJECT(rigth_box), "ID");
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    char id[10];
    snprintf(id, sizeof(id), "%u", toggle_appointment->ID);
    gtk_entry_buffer_set_text(buffer, id, -1);

    entry = g_object_get_data(G_OBJECT(rigth_box), "ClientID");
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    snprintf(id, sizeof(id), "%u", toggle_appointment->cliente->ID);
    gtk_entry_buffer_set_text(buffer, id, -1);

    entry = g_object_get_data(G_OBJECT(rigth_box), "ClientName");
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    gtk_entry_buffer_set_text(buffer, toggle_appointment->cliente->nome, -1);

    entry = g_object_get_data(G_OBJECT(rigth_box), "NIF");
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    char nif[10];
    snprintf(nif, sizeof(nif), "%lu", toggle_appointment->cliente->NIF);
    gtk_entry_buffer_set_text(buffer, nif, -1);
    
    entry = g_object_get_data(G_OBJECT(rigth_box), "DoctorID");
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    snprintf(id, sizeof(id), "%u", toggle_appointment->medico->ID);
    gtk_entry_buffer_set_text(buffer, id, -1);
      
    entry = g_object_get_data(G_OBJECT(rigth_box), "DoctorName");
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    gtk_entry_buffer_set_text(buffer, toggle_appointment->medico->nome, -1);

    entry = g_object_get_data(G_OBJECT(rigth_box), "LicenseNumber");
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    char license_number[7];
    snprintf(license_number, sizeof(license_number), "%u", toggle_appointment->medico->cedula);
    gtk_entry_buffer_set_text(buffer, license_number, -1);

    GtkWidget *toggle_button = g_object_get_data(G_OBJECT(rigth_box), "status");
    bool status = toggle_appointment->estado ? Agendado : Cancelado;
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle_button), status);
    gtk_button_set_label(GTK_BUTTON(toggle_button), status ? "Scheduled" : "Canceled");
  }
}
static void clickedButtonSubmitToggle(GtkButton *button, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;

  ST_CONSULTA *appointments = application->appointments;

  GtkWidget *rigth_box = gtk_widget_get_parent(GTK_WIDGET(button));

  GtkWidget *entry = g_object_get_data(G_OBJECT(rigth_box), "ID");
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));

  ST_CONSULTA *toggle_appointment = procurarConsultasID(appointments, atoi(gtk_entry_buffer_get_text(buffer)));
  if(!toggle_appointment) {
    return;
  }
  
  GtkWidget *toggle_button = g_object_get_data(G_OBJECT(rigth_box), "status");
  bool status = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle_button));
  toggle_appointment->estado = status ? Agendado : Cancelado;

  GtkWidget *stack = gtk_widget_get_parent(rigth_box);
  
  GtkWidget *child = gtk_stack_get_child_by_name(GTK_STACK(stack), "ToggleAppointments");
  if(child) {
    // Update the appointments file.
    atualizarFicheiroConsulta(appointments);

    gtk_stack_remove(GTK_STACK(stack), child);
  }
  
  // Remove the "appointments" page from the stack to update the table with new information.
  child = gtk_stack_get_child_by_name(GTK_STACK(stack), "appointments");
  gtk_stack_remove(GTK_STACK(stack), child);
  
  // Reinitialize with the updated appointments list.
  initializeUIAppointments(stack, application);
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "appointments");
}

static void toggledButton(GtkToggleButton *toggle, gpointer data) {
  (void)data; // unused
  
  bool status = gtk_toggle_button_get_active(toggle);
  gtk_button_set_label(GTK_BUTTON(toggle), status ? "Scheduled" : "Canceled");
}

static void changedSearchViewAppointment(GtkSearchEntry *search_entry, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;
  
  ST_CLIENTE *clients = application->clients;
  ST_MEDICO *doctors = application->doctors;
  ST_CONSULTA *appointments = application->appointments;
  
  ST_CONSULTA *appointments_found = NULL;
  ST_CLIENTE *clients_found = NULL;
  ST_MEDICO *doctors_found = NULL;
  int counter;
  
  GtkWidget *rigth_top_box = gtk_widget_get_parent(GTK_WIDGET(search_entry));
  GtkWidget *rigth_box = gtk_widget_get_parent(GTK_WIDGET(rigth_top_box));
  
  GtkWidget *label = g_object_get_data(G_OBJECT(rigth_box), "label-error");
  gtk_label_set_text(GTK_LABEL(label), "");
  gtk_widget_set_visible(label, false);
  
  const char *input = gtk_editable_get_text(GTK_EDITABLE(search_entry));
  if(strlen(input) == 0) {
    GtkWidget *grid = g_object_get_data(G_OBJECT(rigth_box), "AppointmentTable");
    GtkWidget *scrolled = g_object_get_data(G_OBJECT(rigth_box), "Scrolled");
    gtk_box_remove(GTK_BOX(rigth_box), scrolled);
    
    scrolled = gtk_scrolled_window_new();
    g_object_set_data(G_OBJECT(rigth_box), "Scrolled", scrolled);
    gtk_widget_set_vexpand(scrolled, true);
    gtk_box_append(GTK_BOX(rigth_box), scrolled);
  
    grid = createAppointmentTable(appointments, numberOf(appointments, TYPE_APPOINTMENTS));
    g_object_set_data(G_OBJECT(rigth_box), "AppointmentTable", grid);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), grid);
    return;
  }

  SEARCH_TYPE type = detectSearchType(input);
  
  switch(type) {
    case SEARCH_BY_ID:
      appointments_found = procurarConsultasID(appointments, atoi(input));
      counter = 1; // unique identifier
      break;
    case SEARCH_BY_EMAIL:
      clients_found = procurarClientesEmail(clients, input);
      if(clients_found) {
        counter = procurarConsultasCliente(appointments, &appointments_found, clients_found, 1);
      }else {
        doctors_found = procurarMedicosEmail(doctors, input);
        if(doctors_found) {
          counter = procurarConsultasMedico(appointments, &appointments_found, doctors_found, 1);
        }
      }
      break;
    case SEARCH_BY_NIF:
      clients_found = procurarClientesNIF(clients, atoi(input));
      if(clients_found) {
        counter = procurarConsultasCliente(appointments, &appointments_found, clients_found, 1);
      }
      break;
    case SEARCH_BY_SNS:
      clients_found = procurarClientesSNS(clients, atoi(input));
      if(clients_found) {
        counter = procurarConsultasCliente(appointments, &appointments_found, clients_found, 1);
      }
      break;
    case SEARCH_BY_POSTAL_CODE:
      counter = procurarClientesCodigoPostal(clients, &clients_found, input);
      if(clients_found) {
        counter = procurarConsultasCliente(appointments, &appointments_found, clients_found, numberOf(clients_found, TYPE_CLIENTS));
      }
      break;
    case SEARCH_BY_LICENSE_NUMBER:
      doctors_found = procurarMedicosLicenseNumber(doctors, atoi(input));
      if(doctors_found) {
        counter = procurarConsultasMedico(appointments, &appointments_found, doctors_found, 1);
      }
      break;
    case SEARCH_BY_SPECIALITY:
      counter = procurarMedicosEspecialidade(doctors, &doctors_found, input);
      if(doctors_found) {
        counter = procurarConsultasMedico(appointments, &appointments_found, doctors_found, numberOf(doctors_found, TYPE_DOCTORS));
      }
      break;
    case SEARCH_BY_DATE:
      counter = procurarConsultasData(appointments, &appointments_found, input); 
      break;
    default:
      break;
  }

  if(appointments_found) {
    GtkWidget *grid = g_object_get_data(G_OBJECT(rigth_box), "AppointmentTable");
    GtkWidget *scrolled = g_object_get_data(G_OBJECT(rigth_box), "Scrolled");
    gtk_box_remove(GTK_BOX(rigth_box), scrolled);
    
    scrolled = gtk_scrolled_window_new();
    g_object_set_data(G_OBJECT(rigth_box), "Scrolled", scrolled);
    gtk_widget_set_vexpand(scrolled, true);
    gtk_box_append(GTK_BOX(rigth_box), scrolled);
   
    grid = createAppointmentTable(appointments_found, counter);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), grid); 
    g_object_set_data(G_OBJECT(rigth_box), "AppointmentTable", grid);
  }else {
    counter = procurarConsultasNome(appointments, &appointments_found, input);
    if(counter > 0) {
      GtkWidget *grid = g_object_get_data(G_OBJECT(rigth_box), "AppointmentTable");
      GtkWidget *scrolled = g_object_get_data(G_OBJECT(rigth_box), "Scrolled");
      gtk_box_remove(GTK_BOX(rigth_box), scrolled);

      scrolled = gtk_scrolled_window_new();
      g_object_set_data(G_OBJECT(rigth_box), "Scrolled", scrolled);
      gtk_widget_set_vexpand(scrolled, true);
      gtk_box_append(GTK_BOX(rigth_box), scrolled);
   
      grid = createAppointmentTable(appointments_found, counter);
      gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
      gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), grid); 
      g_object_set_data(G_OBJECT(rigth_box), "AppointmentTable", grid);
    }else {
        label = g_object_get_data(G_OBJECT(rigth_box), "label-error");
        gtk_label_set_text(GTK_LABEL(label), "No appointment found with the provided input. Please check and try again.");
        gtk_widget_set_visible(label, true);
    }
  }
}

static void changedEntryClientID(GtkEntry *entry, gpointer data) {
  ST_CLIENTE *clients = (ST_CLIENTE *)data; 
  
  ST_CLIENTE *clients_active = NULL;
  obterListaClientesAtivos(clients, &clients_active);
  
  GtkWidget *grid = gtk_widget_get_parent(GTK_WIDGET(entry));
  GtkWidget *rigth_box = gtk_widget_get_parent(GTK_WIDGET(grid));
  
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(entry);
  const char *input = gtk_entry_buffer_get_text(buffer);
  if(detectSearchType(input) != SEARCH_BY_ID) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  ST_CLIENTE *client_found = procurarClientesID(clients_active, atoi(input));
  if(!client_found) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  GtkWidget *entry_st = g_object_get_data(G_OBJECT(rigth_box), "ClientNIF");
  g_signal_handlers_disconnect_by_func(entry_st, G_CALLBACK(changedEntryClientNIF), clients);  // Temporarily disconnect the signal to set the text for NIF GtkEntry. 

  char nif[10];
  snprintf(nif, sizeof(nif), "%lu", client_found->NIF);
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, nif, -1);
  gtk_widget_remove_css_class(entry_st, "entry-error");
  gtk_widget_add_css_class(entry_st, "form-entry");

  g_signal_connect(entry_st, "changed", G_CALLBACK(changedEntryClientNIF), clients); 

  entry_st = g_object_get_data(G_OBJECT(rigth_box), "ClientName");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, client_found->nome, -1);
  
  entry_st = g_object_get_data(G_OBJECT(rigth_box), "StartDate");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, "", -1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry_st), "01-01-2025");
  
  GtkWidget *dropdown = g_object_get_data(G_OBJECT(rigth_box), "StartHour");
  gtk_widget_set_sensitive(dropdown, false);
}

static void changedEntryClientNIF(GtkEntry *entry, gpointer data) {
  ST_CLIENTE *clients = (ST_CLIENTE *)data; 
  
  ST_CLIENTE *clients_active = NULL;
  obterListaClientesAtivos(clients, &clients_active);
  
  GtkWidget *grid = gtk_widget_get_parent(GTK_WIDGET(entry));
  GtkWidget *rigth_box = gtk_widget_get_parent(GTK_WIDGET(grid));
  
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(entry);
  const char *input = gtk_entry_buffer_get_text(buffer);
  if(detectSearchType(input) != SEARCH_BY_NIF) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");    
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  ST_CLIENTE *client_found = procurarClientesNIF(clients_active, atoi(input));
  if(!client_found) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  GtkWidget *entry_st = g_object_get_data(G_OBJECT(rigth_box), "ClientID");
  g_signal_handlers_disconnect_by_func(entry_st, G_CALLBACK(changedEntryClientID), clients);  // Temporarily disconnect the signal to set the text for ID GtkEntry. 
  
  char id[10];
  snprintf(id, sizeof(id), "%u", client_found->ID);
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, id, -1);
  gtk_widget_remove_css_class(entry_st, "entry-error");
  gtk_widget_add_css_class(entry_st, "form-entry");

  g_signal_connect(entry_st, "changed", G_CALLBACK(changedEntryClientID), clients); 
  
  entry_st = g_object_get_data(G_OBJECT(rigth_box), "ClientName");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, client_found->nome, -1);      

  entry_st = g_object_get_data(G_OBJECT(rigth_box), "StartDate");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, "", -1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry_st), "01-01-2025");
  
  GtkWidget *dropdown = g_object_get_data(G_OBJECT(rigth_box), "StartHour");
  gtk_widget_set_sensitive(dropdown, false);
}

static void changedEntryDoctorID(GtkEntry *entry, gpointer data) {
  ST_MEDICO *doctors = (ST_MEDICO *)data; 
  
  ST_MEDICO *doctors_active = NULL;
  obterListaMedicosAtivos(doctors, &doctors_active);
  
  GtkWidget *grid = gtk_widget_get_parent(GTK_WIDGET(entry));
  GtkWidget *rigth_box = gtk_widget_get_parent(GTK_WIDGET(grid));
  
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(entry);
  const char *input = gtk_entry_buffer_get_text(buffer);
  if(detectSearchType(input) != SEARCH_BY_ID) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  ST_MEDICO *doctor_found = procurarMedicosID(doctors_active, atoi(input));
  if(!doctor_found) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  GtkWidget *entry_st = g_object_get_data(G_OBJECT(rigth_box), "LicenseNumber");
  g_signal_handlers_disconnect_by_func(entry_st, G_CALLBACK(changedEntryDoctorLicense), doctors);  // Temporarily disconnect the signal to set the text for NIF GtkEntry. 

  char license_number[7];
  snprintf(license_number, sizeof(license_number), "%u", doctor_found->cedula);
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, license_number, -1);
  gtk_widget_remove_css_class(entry_st, "entry-error");
  gtk_widget_add_css_class(entry_st, "form-entry");

  g_signal_connect(entry_st, "changed", G_CALLBACK(changedEntryDoctorLicense), doctors); 

  entry_st = g_object_get_data(G_OBJECT(rigth_box), "DoctorName");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, doctor_found->nome, -1);

  entry_st = g_object_get_data(G_OBJECT(rigth_box), "DoctorSpecialty");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, doctor_found->especialidade, -1);

  entry_st = g_object_get_data(G_OBJECT(rigth_box), "StartDate");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, "", -1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry_st), "01-01-2025");
  
  GtkWidget *dropdown = g_object_get_data(G_OBJECT(rigth_box), "StartHour");
  gtk_widget_set_sensitive(dropdown, false);
}

static void changedEntryDoctorLicense(GtkEntry *entry, gpointer data) {
  ST_MEDICO *doctors = (ST_MEDICO *)data; 
  
  ST_MEDICO *doctors_active = NULL;
  obterListaMedicosAtivos(doctors, &doctors_active);
  
  GtkWidget *grid = gtk_widget_get_parent(GTK_WIDGET(entry));
  GtkWidget *rigth_box = gtk_widget_get_parent(GTK_WIDGET(grid));
  
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(entry);
  const char *input = gtk_entry_buffer_get_text(buffer);
  if(detectSearchType(input) != SEARCH_BY_LICENSE_NUMBER) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  ST_MEDICO *doctor_found = procurarMedicosLicenseNumber(doctors_active, atoi(input));
  if(!doctor_found) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }

  GtkWidget *entry_st = g_object_get_data(G_OBJECT(rigth_box), "DoctorID");
  g_signal_handlers_disconnect_by_func(entry_st, G_CALLBACK(changedEntryDoctorID), doctors);  // Temporarily disconnect the signal to set the text for NIF GtkEntry. 

  char id[10];
  snprintf(id, sizeof(id), "%u", doctor_found->ID);
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, id, -1);
  gtk_widget_remove_css_class(entry_st, "entry-error");
  gtk_widget_add_css_class(entry_st, "form-entry");

  g_signal_connect(entry_st, "changed", G_CALLBACK(changedEntryDoctorID), doctors); 

  entry_st = g_object_get_data(G_OBJECT(rigth_box), "DoctorName");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, doctor_found->nome, -1);  

  entry_st = g_object_get_data(G_OBJECT(rigth_box), "DoctorSpecialty");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, doctor_found->especialidade, -1);

  entry_st = g_object_get_data(G_OBJECT(rigth_box), "StartDate");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, "", -1);
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry_st), "01-01-2025");
  
  GtkWidget *dropdown = g_object_get_data(G_OBJECT(rigth_box), "StartHour");
  gtk_widget_set_sensitive(dropdown, false);
}

static void changedEntryStartDate(GtkEntry *entry, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;
  
  ST_CLIENTE *clients = application->clients;
  ST_MEDICO *doctors = application->doctors;
  ST_CONSULTA *appointments = application->appointments;
  
  GtkWidget *grid = gtk_widget_get_parent(GTK_WIDGET(entry));
  GtkWidget *rigth_box = gtk_widget_get_parent(GTK_WIDGET(grid));
  
  ST_CLIENTE *clients_active = NULL;
  obterListaClientesAtivos(clients, &clients_active);

  ST_CLIENTE *client_found = NULL;
  
  GtkWidget *entry_st = g_object_get_data(G_OBJECT(rigth_box), "ClientID");
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  if(detectSearchType(gtk_entry_buffer_get_text(buffer)) != SEARCH_BY_ID) {
    return;
  }else {
    client_found = procurarClientesID(clients_active, atoi(gtk_entry_buffer_get_text(buffer)));
    if(client_found) {
      entry_st = g_object_get_data(G_OBJECT(rigth_box), "ClientNIF");
      buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
      if(client_found->NIF != strtoul(gtk_entry_buffer_get_text(buffer), NULL, 10)) {
        return;
      }
    }else {
      return;
    }
  }
  
  ST_MEDICO *doctors_active = NULL;
  obterListaMedicosAtivos(doctors, &doctors_active);
  
  ST_MEDICO *doctor_found = NULL;
  
  entry_st = g_object_get_data(G_OBJECT(rigth_box), "DoctorID");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  if(detectSearchType(gtk_entry_buffer_get_text(buffer)) != SEARCH_BY_ID) {
    return;
  }else {
    doctor_found = procurarMedicosID(doctors_active, atoi(gtk_entry_buffer_get_text(buffer)));
    if(doctor_found) {
      entry_st = g_object_get_data(G_OBJECT(rigth_box), "LicenseNumber");
      buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
      if(doctor_found->cedula != strtoul(gtk_entry_buffer_get_text(buffer), NULL, 10)) {
        return;
      }
    }else {
      return;
    }
  }

  buffer = gtk_entry_get_buffer(entry);
  const char *start_date = gtk_entry_buffer_get_text(buffer);
  if(!validarFormatoData(start_date) || validarData(start_date)) {
    gtk_widget_add_css_class(GTK_WIDGET(entry), "entry-error");
    return;
  }else {
    gtk_widget_remove_css_class(GTK_WIDGET(entry), "entry-error");
    gtk_widget_add_css_class(GTK_WIDGET(entry), "form-entry");
  }
   
  entry_st = g_object_get_data(G_OBJECT(rigth_box), "ID");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  ST_CONSULTA *edit_appointment = procurarConsultasID(appointments, atoi(gtk_entry_buffer_get_text(buffer)));

  unsigned int ano, mes, dia;

  if(edit_appointment) {
    char date[11];
    snprintf(date, sizeof(date), "%02u-%02u-%04u", edit_appointment->data_inicial.dia, edit_appointment->data_inicial.mes, edit_appointment->data_inicial.ano);
  
    sscanf(start_date, "%02u-%02u-%04u", &dia, &mes, &ano);
  
    // Reset the date to allow editing the appointment at the same hour again.
    edit_appointment->data_inicial.ano = 0;
    edit_appointment->data_inicial.mes = 0;
    edit_appointment->data_inicial.dia = 0;
  }

  GtkWidget *dropdown = g_object_get_data(G_OBJECT(rigth_box), "StartHour");
  
  char **start_hour = obterHorario(appointments, client_found, doctor_found, start_date);
  if(start_hour[0] == NULL) {
    gtk_widget_set_sensitive(dropdown, false);
    return;
  }

  GtkStringList *list = gtk_string_list_new(NULL);

  for(int i = 0; start_hour[i] != NULL; i++) {
    gtk_string_list_append(list, start_hour[i]);  
  }

  gtk_drop_down_set_model(GTK_DROP_DOWN(dropdown), G_LIST_MODEL(list));  
  gtk_widget_set_sensitive(dropdown, true);
  
  if(edit_appointment) {
    // Restore the old values of the date.
    edit_appointment->data_inicial.ano = ano;
    edit_appointment->data_inicial.mes = mes;
    edit_appointment->data_inicial.dia = dia;
  }

  entry_st = g_object_get_data(G_OBJECT(rigth_box), "EndDate"); 
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_st));
  gtk_entry_buffer_set_text(buffer, start_date, -1);
  
  GtkWidget *label = g_object_get_data(G_OBJECT(rigth_box), "label-error");
  gtk_label_set_text(GTK_LABEL(label), "");
  gtk_widget_set_visible(label, false);
  
  for(int i = 0; start_hour[i] != NULL; i++) {
    free(start_hour[i]);  
  }
  free(start_hour);
}

static void changedDropdownStartHour(GtkDropDown *dropdown, GParamSpec *pspec, gpointer data) { 
  (void)pspec; // unused
  (void)data;  // unused
  
  GtkWidget *grid = gtk_widget_get_parent(GTK_WIDGET(dropdown));
  GtkWidget *rigth_box = gtk_widget_get_parent(GTK_WIDGET(grid));

  GObject *item = gtk_drop_down_get_selected_item(dropdown);
  if(!item) return;

  const char **str = malloc(2 * sizeof(char *));
  if(!str) return;
 
  const char *hour = gtk_string_object_get_string(GTK_STRING_OBJECT(item));
  char *cpy = strdup(hour);

  char *token = strtok(cpy, "h");
  int hour_int = atoi(token) + 1;
  
  char new_hour[16]; // Never exceeds 6 bytes, but this silence compiler warning.
  snprintf(new_hour, sizeof(new_hour), "%02dh00", hour_int);
  
  GtkStringList *list = gtk_string_list_new(NULL);
  gtk_string_list_append(list, new_hour);

  GtkWidget *dropdown_st = g_object_get_data(G_OBJECT(rigth_box), "EndHour");  
  gtk_drop_down_set_model(GTK_DROP_DOWN(dropdown_st), G_LIST_MODEL(list));
 
  GtkWidget *label = g_object_get_data(G_OBJECT(rigth_box), "label-error");
  gtk_label_set_text(GTK_LABEL(label), "");
  gtk_widget_set_visible(label, false);
  
  free(cpy);
  cpy = NULL;
}


