#ifndef UTILS_H 
#define UTILS_H 

#include <gtk/gtk.h>
#include "regex.h"
#include "auxiliares.h"
#include "cliente.h"
#include "cjson/cJSON.h"

typedef struct {
  GtkWidget *button;
  GtkWidget *box;
  GtkWidget *image;
  GtkWidget *label;
} ST_BUTTON;

typedef enum {
  BUTTON_ORIENTATION_HORIZONTAL,
  BUTTON_ORIENTATION_VERTICAL,
} BUTTON_ORIENTATION;

typedef enum {
  BUTTON_POSITION_FIRST_LABEL,
  BUTTON_POSITION_FIRST_IMAGE,
} BUTTON_POSITION;

typedef enum {
  CARD_COLOR_BLUE,    // #2196F3
  CARD_COLOR_GREEN,   // #4CAF50
  CARD_COLOR_ORANGE,  // #ED8936
  CARD_COLOR_RED,     // #E53E3E
  CARD_COLOR_PURPLE,  // #805AD5 
  CARD_COLOR_YELLOW,  // #ECC94B 
  CARD_COLOR_CYAN,    // #319795
} CARD_COLOR;

typedef enum {
  SEARCH_BY_INVALID,
  SEARCH_BY_ID,
  SEARCH_BY_EMAIL,
  SEARCH_BY_NIF,
  SEARCH_BY_SNS,
  SEARCH_BY_DATE,
  SEARCH_BY_POSTAL_CODE,
  SEARCH_BY_LICENSE_NUMBER,
  SEARCH_BY_SPECIALITY
} SEARCH_TYPE;

void createButtonWithImageLabel(ST_BUTTON *button, const char *pathToImage, const char *text, BUTTON_ORIENTATION orientation, BUTTON_POSITION position);
GtkWidget *createAnalyticCard(CARD_COLOR color, const char *pathToImage, const char *title, const char *info, const char *subtitle);
const char **getCardColorClass(CARD_COLOR color);
SEARCH_TYPE detectSearchType(const char *input);
GtkStringList *loadSpecialty();
bool validationTypeSizeDimensions(GFile *file);
void clearStackPages(GtkWidget *stack);
char *get_theme_css_file(void);

#endif
