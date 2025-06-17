#include <stdio.h>
#include <stdlib.h>

#include "../local_search/rvnd.c"
#include "../perturbation/flip_perturbation.c"
#include "../perturbation/hybrid_perturbation.c"
#include "../perturbation/remove_perturbation.c"
#include "../perturbation/add_perturbation.c"
#include "../perturbation/xor_perturbation.c"
#include "../perturbation/overhaul_perturbation.c"

// Implementation of algorithm iterated local search
Solution* greedy_randomized_adaptive_search_procedure(KnapsackInstance *instance, int max_iterations, int no_change_limit, char *type_improvement) {
    // Initialize auxilary variables
    Solution *sol_star = create_solution(instance->item_count);
    Solution *sol_star_prime = create_solution(instance->item_count);
    printf("Point 6\n");
    int prev_obj_val = 0;
    int no_change = 0;
    printf("Point 7\n");

    // Enhance solution until stopping criterion
    for (int k = 0; k <= max_iterations; ++k) {
        Solution *sol_prime = build_initial_solution_3(instance);
        printf("Point 8\n");
        rvnd_local_search(instance, sol_prime, sol_star_prime, type_improvement);
        printf("Point 9\n");
        if (objective_value(sol_star_prime) > objective_value(sol_star)) {
            copy_solution(instance, sol_star_prime, sol_star);
        }
        printf("Point 10\n");
        if (objective_value(sol_star) == prev_obj_val) {
            ++no_change;
        } else {
            no_change = 0;
        }
        printf("Point 11\n");
        prev_obj_val = objective_value(sol_star);
        printf("Point 12\n");
        if (no_change >= no_change_limit) {
            free_solution(sol_prime);
            free_solution(sol_star_prime);
            return sol_star;
        }
        printf("Point 13\n");
        if (k == max_iterations) {
            free(sol_prime);
        }
        printf("Current solution objective value: %d\n", objective_value(sol_star_prime));
        printf("Current solution viability: %d\n", instance->capacity - sol_star_prime->total_weight);
        printf("Best solution objective value: %d\n", objective_value(sol_star));
        printf("Best solution viability: %d\n", instance->capacity - sol_star->total_weight);
        printf("No change value: %d\n", no_change);
    }
    printf("Maximum number of iterations reached\n");
    free_solution(sol_star_prime);
    return sol_star;
}