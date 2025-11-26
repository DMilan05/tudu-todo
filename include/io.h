#ifndef IO_H
#define IO_H

#include <glib.h>
#include "model.h"

// Elmenti a teendok listajat a megadott fajlba.
void io_save_to_file(const gchar *filename, GList *list);

// Betolti a teendok listajat a megadott fajlbol.
// Ha a fajl nem letezik, NULL-t ad vissza.
GList* io_load_from_file(const gchar *filename);

#endif /* IO_H */
