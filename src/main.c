#include <gtk/gtk.h>
#include "view.h"
#include "controller.h"
#include "model.h" // Modellt is használ

static void on_activate(GtkApplication *app, gpointer user_data) {
    (void)user_data; // warning elkerülése

    // Modell és Nézet inicializálása
    Model *model = model_new();
    ViewWidgets *vw = view_create_main_window(app);

    // Eseménykezelõk bekötése (átadva a modellt is)
    controller_connect_signals(vw, model);

    // A kezdeti állapot frissítése (üres listákkal indul)
    model_refresh_lists(model, vw);

    gtk_window_present(GTK_WINDOW(vw->window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    app = gtk_application_new("hu.tudu.todo", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    // *Megjegyzés: A modell felszabadítását (model_free) ideális esetben
    // a program kilépésekor kellene megtenni, de a jelenlegi MVP
    // struktúrában az on_activate-ben inicializált modell a main kilépésekor
    // "elveszik" a g_application_run hívás után, ha nem tesszük globálissá
    // vagy nem használunk destruktor függvényt.*

    return status;
}
