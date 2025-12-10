#include <criterion/criterion.h>
#include <criterion/logging.h>


Test(basic, arithmetic) {
    cr_log_info("Futtatom a basic arithmetic tesztet");
    cr_assert_eq(2 + 2, 4, "Basic arithmetic should work");
}