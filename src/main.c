#include <gtk/gtk.h>
#include "view.h"
#include "controller.h"

static void on_app_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = view_create_main_window(app);
    controller_init(window);
    gtk_widget_show(window);
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("hu.todo.app", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
