#include "search.h"
#include "view.h"
#include <string.h>

GList *search_filter(GList *list, const gchar *query)
{
    if (!query || !*query)
        return list; // Ha üres a kereső, az eredeti listát adjuk vissza

    GList *filtered = NULL;
    for (GList *l = list; l != NULL; l = l->next)
    {
        TodoItem *item = l->data;

        // MÓDOSÍTVA: g_strcmp0 helyett strstr a részleges egyezéshez
        // (Figyelem: ez case-sensitive, azaz kis-nagybetű érzékeny)
        if (strstr(item->title, query) != NULL)
        {
            filtered = g_list_append(filtered, item);
        }
    }
    return filtered; // Az új, szűrt listát adjuk vissza
}

static void on_search_changed(GtkSearchEntry *entry, gpointer user_data)
{
    GList **todos_ptr = (GList **)user_data;
    GList *todos = *todos_ptr; // Kiolvassuk az aktuális listát

    const gchar *query = gtk_editable_get_text(GTK_EDITABLE(entry));

    GList *filtered = search_filter(todos, query);

    view_refresh_list(filtered);

    if (filtered != todos)
    {
        g_list_free(filtered);
    }
}

// GList *todos helyett GList **todos_ptr
void search_init(GtkSearchEntry *search_entry, GtkListBox *list_box, GList **todos_ptr)
{
    // A 'todos' pointer CÍMÉT adjuk át a callback-nek
    g_signal_connect(search_entry, "search-changed",
                     G_CALLBACK(on_search_changed),
                     todos_ptr); // Itt már a pointer címét adjuk át
}
