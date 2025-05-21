#include "../improvement/remove_item_fi.c"
#include "../improvement/remove_item_bi.c"

int remove_item_ls(const KnapsackInstance *instance, Solution *sol, char* type_improvement) {
    int improved = 0;
    int (*try_remove_item)(const KnapsackInstance *, Solution *) = NULL;
    if (strcmp(type_improvement, "fi") == 0) {
        try_remove_item = try_remove_item_fi;
    } else if (strcmp(type_improvement, "bi") == 0) {
        try_remove_item = try_remove_item_bi;
    }
    while (1) {
        if (try_remove_item(instance, sol)) {
            improved = 1;
        } else {
            return improved;
        }
    }
}