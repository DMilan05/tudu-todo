#ifndef IO_H
#define IO_H

#include <glib.h>
#include "model.h"

// Elmenti a teljes listát a megadott fájlba
void io_save_to_file(const gchar *filename, GList *list);

// Betölti a listát a fájlból. Ha a fájl nem létezik, NULL-t ad vissza.
GList* io_load_from_file(const gchar *filename);

#endif /* IO_H */
