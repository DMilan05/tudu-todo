#ifndef VIEW_H
#define VIEW_H

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *window;
    GtkWidget *list;
    GtkWidget *add_btn;
    GtkWidget *del_btn;
} ViewWidgets;

ViewWidgets* view_create_main_window(GtkApplication *app);

#endif
