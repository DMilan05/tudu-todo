#ifndef MODEL_H
#define MODEL_H

#include <gtk/gtk.h>
#include "view.h" // A ViewWidgets miatt

/**
 * A feladat lehetséges állapotai.
 */
typedef enum {
    STATUS_PENDING,     // Függőben (0)
    STATUS_PROGRESS,    // Folyamatban (1)
    STATUS_DONE         // Kész (2)
} TaskStatus;

/**
 * Egy feladat adatai.
 * Egyszerűsítve a controller.c-ben látott logikához.
 */
typedef struct {
    char title[100];
    // char description[256]; // Most nem használjuk a controller-ben
    // char deadline[20];     // Most nem használjuk a controller-ben
    TaskStatus status;
} Task;

/**
 * A program adatmodellje: a feladatok listái állapot szerint csoportosítva.
 */
typedef struct {
    GList *pending;     // Függőben lévő Task-ok listája
    GList *progress;    // Folyamatban lévő Task-ok listája
    GList *done;        // Kész Task-ok listája
} Model;

/**
 * A controller-ben használt adatstruktúra.
 */
typedef struct {
    Model *model;
    ViewWidgets *view;
} AppData;

// Függvény deklarációk
Model* model_new(void);
void model_add_task(Model *m, const char *title, TaskStatus status);
void model_remove_task(Model *m, TaskStatus status, int index);
void model_refresh_lists(Model *m, ViewWidgets *vw);
void model_free(Model *model);

#endif
