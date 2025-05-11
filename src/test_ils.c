#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types/item.h"
#include "types/forfeit_set.h"
#include "types/knacksack_instance.h"
#include "types/solution.h"
#include "util/util_instance.c"
#include "util/util_sol.c"
#include "./meta-heuristics/ils.c"

int main() {
    KnapsackInstance instance;
    const char *filename = "../instances/scenario1/not_correlated_sc1/300/kpfs_1.txt";
    
    if (!loadInstance(filename, &instance)) {
        printf("Instance loaded succesfully!!\n");
        printf("Number of items: %d\n", instance.item_count);
        printf("Capacity: %d\n", instance.capacity);
        printf("Number of forfeit sets: %d\n", instance.forfeit_count);

        // int max_iterations = 10;
        int ls_max_iterations = 5;
        srand(time(NULL));

        // Solution* sol = iterated_local_search(&instance, max_iterations, ls_max_iterations);
        Solution* sol = iterated_local_search(&instance, ls_max_iterations);
        printf("Solution objective value: %d\n", objective_value(sol));
        printf("Viability: %d\n", instance.capacity - sol->total_weight);

        // Free memory
        freeInstance(&instance);
    } else {
        printf("Failed to load instance.\n");
    }
    return 0;
}