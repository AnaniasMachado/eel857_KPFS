#include <stdio.h>
#include <stdlib.h>

// Structure to hold item index and ratio, for sorting purposes
typedef struct {
    int index;
    double ratio;
} ItemRatio;

// Function to compare two ItemRatio for qsort (descending order)
int compare_ratio(const void *a, const void *b) {
    ItemRatio *ir1 = (ItemRatio *)a;
    ItemRatio *ir2 = (ItemRatio *)b;
    if (ir2->ratio > ir1->ratio)
        return 1;
    else if (ir2->ratio < ir1->ratio)
        return -1;
    else
        return 0;
}

// Computes the ratio value/weight for each item and returns an array of indices sorted descendingly by ratio
int* compute_and_sort_ratios(const KnapsackInstance *inst) {
    int n = inst->item_count;
    ItemRatio *ratios = malloc(n * sizeof(ItemRatio));
    for (int i = 0; i < n; i++) {
        ratios[i].index = i;
        if (inst->items[i].weight > 0)
            ratios[i].ratio = inst->items[i].value / inst->items[i].weight;
        else
            ratios[i].ratio = 0.0; // To avoid division by zero
    }

    qsort(ratios, n, sizeof(ItemRatio), compare_ratio);

    int *sorted_indices = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        sorted_indices[i] = ratios[i].index;
    }

    free(ratios);
    return sorted_indices;
}

// Computes solution penalty
double compute_penalty(const KnapsackInstance *instance, Solution *sol) {
    int total_penalty = 0;

    // Computes the penalty for each forfeit set
    for (int i = 0; i < instance->forfeit_count; i++) {
        ForfeitSet *fset = &instance->forfeit_sets[i];

        int count_included = 0;

        // For each item in the forfeit set
        for (int j = 0; j < fset->size; j++) {
            int item_id = fset->items[j];
            // Checks if item is in solution
            if (sol->included[item_id]) {
                count_included++;
            }
        }

        // Checks if exceed threshold
        if (count_included > fset->threshold) {
            total_penalty += (count_included - fset->threshold) * fset->penalty;
        }
    }

    return total_penalty;
}

// Builds a initial solution
Solution* build_initial_solution(const KnapsackInstance *instance) {
    Solution *sol = malloc(sizeof(Solution));
    sol->included = malloc(instance->item_count * sizeof(int));
    sol->total_value = 0;
    sol->total_weight = 0;
    sol->total_penalty = 0;
    int* sorted_indices = compute_and_sort_ratios(instance);

    // Loops through items of sorted_indices 
    for (int i = 0; i < instance->item_count; i++) {
        int item_idx = sorted_indices[i];
        int w = instance->items[item_idx].weight;
        if (sol->total_weight + w <= instance->capacity) {
            // Includes item
            sol->included[item_idx] = 1;
            sol->total_value += instance->items[item_idx].value;
            sol->total_weight += w;
        } else {
            // Does not includes item
            sol->included[item_idx] = 0;
        }
    }

    // Computes forfeit sets penalty
    sol->total_penalty = compute_penalty(instance, sol);
    return sol;
}

// Returns objetive function value
int objective_value(Solution *sol) {
    return sol->total_value - sol->total_penalty;
}

// Checks if solution is feasible
int is_feasible(const KnapsackInstance *instance, Solution *sol) {
    return sol->total_weight <= instance->capacity;
}

// Creates an empty solution
Solution *create_solution(int item_count) {
    Solution *sol = (Solution*) malloc(sizeof(Solution));
    sol->included = (int*) calloc(item_count, sizeof(int));
    sol->total_value = 0;
    sol->total_weight = 0;
    sol->total_penalty = 0;
    return sol;
}

// Copy solution
void copy_solution(const KnapsackInstance *instance, const Solution *src, Solution *dest) {
    for (int i = 0; i < instance->item_count; i++) {
        dest->included[i] = src->included[i];
    }
    dest->total_value = src->total_value;
    dest->total_weight = src->total_weight;
    dest->total_penalty = src->total_penalty;
}

// Free solution memory
void free_solution(Solution *sol) {
    free(sol->included);
    free(sol);
}

// Perturbs solution
void perturb_solution(const KnapsackInstance *instance, Solution *sol, Solution *sol_prime) {
    // Creates a copy of sol
    copy_solution(instance, sol, sol_prime);

    // Perturbs solution
    int perturbation_size = 2; // Number of items to flip
    for (int p = 0; p < perturbation_size; p++) {
        int idx = rand() % instance->item_count;
        // Flips inclusion
        if (sol_prime->included[idx]) {
            // Removes item
            sol_prime->included[idx] = 0;

            // Recomputes total weight and value
            int new_weight = sol_prime->total_weight - instance->items[idx].weight;
            int new_value = sol_prime->total_value - instance->items[idx].value;

            // Recomputes penalties
            int new_penalty = compute_penalty(instance, sol_prime);
        } else {
            // Adds item if feasible
            int new_weight = sol_prime->total_weight + instance->items[idx].weight;
            if (new_weight <= instance->capacity) {
                sol_prime->included[idx] = 1;

                // Recomputes total weight and value
                int new_weight = sol_prime->total_weight + instance->items[idx].weight;
                int new_value = sol_prime->total_value + instance->items[idx].value;

                // Recomputes penalties
                int new_penalty = compute_penalty(instance, sol_prime);
            }
        }
    }
}

Solution* acceptance_criterion(Solution *sol_star, Solution *sol) {
    if (objective_value(sol_star) > objective_value(sol)) {
        return sol_star;
    }
    return sol;
}