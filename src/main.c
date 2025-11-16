#include <gtk/gtk.h>
#include "view.h"
#include "controller.h"

/*
 * Dinamikusan generálja a CSS-t a prioritási szintekhez (0-10).
 * A színek a prioritás növekedésével sötétednek a kategóriájukon (kék, zöld, piros) belül.
 */
static void load_css(void)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    // Alap stílus a körnek: méret, lekerekítés, margó
    GString *css_string = g_string_new(
        ".priority-indicator {\n"
        "  min-width: 10px; min-height: 10px;\n"
        "  border-radius: 5px;\n"
        "  margin-right: 6px;\n"
        "}\n"
        "row label {\n"
        "  font-size: 1.1em;\n"
        "}\n");

    for (gint i = 0; i <= 10; i++)
    {
        gint r, g, b;
        if (i >= 8) // Piros árnyalatok (8, 9, 10)
        {
            // A 224-től (világosabb) 192-ig (sötétebb) terjedő skála
            r = 224 - (i - 8) * 16;
            g = 57;
            b = 43;
        }
        else if (i >= 4) // Zöld árnyalatok (4, 5, 6, 7)
        {
            // A 46-tól (világosabb) 34-ig (sötétebb) terjedő skála
            r = 39;
            g = 174 - (i - 4) * 10;
            b = 96;
        }
        else // Kék árnyalatok (0, 1, 2, 3)
        {
            // A 52-től (világosabb) 41-ig (sötétebb) terjedő skála
            r = 41;
            g = 128;
            b = 185 - (i - 0) * 10;
        }

        // CSS szabály hozzáfűzése a stringhez
        g_string_append_printf(css_string, "row.priority-%d .priority-indicator { background-color: rgb(%d, %d, %d); }\n", i, r, g, b);
    }

    gtk_css_provider_load_from_string(provider, css_string->str);
    g_string_free(css_string, TRUE);
    // Alkalmazzuk a CSS-t az egész alkalmazásra
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

static void on_app_activate(GtkApplication *app, gpointer user_data)
{
    // Töltsük be a CSS-t az ablak létrehozása előtt
    load_css();

    GtkWidget *window = view_create_main_window(app);
    controller_init(window);

    // A gtk_widget_show() elavult GTK4-ben,
    // használd a gtk_window_present()-et.
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[])
{
    // JAVÍTVA: G_APPLICATION_DEFAULT_FLAGS használata
    GtkApplication *app = gtk_application_new("hu.todo.app", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
