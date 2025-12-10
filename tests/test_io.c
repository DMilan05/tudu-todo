#include <criterion/criterion.h>
#include <glib.h>
#include <glib/gstdio.h>
#include "../include/io.h"
#include "../include/model.h"

/* Segédfüggvény TodoItem készítéséhez */
static TodoItem *make_item(const char *title, const char *desc, const char *cat, int prio, gboolean done) {
    TodoItem *t = g_new0(TodoItem, 1);
    t->title = g_strdup(title);
    t->description = g_strdup(desc);
    t->category = g_strdup(cat);
    t->priority = prio;
    t->completed = done;
    return t;
}

/* Törli egy lista elemeit és magukat az elemeket */
static void free_list_items(GList *l) {
    for (GList *it = l; it; it = it->next) {
        TodoItem *ti = it->data;
        if (!ti) continue;
        if (ti->title) g_free(ti->title);
        if (ti->description) g_free(ti->description);
        if (ti->category) g_free(ti->category);
        g_free(ti);
    }
    g_list_free(l);
}

Test(io_roundtrip, save_and_load_roundtrip) {
    gchar *tmp = g_build_filename(g_get_tmp_dir(), "tudu_io_test.tsv", NULL);
    /* építünk egy listát */
    GList *list = NULL;
    TodoItem *a = make_item("One\tTwo", "Desc\nLine", "cat", 3, FALSE);
    TodoItem *b = make_item("Title2", "Desc2", "cat2", 7, TRUE);
    list = g_list_append(list, a);
    list = g_list_append(list, b);

    /* mentés és felszabadítás az eredeti listából (io_save_to_file nem fogyasztja az elemeket) */
    io_save_to_file(tmp, list);

    free_list_items(list);
    list = NULL;

    /* betöltés */
    GList *loaded = io_load_from_file(tmp);
    cr_assert_not_null(loaded);
    cr_assert_eq(g_list_length(loaded), 2);

    TodoItem *la = (TodoItem *)g_list_nth_data(loaded, 0);
    TodoItem *lb = (TodoItem *)g_list_nth_data(loaded, 1);
    cr_assert_str_eq(la->title, "One Two"); /* sanitize_string cseréli a tabot szóközre */
    cr_assert_eq(la->priority, 3);
    cr_assert_eq(lb->completed, TRUE);
    /* cleanup */
    free_list_items(loaded);
    g_remove(tmp);
    g_free(tmp);
}

Test(io_load, nonexistent_file_returns_null) {
    gchar *tmp = g_build_filename(g_get_tmp_dir(), "tudu_io_test_nonexistent.tsv", NULL);
    /* biztosítsuk, hogy nincs ott */
    g_remove(tmp);
    GList *l = io_load_from_file(tmp);
    cr_assert_null(l);
    g_free(tmp);
}