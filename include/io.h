#ifndef IO_H
#define IO_H

#include <glib.h>
#include "model.h"

gboolean io_save_list(const gchar *filename, GList *list);
GList* io_load_list(const gchar *filename);

#endif
