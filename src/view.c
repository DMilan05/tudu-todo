#include "view.h"

ViewWidgets *view_create_main_window(GtkApplication *app)
{
    ViewWidgets *vw = g_new(ViewWidgets, 1);
    vw->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(vw->window), "TUDU TODO");
    gtk_window_set_default_size(GTK_WINDOW(vw->window), 600, 400);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_window_set_child(GTK_WINDOW(vw->window), vbox);
    GtkWidget *title = gtk_label_new("TUDU TODO alkalmazas");
    gtk_widget_add_css_class(title, "title");
    gtk_box_append(GTK_BOX(vbox), title);
    GtkWidget *search_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_entry), "Kereses...");
    gtk_box_append(GTK_BOX(vbox), search_entry);
    vw->list = gtk_list_box_new();
    gtk_box_append(GTK_BOX(vbox), vw->list);
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(vbox), hbox);
    vw->add_btn = gtk_button_new_with_label("Uj feladat");
    vw->del_btn = gtk_button_new_with_label("Torles");
    vw->file_add_btn = gtk_button_new_with_label("Fájl");
    gtk_box_append(GTK_BOX(hbox), vw->add_btn);
    gtk_box_append(GTK_BOX(hbox), vw->del_btn);
    gtk_box_append(GTK_BOX(hbox), vw->file_add_btn);

    return vw;
}
