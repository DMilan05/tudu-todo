#include "controller.h"
#include "model.h"
#include "view.h"

static TodoModel *model = NULL;

// --- CALLBACK: Feladat hozzáadása ---
static void on_add_response(GtkButton *button, gpointer user_data) {
    ViewWidgets *vw = user_data;
    GtkWidget *dialog = gtk_widget_get_ancestor(GTK_WIDGET(button), GTK_TYPE_WINDOW);

    GtkWidget *entry = g_object_get_data(G_OBJECT(dialog), "entry");
    const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));

    if (text && *text) {
        model_add_item(model, text);
        model_refresh_list(model, GTK_LIST_BOX(vw->list));
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}

static void on_add_clicked(GtkButton *button, gpointer user_data) {
    ViewWidgets *vw = user_data;

    GtkWidget *dialog = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Uj feladat");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(vw->window));
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 120);

    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_window_set_child(GTK_WINDOW(dialog), content);

    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Feladat neve...");
    gtk_box_append(GTK_BOX(content), entry);
    g_object_set_data(G_OBJECT(dialog), "entry", entry); // elmentjük pointerként

    GtkWidget *buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(content), buttons);

    GtkWidget *cancel_btn = gtk_button_new_with_label("Megse");
    GtkWidget *ok_btn = gtk_button_new_with_label("Hozzaadas");
    gtk_box_append(GTK_BOX(buttons), cancel_btn);
    gtk_box_append(GTK_BOX(buttons), ok_btn);

    g_signal_connect_swapped(cancel_btn, "clicked", G_CALLBACK(gtk_window_destroy), dialog);
    g_signal_connect(ok_btn, "clicked", G_CALLBACK(on_add_response), vw);

    gtk_window_present(GTK_WINDOW(dialog));
}

// --- CALLBACK: Kijelölt elem törlése ---
static void on_delete_clicked(GtkButton *button, gpointer user_data) {
    ViewWidgets *vw = user_data;

    GtkListBoxRow *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(vw->list));
    if (!row) return;

    int index = gtk_list_box_row_get_index(row);
    model_remove_item(model, index);
    model_refresh_list(model, GTK_LIST_BOX(vw->list));
}

// --- SIGNAL összekötés ---
void controller_connect_signals(ViewWidgets *vw) {
    model = model_new();

    g_signal_connect(vw->add_btn, "clicked", G_CALLBACK(on_add_clicked), vw);
    g_signal_connect(vw->del_btn, "clicked", G_CALLBACK(on_delete_clicked), vw);
}
