#ifndef EXTRAS_H
#define EXTRAS_H

#include <glib.h>
#include <gtk/gtk.h>

gchar* extras_trim(const gchar *str);
gchar* extras_format_priority(gint priority);

// A CSS osztálynevet visszaadó függvény
const gchar* extras_get_css_class_for_priority(gint priority);

#endif /* EXTRAS_H */
