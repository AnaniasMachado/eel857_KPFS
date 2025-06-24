#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// #include "types/item.h"
// #include "types/forfeit_set.h"
// #include "types/knapsack_instance.h"
// #include "types/solution.h"
// #include "util/util_instance.c"
// #include "util/util_sol.c"
// #include "./meta-heuristics/ils.c"

DataInst* run_ils(FILE* f, int scenario_id, const char* corr_type, int size, int inst_idx) {
    KnapsackInstance instance;
    char filename[100];
    sprintf(filename, "../instances/scenario%d/%s_sc%d/%d/kpfs_%d.txt", scenario_id, corr_type, scenario_id, size, inst_idx);
    // const char *filename = "../instances/scenario1/not_correlated_sc1/300/kpfs_1.txt";

    if (!loadInstance(filename, &instance)) {
        int max_iterations = 100;
        int no_change_limit = 10;
        char type_improvement[3];
        strcpy(type_improvement, "fi");
        srand(time(NULL));

        int N = 2;
        struct timespec start, end;
        double elapsed;

        DataInst *data_inst = malloc(sizeof(DataInst));
        data_inst->obj_vals = malloc(N * sizeof(int));
        data_inst->elapseds = malloc(N * sizeof(double));

        for (int i = 0; i < N; i++) {
            clock_gettime(CLOCK_MONOTONIC, &start);
            Solution* sol = iterated_local_search(&instance, max_iterations, no_change_limit, type_improvement);
            clock_gettime(CLOCK_MONOTONIC, &end);
            elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            data_inst->obj_vals[i] = objective_value(sol);

            data_inst->elapseds[i] = elapsed;
            free_solution(sol);
        }

        // Free memory
        freeInstance(&instance);

        // for (int i = 0; i < N; i++) {
        //     printf("Obj val: %d\n", data_inst->obj_vals[i]);
        //     printf("Elapsed: %f\n", data_inst->elapseds[i]);
        //     fprintf(f, "%d,%s,%d,%d,%d, %.6f\n", scenario_id, corr_type, size, inst_idx, data_inst->obj_vals[i], data_inst->elapseds[i]);
        // }

        for (int i = 0; i < N; i++) {
            char buffer[200];
            snprintf(buffer, sizeof(buffer), "%d,%s,%d,%d,%d, %.6f\n", scenario_id, corr_type, size, inst_idx, data_inst->obj_vals[i], data_inst->elapseds[i]);
            printf("%s", buffer);
        }

        return data_inst;
    } else {
        printf("Failed to load instance.\n");
    }
}