#include <criterion/criterion.h>
#include <glib.h>
#include "../include/model.h"

static TodoItem *make_item(const char *title, int prio) {
    TodoItem *t = g_new0(TodoItem, 1);
    t->title = g_strdup(title);
    t->description = g_strdup("");
    t->category = g_strdup("");
    t->priority = prio;
    t->completed = FALSE;
    return t;
}

Test(model_create, returns_null_initially) {
    GList *l = model_create_list();
    cr_assert_null(l);
}

Test(model_add, appends_item) {
    GList *l = NULL;
    TodoItem *t = make_item("foo", 3);
    model_add_item(&l, t);
    cr_assert_eq(g_list_length(l), 1);
    TodoItem *first = (TodoItem*)g_list_nth_data(l, 0);
    cr_assert_str_eq(first->title, "foo");
    /* cleanup */
    model_free_list(l);
}

Test(model_remove, removes_and_keeps_rest) {
    GList *l = NULL;
    TodoItem *a = make_item("a", 1);
    TodoItem *b = make_item("b", 2);
    model_add_item(&l, a);
    model_add_item(&l, b);
    cr_assert_eq(g_list_length(l), 2);
    model_remove_item(&l, 0); /* remove first (a) */
    cr_assert_eq(g_list_length(l), 1);
    TodoItem *rem = (TodoItem*)g_list_nth_data(l, 0);
    cr_assert_str_eq(rem->title, "b");
    model_free_list(l);
}

Test(model_free_list, does_not_crash_on_free) {
    GList *l = NULL;
    model_add_item(&l, make_item("x", 0));
    model_add_item(&l, make_item("y", 1));
    /* should not crash */
    model_free_list(l);
}