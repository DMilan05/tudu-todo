#include "io.h"
#include <glib/gstdio.h>
#include <stdio.h>

// Eltavolitja a specialis karaktereket (tabulator, sortores) a stringbol,
// hogy ne rontsak el a TSV formatumot.
static gchar *sanitize_string(const gchar *str)
{
    if (!str)
        return g_strdup("");
    return g_strdelimit(g_strdup(str), "\t\r\n", ' ');
}

// Elmenti a teljes listat a megadott fajlba, tabulatorral elvalasztott (TSV) formatumban.
void io_save_to_file(const gchar *filename, GList *list)
{
    FILE *file = g_fopen(filename, "w");
    if (!file)
    {
        g_warning("Hiba: A(z) %s fajlt nem sikerult megnyitni irasra.", filename);
        return;
    }

    // Vegigmegy a lista minden elemen es kiirja a fajlba.
    for (GList *l = list; l != NULL; l = l->next)
    {
        TodoItem *item = (TodoItem *)l->data;

        gchar *title_safe = sanitize_string(item->title);
        gchar *desc_safe = sanitize_string(item->description);
        gchar *cat_safe = sanitize_string(item->category);

        fprintf(file, "%s\t%s\t%s\t%d\t%d\n",
                title_safe,
                desc_safe,
                cat_safe,
                item->priority,
                item->completed ? 1 : 0);

        g_free(title_safe);
        g_free(desc_safe);
        g_free(cat_safe);
    }

    fclose(file);
    g_print("Lista elmentve a(z) %s fajlba.\n", filename);
}

// Betolti a listat a megadott fajlbol.
GList *io_load_from_file(const gchar *filename)
{
    if (!g_file_test(filename, G_FILE_TEST_EXISTS))
    {
        g_print("A(z) %s fajl nem talalhato, ures lista betoltve.\n", filename);
        return NULL;
    }

    FILE *file = g_fopen(filename, "r");
    if (!file)
    {
        g_warning("Hiba: A(z) %s fajlt nem sikerult megnyitni olvasasra.", filename);
        return NULL;
    }

    GList *list = NULL;
    char buffer[2048]; // Feltetelezzuk, hogy egy sor max 2KB.
    guint lines_loaded = 0;

    // Soronkent olvassa a fajlt.
    while (fgets(buffer, sizeof(buffer), file))
    {
        g_strchomp(buffer); // Sorvegi '\n' eltavolitasa.

        // A sort a tabulatorok menten 5 reszre bontja.
        gchar **parts = g_strsplit(buffer, "\t", 5);

        if (g_strv_length(parts) == 5)
        {
            TodoItem *item = g_new0(TodoItem, 1);
            item->title = g_strdup(parts[0]);
            item->description = g_strdup(parts[1]);
            item->category = g_strdup(parts[2]);
            item->priority = (gint)g_ascii_strtoll(parts[3], NULL, 10);
            item->completed = (gboolean)(g_ascii_strtoll(parts[4], NULL, 10) != 0);

            list = g_list_append(list, item);
            lines_loaded++;
        }

        g_strfreev(parts);
    }

    fclose(file);
    g_print("%d elem betoltve a(z) %s fajlbol.\n", lines_loaded, filename);
    return list;
}
