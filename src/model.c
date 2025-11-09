#include "model.h"
#include <stdlib.h>

GList* model_create_list(void) {
    return NULL;
}

void model_add_item(GList **list, TodoItem *item) {
    *list = g_list_append(*list, item);
}

void model_remove_item(GList **list, gint index) {
    if (!*list) return;
    *list = g_list_remove(*list, g_list_nth_data(*list, index));
}

void model_free_item(TodoItem *item) {
    g_free(item->title);
    g_free(item->category);
    g_free(item);
}

void model_free_list(GList *list) {
    g_list_free_full(list, (GDestroyNotify)model_free_item);
}
