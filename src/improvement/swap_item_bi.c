#include <stdio.h>
#include <stdlib.h>

// Improvement: Swap two items in the solution with best improvement
int try_swap_item_bi(const KnapsackInstance *instance, Solution *sol) {
    // Initialize best solution found variables
    int best_obj = sol->total_value - sol->total_penalty;
    int best_idx_i = -1;
    int best_idx_j = -1;

    // Try to improve solution
    for (int i = 0; i < instance->item_count; i++) {
        if (sol->included[i]) {
            for (int j = 0; j < instance->item_count; j++) {
                if (!sol->included[j]) {
                    // Try swapping i with j
                    // sol->included[i] = 0;
                    // sol->included[j] = 1;
                    remove_item(sol, i);
                    add_item(sol, j);

                    // Check scapacity
                    int new_weight = sol->total_weight - instance->items[i].weight + instance->items[j].weight;
                    if (new_weight <= instance->capacity) {
                        // Recompute value
                        int new_value = sol->total_value - instance->items[i].value + instance->items[j].value;

                        // Recompute penalties
                        int new_penalty = compute_penalty(instance, sol);

                        int new_obj = new_value - new_penalty;
                        // Accept remove
                        if (new_obj > best_obj) {
                            // Update best solution found variables
                            best_obj = new_obj;
                            best_idx_i = i;
                            best_idx_j = j;
                        }
                    }
                    // Revert swap
                    // sol->included[i] = 1;
                    // sol->included[j] = 0;
                    add_item(sol, i);
                    remove_item(sol, j);
                }
            }
        }
    }

    // After checking all options, do the best move
    if (best_idx_i != -1) {
        // Update solution with best move
        // sol->included[best_idx_i] = 0;
        // sol->included[best_idx_j] = 1;
        remove_item(sol, best_idx_i);
        add_item(sol, best_idx_j);
        sol->total_weight = sol->total_weight - instance->items[best_idx_i].weight + instance->items[best_idx_j].weight;
        sol->total_value = sol->total_value - instance->items[best_idx_i].value + instance->items[best_idx_j].value;
        sol->total_penalty = compute_penalty(instance, sol);
        return 1; // Improvement
    }

    return 0; // No improvement
}