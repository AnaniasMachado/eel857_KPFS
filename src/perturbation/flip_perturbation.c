#include <stdio.h>
#include <stdlib.h>

// Perturbs solution with flips
void flip_perturb_solution(const KnapsackInstance *instance, Solution *sol, Solution *sol_prime) {
    // Creates a copy of sol
    copy_solution(instance, sol, sol_prime);

    // Perturbs solution
    int perturbation_size = 2;
    for (int p = 0; p < perturbation_size; p++) {
        int idx = rand() % instance->item_count;
        // Flips inclusion
        if (sol_prime->included[idx]) {
            // Removes item
            sol_prime->included[idx] = 0;

            // Recomputes total weight and value
            sol_prime->total_weight -= instance->items[idx].weight;
            sol_prime->total_value -= instance->items[idx].value;

            // Recomputes penalties
            sol_prime->total_penalty = compute_penalty(instance, sol_prime);
        } else {
            // Adds item if feasible
            int new_weight = sol_prime->total_weight + instance->items[idx].weight;
            if (new_weight <= instance->capacity) {
                sol_prime->included[idx] = 1;

                // Recomputes total weight and value
                sol_prime->total_weight += instance->items[idx].weight;
                sol_prime->total_value += instance->items[idx].value;

                // Recomputes penalties
                sol_prime->total_penalty = compute_penalty(instance, sol_prime);
            }
        }
    }
}