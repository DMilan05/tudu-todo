#include "io.h"
#include <glib/gstdio.h> // Fájlmûveletekhez
#include <stdio.h>      // Standard C I/O

/*
 * Egyetlen karakterláncot "megtisztít" mentéshez:
 * Kicseréli a TAB és Újsor karaktereket szóközre, hogy ne törje meg a CSV/TSV sort.
 */
static gchar* sanitize_string(const gchar *str) {
    if (!str) return g_strdup("");
    // Kicseréli a \t, \r, \n karaktereket szóközre
    return g_strdelimit(g_strdup(str), "\t\r\n", ' ');
}

/*
 * Elmenti a teljes listát a megadott fájlba (TSV formátumban)
 */
void io_save_to_file(const gchar *filename, GList *list) {
    FILE *file = g_fopen(filename, "w"); // "w" = write (írás)
    if (!file) {
        g_warning("Hiba: A %s fájlt nem sikerült megnyitni írásra.", filename);
        return;
    }

    // Végigmegyünk a listán
    for (GList *l = list; l != NULL; l = l->next) {
        TodoItem *item = (TodoItem*)l->data;

        // "Megtisztítjuk" a stringeket mentés elõtt
        gchar *title_safe = sanitize_string(item->title);
        gchar *desc_safe = sanitize_string(item->description);
        gchar *cat_safe = sanitize_string(item->category);

        // Kiírjuk az adatokat TAB-bal elválasztva
        fprintf(file, "%s\t%s\t%s\t%d\t%d\n",
                title_safe,
                desc_safe,
                cat_safe,
                item->priority,
                item->completed ? 1 : 0 // Mentés 1-ként vagy 0-ként
        );

        // Felszabadítjuk a megtisztított stringeket
        g_free(title_safe);
        g_free(desc_safe);
        g_free(cat_safe);
    }

    fclose(file);
    g_print("Lista elmentve a %s fájlba.\n", filename);
}

/*
 * Betölti a listát a fájlból.
 */
GList* io_load_from_file(const gchar *filename) {
    if (!g_file_test(filename, G_FILE_TEST_EXISTS)) {
        g_print("A %s fájl nem található, üres lista betöltve.\n", filename);
        return NULL;
    }

    FILE *file = g_fopen(filename, "r"); // "r" = read (olvasás)
    if (!file) {
        g_warning("Hiba: A %s fájlt nem sikerült megnyitni olvasásra.", filename);
        return NULL;
    }

    GList *list = NULL;
    char buffer[2048]; // Tegyük fel, hogy egy sor max 2KB
    guint lines_loaded = 0;

    // Olvasunk soronként, amíg a fájl végére nem érünk
    while (fgets(buffer, sizeof(buffer), file)) {
        // Eltávolítjuk a sorvégi \n karaktert
        g_strchomp(buffer);

        // Felbontjuk a sort a TAB karakterek mentén
        gchar **parts = g_strsplit(buffer, "\t", 5); // Max 5 részre vágjuk

        // Ellenõrizzük, hogy megvan-e mind az 5 rész
        if (g_strv_length(parts) == 5) {
            TodoItem *item = g_new0(TodoItem, 1);
            item->title = g_strdup(parts[0]);
            item->description = g_strdup(parts[1]);
            item->category = g_strdup(parts[2]);
            // Sztringbõl integerré alakítás
            item->priority = (gint)g_ascii_strtoll(parts[3], NULL, 10);
            item->completed = (gboolean)(g_ascii_strtoll(parts[4], NULL, 10) != 0);

            list = g_list_append(list, item);
            lines_loaded++;
        }

        g_strfreev(parts); // Felszabadítjuk a felbontott tömböt
    }

    fclose(file);
    g_print("%d elem betöltve a %s fájlból.\n", lines_loaded, filename);
    return list;
}
