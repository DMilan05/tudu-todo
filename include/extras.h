#ifndef EXTRAS_H
#define EXTRAS_H

#include <gtk/gtk.h>

gchar* extras_trim(const gchar *str);
gchar* extras_format_priority(gint priority);
GdkRGBA extras_get_color_for_priority(gint priority);

#endif
