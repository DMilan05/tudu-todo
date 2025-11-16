#include "search.h"
#include "view.h" // hogy lehessen frissíteni a listát
#include <glib.h> // JAVÍTÁS: Hiányzó header a g_strcasestr függvényhez
#include <string.h>
#include <glib/gi18n.h>

/*
 * A GtkListBox szűrőfüggvénye.
 * Akkor ad vissza TRUE-t (megjeleníti a sort), ha a keresési szöveg
 * üres, vagy ha a sorhoz tartozó feladat címe tartalmazza a keresési szöveget.
 * A keresés nem különbözteti meg a kis- és nagybetűket.
 */
static gboolean search_filter_func(GtkListBoxRow *row, gpointer user_data)
{
    const gchar *query = (const gchar *)user_data;
    TodoItem *item = g_object_get_data(G_OBJECT(row), "todo_ptr");

    // Ha nincs keresési szöveg, vagy nincs elem a sorhoz rendelve, mindent megjelenítünk
    if (!query || !*query || !item || !item->title)
    {
        return TRUE;
    }

    // Részleges, kis-nagybetűre nem érzékeny keresés
    // A g_strcasestr helyett egy robusztusabb, Unicode-kompatibilis megoldást használunk,
    // ami kiküszöböli a régebbi GLib verziókkal kapcsolatos hibákat.
    gchar *title_folded = g_utf8_casefold(item->title, -1);
    gchar *query_folded = g_utf8_casefold(query, -1);

    gboolean found = (g_strstr_len(title_folded, -1, query_folded) != NULL);

    g_free(title_folded);
    g_free(query_folded);

    return found;
}

/*
 * Callback függvény, ami lefut, amikor a keresőmező tartalma megváltozik.
 * Érvényteleníti a lista szűrését, ami a search_filter_func újrafuttatását eredményezi.
 */
static void on_search_changed(GtkSearchEntry *entry, GtkListBox *list_box)
{
    const gchar *query = gtk_editable_get_text(GTK_EDITABLE(entry));
    gtk_list_box_set_filter_func(list_box, search_filter_func, (gpointer)query, NULL);
}

/* Inicializáló függvény – beköti a "search-changed" jelet */
void search_init(GtkSearchEntry *search_entry, GtkListBox *list_box)
{
    g_signal_connect(search_entry, "search-changed", G_CALLBACK(on_search_changed), list_box);
}
