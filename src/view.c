#include "view.h"
#include <gtk/gtk.h>
#include "model.h"
#include "extras.h"
#include "controller.h" // Hozzáadjuk a controller headert a függvénydeklarációk miatt

// Itt van az összes statikus változó
static GtkWidget *add_button;
static GtkWidget *list_box;
static GtkWidget *delete_button;
static GtkWidget *save_button;
static GtkWidget *mark_done_button;
static GtkWidget *edit_button;
static GtkWidget *search_entry;
static GtkWidget *sort_dropdown;
static GtkWidget *delete_completed_button;

static GtkWidget *add_window = NULL;
static GtkWidget *edit_window = NULL;

GtkWidget *view_create_main_window(GtkApplication *app)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tudu-ToDo");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    GtkWidget *menu_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(vbox), menu_bar);

    add_button = gtk_button_new_with_label("Hozzáadás");
    gtk_box_append(GTK_BOX(menu_bar), add_button);

    save_button = gtk_button_new_with_label("Mentés");
    gtk_box_append(GTK_BOX(menu_bar), save_button);

    // Rendezési opciók legördülő menüje
    const char *sort_options[] = {"Rendezés...", "ABC szerint", "Prioritás szerint", NULL};
    sort_dropdown = gtk_drop_down_new_from_strings(sort_options);
    gtk_box_append(GTK_BOX(menu_bar), sort_dropdown);
    // A "Rendezés..." felirat legyen a placeholder, ne egy választható opció.
    // Ehhez a 0. elemet nem tesszük választhatóvá.
    gtk_drop_down_set_enable_search(GTK_DROP_DOWN(sort_dropdown), FALSE);

    search_entry = gtk_search_entry_new();
    gtk_search_entry_set_placeholder_text(GTK_SEARCH_ENTRY(search_entry), "Keresés...");
    gtk_box_append(GTK_BOX(menu_bar), search_entry);

    list_box = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(list_box), GTK_SELECTION_SINGLE);
    gtk_list_box_set_show_separators(GTK_LIST_BOX(list_box), TRUE);

    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), list_box);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_box_append(GTK_BOX(vbox), scrolled_window);

    GtkWidget *controls = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(vbox), controls);

    edit_button = gtk_button_new_with_label("Szerkesztés");
    gtk_box_append(GTK_BOX(controls), edit_button);

    mark_done_button = gtk_button_new_with_label("Késznek jelölés");
    gtk_box_append(GTK_BOX(controls), mark_done_button);

    delete_button = gtk_button_new_with_label("Törlés");
    gtk_box_append(GTK_BOX(controls), delete_button);

    delete_completed_button = gtk_button_new_with_label("Kész elemek törlése");
    gtk_box_append(GTK_BOX(controls), delete_completed_button);

    return window;
}

/* Getterek */
GtkWidget *view_get_add_button(void) { return add_button; }
GtkWidget *view_get_list_box(void) { return list_box; }
GtkWidget *view_get_delete_button(void) { return delete_button; }
GtkWidget *view_get_save_button(void) { return save_button; }
GtkWidget *view_get_mark_done_button(void) { return mark_done_button; }
GtkWidget *view_get_edit_button(void) { return edit_button; }
GtkWidget *view_get_search_entry(void) { return search_entry; }
GtkWidget *view_get_sort_dropdown(void) { return sort_dropdown; }
GtkWidget *view_get_delete_completed_button(void) { return delete_completed_button; }

/* --- Lista frissítése --- */
void view_refresh_list(GList *list)
{
    GtkWidget *child;
    while ((child = gtk_widget_get_first_child(list_box)) != NULL)
    {
        gtk_list_box_remove(GTK_LIST_BOX(list_box), child);
    }

    for (GList *l = list; l; l = l->next)
    {
        TodoItem *item = l->data;
        if (!item || !item->title)
            continue;

        GtkWidget *row = gtk_list_box_row_new();

        gchar *label_text;
        if (item->completed)
        {
            label_text = g_markup_printf_escaped("<s>%s</s> (Prioritás: %d)", item->title, item->priority);
        }
        else
        {
            label_text = g_markup_printf_escaped("%s (Prioritás: %d)", item->title, item->priority);
        }

        // Létrehozunk egy horizontális dobozt a körnek és a szövegnek
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
        gtk_widget_set_halign(hbox, GTK_ALIGN_START);
        gtk_widget_set_valign(hbox, GTK_ALIGN_CENTER);
        // Egy kis margót adunk a dobozhoz, hogy a kör ne vágódjon le a sor szélén.
        // A 6 pixeles margó általában elegendő helyet biztosít.
        gtk_widget_set_margin_start(hbox, 6);
        // Függőleges margót adunk hozzá, hogy a sorok magasabbak legyenek.
        // Ettől az elemek nagyobbnak és szellősebbnek tűnnek.
        gtk_widget_set_margin_top(hbox, 8);
        gtk_widget_set_margin_bottom(hbox, 8);

        // Létrehozzuk a prioritásjelző "kört" (egy egyszerű widget, amit CSS-sel formázunk)
        GtkWidget *prio_indicator = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_widget_add_css_class(prio_indicator, "priority-indicator");
        gtk_box_append(GTK_BOX(hbox), prio_indicator);

        GtkWidget *label = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(label), label_text);
        g_free(label_text);

        gtk_label_set_xalign(GTK_LABEL(label), 0.0);
        gtk_box_append(GTK_BOX(hbox), label); // A címkét a dobozhoz adjuk

        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), hbox); // A dobozt tesszük a sorba
        // A korábbi CSS osztályok eltávolítása. Mivel most már 11 különböző van,
        // egyszerűbb, ha egy ciklussal távolítjuk el őket.
        for (int i = 0; i <= 10; ++i)
        {
            gtk_widget_remove_css_class(row, g_strdup_printf("priority-%d", i));
        }

        // Megfelelő CSS osztály hozzáadása a prioritás alapján
        if (!item->completed)
        { // Csak akkor színezzük, ha nincs kész
            gchar *css_class = (gchar *)extras_get_css_class_for_priority(item->priority);
            gtk_widget_add_css_class(row, css_class);
        }

        g_object_set_data(G_OBJECT(row), "todo_ptr", item);
        gtk_list_box_append(GTK_LIST_BOX(list_box), row);
    }
}

static void on_add_window_destroyed(GtkWidget *widget, gpointer data)
{
    add_window = NULL;
}

/* --- Hozzáadás ablak --- */
GtkWidget *view_create_add_window(GtkWindow *parent)
{
    if (add_window)
    {
        gtk_window_present(GTK_WINDOW(add_window));
        return add_window;
    }

    add_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(add_window), "Új feladat hozzáadása");
    gtk_window_set_transient_for(GTK_WINDOW(add_window), parent);
    gtk_window_set_default_size(GTK_WINDOW(add_window), 300, 200);
    gtk_window_set_modal(GTK_WINDOW(add_window), TRUE);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_window_set_child(GTK_WINDOW(add_window), box);

    GtkWidget *title = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(title), "Feladat címe");
    gtk_box_append(GTK_BOX(box), title);

    GtkWidget *hbox_prio = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(box), hbox_prio);

    GtkWidget *prio_label = gtk_label_new("Prioritás (0-10):");
    gtk_box_append(GTK_BOX(hbox_prio), prio_label);

    GtkWidget *prio_spin = gtk_spin_button_new_with_range(0, 10, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(prio_spin), 5);
    gtk_box_append(GTK_BOX(hbox_prio), prio_spin);

    GtkWidget *buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(box), buttons);

    GtkWidget *save_btn = gtk_button_new_with_label("Mentés");
    gtk_box_append(GTK_BOX(buttons), save_btn);

    GtkWidget *cancel_btn = gtk_button_new_with_label("Mégse");
    gtk_box_append(GTK_BOX(buttons), cancel_btn);

    g_object_set_data(G_OBJECT(add_window), "title_entry", title);
    g_object_set_data(G_OBJECT(add_window), "prio_spin", prio_spin);
    g_object_set_data(G_OBJECT(add_window), "save_btn", save_btn);

    g_signal_connect_swapped(cancel_btn, "clicked", G_CALLBACK(gtk_window_close), add_window);
    g_signal_connect(add_window, "destroy", G_CALLBACK(on_add_window_destroyed), NULL);

    return add_window;
}

static void on_edit_window_destroyed(GtkWidget *widget, gpointer data)
{
    edit_window = NULL;
}

/* --- Szerkesztés ablak --- */
GtkWidget *view_create_edit_window(GtkWindow *parent, TodoItem *item)
{
    if (edit_window)
    {
        gtk_window_present(GTK_WINDOW(edit_window));
        return edit_window;
    }

    edit_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(edit_window), "Feladat szerkesztése");
    gtk_window_set_transient_for(GTK_WINDOW(edit_window), parent);
    gtk_window_set_default_size(GTK_WINDOW(edit_window), 400, 200);
    gtk_window_set_modal(GTK_WINDOW(edit_window), TRUE);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_window_set_child(GTK_WINDOW(edit_window), box);

    GtkWidget *title = gtk_entry_new();
    if (item && item->title)
        gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(title)), item->title, -1);
    gtk_box_append(GTK_BOX(box), title);

    GtkWidget *hbox_details = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(box), hbox_details);

    GtkWidget *prio_label = gtk_label_new("Prioritás (0-10):");
    gtk_box_append(GTK_BOX(hbox_details), prio_label);

    GtkWidget *prio_spin = gtk_spin_button_new_with_range(0, 10, 1);
    if (item)
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(prio_spin), item->priority);
    gtk_box_append(GTK_BOX(hbox_details), prio_spin);

    // A "Kész" állapotot egy egyszerű jelölőnégyzet kezeli.
    GtkWidget *status = gtk_check_button_new_with_label("Kész");
    if (item)
        gtk_check_button_set_active(GTK_CHECK_BUTTON(status), item->completed);
    gtk_box_append(GTK_BOX(box), status); // Külön sorba tesszük a jobb átláthatóságért

    GtkWidget *buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(box), buttons);

    GtkWidget *save_btn = gtk_button_new_with_label("Mentés");
    gtk_widget_set_hexpand(save_btn, TRUE); // A gombok kitöltik a helyet
    gtk_box_append(GTK_BOX(buttons), save_btn);

    // Új "Törlés" gomb hozzáadása a szerkesztő ablakhoz
    GtkWidget *delete_btn = gtk_button_new_with_label("Törlés");
    gtk_widget_add_css_class(delete_btn, "destructive-action"); // Piros szín a veszélyes művelethez
    gtk_box_append(GTK_BOX(buttons), delete_btn);

    GtkWidget *cancel_btn = gtk_button_new_with_label("Mégse");
    gtk_widget_set_hexpand(cancel_btn, TRUE);
    gtk_box_append(GTK_BOX(buttons), cancel_btn);

    g_object_set_data(G_OBJECT(edit_window), "title_entry", title);
    g_object_set_data(G_OBJECT(edit_window), "prio_spin", prio_spin);
    g_object_set_data(G_OBJECT(edit_window), "done_check", status);
    g_object_set_data(G_OBJECT(edit_window), "save_btn", save_btn);

    // A Törlés gomb eseménykezelője. A főablak törlés funkcióját hívja meg,
    // majd bezárja a szerkesztő ablakot.
    g_signal_connect_swapped(delete_btn, "clicked", G_CALLBACK(controller_on_delete_clicked), NULL);
    g_signal_connect_swapped(delete_btn, "clicked", G_CALLBACK(gtk_window_close), edit_window);
    g_signal_connect_swapped(cancel_btn, "clicked", G_CALLBACK(gtk_window_close), edit_window);
    g_signal_connect(edit_window, "destroy", G_CALLBACK(on_edit_window_destroyed), NULL);

    return edit_window;
}
