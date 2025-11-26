#include "model.h"

GList* model_create_list(void) {
    return NULL;
}

void model_add_item(GList **list, TodoItem *item) {
    *list = g_list_append(*list, item);
}

void model_remove_item(GList **list, gint index) {
    GList *nth = g_list_nth(*list, index);
    if (nth) {
        TodoItem *item = nth->data;
        *list = g_list_remove_link(*list, nth);
        model_free_item(item);
        g_list_free(nth);
    }
}

void model_free_item(TodoItem *item) {
    if (item) {
        g_free(item->title);
        g_free(item->description);
        g_free(item->category);
        g_free(item);
    }
}

// A teljes lista felszabadítása (kilépéskor hívandó)
void model_free_list(GList *list) {
    g_list_free_full(list, (GDestroyNotify)model_free_item);
}
