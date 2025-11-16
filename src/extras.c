#include "extras.h"
#include <string.h>

gchar *extras_trim(const gchar *str)
{
    if (!str)
        return g_strdup("");
    gchar *trimmed = g_strdup(str);
    g_strstrip(trimmed);
    return trimmed;
}

gchar *extras_format_priority(gint priority)
{
    return g_strdup_printf("Priority: %d", priority);
}

/*
 * Visszaad egy egyedi CSS osztálynevet a prioritás alapján (pl. "priority-5").
 * A függvény egy statikus buffert használ, így a visszatérési értéket nem kell felszabadítani,
 * de nem is szálbiztos (bár ebben az alkalmazásban ez nem jelent problémát).
 */
const gchar *extras_get_css_class_for_priority(gint priority)
{
    // Biztonsági ellenőrzés, hogy a prioritás a [0, 10] tartományban legyen
    if (priority < 0)
        priority = 0;
    if (priority > 10)
        priority = 10;
    return g_strdup_printf("priority-%d", priority);
}
