#include "../improvement/add_item_fi.c"
#include "../improvement/add_item_bi.c"

int add_item_ls(const KnapsackInstance *instance, Solution *sol, char* type_improvement) {
    int improved = 0;
    int (*try_add_item)(const KnapsackInstance *, Solution *) = NULL;
    if (strcmp(type_improvement, "fi") == 0) {
        try_add_item = try_add_item_fi;
    } else if (strcmp(type_improvement, "bi") == 0) {
        try_add_item = try_add_item_bi;
    }
    while (1) {
        if (try_add_item(instance, sol)) {
            improved = 1;
        } else {
            return improved;
        }
    }
}