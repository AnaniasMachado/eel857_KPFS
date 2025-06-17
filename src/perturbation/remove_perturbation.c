#include <stdio.h>
#include <stdlib.h>

// Perturb solution removing items
void remove_perturb_solution(const KnapsackInstance *instance, const Solution *sol, Solution *sol_prime) {
    // Create a copy of sol
    copy_solution(instance, sol, sol_prime);

    // Perturb solution
    int upper_lim = (int) (instance->item_count * 0.05);
    int lower_lim = 1;
    int perturbation_size = lower_lim + rand() % (upper_lim - lower_lim + 1);
    for (int p = 0; p < perturbation_size; p++) {
        int idx = sol_prime->not_included_items[rand() % sol_prime->size_not_included];
        // Check inclusion
        if (sol_prime->included[idx]) {
            // Remove item
            // sol_prime->included[idx] = 0;
            remove_item(sol_prime, idx);

            // Recompute total weight and value
            sol_prime->total_weight -= instance->items[idx].weight;
            sol_prime->total_value -= instance->items[idx].value;

            // Recompute penalties
            sol_prime->total_penalty = compute_penalty(instance, sol_prime);
        }
    }
}