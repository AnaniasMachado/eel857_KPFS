#include <stdio.h>
#include <stdlib.h>

#include "../local_search/rvnd.c"
#include "../perturbation/flip_perturbation.c"

// Implementation of algorithm iterated local search
Solution* iterated_local_search(KnapsackInstance *instance, int ls_max_iterations) {
    // Builds a initial solution using a constructive algorithm
    Solution *init_sol = build_initial_solution_2(instance);
    printf("Initial solution objective value: %d\n", objective_value(init_sol));
    printf("Initial solution viability: %d\n", instance->capacity - init_sol->total_weight);

    // Initializes auxilary variables
    Solution *sol_star = create_solution(instance->item_count);
    Solution *sol_prime = create_solution(instance->item_count);
    Solution *sol_star_prime = create_solution(instance->item_count);
    int prev_obj_val = objective_value(init_sol);
    int no_change = 0;

    // Applies local search on initial solution
    rvnd_local_search(instance, init_sol, sol_star, ls_max_iterations);

    printf("Is same solution after ls? %d\n", is_same_solution(instance, init_sol, sol_star));

    // Enhances solution until max_iterations
    while (1) {
        flip_perturb_solution(instance, sol_star, sol_prime);
        printf("Is same solution after perturb? %d\n", is_same_solution(instance, sol_star, sol_prime));
        rvnd_local_search(instance, sol_prime, sol_star_prime, ls_max_iterations);
        printf("LS solution objective value: %d\n", objective_value(sol_star_prime));
        printf("LS solution viability: %d\n", instance->capacity - sol_star_prime->total_weight);
        printf("Is same solution after ls? %d\n", is_same_solution(instance, sol_prime, sol_star_prime));
        acceptance_criterion(instance, sol_star, sol_star_prime);
        if (objective_value(sol_star) == prev_obj_val) {
            ++no_change;
        } else {
            no_change = 0;
        }
        prev_obj_val = objective_value(sol_star);
        if (no_change >= 10) {
            free_solution(sol_prime);
            free_solution(sol_star_prime);
            return sol_star;
        }
        printf("Current solution objective value: %d\n", objective_value(sol_star));
        printf("Current solution viability: %d\n", instance->capacity - sol_star->total_weight);
        printf("No change value: %d\n", no_change);
    }
}