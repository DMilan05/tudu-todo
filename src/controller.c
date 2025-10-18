#include "controller.h"
#include "view.h"

static int task_counter = 1;

static void on_add_clicked(GtkButton *button, gpointer user_data) {
    ViewWidgets *vw = user_data;

    char text[64];
    snprintf(text, sizeof(text), "Uj feladat #%d", task_counter++);

    GtkWidget *row = gtk_label_new(text);
    gtk_list_box_append(GTK_LIST_BOX(vw->list), row);

    g_print("[INFO] Hozzaadva: %s\n", text);
}

static void on_delete_clicked(GtkButton *button, gpointer user_data) {
    ViewWidgets *vw = user_data;

    GtkListBoxRow *selected = gtk_list_box_get_selected_row(GTK_LIST_BOX(vw->list));
    if (selected) {
        gtk_list_box_remove(GTK_LIST_BOX(vw->list), GTK_WIDGET(selected));
        g_print("[INFO] Feladat torolve.\n");
    } else {
        g_print("[WARN] Nincs kivalasztott elem torleshez.\n");
    }
}

void controller_connect_signals(ViewWidgets *vw) {
    g_signal_connect(vw->add_btn, "clicked", G_CALLBACK(on_add_clicked), vw);
    g_signal_connect(vw->del_btn, "clicked", G_CALLBACK(on_delete_clicked), vw);
}
