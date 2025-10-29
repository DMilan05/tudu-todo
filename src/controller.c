#include "controller.h"
#include "view.h"
#include "model.h"
#include "io.h"
#include "search.h"

static GList *todos = NULL;

void controller_on_add_clicked(GtkButton *button, gpointer user_data)
{
    const gchar *text = gtk_editable_get_text(GTK_EDITABLE(view_get_entry_field()));
    if (text && *text)
    {
        TodoItem *item = g_new0(TodoItem, 1);
        item->title = g_strdup(text);
        item->priority = 1;
        model_add_item(&todos, item);
        view_refresh_list(todos);
    }
}

void controller_on_delete_clicked(GtkButton *button, gpointer user_data)
{
    gint index = view_get_selected_index();
    if (index >= 0)
    {
        TodoItem *item_to_free = g_list_nth_data(todos, index);
        model_remove_item(&todos, index);
        model_free_item(item_to_free);
        view_refresh_list(todos);
    }
}

void controller_on_save_clicked(GtkButton *button, gpointer user_data)
{
    io_save_list("todos.csv", todos);
}

void controller_init(GtkWidget *window)
{
    // Add gomb bekötése
    g_signal_connect(view_get_add_button(), "clicked", G_CALLBACK(controller_on_add_clicked), NULL);

    // Delete gomb bekötése – ÚJ BEKÖTÉS
    g_signal_connect(view_get_delete_button(), "clicked", G_CALLBACK(controller_on_delete_clicked), NULL);
    // ÚJ: Kereső inicializálása
    // Átadjuk neki a widgeteket és a 'todos' pointert, amit a search modul kezelni fog
    // ÚJ: Kereső inicializálása
    GtkWidget *search_widget = view_get_search_entry();
    GtkWidget *list_widget = view_get_list_box();

    // A 'todos' pointer CÍMÉT adjuk át (&todos)
    search_init(GTK_SEARCH_ENTRY(search_widget),
                GTK_LIST_BOX(list_widget),
                &todos);
}
