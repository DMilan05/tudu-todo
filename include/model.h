#ifndef MODEL_H
#define MODEL_H

#include <glib.h>

typedef struct {
    gchar *title;
    gchar *category;
    gint priority;
    gboolean completed;
} TodoItem;

GList* model_create_list(void);
void model_add_item(GList **list, TodoItem *item);
void model_remove_item(GList **list, gint index);
void model_free_item(TodoItem *item);
void model_free_list(GList *list);

#endif
