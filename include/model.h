#ifndef MODEL_H
#define MODEL_H

#include <gtk/gtk.h>

// Egyetlen TODO elem
typedef struct {
    char *text;
} TodoItem;

// A teljes alkalmazás modellje
typedef struct {
    GList *items; // Dinamikus lista a feladatokról
} TodoModel;

// Modell inicializálása
TodoModel* model_new(void);

// Új elem hozzáadása
void model_add_item(TodoModel *model, const char *text);

// Elem törlése (index alapján)
void model_remove_item(TodoModel *model, int index);

// A GTK listához szinkronizálás (View frissítés)
void model_refresh_list(TodoModel *model, GtkListBox *list);

#endif
