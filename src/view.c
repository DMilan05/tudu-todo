#include "view.h"

    ViewWidgets* view_create_main_window(GtkApplication *app) {
    ViewWidgets *vw = g_new(ViewWidgets, 1);

    // Ablak beállítása
    vw->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(vw->window), "TUDU ToDo Board");
    gtk_window_set_default_size(GTK_WINDOW(vw->window), 900, 500);

    // Fõ vertikális box
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_window_set_child(GTK_WINDOW(vw->window), vbox);

    // Cím
    GtkWidget *title = gtk_label_new("TUDU - Feladatkezelõ");
    gtk_widget_add_css_class(title, "title");
    gtk_box_append(GTK_BOX(vbox), title);

    // Keresõmezõ
    GtkWidget *search_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_entry), "Keresés...");
    gtk_box_append(GTK_BOX(vbox), search_entry);

    // --- 3 oszlop egy sorban ---
    GtkWidget *hbox_lists = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(vbox), hbox_lists);

    struct {
        const char *label;
        GtkWidget **target;
    } columns[] = {
        {"Függõben", &vw->pending_list},
        {"Folyamatban", &vw->progress_list},
        {"Kész", &vw->done_list}
    };

    for (int i = 0; i < 3; i++) {
        GtkWidget *frame = gtk_frame_new(columns[i].label);
        gtk_frame_set_label_align(GTK_FRAME(frame), 0.5);
        gtk_widget_set_hexpand(frame, TRUE);
        gtk_widget_set_vexpand(frame, TRUE);

        GtkWidget *list = gtk_list_box_new();
        gtk_widget_set_vexpand(list, TRUE);
        gtk_frame_set_child(GTK_FRAME(frame), list);
        gtk_box_append(GTK_BOX(hbox_lists), frame);

        *columns[i].target = list;
    }

    // --- Gombok alul ---
    GtkWidget *hbox_btns = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(vbox), hbox_btns);

    vw->add_btn = gtk_button_new_with_label("Új feladat");
    vw->del_btn = gtk_button_new_with_label("Törlés");

    gtk_box_append(GTK_BOX(hbox_btns), vw->add_btn);
    gtk_box_append(GTK_BOX(hbox_btns), vw->del_btn);

    return vw;
}
