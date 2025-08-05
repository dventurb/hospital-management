#include "settings.h"

// CALLBACKS
static void changedDropdownTheme(GtkDropDown *dropdown, GParamSpec *pspec, gpointer data);

void initializeSettings(GtkWidget *stack, ST_APPLICATION *application) {
  GtkWidget *rigth_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_stack_add_named(GTK_STACK(stack), rigth_box, "settings");
  gtk_widget_add_css_class(rigth_box, "box");

  GtkWidget *rigth_top_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_css_class(rigth_top_box, "rigth_top_box");
  gtk_widget_set_size_request(rigth_top_box, -1, 60);
  gtk_box_append(GTK_BOX(rigth_box), rigth_top_box);
  
  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_hexpand(spacer, true);
  gtk_box_append(GTK_BOX(rigth_top_box), spacer);
  
  initializeUserMenu(rigth_top_box, application, "settings");
 
  GtkWidget *grid = gtk_grid_new();
  gtk_widget_set_hexpand(grid, true);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  gtk_widget_set_margin_start(grid, 5);
  gtk_widget_set_margin_end(grid, 5);
  gtk_widget_set_margin_bottom(grid, 30);
  gtk_box_append(GTK_BOX(rigth_box), grid);

  addGeneralToGrid(grid);
  addAppearanceToGrid(grid, application);
}

void addGeneralToGrid(GtkWidget *grid) {
  GtkWidget *label = gtk_label_new("General");
  gtk_widget_add_css_class(label, "title-settings");
  gtk_widget_set_halign(label, GTK_ALIGN_START);
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_css_class(box, "box-settings");
  gtk_grid_attach(GTK_GRID(grid), box, 0, 1, 1, 1);

  label = gtk_label_new("Language");
  gtk_widget_add_css_class(label, "label-settings");
  gtk_box_append(GTK_BOX(box), label);

  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_hexpand(spacer, true);
  gtk_box_append(GTK_BOX(box), spacer);
  
  GtkStringList *list = gtk_string_list_new(NULL);
  gtk_string_list_append(list, "English");

  GtkWidget *dropdown = gtk_drop_down_new(G_LIST_MODEL(list), NULL);
  gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(dropdown), true);
  gtk_widget_add_css_class(dropdown, "dropdown-settings");
  gtk_box_append(GTK_BOX(box), dropdown);
  
  box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_css_class(box, "box-settings");
  gtk_grid_attach(GTK_GRID(grid), box, 0, 2, 1, 1);

  label = gtk_label_new("Data backup");
  gtk_widget_add_css_class(label, "label-settings");
  gtk_box_append(GTK_BOX(box), label);

  spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_hexpand(spacer, true);
  gtk_box_append(GTK_BOX(box), spacer);
    
  ST_BUTTON button;
  createButtonWithImageLabel(&button, EXPORT_DATA_PATH, NULL, BUTTON_ORIENTATION_VERTICAL, BUTTON_POSITION_FIRST_IMAGE);
  gtk_widget_set_margin_top(button.button, 5);
  gtk_widget_set_margin_bottom(button.button, 5);
  gtk_widget_set_margin_end(button.button, 10);
  gtk_widget_set_size_request(button.button, 30, 10);
  gtk_widget_add_css_class(button.button, "button-settings");
  gtk_box_append(GTK_BOX(box), button.button);

  box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_css_class(box, "box-settings");
  gtk_grid_attach(GTK_GRID(grid), box, 0, 3, 1, 1);

  label = gtk_label_new("Data recovery");
  gtk_widget_add_css_class(label, "label-settings");
  gtk_box_append(GTK_BOX(box), label);

  spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_hexpand(spacer, true);
  gtk_box_append(GTK_BOX(box), spacer);
  
  createButtonWithImageLabel(&button, IMPORT_DATA_PATH, NULL, BUTTON_ORIENTATION_VERTICAL, BUTTON_POSITION_FIRST_IMAGE);
  gtk_widget_set_margin_top(button.button, 5);
  gtk_widget_set_margin_bottom(button.button, 5);
  gtk_widget_set_margin_end(button.button, 10);
  gtk_widget_set_size_request(button.button, 30, 10);
  gtk_widget_add_css_class(button.button, "button-settings");
  gtk_box_append(GTK_BOX(box), button.button);
}  

void addAppearanceToGrid(GtkWidget *grid, ST_APPLICATION *application) {
  GtkWidget *label = gtk_label_new("Appearance");
  gtk_widget_add_css_class(label, "title-settings");
  gtk_widget_set_halign(label, GTK_ALIGN_START);
  gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_css_class(box, "box-settings");
  gtk_grid_attach(GTK_GRID(grid), box, 0, 5, 1, 1);

  label = gtk_label_new("Theme");
  gtk_widget_add_css_class(label, "label-settings");
  gtk_box_append(GTK_BOX(box), label);

  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_hexpand(spacer, true);
  gtk_box_append(GTK_BOX(box), spacer);

  GtkStringList *list = gtk_string_list_new(NULL);
  gtk_string_list_append(list, "Light");
  gtk_string_list_append(list, "Dark");

  GtkWidget *dropdown = gtk_drop_down_new(G_LIST_MODEL(list), NULL);
  gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(dropdown), true);
  gtk_widget_add_css_class(dropdown, "dropdown-settings");
  gtk_box_append(GTK_BOX(box), dropdown);
  g_signal_connect(dropdown, "notify::selected", G_CALLBACK(changedDropdownTheme), application);
}

static void changedDropdownTheme(GtkDropDown *dropdown, GParamSpec *pspec, gpointer data) {
  (void)pspec; // unused

  ST_APPLICATION *application = (ST_APPLICATION *)data;

  int position = gtk_drop_down_get_selected(GTK_DROP_DOWN(dropdown));

  if(position == 0) {
    set_theme_settings(LIGHT_THEME_PATH, "Light");
  }else if(position == 1) {
    set_theme_settings(DARK_THEME_PATH, "Dark");
  }
}
