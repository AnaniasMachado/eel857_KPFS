#include "../improvement/remove_item_fi.c"

int remove_item_ls(const KnapsackInstance *instance, Solution *sol) {
    int improved = 0;
    while (1) {
        if (try_remove_item_fi(instance, sol)) {
            improved = 1;
        } else {
            return improved;
        }
    }
}