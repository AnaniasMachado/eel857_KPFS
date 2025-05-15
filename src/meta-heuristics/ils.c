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
Solution* iterated_local_search(KnapsackInstance *instance, int max_iterations, int ls_max_iterations, int no_change_limit) {
    // Build a initial solution using a constructive algorithm
    Solution *init_sol = build_initial_solution_2(instance);
    printf("Initial solution objective value: %d\n", objective_value(init_sol));
    printf("Initial solution viability: %d\n", instance->capacity - init_sol->total_weight);

    // Initialize auxilary variables
    Solution *sol_star = create_solution(instance->item_count);
    Solution *sol_star_cur = create_solution(instance->item_count);
    Solution *sol_prime = create_solution(instance->item_count);
    Solution *sol_star_prime = create_solution(instance->item_count);
    int prev_obj_val = objective_value(init_sol);
    int no_change = 0;
    typedef void (*PerturbationFunc)(const KnapsackInstance *, const Solution *, Solution *);
    PerturbationFunc perturbationfuncs[] = {flip_perturb_solution, hybrid_perturb_solution, remove_perturb_solution, add_perturb_solution, xor_perturb_solution};
    int perturbationsfuncs_count = 5;

    // Apply local search on initial solution
    rvnd_local_search_fi(instance, init_sol, sol_star, ls_max_iterations);
    copy_solution(instance, sol_star, sol_star_cur);

    printf("Is same solution after ls? %d\n", is_same_solution(instance, init_sol, sol_star));

    // Enhance solution until max_iterations
    for (int k = 0; k <= max_iterations; ++k) {
        int idx = rand() % perturbationsfuncs_count;
        void (*perturbationfunc)(const KnapsackInstance *, const Solution *, Solution *) = perturbationfuncs[idx];
        int threshold = (int) ((double) no_change / (double) (no_change_limit - 1));
        int rand_num = rand() % no_change_limit;
        if (rand_num <= threshold) {
            overhaul_perturb_solution(instance, sol_star_cur, sol_prime);
        } else {
            perturbationfunc(instance, sol_star_cur, sol_prime);
        }
        printf("Is same solution after perturb? %d\n", is_same_solution(instance, sol_star_cur, sol_prime));
        rvnd_local_search_fi(instance, sol_prime, sol_star_prime, ls_max_iterations);
        printf("LS solution objective value: %d\n", objective_value(sol_star_prime));
        printf("LS solution viability: %d\n", instance->capacity - sol_star_prime->total_weight);
        printf("Is same solution after ls? %d\n", is_same_solution(instance, sol_prime, sol_star_prime));
        if (objective_value(sol_star_prime) > objective_value(sol_star)) {
            copy_solution(instance, sol_star_prime, sol_star);
        }
        acceptance_criterion(instance, sol_star_cur, sol_star_prime, no_change, no_change_limit);
        if (objective_value(sol_star_cur) == prev_obj_val) {
            ++no_change;
        } else {
            no_change = 0;
        }
        prev_obj_val = objective_value(sol_star_cur);
        if (no_change >= no_change_limit) {
            free_solution(sol_star_cur);
            free_solution(sol_prime);
            free_solution(sol_star_prime);
            return sol_star;
        }
        printf("Current solution objective value: %d\n", objective_value(sol_star_cur));
        printf("Current solution viability: %d\n", instance->capacity - sol_star_cur->total_weight);
        printf("Best solution objective value: %d\n", objective_value(sol_star));
        printf("Best solution viability: %d\n", instance->capacity - sol_star->total_weight);
        printf("No change value: %d\n", no_change);
    }
    printf("Maximum number of iterations reached\n");
    free_solution(sol_star_cur);
    free_solution(sol_prime);
    free_solution(sol_star_prime);
    return sol_star;
}