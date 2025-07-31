#include "authWindow.h"

// CALLBACKS
static void clickedButtonLogin(GtkButton *button, gpointer data);
static void clickedButtonRegister(GtkButton *button, gpointer data);

static void pressedGestureLogin(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data);
static void pressedGestureRegister(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data);

/** 
 *  @brief Create the window for the authentication. 
 *
 *  @param app    Pointer to the GtkApplication.
 *  @param data   Pointer to the application data which contains clients, doctors, 
 *                  appointments and staff. 
 *
 */
void createAuthWindow(GtkApplication *app, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;
  
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Hospital Management");
  gtk_window_set_default_size(GTK_WINDOW(window), 1100, 700);
  gtk_widget_add_css_class(window, "window");

  g_object_set_data(G_OBJECT(window), "app", app);

  GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_add_css_class(main_box, "box");
  gtk_window_set_child(GTK_WINDOW(window), main_box);

  GtkWidget *stack = gtk_stack_new();
  gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);
  gtk_box_append(GTK_BOX(main_box), stack);

  createRightBox(main_box);
  createLoginForm(application, stack);
  createRegisterForm(application, stack);

  GtkCssProvider *provider = gtk_css_provider_new();
  GFile *css_file = g_file_new_for_path(DARK_CSS_PATH);
  gtk_css_provider_load_from_file(provider, css_file);
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_window_present(GTK_WINDOW(window));

  g_object_unref(css_file);
}

void createRightBox(GtkWidget *main_box) {
  GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_add_css_class(right_box, "right-box");
  gtk_widget_set_size_request(right_box, 320, -1);
  gtk_box_append(GTK_BOX(main_box), right_box);
  
  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_vexpand(spacer, true);
  gtk_box_append(GTK_BOX(right_box), spacer);
  
  GtkWidget *image = gtk_image_new_from_file(LOGOTIPO_PATH);
  gtk_widget_set_size_request(image, 64, 64);
  gtk_widget_set_valign(image, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(right_box), image);
 
  spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(spacer, -1, 30);
  gtk_widget_set_valign(spacer, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(right_box), spacer);
  
  GtkWidget *label = gtk_label_new("Hospital Management");
  gtk_widget_add_css_class(label, "title");
  gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(right_box), label);

  label = gtk_label_new("A simple CRUD, carefully built for hospitals.");
  gtk_widget_add_css_class(label, "subtitle");
  gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(right_box), label); 

  spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_vexpand(spacer, true);
  gtk_box_append(GTK_BOX(right_box), spacer);
}

/** 
 *  @brief Creates and adds the login interface. 
 *
 *  @param stack    Pointer to the GtkStack.
 *
 */
void createLoginForm(ST_APPLICATION *application, GtkWidget *stack) {
  GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_halign(left_box, GTK_ALIGN_CENTER);
  gtk_widget_set_hexpand(left_box, true);
  gtk_widget_add_css_class(left_box, "box");
  gtk_stack_add_named(GTK_STACK(stack), left_box, "login");

  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_vexpand(spacer, true);
  gtk_box_append(GTK_BOX(left_box), spacer);
  
  GtkWidget *label = gtk_label_new("Log in");
  gtk_widget_add_css_class(label, "login-title");
  gtk_box_append(GTK_BOX(left_box), label);

  spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(spacer, -1, 30);
  gtk_box_append(GTK_BOX(left_box), spacer);

  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(left_box), grid);

  label = gtk_label_new("Username");
  gtk_widget_add_css_class(label, "login-label");
  gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

  GtkWidget *entry = gtk_entry_new();
  g_object_set_data(G_OBJECT(left_box), "Username", entry);
  gtk_entry_set_max_length(GTK_ENTRY(entry), 100);
  gtk_widget_add_css_class(entry, "login-entry");
  gtk_widget_set_halign(entry, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 1, 1);
  
  label = gtk_label_new("Password");
  gtk_widget_add_css_class(label, "login-label");
  gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

  entry = gtk_entry_new();
  g_object_set_data(G_OBJECT(left_box), "Password", entry);
  gtk_entry_set_max_length(GTK_ENTRY(entry), 100);
  gtk_entry_set_visibility(GTK_ENTRY(entry), false);
  gtk_entry_set_invisible_char(GTK_ENTRY(entry), '*');
  gtk_widget_add_css_class(entry, "login-entry");
  gtk_widget_set_halign(entry, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), entry, 0, 3, 1, 1);
 
  spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(spacer, -1, 15);
  gtk_box_append(GTK_BOX(left_box), spacer);

  ST_BUTTON btn; 
  createButtonWithImageLabel(&btn, NULL,"Login", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_IMAGE);  
  gtk_widget_add_css_class(btn.button, "login-button");
  gtk_widget_add_css_class(btn.label, "login-button-label");
  gtk_widget_set_size_request(btn.button, 80, 40);
  gtk_widget_set_halign(btn.box, GTK_ALIGN_CENTER);
  gtk_widget_set_hexpand(btn.button, false);
  gtk_box_append(GTK_BOX(left_box), btn.button);
  g_signal_connect(btn.button, "clicked", G_CALLBACK(clickedButtonLogin), application);
  
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_append(GTK_BOX(left_box), box);

  label = gtk_label_new("Don't have an account?");
  gtk_widget_add_css_class(label, "login-label");
  gtk_box_append(GTK_BOX(box), label);
  
  label = gtk_label_new("Sign up");
  gtk_widget_set_focusable(label, true);
  gtk_widget_add_css_class(label, "login-label-signup");
  gtk_box_append(GTK_BOX(box), label);

  GtkGesture *gesture = gtk_gesture_click_new();
  gtk_widget_add_controller(label, GTK_EVENT_CONTROLLER(gesture));
  g_signal_connect(gesture, "pressed", G_CALLBACK(pressedGestureLogin), stack);  
  
  label = gtk_label_new("");
  gtk_widget_add_css_class(label, "label-error");
  gtk_widget_set_visible(label, false);
  g_object_set_data(G_OBJECT(left_box), "label-error", label);
  gtk_box_append(GTK_BOX(left_box), label);
  
  spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_vexpand(spacer, true);
  gtk_box_append(GTK_BOX(left_box), spacer);

  gtk_stack_set_visible_child_name(GTK_STACK(stack), "login");
}

/** 
 *  @brief Creates the register interface. 
 *
 *  @param stack    Pointer to the GtkStack.
 *
 */
void createRegisterForm(ST_APPLICATION *application, GtkWidget *stack) {
  GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_halign(left_box, GTK_ALIGN_CENTER);
  gtk_widget_set_hexpand(left_box, true);
  gtk_widget_add_css_class(left_box, "box");
  gtk_stack_add_named(GTK_STACK(stack), left_box, "register");

  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_vexpand(spacer, true);
  gtk_box_append(GTK_BOX(left_box), spacer);
  
  GtkWidget *label = gtk_label_new("Sign up");
  gtk_widget_add_css_class(label, "login-title");
  gtk_box_append(GTK_BOX(left_box), label);

  spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(spacer, -1, 30);
  gtk_box_append(GTK_BOX(left_box), spacer);

  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(left_box), grid);
  
  label = gtk_label_new("Name");
  gtk_widget_add_css_class(label, "login-label");
  gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

  GtkWidget *entry = gtk_entry_new();
  g_object_set_data(G_OBJECT(left_box), "Name", entry);
  gtk_entry_set_max_length(GTK_ENTRY(entry), 100);
  gtk_widget_add_css_class(entry, "login-entry");
  gtk_widget_set_halign(entry, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 1, 1);
  
  label = gtk_label_new("Username");
  gtk_widget_add_css_class(label, "login-label");
  gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

  entry = gtk_entry_new();
  g_object_set_data(G_OBJECT(left_box), "Username", entry);
  gtk_entry_set_max_length(GTK_ENTRY(entry), 100);
  gtk_widget_add_css_class(entry, "login-entry");
  gtk_widget_set_halign(entry, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), entry, 0, 3, 1, 1);
  
  label = gtk_label_new("Password");
  gtk_widget_add_css_class(label, "login-label");
  gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);

  entry = gtk_entry_new();
  g_object_set_data(G_OBJECT(left_box), "Password", entry);
  gtk_entry_set_visibility(GTK_ENTRY(entry), false);
  gtk_entry_set_invisible_char(GTK_ENTRY(entry), '*');
  gtk_entry_set_max_length(GTK_ENTRY(entry), 100);
  gtk_widget_add_css_class(entry, "login-entry");
  gtk_widget_set_halign(entry, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), entry, 0, 5, 1, 1);
 
  spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(spacer, -1, 15);
  gtk_box_append(GTK_BOX(left_box), spacer);

  ST_BUTTON btn; 
  createButtonWithImageLabel(&btn, NULL,"Sign up", BUTTON_ORIENTATION_HORIZONTAL, BUTTON_POSITION_FIRST_IMAGE);  
  gtk_widget_add_css_class(btn.button, "login-button");
  gtk_widget_add_css_class(btn.label, "login-button-label");
  gtk_widget_set_size_request(btn.button, 80, 40);
  gtk_widget_set_halign(btn.box, GTK_ALIGN_CENTER);
  gtk_widget_set_hexpand(btn.button, false);
  gtk_box_append(GTK_BOX(left_box), btn.button);
  g_signal_connect(btn.button, "clicked", G_CALLBACK(clickedButtonRegister), application);
  
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_append(GTK_BOX(left_box), box);

  label = gtk_label_new("Already have an account?");
  gtk_widget_add_css_class(label, "login-label");
  gtk_box_append(GTK_BOX(box), label);
  
  label = gtk_label_new("Log in");
  gtk_widget_set_focusable(label, true);
  gtk_widget_add_css_class(label, "login-label-signup");
  gtk_box_append(GTK_BOX(box), label);

  GtkGesture *gesture = gtk_gesture_click_new();
  gtk_widget_add_controller(label, GTK_EVENT_CONTROLLER(gesture));
  g_signal_connect(gesture, "pressed", G_CALLBACK(pressedGestureRegister), stack);  
  
  label = gtk_label_new("");
  gtk_widget_add_css_class(label, "label-error");
  gtk_widget_set_visible(label, false);
  g_object_set_data(G_OBJECT(left_box), "label-error", label);
  gtk_box_append(GTK_BOX(left_box), label);
  
  spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_vexpand(spacer, true);
  gtk_box_append(GTK_BOX(left_box), spacer);
}

static void clickedButtonLogin(GtkButton *button, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;

  ST_FUNCIONARIO *staff = application->staff;

  GtkWidget *left_box = gtk_widget_get_parent(GTK_WIDGET(button));

  GtkWidget *entry = g_object_get_data(G_OBJECT(left_box), "Username");
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  char username[STRING_MAX];
  strncpy(username, gtk_entry_buffer_get_text(buffer), STRING_MAX - 1);
  username[STRING_MAX - 1] = '\0';

  entry = g_object_get_data(G_OBJECT(left_box), "Password");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  char password[STRING_MAX];
  strncpy(password, gtk_entry_buffer_get_text(buffer), STRING_MAX - 1);
  password[STRING_MAX - 1] = '\0';
  
  if(!authValidate(staff, username, password)) {
    GtkWidget *label = g_object_get_data(G_OBJECT(left_box), "label-error");
    gtk_label_set_text(GTK_LABEL(label), "Invalid username or password.\n\t Please try again.");
    gtk_widget_set_visible(label, true);
    return;
  }else {
    GtkNative *native = gtk_widget_get_native(left_box);
    if(GTK_IS_WINDOW(native)) {
      GtkWindow *window = GTK_WINDOW(native);
      
      GtkApplication *app = g_object_get_data(G_OBJECT(window), "app");
     
      application->staff = getCurrentUser(staff, username); 
      createMainWindow(app, application);
      
      gtk_window_destroy(window);
    }
  }
}

static void clickedButtonRegister(GtkButton *button, gpointer data) {
  ST_APPLICATION *application = (ST_APPLICATION *)data;

  ST_FUNCIONARIO *staff = application->staff;

  GtkWidget *left_box = gtk_widget_get_parent(GTK_WIDGET(button));
  
  if(numberOf(staff, TYPE_STAFF) >= MAX_FUNCIONARIOS) {
    GtkWidget *label = g_object_get_data(G_OBJECT(left_box), "label-error");
    gtk_label_set_text(GTK_LABEL(label), "Maximum number of staff reached.");
    gtk_widget_set_visible(label, true);
    return;
  }
  
  GtkWidget *entry = g_object_get_data(G_OBJECT(left_box), "Name");
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *name = gtk_entry_buffer_get_text(buffer);

  entry = g_object_get_data(G_OBJECT(left_box), "Username");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *username = gtk_entry_buffer_get_text(buffer);
  
  entry = g_object_get_data(G_OBJECT(left_box), "Password");
  buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
  const char *password = gtk_entry_buffer_get_text(buffer);
  
  if(!usernameValidate(staff, username) || (strlen(username) < 3 || strlen(password) < 5)) {
    GtkWidget *label = g_object_get_data(G_OBJECT(left_box), "label-error");
    gtk_label_set_text(GTK_LABEL(label), "Username or password not available.\n\t\tPlease try another.");
    gtk_widget_set_visible(label, true);
    return;
  }

  ST_FUNCIONARIO new = {
    .ID = numberOf(staff, TYPE_STAFF) + 1,
  };
  
  strncpy(new.nome, name, STRING_MAX - 1);
  new.nome[STRING_MAX - 1] = '\0';

  strncpy(new.username, username, STRING_MAX - 1);
  new.username[STRING_MAX - 1] = '\0';
  
  if(!encryptPassword(&new, password)) return;
  
  int image = (new.ID % 4) + 1;
  switch (image) {
    case 1:
      strncpy(new.pathToImage, USER_IMAGE_1_PATH, STRING_MAX - 1);
      new.pathToImage[STRING_MAX - 1] = '\0';
      break;
    case 2:
      strncpy(new.pathToImage, USER_IMAGE_2_PATH, STRING_MAX - 1);
      new.pathToImage[STRING_MAX - 1] = '\0';
      break;
    case 3:
      strncpy(new.pathToImage, USER_IMAGE_3_PATH, STRING_MAX - 1);
      new.pathToImage[STRING_MAX - 1] = '\0';
      break;
    case 4:
      strncpy(new.pathToImage, USER_IMAGE_4_PATH, STRING_MAX - 1);
      new.pathToImage[STRING_MAX - 1] = '\0';
      break;
    default:
      break;
  }

  GtkWidget *stack = gtk_widget_get_parent(left_box);
  
  // Confirm the new user by adding it to the staff list.
  createUser(staff, new);

  // Save the new user to the file.
  insertUserFile(&new);

  gtk_stack_set_visible_child_name(GTK_STACK(stack), "login");
}

static void pressedGestureLogin(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data) {
  (void)gesture; // unused parameter
  (void)n_press; // unused parameter
  (void)x;       // unused parameter
  (void)y;       // unused parameter

  GtkWidget *stack = (GtkWidget *)data;
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "register");
}

static void pressedGestureRegister(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data) {
  (void)gesture;  // unused parameter
  (void)n_press;  // unused parameter
  (void)x;        // unused parameter
  (void)y;        // unused parameter

  GtkWidget *stack = (GtkWidget *)data;
  gtk_stack_set_visible_child_name(GTK_STACK(stack), "login");
}
