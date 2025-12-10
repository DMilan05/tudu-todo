#include <criterion/criterion.h>
#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>
#include "../include/model.h"

/* Deklarációk a controller statikus függvényeinek láthatóvá tételéhez (-Dstatic= fordításnál) */
gint controller_sort_by_priority(gconstpointer a, gconstpointer b);
gint controller_sort_by_title(gconstpointer a, gconstpointer b);

/* Segédfüggvény élő TodoItem készítéséhez */
static TodoItem *make_item(const char *title, int prio) {
    TodoItem *t = g_new0(TodoItem, 1);
    t->title = g_strdup(title);
    t->description = g_strdup("");
    t->category = g_strdup("");
    t->priority = prio;
    t->completed = FALSE;
    return t;
}

Test(controller_sort, priority_descending) {
    GList *l = NULL;
    TodoItem *a = make_item("A", 1);
    TodoItem *b = make_item("B", 5);
    TodoItem *c = make_item("C", 3);

    l = g_list_append(l, a);
    l = g_list_append(l, b);
    l = g_list_append(l, c);

    l = g_list_sort(l, (GCompareFunc)controller_sort_by_priority);

    TodoItem *first = (TodoItem *)l->data;
    cr_assert_eq(first->priority, 5, "Első elem prioritása 5 kell legyen");

    for (GList *it = l; it; it = it->next) {
        TodoItem *ti = it->data;
        g_free(ti->title); g_free(ti->description); g_free(ti->category); g_free(ti);
    }
    g_list_free(l);
}

Test(controller_sort, title_lexicographic) {
    GList *l = NULL;
    TodoItem *a = make_item("beta", 1);
    TodoItem *b = make_item("alpha", 1);
    l = g_list_append(l, a);
    l = g_list_append(l, b);

    l = g_list_sort(l, (GCompareFunc)controller_sort_by_title);

    TodoItem *first = (TodoItem *)l->data;
    cr_assert_str_eq(first->title, "alpha", "Első elem címe 'alpha' kell legyen");

    for (GList *it = l; it; it = it->next) {
        TodoItem *ti = it->data;
        g_free(ti->title); g_free(ti->description); g_free(ti->category); g_free(ti);
    }
    g_list_free(l);
}