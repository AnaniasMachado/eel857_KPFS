#include <stdio.h>
#include <stdlib.h>

// #include "../util/util_sol.c"
#include "../local_search/rvnd.c"

// // Implementation of algorithm iterated local search
// Solution* iterated_local_search(KnapsackInstance *instance, int max_iterations, int ls_max_iterations) {
//     // Builds a initial solution using a constructive algorithm
//     Solution *init_sol = build_initial_solution(instance);
//     printf("Initial solution objective value: %d\n", objective_value(init_sol));

//     // Initializes auxilary variables
//     Solution *sol_star = create_solution(instance->item_count);
//     Solution *sol_prime = create_solution(instance->item_count);
//     Solution *sol_star_prime = create_solution(instance->item_count);

//     // Applies local search on initial solution
//     rvnd_local_search(instance, init_sol, sol_star, ls_max_iterations);

//     // Enhances solution until max_iterations
//     for (int i = 1; i < max_iterations; i++) {
//         perturb_solution(instance, sol_star, sol_prime);
//         rvnd_local_search(instance, sol_prime, sol_star_prime, ls_max_iterations);
//         sol_star = acceptance_criterion(sol_star_prime, sol_prime);
//         printf("Current solution objective value: %d\n", objective_value(sol_star));
//     }
//     return sol_star;
// }

// Implementation of algorithm iterated local search
Solution* iterated_local_search(KnapsackInstance *instance, int ls_max_iterations) {
    // Builds a initial solution using a constructive algorithm
    Solution *init_sol = build_initial_solution(instance);
    printf("Initial solution objective value: %d\n", objective_value(init_sol));

    // Initializes auxilary variables
    Solution *sol_star = create_solution(instance->item_count);
    Solution *sol_prime = create_solution(instance->item_count);
    Solution *sol_star_prime = create_solution(instance->item_count);
    int prev_obj_val = objective_value(init_sol);
    int no_change = 0;

    // Applies local search on initial solution
    rvnd_local_search(instance, init_sol, sol_star, ls_max_iterations);

    // Enhances solution until max_iterations
    while (1) {
        perturb_solution(instance, sol_star, sol_prime);
        rvnd_local_search(instance, sol_prime, sol_star_prime, ls_max_iterations);
        sol_star = acceptance_criterion(sol_star_prime, sol_prime);
        if (objective_value(sol_star) == prev_obj_val) {
            ++no_change;
        } else {
            no_change = 0;
        }
        prev_obj_val = objective_value(sol_star);
        if (no_change >= 20) {
            return sol_star;
        }
        printf("Current solution objective value: %d\n", objective_value(sol_star));
        printf("No change value: %d\n", no_change);
    }
}