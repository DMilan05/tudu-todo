#ifndef MODEL_H
#define MODEL_H

#include <gtk/gtk.h>

// Egyetlen TODO elem
typedef struct {
    char *text;
} TodoItem;

// A teljes alkalmaz�s modellje
typedef struct {
    GList *items; // Dinamikus lista a feladatokr�l
} TodoModel;

// Modell inicializ�l�sa
TodoModel* model_new(void);

// �j elem hozz�ad�sa
void model_add_item(TodoModel *model, const char *text);

// Elem t�rl�se (index alapj�n)
void model_remove_item(TodoModel *model, int index);

// A GTK list�hoz szinkroniz�l�s (View friss�t�s)
void model_refresh_list(TodoModel *model, GtkListBox *list);

#endif
