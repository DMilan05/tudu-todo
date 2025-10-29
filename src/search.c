#include "search.h"
#include "view.h"   // hogy lehessen friss�teni a list�t
#include <string.h>

// Sz�r� f�ggv�ny � pontos egyez�sre sz�r
GList* search_filter(GList *list, const gchar *query) {
    if (!query || !*query) return list;

    GList *filtered = NULL;
    for (GList *l = list; l != NULL; l = l->next) {
        TodoItem *item = l->data;
        if (g_strcmp0(item->title, query) == 0) {
            filtered = g_list_append(filtered, item);
        }
    }
    return filtered;
}

// Callback f�ggv�ny a keres�shez
static void on_search_changed(GtkSearchEntry *entry, gpointer user_data) {
    GList *todos = (GList *)user_data;
    const gchar *query = gtk_editable_get_text(GTK_EDITABLE(entry));
    GList *filtered = search_filter(todos, query);

    // Friss�tj�k a n�zetet
    view_refresh_list(filtered);

    // A filtered list�t nem szabad felszabad�tani, mert csak pointereket tartalmaz
}

// Inicializ�l� f�ggv�ny � bek�ti a jelet
void search_init(GtkSearchEntry *search_entry, GtkListBox *list_box, GList *todos) {
    g_signal_connect(search_entry, "search-changed",
                     G_CALLBACK(on_search_changed),
                     todos);
}
