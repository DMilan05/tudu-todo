#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>
#include <stdlib.h>
#include "../include/model.h"

/* Forward-declarations for GTK types used in stubs (avoid including <gtk/gtk.h>) */
typedef struct _GtkWidget GtkWidget;
typedef struct _GtkApplication GtkApplication;
typedef struct _GtkWindow GtkWindow;
typedef struct _GtkAlertDialog GtkAlertDialog;

/* Counters for tests */
int stub_controller_init_called = 0;
int stub_controller_shutdown_called = 0;
int stub_view_create_main_window_called = 0;

/* controller stubbok */
void controller_init(GtkWidget *window) { (void)window; stub_controller_init_called++; }
void controller_shutdown(void) { stub_controller_shutdown_called++; }
gboolean controller_on_close_request(GtkWidget *w, gpointer user_data) { (void)w; (void)user_data; return TRUE; }

/* Keep minimal view stubs/decls used by tests (implementations live in ../src/view.c) */
/* Only declare the functions here to avoid duplicate symbols when linking the real view.c */
GtkWidget *view_create_main_window(GtkApplication *app);
void view_refresh_list(GList *list);
GtkWidget *view_get_list_box(void);
GtkWidget *view_get_add_button(void);
GtkWidget *view_get_edit_button(void);
GtkWidget *view_get_delete_button(void);
GtkWidget *view_get_mark_done_button(void);
GtkWidget *view_get_save_button(void);
GtkWidget *view_get_delete_completed_button(void);
GtkWidget *view_get_sort_dropdown(void);
GtkWidget *view_get_search_entry(void);
GtkWidget *view_create_add_window(GtkWindow *parent);
GtkWidget *view_create_edit_window(GtkWindow *parent, TodoItem *item);
void view_show_message(const char *msg);
void view_show_error(const char *msg);

/* Model stubbok: csak deklarációk — a valós implementációt a ../src/model.c biztosítja */
void model_add_item(GList **list, TodoItem *item);
void model_free_item(TodoItem *item);
void model_free_list(GList *list);

/* GtkAlertDialog minimal stubs */
GtkAlertDialog* gtk_alert_dialog_new(const char *msg) { (void)msg; return NULL; }
void gtk_alert_dialog_show(GtkAlertDialog *d, void *w) { (void)d; (void)w; }
void gtk_alert_dialog_set_buttons(GtkAlertDialog *d, const gchar **buttons) { (void)d; (void)buttons; }
void gtk_alert_dialog_choose(GtkAlertDialog *d, void *w, void *a, void (*cb)(GObject*,GAsyncResult*,gpointer), gpointer user_data) {
    (void)d; (void)w; (void)a; (void)cb; (void)user_data;
}
gint gtk_alert_dialog_choose_finish(GtkAlertDialog *d, GAsyncResult *res, GError **err) { (void)d; (void)res; (void)err; return 1; }

/* controller rendezőfüggvények (tesztekhez) */
gint controller_sort_by_priority(gconstpointer a, gconstpointer b) {
    const TodoItem *ia = a;
    const TodoItem *ib = b;
    return (ib->priority - ia->priority);
}

gint controller_sort_by_title(gconstpointer a, gconstpointer b) {
    const TodoItem *ia = a;
    const TodoItem *ib = b;
    return g_strcmp0(ia->title, ib->title);
}

void controller_on_delete_clicked(GtkWidget *w, gpointer user_data) { (void)w; (void)user_data; }