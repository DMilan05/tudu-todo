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

// Piros/Zöld/Kék logika alapján CSS osztályneveket ad vissza
const gchar* extras_get_css_class_for_priority(gint priority) {
    if (priority > 5) { // 6-10
        return "priority-high"; // Piros
    } else if (priority == 5) { // Pont 5
        return "priority-medium"; // Zöld
    } else { // 0-4
        return "priority-low"; // Kék
    }
}
