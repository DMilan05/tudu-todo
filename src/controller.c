#include "controller.h"
#include "model.h"
#include "view.h"
#include <string.h>

// --- Hozzáadás gomb esemény ---
static void on_add_clicked(GtkButton *button, gpointer user_data) {
    (void)button; // warning elkerülése

    AppData *app = (AppData*) user_data;
    ViewWidgets *vw = app->view;
    Model *model = app->model;

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Új feladat",
        GTK_WINDOW(vw->window),
        GTK_DIALOG_MODAL,
        "_Mégse", GTK_RESPONSE_CANCEL,
        "_Hozzáadás", GTK_RESPONSE_OK,
        NULL
    );

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Feladat neve...");
    gtk_box_append(GTK_BOX(content), entry);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
        if (strlen(text) > 0) {
            // Csak a címet és az állapotot adja át. A státusz mindig STATUS_PENDING az új feladatoknál.
            model_add_task(model, text, STATUS_PENDING);
            // A modell frissíti a listákat a hozzáadás után.
            model_refresh_lists(model, vw);
        }
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}

// --- Törlés gomb esemény ---
static void on_delete_clicked(GtkButton *button, gpointer user_data) {
    (void)button;

    AppData *app = (AppData*) user_data;
    ViewWidgets *vw = app->view;
    Model *model = app->model;

    // próbáljuk lekérni, melyik listában van kijelölés
    GtkListBoxRow *row = NULL;
    TaskStatus status = STATUS_PENDING;

    if ((row = gtk_list_box_get_selected_row(GTK_LIST_BOX(vw->pending_list))))
        status = STATUS_PENDING;
    else if ((row = gtk_list_box_get_selected_row(GTK_LIST_BOX(vw->progress_list))))
        status = STATUS_PROGRESS;
    else if ((row = gtk_list_box_get_selected_row(GTK_LIST_BOX(vw->done_list))))
        status = STATUS_DONE;

    if (!row) return;

    int index = gtk_list_box_row_get_index(row);
    model_remove_task(model, status, index);
    model_refresh_lists(model, vw);
}

// --- Fő eseménykezelők regisztrálása ---
g_signal_connect(dialog, "response", G_CALLBACK(on_dialog_response), app);
gtk_window_present(GTK_WINDOW(dialog));

// REMOVE the line 'gint result = gtk_dialog_run(GTK_DIALOG(dialog));'

// ... and encapsulate the logic inside a new response handler function
static void on_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    AppData *app = (AppData*) user_data;
    Model *model = app->model;
    ViewWidgets *vw = app->view;
    GtkWidget *content = gtk_dialog_get_content_area(dialog);
    GtkWidget *entry = gtk_widget_get_first_child(content); // Get the entry widget

    if (response_id == GTK_RESPONSE_OK) {
        const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
        if (strlen(text) > 0) {
            model_add_task(model, text, STATUS_PENDING);
            model_refresh_lists(model, vw);
        }
    }
    // Clean up the dialog
    gtk_window_destroy(GTK_WINDOW(dialog));
}
