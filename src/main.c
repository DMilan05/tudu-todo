#include <gtk/gtk.h>
#include "view.h"
#include "controller.h"

// JAVÍTVA: Új függvény a CSS betöltéséhez
static void load_css(void) {
    GtkCssProvider *provider = gtk_css_provider_new();

    // Ez a CSS definiálja a színeket, amiket az extras.c-ben beállítottunk
    const char *css =
        // A .priority-high osztályú GtkListBoxRow-k pirosak lesznek
        "listboxrow.priority-high { background-color: #FF6B6B; }"
        // A .priority-medium osztályúak zöldek
        "listboxrow.priority-medium { background-color: #8AEE93; }"
        // A .priority-low osztályúak kékek
        "listboxrow.priority-low { background-color: #87CEEB; }";

    gtk_css_provider_load_from_string(provider, css);

    // Alkalmazzuk a CSS-t az egész alkalmazásra
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    g_object_unref(provider);
}


static void on_app_activate(GtkApplication *app, gpointer user_data) {
    // Töltsük be a CSS-t az ablak létrehozása előtt
    load_css();

    GtkWidget *window = view_create_main_window(app);
    controller_init(window);

    // A gtk_widget_show() elavult GTK4-ben,
    // használd a gtk_window_present()-et.
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[]) {
    // JAVÍTVA: G_APPLICATION_DEFAULT_FLAGS használata
    GtkApplication *app = gtk_application_new("hu.todo.app", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
