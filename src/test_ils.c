#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "types/item.h"
#include "types/forfeit_set.h"
#include "types/knapsack_instance.h"
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

        int max_iterations = 100;
        int no_change_limit = 10;
        char type_improvement[3];
        strcpy(type_improvement, "fi");
        srand(time(NULL));

        struct timespec start, end;
        double elapsed;

        clock_gettime(CLOCK_MONOTONIC, &start);

        Solution* sol = iterated_local_search(&instance, max_iterations, no_change_limit, type_improvement);

        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        printf("Solution objective value: %d\n", objective_value(sol));
        printf("Viability: %d\n", instance.capacity - sol->total_weight);
        printf("Time: %.9f\n", elapsed);

        // Free memory
        freeInstance(&instance);
    } else {
        printf("Failed to load instance.\n");
    }
    return 0;
}