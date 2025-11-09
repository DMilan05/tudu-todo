#ifndef MODEL_H
#define MODEL_H

#include <glib.h>

typedef struct {
    gchar *title;        // A feladat címe
    gchar *description;  // A feladat részletes leírása
    gchar *category;     // Kategória vagy címke (opcionális)
    gint priority;       // Prioritás (0-10)
    gboolean completed;  // Kész van-e
} TodoItem;

GList* model_create_list(void);
void model_add_item(GList **list, TodoItem *item);
void model_remove_item(GList **list, gint index);
void model_free_item(TodoItem *item);
void model_free_list(GList *list);

#endif
