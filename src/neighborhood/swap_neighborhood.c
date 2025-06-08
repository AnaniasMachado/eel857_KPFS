#include <stdio.h>
#include <stdlib.h>

// Improvement: Swap two items in the solution with first improvement
void try_swap_item_neighboord(const KnapsackInstance *instance, const Solution * sol_prime, Solution *sol_double_prime) {
    // Initialize an array of indices
    int *indices_arr = init_indices_array(instance->item_count);
    // Create a permutation of the array of indices
    shuffle(indices_arr, instance->item_count);
    // Copy sol_prime to sol_double_prime
    copy_solution(instance, sol_prime, sol_double_prime);

    // Try to improve solution
    for (int i = 0; i < instance->item_count; i++) {
        int idx_i = indices_arr[i];
        if (sol_double_prime->included[idx_i]) {
            for (int j = 0; j < instance->item_count; j++) {
                int idx_j = indices_arr[j];
                if (!sol_double_prime->included[idx_j]) {
                    // Try swapping i with j
                    sol_double_prime->included[idx_i] = 0;
                    sol_double_prime->included[idx_j] = 1;

                    // Check scapacity
                    int new_weight = sol_double_prime->total_weight - instance->items[idx_i].weight + instance->items[idx_j].weight;
                    if (new_weight <= instance->capacity) {
                        // Recompute value
                        int new_value = sol_double_prime->total_value - instance->items[idx_i].value + instance->items[idx_j].value;

                        // Recompute penalties
                        int new_penalty = compute_penalty(instance, sol_double_prime);

                        sol_double_prime->total_value = new_value;
                        sol_double_prime->total_weight = new_weight;
                        sol_double_prime->total_penalty = new_penalty;
                        free(indices_arr);
                        return;
                    }
                    // Revert swap
                    sol_double_prime->included[idx_i] = 1;
                    sol_double_prime->included[idx_j] = 0;
                }
            }
        }
    }
    free(indices_arr);
}