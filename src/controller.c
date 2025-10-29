#include "controller.h"
#include "view.h"
#include "model.h"
#include "io.h"

static GList *todos = NULL;

void controller_on_add_clicked(GtkButton *button, gpointer user_data) {
    const gchar *text = gtk_editable_get_text(GTK_EDITABLE(view_get_entry_field()));
    if (text && *text) {
        TodoItem *item = g_new0(TodoItem, 1);
        item->title = g_strdup(text);
        item->priority = 1;
        model_add_item(&todos, item);
        view_refresh_list(todos);
    }
}

void controller_on_delete_clicked(GtkButton *button, gpointer user_data) {
    // TODO: implement deletion
}

void controller_on_save_clicked(GtkButton *button, gpointer user_data) {
    io_save_list("todos.csv", todos);
}

void controller_init(GtkWidget *window) {
    g_signal_connect(view_get_add_button(), "clicked", G_CALLBACK(controller_on_add_clicked), NULL);
}
