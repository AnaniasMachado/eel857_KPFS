#include "../improvement/swap_item_fi.c"
#include "../improvement/swap_item_bi.c"

int swap_item_ls(const KnapsackInstance *instance, Solution *sol, char* type_improvement) {
    int improved = 0;
    int (*try_swap_item)(const KnapsackInstance *, Solution *) = NULL;
    if (strcmp(type_improvement, "fi") == 0) {
        try_swap_item = try_swap_item_fi;
    } else if (strcmp(type_improvement, "bi") == 0) {
        try_swap_item = try_swap_item_bi;
    }
    while (1) {
        if (try_swap_item(instance, sol)) {
            improved = 1;
        } else {
            return improved;
        }
    }
}