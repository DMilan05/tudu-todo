#ifndef MODEL_H
#define MODEL_H

#include <glib.h>

// TODO szöveges fájlbol lácolt lista, minden manipulációt a memóriában tartok, program bezárásakor a memóriábol felülirni a fájlt

typedef struct
{
    gchar *title;       // A feladat c�me
    gchar *description; // A feladat r�szletes le�r�sa
    gchar *category;    // Kateg�ria vagy c�mke (opcion�lis)
    gint priority;      // Priorit�s (0-10)
    gboolean completed; // K�sz van-e
    // TODO dátum kezelés hozzáaddása
} TodoItem;

GList *model_create_list(void);
void model_add_item(GList **list, TodoItem *item);
void model_remove_item(GList **list, gint index);
void model_free_item(TodoItem *item);
void model_free_list(GList *list);

#endif
