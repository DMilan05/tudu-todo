#include "model.h"

TodoModel* model_new(void) {
    TodoModel *model = g_new(TodoModel, 1);
    model->items = NULL;
    return model;
}

void model_add_item(TodoModel *model, const char *text) {
    if (!text || !*text) return;

    TodoItem *item = g_new(TodoItem, 1);
    item->text = g_strdup(text);
    model->items = g_list_append(model->items, item);
}

void model_remove_item(TodoModel *model, int index) {
    if (!model || index < 0) return;

    GList *node = g_list_nth(model->items, index);
    if (node) {
        TodoItem *item = node->data;
        g_free(item->text);
        g_free(item);
        model->items = g_list_delete_link(model->items, node);
    }
}

// --- A lista frissítése a GUI-ban ---
void model_refresh_list(TodoModel *model, GtkListBox *list) {
    gtk_list_box_remove_all(list);

    for (GList *l = model->items; l != NULL; l = l->next) {
        TodoItem *item = l->data;
        GtkWidget *row = gtk_label_new(item->text);
        gtk_list_box_append(list, row);
    }
}
