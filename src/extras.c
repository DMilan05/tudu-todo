#include "extras.h"
#include <string.h>

gchar* extras_trim(const gchar *str) {
    if (!str) return g_strdup("");
    gchar *trimmed = g_strdup(str);
    g_strstrip(trimmed);
    return trimmed;
}

gchar* extras_format_priority(gint priority) {
    return g_strdup_printf("Priority: %d", priority);
}

GdkRGBA extras_get_color_for_priority(gint priority) {
    GdkRGBA color;
    if (priority > 5)
        gdk_rgba_parse(&color, "red");
    else if (priority > 2)
        gdk_rgba_parse(&color, "orange");
    else
        gdk_rgba_parse(&color, "green");
    return color;
}
