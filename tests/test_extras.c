#include <criterion/criterion.h>
#include <glib.h>
#include "../include/extras.h"

Test(extras_trim, null_returns_empty) {
    gchar *r = extras_trim(NULL);
    cr_assert_not_null(r);
    cr_assert_str_eq(r, "", "NULL should return empty string");
    g_free(r);
}

Test(extras_trim, trims_whitespace) {
    gchar *r = extras_trim("   \t Hello World \n  ");
    cr_assert_str_eq(r, "Hello World");
    g_free(r);
}

Test(extras_trim, no_change) {
    const gchar *s = "nochange";
    gchar *r = extras_trim(s);
    cr_assert_str_eq(r, s);
    g_free(r);
}

Test(extras_format_priority, formats_correctly) {
    gchar *s = extras_format_priority(7);
    cr_assert_str_eq(s, "Priority: 7");
    g_free(s);
}

Test(extras_get_css_class_for_priority, in_range_values) {
    const gchar *c0 = extras_get_css_class_for_priority(0);
    const gchar *c5 = extras_get_css_class_for_priority(5);
    const gchar *c10 = extras_get_css_class_for_priority(10);

    cr_assert_str_eq(c0, "priority-0");
    cr_assert_str_eq(c5, "priority-5");
    cr_assert_str_eq(c10, "priority-10");
}

Test(extras_get_css_class_for_priority, out_of_range_clamps) {
    const gchar *cneg = extras_get_css_class_for_priority(-3);
    const gchar *cbig = extras_get_css_class_for_priority(99);

    cr_assert_str_eq(cneg, "priority-0");
    cr_assert_str_eq(cbig, "priority-10");
}