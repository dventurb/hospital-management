#include "leftbar.h"

void initializeLeftBar(GtkWidget *left_box, GtkWidget *stack, ST_APPLICATION *application) {
  GtkWidget *image = gtk_image_new_from_file(LOGOTIPO_PATH);
  gtk_widget_set_size_request(image, 64, 64);
  gtk_widget_set_margin_start(image, 10);
  gtk_box_append(GTK_BOX(left_box), image);
  
  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(spacer, -1, 40);
  gtk_box_append(GTK_BOX(left_box), spacer);
  
  g_object_set_data(G_OBJECT(stack), "application", application);

  ST_BUTTON button;
  createButtonWithImageLabel(&button, DASHBOARD_ICON_PATH, "DASHBOARD", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_IMAGE);
  gtk_widget_add_css_class(button.button, "leftbar-button-selected");
  gtk_widget_add_css_class(button.label, "leftbar-button-label");
  gtk_box_append(GTK_BOX(left_box), button.button);
  g_signal_connect(button.button, "clicked", G_CALLBACK(clickedButtonDashboard), stack);
  g_object_set_data(G_OBJECT(stack), "dashboard", button.button);

  createButtonWithImageLabel(&button, CLIENT_ICON_PATH, "CLIENTS", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_IMAGE);
  gtk_widget_add_css_class(button.button, "leftbar-button");
  gtk_widget_add_css_class(button.label, "leftbar-button-label");
  gtk_box_append(GTK_BOX(left_box), button.button);
  g_signal_connect(button.button, "clicked", G_CALLBACK(clickedButtonClients), stack);
  g_object_set_data(G_OBJECT(stack), "clients", button.button);
  
  createButtonWithImageLabel(&button, DOCTOR_ICON_PATH, "DOCTORS", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_IMAGE);
  gtk_widget_add_css_class(button.button, "leftbar-button");
  gtk_widget_add_css_class(button.label, "leftbar-button-label");
  gtk_box_append(GTK_BOX(left_box), button.button);
  g_signal_connect(button.button, "clicked", G_CALLBACK(clickedButtonDoctors), stack);
  g_object_set_data(G_OBJECT(stack), "doctors", button.button);
  
  createButtonWithImageLabel(&button, APPOINTMENT_ICON_PATH, "APPOINTMENTS", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_IMAGE);
  gtk_widget_add_css_class(button.button, "leftbar-button");
  gtk_widget_add_css_class(button.label, "leftbar-button-label");
  gtk_box_append(GTK_BOX(left_box), button.button);
  g_signal_connect(button.button, "clicked", G_CALLBACK(clickedButtonAppointments), stack);
  g_object_set_data(G_OBJECT(stack), "appointments", button.button);
  
  spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_vexpand(spacer, true);
  gtk_box_append(GTK_BOX(left_box), spacer);
  
  createButtonWithImageLabel(&button, SETTINGS_ICON_PATH, "SETTINGS", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_IMAGE);
  gtk_widget_add_css_class(button.button, "leftbar-button");
  gtk_widget_add_css_class(button.label, "leftbar-button-label");
  gtk_box_append(GTK_BOX(left_box), button.button);
  g_signal_connect(button.button, "clicked", G_CALLBACK(clickedButtonSettings), stack);
  g_object_set_data(G_OBJECT(stack), "settings", button.button);
}

void clickedButtonDashboard(GtkButton *button, gpointer data) {
  GtkWidget *stack = (GtkWidget *)data;
  
  ST_APPLICATION *application = g_object_get_data(G_OBJECT(stack), "application");

  clearStackPages(stack);

  const char *strings[] = { 
    "dashboard", 
    "clients", 
    "doctors", 
    "appointments",
    "settings"
  };
  
  // Reset CSS Style from the leftbar buttons.
  for (int i = 0; i < 5; i++) {
    GtkWidget *btn = g_object_get_data(G_OBJECT(stack), strings[i]);
    gtk_widget_remove_css_class(btn, "leftbar-button-selected");
    gtk_widget_add_css_class(btn, "leftbar-button");
  }

  // Update the Image of UserMenu in every Stack page.
  for (int i = 0; i < 5; i++) {
    GtkWidget *child = gtk_stack_get_child_by_name(GTK_STACK(stack), strings[i]);
    if(!child) continue;
    
    GtkWidget *image = g_object_get_data(G_OBJECT(child), "Image");
    if(!image) continue;

    if(!GTK_IS_WIDGET(image)) continue;

    if(GTK_IS_IMAGE(image)) {
      gtk_image_set_from_file(GTK_IMAGE(image), application->staff->pathToImage);
    }
  }
  
  // Add Selected-Button Style to the current button.
  gtk_widget_add_css_class(GTK_WIDGET(button), "leftbar-button-selected");
  
  // Remove the Dashboard page.
  GtkWidget *child = gtk_stack_get_child_by_name(GTK_STACK(stack), "dashboard");
  if(child) {
    gtk_stack_remove(GTK_STACK(stack), child);
  }
  
  // Create again the Dashboard to update the information.
  initializeDashboard(stack, application);
  
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "dashboard");
}

void clickedButtonClients(GtkButton *button, gpointer data) {
  GtkWidget *stack = (GtkWidget *)data;

  ST_APPLICATION *application = g_object_get_data(G_OBJECT(stack), "application");
  
  clearStackPages(stack);

  const char *strings[] = { 
    "dashboard", 
    "clients", 
    "doctors", 
    "appointments",
    "settings"
  };
  
  // Reset CSS Style from the leftbar buttons.
  for (int i = 0; i < 5; i++) {
    GtkWidget *btn = g_object_get_data(G_OBJECT(stack), strings[i]);
    gtk_widget_remove_css_class(btn, "leftbar-button-selected");
    gtk_widget_add_css_class(btn, "leftbar-button");
  }
  
  // Update the Image of UserMenu in every Stack page.
  for (int i = 0; i < 5; i++) {
    GtkWidget *child = gtk_stack_get_child_by_name(GTK_STACK(stack), strings[i]);
    if(!child) continue;
    
    GtkWidget *image = g_object_get_data(G_OBJECT(child), "Image");
    if(!image) continue;

    if(!GTK_IS_WIDGET(image)) continue;
    
    if(GTK_IS_IMAGE(image)) {
      gtk_image_set_from_file(GTK_IMAGE(image), application->staff->pathToImage);
    }
  }
  
  // Add Selected-Button Style to the current button.
  gtk_widget_add_css_class(GTK_WIDGET(button), "leftbar-button-selected");
  
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "clients");
}

void clickedButtonDoctors(GtkButton *button, gpointer data) {
  GtkWidget *stack = (GtkWidget *)data;

  ST_APPLICATION *application = g_object_get_data(G_OBJECT(stack), "application");
 
  const char *strings[] = { 
    "dashboard", 
    "clients", 
    "doctors", 
    "appointments",
    "settings"
  };
  
  // Reset CSS Style from the leftbar buttons.
  for (int i = 0; i < 5; i++) {
    GtkWidget *btn = g_object_get_data(G_OBJECT(stack), strings[i]);
    gtk_widget_remove_css_class(btn, "leftbar-button-selected");
    gtk_widget_add_css_class(btn, "leftbar-button");
  }
  
  // Update the Image of UserMenu in every Stack page.
  for (int i = 0; i < 5; i++) {
    GtkWidget *child = gtk_stack_get_child_by_name(GTK_STACK(stack), strings[i]);
    if(!child) continue;
    
    GtkWidget *image = g_object_get_data(G_OBJECT(child), "Image");
    if(!image) continue;

    if(!GTK_IS_WIDGET(image)) continue;
       
    if(GTK_IS_IMAGE(image)) {
      gtk_image_set_from_file(GTK_IMAGE(image), application->staff->pathToImage);
    }
  }
 
  // Add Selected-Button Style to the current button.
  gtk_widget_add_css_class(GTK_WIDGET(button), "leftbar-button-selected");
  
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "doctors");
}

void clickedButtonAppointments(GtkButton *button, gpointer data) {
  GtkWidget *stack = (GtkWidget *)data;

  ST_APPLICATION *application = g_object_get_data(G_OBJECT(stack), "application");
  
  clearStackPages(stack);

  const char *strings[] = { 
    "dashboard", 
    "clients", 
    "doctors", 
    "appointments",
    "settings"
  };
  
  // Reset CSS Style from the leftbar buttons.
  for (int i = 0; i < 5; i++) {
    GtkWidget *btn = g_object_get_data(G_OBJECT(stack), strings[i]);
    gtk_widget_remove_css_class(btn, "leftbar-button-selected");
    gtk_widget_add_css_class(btn, "leftbar-button");
  }
  
  // Update the Image of UserMenu in every Stack page.
  for (int i = 0; i < 5; i++) {
    GtkWidget *child = gtk_stack_get_child_by_name(GTK_STACK(stack), strings[i]);
    if(!child) continue;
    
    GtkWidget *image = g_object_get_data(G_OBJECT(child), "Image");
    if(!image) continue;

    if(!GTK_IS_WIDGET(image)) continue;
        

    if(GTK_IS_IMAGE(image)) {
      gtk_image_set_from_file(GTK_IMAGE(image), application->staff->pathToImage);
    }
  }
  
  // Add Selected-Button Style to the current button.
  gtk_widget_add_css_class(GTK_WIDGET(button), "leftbar-button-selected");
  
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "appointments");
}

void clickedButtonSettings(GtkButton *button, gpointer data) {
  GtkWidget *stack = (GtkWidget *)data;

  ST_APPLICATION *application = g_object_get_data(G_OBJECT(stack), "application");
  
  clearStackPages(stack);

  const char *strings[] = { 
    "dashboard", 
    "clients", 
    "doctors", 
    "appointments",
    "settings"
  };
  
  // Reset CSS Style from the leftbar buttons.
  for (int i = 0; i < 5; i++) {
    GtkWidget *btn = g_object_get_data(G_OBJECT(stack), strings[i]);
    gtk_widget_remove_css_class(btn, "leftbar-button-selected");
    gtk_widget_add_css_class(btn, "leftbar-button");
  }
  
  // Update the Image of UserMenu in every Stack page.
  for (int i = 0; i < 5; i++) {
    GtkWidget *child = gtk_stack_get_child_by_name(GTK_STACK(stack), strings[i]);
    if(!child) continue;
    
    GtkWidget *image = g_object_get_data(G_OBJECT(child), "Image");
    if(!image) continue;

    if(!GTK_IS_WIDGET(image)) continue;
        

    if(GTK_IS_IMAGE(image)) {
      gtk_image_set_from_file(GTK_IMAGE(image), application->staff->pathToImage);
    }
  }
  
  // Add Selected-Button Style to the current button.
  gtk_widget_add_css_class(GTK_WIDGET(button), "leftbar-button-selected");
  
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "settings");
}
