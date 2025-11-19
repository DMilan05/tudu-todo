#include "io.h"
#include <glib/gstdio.h> // F�jlm�veletekhez
#include <stdio.h>       // Standard C I/O

/*
 * Egyetlen karakterl�ncot "megtiszt�t" ment�shez:
 * Kicser�li a TAB �s �jsor karaktereket sz�k�zre, hogy ne t�rje meg a CSV/TSV sort.
 */
static gchar *sanitize_string(const gchar *str)
{
    if (!str)
        return g_strdup("");
    // Kicser�li a \t, \r, \n karaktereket sz�k�zre
    return g_strdelimit(g_strdup(str), "\t\r\n", ' ');
}

/*
 * Elmenti a teljes list�t a megadott f�jlba (TSV form�tumban)
 */
void io_save_to_file(const gchar *filename, GList *list)
{
    FILE *file = g_fopen(filename, "w"); // "w" = write (�r�s)
    if (!file)
    {
        g_warning("Hiba: A %s f�jlt nem siker�lt megnyitni �r�sra.", filename);
        return;
    }

    // V�gigmegy�nk a list�n
    for (GList *l = list; l != NULL; l = l->next)
    {
        TodoItem *item = (TodoItem *)l->data;

        // "Megtiszt�tjuk" a stringeket ment�s el�tt
        gchar *title_safe = sanitize_string(item->title);
        gchar *desc_safe = sanitize_string(item->description);
        gchar *cat_safe = sanitize_string(item->category);

        // Ki�rjuk az adatokat TAB-bal elv�lasztva
        fprintf(file, "%s\t%s\t%s\t%d\t%d\n",
                title_safe,
                desc_safe,
                cat_safe,
                item->priority,
                item->completed ? 1 : 0 // Ment�s 1-k�nt vagy 0-k�nt
        );

        // Felszabad�tjuk a megtiszt�tott stringeket
        g_free(title_safe);
        g_free(desc_safe);
        g_free(cat_safe);
    }

    fclose(file);
    g_print("Lista elmentve a %s f�jlba.\n", filename);
}

/*
 * Bet�lti a list�t a f�jlb�l.
 */
GList *io_load_from_file(const gchar *filename)
{
    if (!g_file_test(filename, G_FILE_TEST_EXISTS))
    {
        g_print("A %s f�jl nem tal�lhat�, �res lista bet�ltve.\n", filename);
        return NULL;
    }

    FILE *file = g_fopen(filename, "r"); // "r" = read (olvas�s)
    if (!file)
    {
        g_warning("Hiba: A %s f�jlt nem siker�lt megnyitni olvas�sra.", filename);
        return NULL;
    }

    GList *list = NULL;
    char buffer[2048]; // Tegy�k fel, hogy egy sor max 2KB
    guint lines_loaded = 0;

    // Olvasunk soronk�nt, am�g a f�jl v�g�re nem �r�nk
    while (fgets(buffer, sizeof(buffer), file))
    {
        // Elt�vol�tjuk a sorv�gi \n karaktert
        g_strchomp(buffer);

        // Felbontjuk a sort a TAB karakterek ment�n
        gchar **parts = g_strsplit(buffer, "\t", 5); // Max 5 r�szre v�gjuk

        // Ellen�rizz�k, hogy megvan-e mind az 5 r�sz
        if (g_strv_length(parts) == 5)
        {
            TodoItem *item = g_new0(TodoItem, 1);
            item->title = g_strdup(parts[0]);
            item->description = g_strdup(parts[1]);
            item->category = g_strdup(parts[2]);
            // Sztringb�l integerr� alak�t�s
            item->priority = (gint)g_ascii_strtoll(parts[3], NULL, 10);
            item->completed = (gboolean)(g_ascii_strtoll(parts[4], NULL, 10) != 0);

            list = g_list_append(list, item);
            lines_loaded++;
        }

        g_strfreev(parts); // Felszabad�tjuk a felbontott t�mb�t
    }

    fclose(file);
    g_print("%d elem bet�ltve a %s f�jlb�l.\n", lines_loaded, filename);
    return list;
}
