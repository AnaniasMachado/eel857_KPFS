#include <stdio.h>
#include <stdlib.h>

#include "../neighborhood/add_item.c"
#include "../neighborhood/remove_item.c"
#include "../neighborhood/swap_item.c"

// RVND algorithm
void rvnd_local_search_fi(const KnapsackInstance *instance, Solution *initial_sol, Solution *sol_star_prime, int max_iterations) {
    // List of neighborhoods
    typedef int (*NeighborhoodFunc)(const KnapsackInstance *, Solution *);
    NeighborhoodFunc neighborhoods[] = {try_add_item_fi, try_remove_item_fi, try_swap_fi};
    int neighborhood_count = 3;

    Solution *current = create_solution(instance->item_count);
    copy_solution(instance, initial_sol, current);

    int iterations = 0;
    while (iterations < max_iterations) {
        int neighborhood_indices[] = {0, 1, 2};
        shuffle(neighborhood_indices, neighborhood_count);

        int improved = 0;
        for (int n_idx = 0; n_idx < neighborhood_count; n_idx++) {
            int n = neighborhood_indices[n_idx];
            int (*neighborhood)(const KnapsackInstance *, Solution *) = neighborhoods[n];

            if (neighborhood(instance, current)) {
                // If improved, reshuffle neighborhoods
                improved = 1;
                break; // Restart with shuffled neighborhoods
            }
        }
        if (!improved) {
            // No improvement in any neighborhood
            break;
        }
        iterations++;
    }
    // Copy the best solution back
    copy_solution(instance, current, sol_star_prime);
    free_solution(current);
}