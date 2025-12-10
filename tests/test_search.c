#include <criterion/criterion.h>
#include <glib.h>
#include <glib-object.h>
#include "../include/model.h"

/* Forward-declare GTK type used by wrapper to avoid including <gtk/gtk.h> here */
typedef struct _GtkListBoxRow GtkListBoxRow;

/* Wrapper deklaráció (implementáció a src/search.c-ben) */
gboolean search_filter_wrapper(GtkListBoxRow *row, gpointer user_data);

static TodoItem *make_item(const char *title) {
    TodoItem *t = g_new0(TodoItem, 1);
    t->title = g_strdup(title);
    t->description = g_strdup("");
    t->category = g_strdup("");
    t->priority = 0;
    t->completed = FALSE;
    return t;
}

Test(search_filter, null_query_shows_all) {
    GObject *row = g_object_new(G_TYPE_OBJECT, NULL);
    TodoItem *t = make_item("Foo");
    g_object_set_data(G_OBJECT(row), "todo_ptr", t);

    cr_assert(search_filter_wrapper((GtkListBoxRow*)row, NULL) == TRUE);

    /* cleanup */
    g_free(t->title); g_free(t->description); g_free(t->category); g_free(t);
    g_object_unref(row);
}

Test(search_filter, matches_case_insensitive) {
    GObject *row = g_object_new(G_TYPE_OBJECT, NULL);
    TodoItem *t = make_item("Hello World");
    g_object_set_data(G_OBJECT(row), "todo_ptr", t);

    cr_assert(search_filter_wrapper((GtkListBoxRow*)row, (gpointer)"hello") == TRUE);
    cr_assert(search_filter_wrapper((GtkListBoxRow*)row, (gpointer)"WORLD") == TRUE);

    g_free(t->title); g_free(t->description); g_free(t->category); g_free(t);
    g_object_unref(row);
}

Test(search_filter, no_match_returns_false) {
    GObject *row = g_object_new(G_TYPE_OBJECT, NULL);
    TodoItem *t = make_item("Alpha Beta");
    g_object_set_data(G_OBJECT(row), "todo_ptr", t);

    cr_assert(search_filter_wrapper((GtkListBoxRow*)row, (gpointer)"gamma") == FALSE);

    g_free(t->title); g_free(t->description); g_free(t->category); g_free(t);
    g_object_unref(row);
}