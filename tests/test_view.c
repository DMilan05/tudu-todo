#include <criterion/criterion.h>
#include <glib.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include "../include/model.h"
#include "../include/view.h"

static TodoItem *make_item(const char *title, int prio) {
    TodoItem *t = g_new0(TodoItem, 1);
    t->title = g_strdup(title);
    t->description = g_strdup("");
    t->category = g_strdup("");
    t->priority = prio;
    t->completed = FALSE;
    return t;
}

Test(view_create_and_refresh, create_main_window_and_refresh_list) {
    /* Initialize GTK (do not override DBUS env here) */
    gtk_init();

    GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
    /* register app to emit startup so new windows can be created */
    gboolean ok = g_application_register(G_APPLICATION(app), NULL, NULL);
    cr_assert(ok, "g_application_register failed");

    GtkWidget *win = view_create_main_window(app);
    cr_assert_not_null(win);

    /* építünk egy listát */
    GList *l = NULL;
    TodoItem *a = make_item("One", 2);
    TodoItem *b = make_item("Two", 5);
    l = g_list_append(l, a);
    l = g_list_append(l, b);

    view_refresh_list(l);

    GtkWidget *list_box = view_get_list_box();
    cr_assert_not_null(list_box);

    /* számoljuk meg a sorokat */
    int cnt = 0;
    GtkWidget *child = gtk_widget_get_first_child(list_box);
    while (child) {
        cnt++;
        /* ellenőrizzük, hogy a todo_ptr benne van */
        gpointer p = g_object_get_data(G_OBJECT(child), "todo_ptr");
        cr_assert_not_null(p);
        child = gtk_widget_get_next_sibling(child);
    }
    cr_assert_eq(cnt, 2);

    /* cleanup */
    model_free_list(l);
    g_object_unref(win);
    g_object_unref(app);
}

Test(view_windows_singleton, add_and_edit_window_singleton) {
    gtk_init();

    GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
    gboolean ok = g_application_register(G_APPLICATION(app), NULL, NULL);
    cr_assert(ok, "g_application_register failed");

    GtkWidget *win = view_create_main_window(app);

    GtkWidget *add1 = view_create_add_window(GTK_WINDOW(win));
    GtkWidget *add2 = view_create_add_window(GTK_WINDOW(win));
    cr_assert(add1 == add2, "add windows should be same pointer");

    TodoItem *t = make_item("EditMe", 3);
    GtkWidget *edit1 = view_create_edit_window(GTK_WINDOW(win), t);
    GtkWidget *edit2 = view_create_edit_window(GTK_WINDOW(win), t);
    cr_assert(edit1 == edit2, "edit windows should be same pointer");

    /* cleanup */
    model_free_item(t);
    g_object_unref(win);
    g_object_unref(app);
}