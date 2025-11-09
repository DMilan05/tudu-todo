#include "view.h"

static GtkWidget *add_button;
static GtkWidget *entry_field;
static GtkWidget *list_box;

GtkWidget* view_create_main_window(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "ToDo App");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), box);

    entry_field = gtk_entry_new();
    gtk_box_append(GTK_BOX(box), entry_field);

    add_button = gtk_button_new_with_label("Add");
    gtk_box_append(GTK_BOX(box), add_button);

    list_box = gtk_list_box_new();
    gtk_box_append(GTK_BOX(box), list_box);

    return window;
}

GtkWidget* view_get_add_button(void) { return add_button; }
GtkWidget* view_get_entry_field(void) { return entry_field; }
GtkWidget* view_get_list_box(void) { return list_box; }

void view_refresh_list(GList *list) {
    gtk_list_box_remove_all(GTK_LIST_BOX(list_box));
    for (GList *l = list; l != NULL; l = l->next) {
        TodoItem *item = l->data;
        GtkWidget *row = gtk_label_new(item->title);
        gtk_list_box_append(GTK_LIST_BOX(list_box), row);
    }
}
