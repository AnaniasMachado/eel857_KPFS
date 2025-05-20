#include "../improvement/add_item_fi.c"

int add_item_ls(const KnapsackInstance *instance, Solution *sol) {
    int improved = 0;
    while (1) {
        if (try_add_item_fi(instance, sol)) {
            improved = 1;
        } else {
            return improved;
        }
    }
}