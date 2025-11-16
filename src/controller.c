#include <gtk/gtk.h>
#include "controller.h"
#include "view.h"
#include "model.h"
#include "io.h"     // <- FONTOS: IO beillesztése
#include "search.h" // JAVÍTÁS: Hiányzó include a search_init függvényhez

GList *todos = NULL;

/* --- Helper függvény a user_data felszabadításához --- */
static void free_widgets_array(gpointer data, GClosure *closure)
{
    g_free(data);
}

/* --- Mentés gomb az új feladat ablakban --- */
// (Ez a függvény rendben volt, változatlan)
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
        const gchar *buttons[] = {"OK", NULL};
        gtk_alert_dialog_set_buttons(dialog, buttons);
        gtk_alert_dialog_show(dialog, GTK_WINDOW(window));
        g_object_unref(dialog);
        return;
    }

    TodoItem *item = g_new0(TodoItem, 1);
    item->title = g_strdup(title);
    item->description = g_strdup(""); // Nincs leírás mező
    item->category = g_strdup("");
    item->priority = priority;
    item->completed = FALSE;

    model_add_item(&todos, item);
    view_refresh_list(todos);

    gtk_window_close(GTK_WINDOW(window));
}

/* --- Hozzáadás gomb --- */
// (Ez a függvény rendben volt, változatlan)
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

    g_signal_connect_data(save_btn,
                          "clicked",
                          G_CALLBACK(on_add_save_clicked),
                          widgets,
                          free_widgets_array,
                          0);

    gtk_window_present(GTK_WINDOW(add_window));
}

/* --- Szerkesztés mentése --- */
// (Ez a függvény rendben volt, változatlan)
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
        const gchar *buttons[] = {"OK", NULL};
        gtk_alert_dialog_set_buttons(dialog, buttons);
        gtk_alert_dialog_show(dialog, GTK_WINDOW(window));
        g_object_unref(dialog);
        return;
    }

    if (done)
    {
        // Töröljük az elemet
        todos = g_list_remove(todos, item);
        model_free_item(item);
    }
    else
    {
        // Frissítjük az elemet
        g_free(item->title);
        g_free(item->description); // Biztonság kedvéért (bár ""-re állítjuk)

        item->title = g_strdup(title);
        item->description = g_strdup(""); // Nincs leírás
        item->priority = priority;
        item->completed = FALSE;
    }

    view_refresh_list(todos);
    gtk_window_close(GTK_WINDOW(window));
}

/* --- Szerkesztés gomb --- */
// (Ez a függvény rendben volt, változatlan)
void controller_on_edit_clicked(GtkButton *button, gpointer user_data)
{
    GtkWindow *parent = GTK_WINDOW(user_data);
    GtkListBoxRow *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(view_get_list_box()));
    if (!row)
        return;

    TodoItem *item = g_object_get_data(G_OBJECT(row), "todo_ptr");
    if (!item)
        return;

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

    g_signal_connect_data(save_btn,
                          "clicked",
                          G_CALLBACK(on_edit_save_clicked),
                          widgets,
                          free_widgets_array,
                          0);

    gtk_window_present(GTK_WINDOW(edit_window));
}

/* --- Késznek jelölés --- */
// (Ez a függvény rendben volt, változatlan)
void controller_on_mark_done_clicked(GtkButton *button, gpointer user_data)
{
    GtkListBoxRow *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(view_get_list_box()));
    if (!row)
        return;

    TodoItem *item = g_object_get_data(G_OBJECT(row), "todo_ptr");
    if (!item)
        return;

    item->completed = !item->completed;
    view_refresh_list(todos);
}

/* --- Törlés --- */
// (Ez a függvény rendben volt, változatlan)
void controller_on_delete_clicked(GtkButton *button, gpointer user_data)
{
    GtkListBoxRow *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(view_get_list_box()));
    if (!row)
        return;

    TodoItem *item = g_object_get_data(G_OBJECT(row), "todo_ptr");
    if (!item)
        return;

    todos = g_list_remove(todos, item);
    model_free_item(item);
    view_refresh_list(todos);
}

/* --- Mentés fájlba --- */
void controller_on_save_clicked(GtkButton *button, gpointer user_data)
{
    // JAVÍTVA: CSV mentés hívása
    io_save_to_file("todos.csv", todos);
}

/* --- Inicializálás --- */
void controller_init(GtkWidget *window)
{
    // JAVÍTVA: CSV betöltés hívása indításkor
    todos = io_load_from_file("todos.csv");
    view_refresh_list(todos); // Frissítjük a listát a betöltött adatokkal

    g_signal_connect(view_get_add_button(), "clicked", G_CALLBACK(controller_on_add_clicked), window);
    g_signal_connect(view_get_edit_button(), "clicked", G_CALLBACK(controller_on_edit_clicked), window);
    g_signal_connect(view_get_delete_button(), "clicked", G_CALLBACK(controller_on_delete_clicked), window);
    g_signal_connect(view_get_mark_done_button(), "clicked", G_CALLBACK(controller_on_mark_done_clicked), window);
    g_signal_connect(view_get_save_button(), "clicked", G_CALLBACK(controller_on_save_clicked), window);

    // A keresés inicializálása
    search_init(GTK_SEARCH_ENTRY(view_get_search_entry()), GTK_LIST_BOX(view_get_list_box()));
}

/* --- Leállítás --- */
// JAVÍTVA: Új függvény a memória felszabadításához kilépéskor
void controller_shutdown(void)
{
    // Mielőtt bezárul a program, mentsük el automatikusan
    io_save_to_file("todos.csv", todos);
    // És szabadítsuk fel a listát
    model_free_list(todos);
}
