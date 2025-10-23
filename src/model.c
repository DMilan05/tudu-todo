#include "model.h"
#include "view.h" // A ViewWidgets miatt
#include <string.h>

/**
 * Segédfüggvény egy lista frissítésére a GUI-ban.
 */
static void model_refresh_list_gui(GList *list, GtkListBox *gui_list) {
    // Eltávolítja az összes elemet a listbox-ból
    GtkListBoxRow *row;
    while ((row = gtk_list_box_get_row_at_index(gui_list, 0))) {
        gtk_list_box_remove(gui_list, GTK_WIDGET(row));
    }

    // Hozzáadja a modell elemeit
    for (GList *l = list; l; l = l->next) {
        Task *t = l->data;

        // Egy egyszerű label létrehozása a feladat címével
        GtkWidget *row_label = gtk_label_new(t->title);
        gtk_list_box_append(gui_list, row_label);
    }
}

// Új modell inicializálása
Model* model_new(void) {
    Model *model = g_new(Model, 1);
    model->pending = NULL;
    model->progress = NULL;
    model->done = NULL;
    return model;
}

// Új feladat hozzáadása
void model_add_task(Model *model, const char *title, TaskStatus status) {
    if (!model || !title || !*title) return;

    Task *task = g_new(Task, 1);
    g_strlcpy(task->title, title, sizeof(task->title));
    task->status = status;

    switch (status) {
        case STATUS_PENDING:
            model->pending = g_list_append(model->pending, task);
            break;
        case STATUS_PROGRESS:
            model->progress = g_list_append(model->progress, task);
            break;
        case STATUS_DONE:
            model->done = g_list_append(model->done, task);
            break;
    }
}

// Feladat törlése index alapján egy adott állapotlistából
void model_remove_task(Model *model, TaskStatus status, int index) {
    if (!model || index < 0) return;

    GList **list_ptr = NULL;

    switch (status) {
        case STATUS_PENDING:
            list_ptr = &model->pending;
            break;
        case STATUS_PROGRESS:
            list_ptr = &model->progress;
            break;
        case STATUS_DONE:
            list_ptr = &model->done;
            break;
        default:
            return;
    }

    if (!*list_ptr) return;

    GList *node = g_list_nth(*list_ptr, index);
    if (!node) return;

    Task *task = node->data;
    g_free(task);
    *list_ptr = g_list_delete_link(*list_ptr, node);
}

// Mindhárom lista frissítése a GUI-ban
void model_refresh_lists(Model *model, ViewWidgets *vw) {
    if (!model || !vw) return;

    model_refresh_list_gui(model->pending, GTK_LIST_BOX(vw->pending_list));
    model_refresh_list_gui(model->progress, GTK_LIST_BOX(vw->progress_list));
    model_refresh_list_gui(model->done, GTK_LIST_BOX(vw->done_list));
}

// Modell felszabadítása
void model_free(Model *model) {
    if (!model) return;

    // Segédfüggvény a listák elemeinek és maguknak a listáknak a felszabadítására
    void free_glist(GList *list) {
        for (GList *l = list; l; l = l->next) {
            g_free(l->data); // Felszabadítja a Task struktúrát
        }
        g_list_free(list); // Felszabadítja a GList csomópontokat
    }

    free_glist(model->pending);
    free_glist(model->progress);
    free_glist(model->done);
    g_free(model); // Felszabadítja a Model struktúrát
}
