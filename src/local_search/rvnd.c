#include <stdio.h>
#include <stdlib.h>

// Neighborhood: Adds item
int try_add_item(const KnapsackInstance *instance, Solution *sol) {
    for (int i = 0; i < instance->item_count; i++) {
        if (!sol->included[i]) {
            // Try adding item
            sol->included[i] = 1;

            // Update total weight and value
            int new_weight = sol->total_weight + instance->items[i].weight;
            if (new_weight <= instance->capacity) {
                // Recompute total value and penalties
                int new_value = sol->total_value + instance->items[i].value;
                // Recompute penalties
                int new_penalty = compute_penalty(instance, sol);
                // Accept move
                int new_obj = new_value - new_penalty;
                if (new_obj > (sol->total_value - sol->total_penalty)) {
                    // Update solution
                    sol->total_value = new_value;
                    sol->total_weight = new_weight;
                    sol->total_penalty = new_penalty;
                    return 1; // Improvement
                } else {
                    // Revert addition
                    sol->included[i] = 0;
                }
            } else {
                // Capacity exceeded, revert
                sol->included[i] = 0;
            }
        }
    }
    return 0; // No improvement
}

// Neighborhood: Removes item
int try_remove_item(const KnapsackInstance *instance, Solution *sol) {
    for (int i = 0; i < instance->item_count; i++) {
        if (sol->included[i]) {
            // Try removing item
            sol->included[i] = 0;

            // Recompute total weight and value
            int new_weight = sol->total_weight - instance->items[i].weight;
            int new_value = sol->total_value - instance->items[i].value;

            // Recompute penalties
            int new_penalty = compute_penalty(instance, sol);

            // Check feasibility
            if (new_weight <= instance->capacity) {
                double new_obj = new_value - new_penalty;
                if (new_obj > (sol->total_value - sol->total_penalty)) {
                    // Accept removal
                    sol->total_value = new_value;
                    sol->total_weight = new_weight;
                    sol->total_penalty = new_penalty;
                    return 1; // Improvement
                }
            }
            // Revert removal
            sol->included[i] = 1;
        }
    }
    return 0; // No improvement
}

// Swap two items in the solution
int try_swap(const KnapsackInstance *instance, Solution *sol) {
    for (int i = 0; i < instance->item_count; i++) {
        if (sol->included[i]) {
            for (int j = 0; j < instance->item_count; j++) {
                if (!sol->included[j]) {
                    // Try swapping i with j
                    sol->included[i] = 0;
                    sol->included[j] = 1;

                    // Check scapacity
                    int new_weight = sol->total_weight - instance->items[i].weight + instance->items[j].weight;
                    if (new_weight <= instance->capacity) {
                        // Recompute value
                        int new_value = sol->total_value - instance->items[i].value;

                        // Recompute penalties
                        int new_penalty = compute_penalty(instance, sol);

                        double new_obj = new_value - new_penalty;
                        if (new_obj > (sol->total_value - sol->total_penalty)) {
                            // Accept swap
                            sol->total_value = new_value;
                            sol->total_weight = new_weight;
                            sol->total_penalty = new_penalty;
                            return 1;
                        }
                    }
                    // Revert swap
                    sol->included[i] = 1;
                    sol->included[j] = 0;
                }
            }
        }
    }
    return 0; // No improvement
}

// RVND algorithm
void rvnd_local_search(const KnapsackInstance *instance, Solution *initial_sol, Solution *sol_star_prime, int max_iterations) {
    // List of neighborhoods
    typedef int (*NeighborhoodFunc)(const KnapsackInstance *, Solution *);
    NeighborhoodFunc neighborhoods[] = {try_add_item, try_remove_item, try_swap};
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