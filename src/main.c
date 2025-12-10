#include <gtk/gtk.h>
#include "view.h"
#include "controller.h"

// Betölti a dinamikusan generált CSS-t a prioritási szintek színezéséhez.
static void load_css(void)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    GString *css_string = g_string_new(
        ".priority-indicator { min-width: 10px; min-height: 10px; border-radius: 5px; margin-right: 6px; }\n"
        "row label { font-size: 1.1em; }\n");

    // Színskála generálása a prioritásokhoz (0-10).
    for (gint i = 0; i <= 10; i++)
    {
        gint r, g, b;
        if (i >= 8) { r = 224 - (i - 8) * 16; g = 57; b = 43; }      // Piros árnyalatok
        else if (i >= 4) { r = 39; g = 174 - (i - 4) * 10; b = 96; } // Zöld árnyalatok
        else { r = 41; g = 128; b = 185 - i * 10; }                  // Kék árnyalatok
        g_string_append_printf(css_string, "row.priority-%d .priority-indicator { background-color: rgb(%d, %d, %d); }\n", i, r, g, b);
    }

    gtk_css_provider_load_from_string(provider, css_string->str);
    g_string_free(css_string, TRUE);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

// Az alkalmazás "activate" eseménykezelője. Létrehozza a főablakot.
static void on_app_activate(GtkApplication *app, gpointer user_data)
{
    /* csak ha van elérhető display, töltsük a CSS-t (tesztek headless környezetben megússzák) */
    if (gdk_display_get_default())
        load_css();

    GtkWidget *window = view_create_main_window(app);
    controller_init(window);

    /* csak érvényes GObject-re kötünk jelet */
    if (G_IS_OBJECT(window)) {
        g_signal_connect(window, "close-request", G_CALLBACK(controller_on_close_request), NULL);
    }

    /* csak ha valódi GtkWindow, mutassuk be */
    if (GTK_IS_WINDOW(window)) {
        gtk_window_present(GTK_WINDOW(window));
    }
}

// Az alkalmazás "shutdown" eseménykezelője. Felszabadítja az erőforrásokat.
static void on_app_shutdown(GtkApplication *app, gpointer user_data)
{
    controller_shutdown();
}

// Az alkalmazás belépési pontja.
int main(int argc, char *argv[])
{
    GtkApplication *app = gtk_application_new("hu.tudu.app", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);
    g_signal_connect(app, "shutdown", G_CALLBACK(on_app_shutdown), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
