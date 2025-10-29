#include "io.h"
#include <stdio.h>

gboolean io_save_list(const gchar *filename, GList *list) {
    // TODO: implement CSV save
    g_print("Saving to %s (not yet implemented)\n", filename);
    return TRUE;
}

GList* io_load_list(const gchar *filename) {
    // TODO: implement CSV load
    g_print("Loading from %s (not yet implemented)\n", filename);
    return NULL;
}
