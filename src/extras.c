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
    static const gchar *classes[11] = { NULL };
    static gboolean inited = FALSE;

    if (!inited) {
        for (int i = 0; i <= 10; ++i) {
            /* allokálunk konstans stringeket egyszer (nem szükséges felszabadítani a tesztekhez) */
            gchar *s = g_strdup_printf("priority-%d", i);
            classes[i] = s;
        }
        inited = TRUE;
    }

    if (priority < 0) priority = 0;
    if (priority > 10) priority = 10;
    return classes[priority];
}
