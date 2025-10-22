#include "view.h"

GtkWidget* view_create_main_window(GtkApplication *app);
GtkWidget* view_get_add_button(void);
GtkWidget* view_get_entry_field(void);
GtkWidget* view_get_list_box(void);
void view_refresh_list(GList *list);

}
