#include <gtk/gtk.h>

static void on_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Hello GTK4!\n");
}

static void on_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *box;
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK4 Demo");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);
    gtk_window_set_child(GTK_WINDOW(window), box);
    button = gtk_button_new_with_label("Click to close window");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_close), window);
    gtk_box_append(GTK_BOX(box), button);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;
    app = gtk_application_new("hu.nye.inf.tudu_todo", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
