#include "utils.h"

/** 
  * @brief Creates a button with an image and label.
  * 
  * @param button         Pointer to the ST_BUTTON struct. 
  * @param pathToImage    Path to the image file. Can be NULL if no image is used.
  * @param text           Label to display on the button. Can be NULL if no text is used. 
  * @param orientation    Orientation of the image and label (horizontal or vertical).
  * @param position       Order in which image and label apper (label first or image first).
  *
  */
void createButtonWithImageLabel(ST_BUTTON *button, const char *pathToImage, const char *text, BUTTON_ORIENTATION orientation, BUTTON_POSITION position) {
  button->button = gtk_button_new();
  
  if (orientation == BUTTON_ORIENTATION_HORIZONTAL) {
    button->box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_margin_start(button->box, 5);
    gtk_widget_set_margin_end(button->box, 5);
    gtk_button_set_child(GTK_BUTTON(button->button), button->box);
  }else if(orientation == BUTTON_ORIENTATION_VERTICAL) {
    button->box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(button->box, 5);
    gtk_widget_set_margin_end(button->box, 5);
    gtk_button_set_child(GTK_BUTTON(button->button), button->box);
  }
  
  if(position == BUTTON_POSITION_FIRST_IMAGE) {
    if(pathToImage != NULL) {
      button->image = gtk_image_new_from_file(pathToImage);
      gtk_widget_set_size_request(button->image, 30, 30);
      gtk_box_append(GTK_BOX(button->box), button->image);
    }

    if(text != NULL) {
      button->label = gtk_label_new(text);
      gtk_box_append(GTK_BOX(button->box), button->label);
    }
  }else if(position == BUTTON_POSITION_FIRST_LABEL) {
    if(text != NULL) {
      button->label = gtk_label_new(text);
      gtk_box_append(GTK_BOX(button->box), button->label);
    }

    if(pathToImage != NULL) {
      button->image = gtk_image_new_from_file(pathToImage);
      gtk_widget_set_size_request(button->image, 30, 30);
      gtk_box_append(GTK_BOX(button->box), button->image);
    }
  }
}

GtkWidget *createAnalyticCard(CARD_COLOR color, const char *pathToImage, const char *title, const char *info, const char *subtitle)  {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_set_size_request(box, 200, 200);
  
  GtkWidget *image_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_halign(image_box, GTK_ALIGN_START);
  gtk_widget_set_hexpand(image_box, false);
  gtk_widget_set_size_request(image_box, 40, 40);
  gtk_box_append(GTK_BOX(box), image_box);

  if(pathToImage) {
    GtkWidget *image = gtk_image_new_from_file(pathToImage);
    gtk_widget_set_size_request(image, 30, 30);
    gtk_widget_set_margin_top(image, 5);
    gtk_widget_set_valign(image, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(image_box), image);
  }
  
  if(title) {
    GtkWidget *label = gtk_label_new(title);
    gtk_widget_set_margin_top(label, 10);
    gtk_widget_add_css_class(label, "box-card-title");
    gtk_box_append(GTK_BOX(box), label);
  }

  if(info) {
    GtkWidget *label = gtk_label_new(info);
    gtk_widget_set_margin_top(label, 10);
    gtk_widget_add_css_class(label, "box-card-info");
    gtk_box_append(GTK_BOX(box), label);
  }
  
  if(subtitle) {
    GtkWidget *label = gtk_label_new(subtitle);
    gtk_widget_set_margin_top(label, 10);
    gtk_widget_add_css_class(label, "box-card-subtitle");
    gtk_box_append(GTK_BOX(box), label);
  }

  const char **css_style = getCardColorClass(color);
  if(css_style) {
    gtk_widget_add_css_class(box, css_style[0]);
    gtk_widget_add_css_class(image_box, css_style[1]);

    for(int i = 0; i < 2; i++) {
      free((void *)css_style[i]);
      css_style[i] = NULL;
    }
  }
  free(css_style);
  css_style = NULL;

  return box;
}

const char **getCardColorClass(CARD_COLOR color) {
  const char **strings = malloc( 2 * sizeof(char *));
  if(!strings) return NULL;

  switch (color) {
    case CARD_COLOR_BLUE: 
      strings[0] = strdup("box-blue");
      strings[1] = strdup("box-image-blue");
      break;
    case CARD_COLOR_GREEN: 
      strings[0] = strdup("box-green");
      strings[1] = strdup("box-image-green");
      break;
    case CARD_COLOR_ORANGE: 
      strings[0] = strdup("box-orange");
      strings[1] = strdup("box-image-orange");
      break;
    case CARD_COLOR_RED:    
      strings[0] = strdup("box-red");
      strings[1] = strdup("box-image-red");
      break;
    case CARD_COLOR_PURPLE: 
      strings[0] = strdup("box-purple");
      strings[1] = strdup("box-image-purple");
      break;
    case CARD_COLOR_YELLOW: 
      strings[0] = strdup("box-yellow");
      strings[1] = strdup("box-image-yellow");
      break;
    case CARD_COLOR_CYAN:   
      strings[0] = strdup("box-cyan");
      strings[1] = strdup("box-image-cyan");
      break;
    default: 
      strings[0] = strdup("box-blue");
      strings[1] = strdup("box-image-blue");
      break;
  }
  return strings;
}

/**
  * @brief Detects the type of input used for search the client.
  *
  * This function detect the SearchEntry input and determines whether it 
  * corresponds to an email, NIF, SNS, ID or invalid input. 
  *
  * @param input    String representing the user input. 
  * @return A value from the SEARCH_TYPE enum. 
  *
  */
SEARCH_TYPE detectSearchType(const char *input) {

  // Checking if the input is a valid email format.
  const char *email_regex = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";
  
  regex_t regex;
  int reti = regcomp(&regex, email_regex, REG_EXTENDED);

  reti = regexec(&regex, input, 0, NULL, 0);

  if(reti == REG_NOMATCH) {
    regfree(&regex);
  }else if(reti) {
    regfree(&regex);
  }else {
    regfree(&regex);
    return SEARCH_BY_EMAIL;
  }
  
  // Check if the input is specialty of a doctor.
  GtkStringList *list = loadSpecialty();
  for(unsigned int i = 0; i < g_list_model_get_n_items(G_LIST_MODEL(list)); i++) {
    const char *specialty = gtk_string_list_get_string(list, i);
    if(strncmp(specialty, convertToUppercase(input), 4) == 0) {
      return SEARCH_BY_SPECIALITY;
    }
  }
  
  // Check if the input is composed of digits.
  int length = strlen(input);
  
  if(validarFormatoData(input)) {
    return SEARCH_BY_DATE;
  }else if(validarCodigoPostal(input)) {
    return SEARCH_BY_POSTAL_CODE;
  }

  for(int i = 0; i < length; i++) {
    if(!isdigit((unsigned char)input[i])) {
      return SEARCH_BY_INVALID;
    }
  }
  
  // If is 9 digits long, perfome a checksum validation to check if is between NIF or SNS.
  if(length == 9) {
    int digit[8] = { 9, 8, 7, 6, 5, 4, 3, 2 };
    int sum = 0;
    for(int i = 0; i < 8; i++) {
      sum += (input[i] - '0') * digit[i];
    }

    int remainer = sum % 11;
    int checksum = (remainer == 0 || remainer == 1) ? 0 : 11 - remainer;

    if((11 - remainer) == (input[8] - '0')) {
      return SEARCH_BY_NIF;
    }else {
      return SEARCH_BY_SNS;
    }
  }else if(length == 6) {
    return SEARCH_BY_LICENSE_NUMBER;  // Doctor license numbers typically have 6 digits.
  }else { 
    return SEARCH_BY_ID;  // Assume the input is an ID. 
  }
}

GtkStringList *loadSpecialty() {
GtkStringList *list = gtk_string_list_new(NULL);

  FILE *file = fopen("data/especialidade.txt", "r");
  if(!file) {
    return list;
  }

  char row[64];
  while(fgets(row, sizeof(row), file)) {
    row[strcspn(row, "\n")] = '\0';
    gtk_string_list_append(list, row);
  }

  fclose(file);
  return list;
}

bool validationTypeSizeDimensions(GFile *file) {
  GFileInfo *info = g_file_query_info(file, "standard::size,standard::content-type", G_FILE_QUERY_INFO_NONE, NULL, NULL);
  if(!info) return false;
  
  // 1MB 
  if(g_file_info_get_size(info) > (1024 * 1024)) {
    g_object_unref(info);
    return false;
  }
  
  GdkTexture *texture = gdk_texture_new_from_file(file, NULL);
  if(!texture) {
    g_object_unref(info);
    return 0;
  }
  
  // 128px x 128px
  if(gdk_texture_get_width(texture) > 128 || gdk_texture_get_height(texture) > 128) {
    g_object_unref(info);
    g_object_unref(texture);
    return false;
  }

  const char *type = g_file_info_get_content_type(info);
  if(strcmp(type, "image/png") != 0 && strcmp(type, "image/jpeg") != 0) {
    g_object_unref(info); 
    return false;
  }
  
  g_object_unref(info);
  g_object_unref(texture);
  
  return true;
}

/**
  * @brief Removes dynamic pages from the GtkStack.
  *
  * This function checks for and removes stack pages from the GtkStack. 
  *
  * @param stack    A pointer to the GtkStack. 
  *
  */
void clearStackPages(GtkWidget *stack) {

  const char *strings[] = {
    "AddClients",
    "EditClients",
    "ToggleClients",
    "ViewClients",
    "AddDoctors",
    "EditDoctors",
    "ToggleDoctors",
    "ViewDoctors",
    "AddAppointments",
    "EditAppointments",
    "ToggleAppointments",
    "ViewAppointments",
    "userInterface"
  };

  for (int i = 0; i < 13; i++) {
    GtkWidget *child = gtk_stack_get_child_by_name(GTK_STACK(stack), strings[i]);
    if(child) {
      gtk_stack_remove(GTK_STACK(stack), child);
    }
  }
}

char *get_current_theme_path(void) {
  FILE *file = fopen(SETTINGS_PATH, "r");
  if(!file) {
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *data = malloc(size + 1);
  if(!data) {
    fclose(file);
    return NULL;
  }

  fread(data, 1, size, file);
  data[size] = '\0';

  fclose(file);

  cJSON *json = cJSON_Parse(data);
  if(!json) {
    free(data);
    return NULL;
  }
  free(data);

  const cJSON *theme = cJSON_GetObjectItemCaseSensitive(json, "css_file");
  char *string = NULL;
  if(cJSON_IsString(theme)) {
    string = strdup(theme->valuestring);
  }

  cJSON_Delete(json);
  
  return string;
}

char *get_current_theme_name(void) {
  FILE *file = fopen(SETTINGS_PATH, "r");
  if(!file) {
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *data = malloc(size + 1);
  if(!data) {
    fclose(file);
    return NULL;
  }

  fread(data, 1, size, file);
  data[size] = '\0';

  fclose(file);

  cJSON *json = cJSON_Parse(data);
  if(!json) {
    free(data);
    return NULL;
  }
  free(data);

  const cJSON *theme = cJSON_GetObjectItemCaseSensitive(json, "theme");
  char *string = NULL;
  if(cJSON_IsString(theme)) {
    string = strdup(theme->valuestring);
  }

  cJSON_Delete(json);
  
  return string;
}

void set_theme_settings(const char *path, const char *name) {
  FILE *file = fopen(SETTINGS_PATH, "r");
  if(!file) {
    return;
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *data = malloc(size + 1);
  if(!data) {
    fclose(file);
    return;
  }

  fread(data, 1, size, file);
  data[size] = '\0';

  fclose(file);

  cJSON *json = cJSON_Parse(data);
  if(!json) {
    free(data);
    return;
  }
  free(data);
  
  cJSON_ReplaceItemInObjectCaseSensitive(json, "theme", cJSON_CreateString(name));

  cJSON_ReplaceItemInObjectCaseSensitive(json, "css_file", cJSON_CreateString(path));

  char *new = cJSON_Print(json);
  if(!new) {
    return;
  }

  cJSON_Delete(json);

  file = fopen(SETTINGS_PATH, "w");
  if(!file) {
    return;
  }

  fwrite(new, 1, strlen(new), file);
  fclose(file);
  free(new);
}
