#include <gtk/gtk.h>
#include "controller.h"
#include "view.h"
#include "model.h"
#include "io.h"
#include "search.h"

// Globális lista a teendőkhöz.
GList *todos = NULL;

// Helper függvény a g_signal_connect_data által átadott widget tömb felszabadításához.
static void free_widgets_array(gpointer data, GClosure *closure)
{
    g_free(data);
}

// Eseménykezelő: a "Hozzáadás" ablak "Mentés" gombja.
static void on_add_save_clicked(GtkButton *button, gpointer user_data)
{
    gpointer *widgets = (gpointer *)user_data;
    GtkWidget *title_entry = GTK_WIDGET(widgets[0]);
    GtkWidget *prio_spin = GTK_WIDGET(widgets[1]);
    GtkWidget *window = GTK_WIDGET(widgets[2]);

    const gchar *title = gtk_editable_get_text(GTK_EDITABLE(title_entry));
    gint priority = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(prio_spin));

    if (!title || g_strcmp0(title, "") == 0)
    {
        GtkAlertDialog *dialog = gtk_alert_dialog_new("A cím nem lehet üres!");
        gtk_alert_dialog_show(dialog, GTK_WINDOW(window));
        g_object_unref(dialog);
        return;
    }

    TodoItem *item = g_new0(TodoItem, 1);
    item->title = g_strdup(title);
    item->description = g_strdup("");
    item->category = g_strdup("");
    item->priority = priority;
    item->completed = FALSE;

    model_add_item(&todos, item);
    view_refresh_list(todos);

    gtk_window_close(GTK_WINDOW(window));
}

// Eseménykezelő: a főablak "Hozzáadás" gombja. Megnyitja a hozzáadó ablakot.
void controller_on_add_clicked(GtkButton *button, gpointer user_data)
{
    GtkWindow *parent = GTK_WINDOW(user_data);
    GtkWidget *add_window = view_create_add_window(parent);

    GtkWidget *title_entry = g_object_get_data(G_OBJECT(add_window), "title_entry");
    GtkWidget *prio_spin = g_object_get_data(G_OBJECT(add_window), "prio_spin");
    GtkWidget *save_btn = g_object_get_data(G_OBJECT(add_window), "save_btn");

    gpointer *widgets = g_new(gpointer, 3);
    widgets[0] = title_entry;
    widgets[1] = prio_spin;
    widgets[2] = add_window;

    g_signal_connect_data(save_btn, "clicked", G_CALLBACK(on_add_save_clicked), widgets, free_widgets_array, 0);

    gtk_window_present(GTK_WINDOW(add_window));
}

// Eseménykezelő: a "Szerkesztés" ablak "Mentés" gombja.
static void on_edit_save_clicked(GtkButton *button, gpointer user_data)
{
    gpointer *widgets = (gpointer *)user_data;
    GtkWidget *title_entry = GTK_WIDGET(widgets[0]);
    GtkWidget *prio_spin = GTK_WIDGET(widgets[1]);
    GtkWidget *done_check = GTK_WIDGET(widgets[2]);
    GtkWidget *window = GTK_WIDGET(widgets[3]);
    TodoItem *item = (TodoItem *)widgets[4];

    const gchar *title = gtk_editable_get_text(GTK_EDITABLE(title_entry));
    gint priority = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(prio_spin));
    gboolean done = gtk_check_button_get_active(GTK_CHECK_BUTTON(done_check));

    if (!title || g_strcmp0(title, "") == 0)
    {
        GtkAlertDialog *dialog = gtk_alert_dialog_new("A cím nem lehet üres!");
        gtk_alert_dialog_show(dialog, GTK_WINDOW(window));
        g_object_unref(dialog);
        return;
    }

    g_free(item->title);
    g_free(item->description);

    item->title = g_strdup(title);
    item->description = g_strdup("");
    item->priority = priority;
    item->completed = done;

    view_refresh_list(todos);
    gtk_window_close(GTK_WINDOW(window));
}

// Eseménykezelő: a főablak "Szerkesztés" gombja. Megnyitja a szerkesztő ablakot.
void controller_on_edit_clicked(GtkButton *button, gpointer user_data)
{
    GtkWindow *parent = GTK_WINDOW(user_data);
    GtkListBoxRow *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(view_get_list_box()));
    if (!row) return;

    TodoItem *item = g_object_get_data(G_OBJECT(row), "todo_ptr");
    if (!item) return;

    GtkWidget *edit_window = view_create_edit_window(parent, item);
    GtkWidget *title_entry = g_object_get_data(G_OBJECT(edit_window), "title_entry");
    GtkWidget *prio_spin = g_object_get_data(G_OBJECT(edit_window), "prio_spin");
    GtkWidget *done_check = g_object_get_data(G_OBJECT(edit_window), "done_check");
    GtkWidget *save_btn = g_object_get_data(G_OBJECT(edit_window), "save_btn");

    gpointer *widgets = g_new(gpointer, 5);
    widgets[0] = title_entry;
    widgets[1] = prio_spin;
    widgets[2] = done_check;
    widgets[3] = edit_window;
    widgets[4] = item;

    g_signal_connect_data(save_btn, "clicked", G_CALLBACK(on_edit_save_clicked), widgets, free_widgets_array, 0);

    gtk_window_present(GTK_WINDOW(edit_window));
}

// Eseménykezelő: a "Kész" gomb. Megfordítja a kijelölt elem állapotát.
void controller_on_mark_done_clicked(GtkButton *button, gpointer user_data)
{
    GtkListBoxRow *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(view_get_list_box()));
    if (!row) return;

    TodoItem *item = g_object_get_data(G_OBJECT(row), "todo_ptr");
    if (!item) return;

    item->completed = !item->completed;
    view_refresh_list(todos);
}

// Eseménykezelő: a "Törlés" gomb. Törli a kijelölt elemet.
void controller_on_delete_clicked(GtkButton *button, gpointer user_data)
{
    GtkListBoxRow *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(view_get_list_box()));
    if (!row) return;

    TodoItem *item = g_object_get_data(G_OBJECT(row), "todo_ptr");
    if (!item) return;

    todos = g_list_remove(todos, item);
    model_free_item(item);
    view_refresh_list(todos);
}

// Eseménykezelő: a "Kész elemek törlése" gomb.
void controller_on_delete_completed_clicked(GtkButton *button, gpointer user_data)
{
    GList *items_to_delete = NULL;
    for (GList *l = todos; l != NULL; l = l->next)
    {
        TodoItem *item = (TodoItem *)l->data;
        if (item->completed)
        {
            items_to_delete = g_list_append(items_to_delete, item);
        }
    }

    for (GList *l = items_to_delete; l != NULL; l = l->next)
    {
        todos = g_list_remove(todos, l->data);
        model_free_item((TodoItem *)l->data);
    }
    g_list_free(items_to_delete);
    view_refresh_list(todos);
}

// Rendezési segédfüggvény: prioritás szerint.
static gint sort_by_priority(gconstpointer a, gconstpointer b)
{
    return ((const TodoItem *)b)->priority - ((const TodoItem *)a)->priority;
}

// Rendezési segédfüggvény: cím szerint (ABC).
static gint sort_by_title(gconstpointer a, gconstpointer b)
{
    return g_utf8_collate(((const TodoItem *)a)->title, ((const TodoItem *)b)->title);
}

// Eseménykezelő: a rendezési legördülő menü megváltozása.
void controller_on_sort_changed(GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data)
{
    guint selected = gtk_drop_down_get_selected(dropdown);
    if (selected == 1)
    {
        todos = g_list_sort(todos, (GCompareFunc)sort_by_title);
    }
    else if (selected == 2)
    {
        todos = g_list_sort(todos, (GCompareFunc)sort_by_priority);
    }
    view_refresh_list(todos);
}

// Eseménykezelő: a "Mentés" gomb. Elmenti a listát a fájlba.
void controller_on_save_clicked(GtkButton *button, gpointer user_data)
{
    io_save_to_file("todos.csv", todos);
}

// Callback: a bezárási párbeszédablak válaszát dolgozza fel.
static void on_close_dialog_response(GObject *source, GAsyncResult *res, gpointer user_data)
{
    GtkWindow *window = GTK_WINDOW(user_data);
    gint choice = gtk_alert_dialog_choose_finish(GTK_ALERT_DIALOG(source), res, NULL);

    switch (choice)
    {
    case 0: // Mentés
        io_save_to_file("todos.csv", todos);
        gtk_window_destroy(window);
        break;
    case 1: // Bezárás mentés nélkül
        gtk_window_destroy(window);
        break;
    case 2: // Mégse
    default:
        // Nem történik semmi, az ablak nyitva marad.
        break;
    }
}

// Eseménykezelő: az ablak bezárási kérelme (pl. X gomb).
gboolean controller_on_close_request(GtkWindow *window, gpointer user_data)
{
    G_GNUC_UNUSED gpointer unused_user_data = user_data;
    GtkAlertDialog *dialog = gtk_alert_dialog_new("Menti a változtatásokat?");
    const gchar *buttons[] = {"Mentés", "Bezárás mentés nélkül", "Mégse", NULL};
    gtk_alert_dialog_set_buttons(dialog, buttons);

    // Aszinkron módon jelenítjük meg a párbeszédablakot, és a választ
    // a 'on_close_dialog_response' callback kezeli.
    gtk_alert_dialog_choose(dialog, window, NULL, on_close_dialog_response, window);
    
    g_object_unref(dialog);

    // TRUE-t adunk vissza, hogy megakadályozzuk az ablak azonnali bezárását.
    // A bezárást a callback fogja végezni a felhasználó döntése után.
    return TRUE;
}

// Inicializálja a controllert.
void controller_init(GtkWidget *window)
{
    todos = io_load_from_file("todos.csv");
    view_refresh_list(todos);

    g_signal_connect(view_get_add_button(), "clicked", G_CALLBACK(controller_on_add_clicked), window);
    g_signal_connect(view_get_edit_button(), "clicked", G_CALLBACK(controller_on_edit_clicked), window);
    g_signal_connect(view_get_delete_button(), "clicked", G_CALLBACK(controller_on_delete_clicked), window);
    g_signal_connect(view_get_mark_done_button(), "clicked", G_CALLBACK(controller_on_mark_done_clicked), window);
    g_signal_connect(view_get_save_button(), "clicked", G_CALLBACK(controller_on_save_clicked), window);
    g_signal_connect(view_get_delete_completed_button(), "clicked", G_CALLBACK(controller_on_delete_completed_clicked), NULL);
    g_signal_connect(view_get_sort_dropdown(), "notify::selected", G_CALLBACK(controller_on_sort_changed), NULL);

    search_init(GTK_SEARCH_ENTRY(view_get_search_entry()), GTK_LIST_BOX(view_get_list_box()));
}

// Az alkalmazás leállításakor felszabadítja a 'todos' listát.
void controller_shutdown(void)
{
    model_free_list(todos);
}
