void search_init(GtkSearchEntry *search_entry, GtkListBox *list_box, GList *todos);
GList* search_filter(GList *list, const gchar *query);
