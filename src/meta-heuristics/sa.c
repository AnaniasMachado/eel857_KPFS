#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../local_search/rvnd.c"

#include "../neighborhood/add_neighborhood.c"
#include "../neighborhood/remove_neighborhood.c"
#include "../neighborhood/swap_neighborhood.c"

#include "../perturbation/flip_perturbation.c"
#include "../perturbation/hybrid_perturbation.c"
#include "../perturbation/remove_perturbation.c"
#include "../perturbation/add_perturbation.c"
#include "../perturbation/xor_perturbation.c"
#include "../perturbation/overhaul_perturbation.c"

// Implementation of algorithm iterated local search
Solution* simulated_annealing(KnapsackInstance *instance, int max_iterations, double T, double t_0, double alpha, int no_change_limit, char *type_improvement) {
    // Build a initial solution using a constructive algorithm
    printf("Point 7\n");
    Solution *init_sol = build_initial_solution_2(instance);
    printf("Point 8\n");

    // Initialize auxilary variables
    Solution *sol_star = create_solution(instance->item_count);
    Solution *sol_prime = create_solution(instance->item_count);
    Solution *sol_double_prime = create_solution(instance->item_count);
    printf("Point 9\n");
    int prev_obj_val = objective_value(init_sol);
    int no_change = 0;
    // typedef void (*NeighborhoodFunc)(const KnapsackInstance *, const Solution*, Solution *);
    // NeighborhoodFunc neighborhoodfuncs[] = {try_add_item_neighboord, try_remove_item_neighboord, try_swap_item_neighboord};
    // int neighborhoodfuncs_count = 3;
    typedef void (*NeighborhoodFunc)(const KnapsackInstance *, const Solution *, Solution *);
    NeighborhoodFunc neighborhoodfuncs[] = {flip_perturb_solution, hybrid_perturb_solution, remove_perturb_solution, add_perturb_solution, xor_perturb_solution};
    int neighborhoodfuncs_count = 5;
    printf("Point 10\n");

    // Apply local search on initial solution
    rvnd_local_search(instance, init_sol, sol_star, type_improvement);
    printf("Point 11\n");

    printf("Is same solution after ls? %d\n", is_same_solution(instance, init_sol, sol_star));
    free_solution(init_sol);

    int iter = 0;
    double t = t_0;

    while (t > T) {
        while (iter < max_iterations) {
            iter += 1;
            int idx = rand() % neighborhoodfuncs_count;
            void (*neighborhoodfunc)(const KnapsackInstance *, const Solution *, Solution *) = neighborhoodfuncs[idx];
            printf("Point 12\n");
            neighborhoodfunc(instance, sol_prime, sol_double_prime);
            printf("Point 13\n");
            int delta = objective_value(sol_double_prime) - objective_value(sol_prime);
            printf("Point 14\n");
            if (delta >= 0) {
                copy_solution(instance, sol_prime, sol_double_prime);
                if (objective_value(sol_star) < objective_value(sol_prime)) {
                    copy_solution(instance, sol_prime, sol_star);
                    printf("Point 15\n");
                }
                else {
                    double rand_num = (double) rand() / (double) RAND_MAX;
                    double prob = exp(-delta / t);
                    if (rand_num < prob) {
                        copy_solution(instance, sol_double_prime, sol_prime);
                    }
                    printf("Point 16\n");
                }
            }
        }
        t = alpha * t;
        iter = 0;
        printf("Point 17\n");
    }
    free_solution(sol_prime);
    printf("Point 18\n");
    free_solution(sol_double_prime);
    printf("Point 19\n");
    return sol_star;
}