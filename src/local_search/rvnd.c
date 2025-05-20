#include <stdio.h>
#include <stdlib.h>

#include "add_item_ls.c"
#include "remove_item_ls.c"
#include "swap_item_ls.c"

// RVND algorithm
void rvnd_local_search_fi(const KnapsackInstance *instance, Solution *initial_sol, Solution *sol_star_prime) {
    // List of neighborhoods
    typedef int (*NeighborhoodFunc)(const KnapsackInstance *, Solution *);
    NeighborhoodFunc neighborhoods[] = {add_item_ls, remove_item_ls, swap_item_ls};
    int neighborhood_count = 3;

    // Shuffle neighborhoods
    int neighborhood_indices[] = {0, 1, 2};
    shuffle(neighborhood_indices, neighborhood_count);

    Solution *current = create_solution(instance->item_count);
    copy_solution(instance, initial_sol, current);

    while (1) {
        int improved = 0;
        for (int n_idx = 0; n_idx < neighborhood_count; n_idx++) {
            int n = neighborhood_indices[n_idx];
            int (*neighborhood)(const KnapsackInstance *, Solution *) = neighborhoods[n];

            if (neighborhood(instance, current)) {
                // If improved, restart loop
                improved = 1;
                break;
            }
        }
        if (!improved) {
            // No improvement in any neighborhood
            break;
        }
    }
    // Copy the best solution back
    copy_solution(instance, current, sol_star_prime);
    free_solution(current);
}