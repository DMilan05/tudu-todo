#include "view.h"

static GtkWidget *add_button;
static GtkWidget *entry_field;
static GtkWidget *list_box;
static GtkWidget *delete_button;
static GtkWidget *search_entry;

GtkWidget *view_create_main_window(GtkApplication *app)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "ToDo App");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // Fő függőleges doboz
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    // Vízszintes doboz a beviteli mezőnek és a gomboknak
    GtkWidget *hbox_controls = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(vbox), hbox_controls);

    entry_field = gtk_entry_new();
    // A beviteli mező kitölti a rendelkezésre álló helyet a gombok mellett
    gtk_widget_set_hexpand(entry_field, TRUE);
    gtk_box_append(GTK_BOX(hbox_controls), entry_field);

    add_button = gtk_button_new_with_label("Add");
    gtk_box_append(GTK_BOX(hbox_controls), add_button);

    delete_button = gtk_button_new_with_label("Delete"); // ÚJ GOMB
    gtk_box_append(GTK_BOX(hbox_controls), delete_button);

    search_entry = gtk_search_entry_new();
    gtk_box_append(GTK_BOX(vbox), search_entry);

    list_box = gtk_list_box_new();
    // A listbox foglalja el a maradék helyet
    gtk_widget_set_vexpand(list_box, TRUE);
    gtk_box_append(GTK_BOX(vbox), list_box);

    return window;
}

GtkWidget *view_get_add_button(void) { return add_button; }
GtkWidget *view_get_entry_field(void) { return entry_field; }
GtkWidget *view_get_list_box(void) { return list_box; }
GtkWidget *view_get_delete_button(void) { return delete_button; }
GtkWidget *view_get_search_entry(void) { return search_entry; }

gint view_get_selected_index(void)
{
    GtkListBoxRow *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(list_box));
    if (row)
    {
        return gtk_list_box_row_get_index(row);
    }
    return -1;
}

void view_refresh_list(GList *list)
{
    gtk_list_box_remove_all(GTK_LIST_BOX(list_box));
    for (GList *l = list; l != NULL; l = l->next)
    {
        TodoItem *item = l->data;
        GtkWidget *row = gtk_label_new(item->title);
        gtk_list_box_append(GTK_LIST_BOX(list_box), row);
    }
}
