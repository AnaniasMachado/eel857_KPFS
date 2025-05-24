#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../util/util_misc.c"

int objective_value(Solution *sol);
int is_feasible(const KnapsackInstance *instance, Solution *sol);
Solution *create_solution(int item_count);
void copy_solution(const KnapsackInstance *instance, const Solution *src, Solution *dest);
void free_solution(Solution *sol);
void add_item(Solution *sol, int idx);
void remove_item(Solution *sol, int idx);

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

// Compute the ratio value/weight for each item and returns an array of indices sorted descendingly by ratio
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

// Compute solution penalty
int compute_penalty(const KnapsackInstance *instance, Solution *sol) {
    int total_penalty = 0;

    // Compute the penalty for each forfeit set
    for (int i = 0; i < instance->forfeit_count; i++) {
        ForfeitSet *fset = &instance->forfeit_sets[i];

        int count_included = 0;

        // For each item in the forfeit set
        for (int j = 0; j < fset->size; j++) {
            int item_id = fset->items[j];
            // Check if item is in solution
            if (sol->included[item_id]) {
                count_included++;
            }
        }

        // Check if exceed threshold
        if (count_included > fset->threshold) {
            total_penalty += (count_included - fset->threshold) * fset->penalty;
        }
    }

    return total_penalty;
}

// Build a initial solution
Solution* build_initial_solution(const KnapsackInstance *instance) {
    Solution *sol = create_solution(instance->item_count);
    int* sorted_indices = compute_and_sort_ratios(instance);

    // Loop through items of sorted_indices 
    for (int i = 0; i < instance->item_count; i++) {
        int item_idx = sorted_indices[i];
        int w = instance->items[item_idx].weight;
        if (sol->total_weight + w <= instance->capacity) {
            // Include item
            add_item(sol, item_idx);
            sol->total_value += instance->items[item_idx].value;
            sol->total_weight += w;
        } else {
            // Do not include item
            remove_item(sol, item_idx);
        }
    }

    // Compute forfeit sets penalty
    sol->total_penalty = compute_penalty(instance, sol);
    return sol;
}

// Build a initial solution
Solution* build_initial_solution_2(const KnapsackInstance *instance) {
    Solution *sol = create_solution(instance->item_count);

    // Initialize an array of indices
    int *indices_arr = init_indeces_array(instance->item_count);
    // Create a permutation of the array of indices
    shuffle(indices_arr, instance->item_count);

    // Compute the maximum number of items in the initial solution
    int upper_lim = (int) (instance->item_count * 0.7);
    int lower_lim = (int) (instance->item_count * 0.1);
    int num_items = lower_lim + rand() % (upper_lim - lower_lim + 1);

    // Loop through items of indices_arr
    for (int i = 0; i < num_items; i++) {
        int item_idx = indices_arr[i];
        int w = instance->items[item_idx].weight;
        int v = 0;
        int p = 0;
        // Check if solution is feasible
        if (sol->total_weight + w <= instance->capacity) {
            add_item(sol, item_idx);
            // Compute solution value and penalty
            v = sol->total_value + instance->items[item_idx].value;
            p = compute_penalty(instance, sol);
            // Check if there is a increase in objective function value
            if (v - p > sol->total_value - sol->total_penalty) {
                // Include item
                sol->total_value = v;
                sol->total_weight += w;
                sol->total_penalty = p;
            } else {
                remove_item(sol, item_idx);
            }
        }
    }
    return sol;
}

// Return objetive function value
int objective_value(Solution *sol) {
    return sol->total_value - sol->total_penalty;
}

// Check if solution is feasible
int is_feasible(const KnapsackInstance *instance, Solution *sol) {
    return sol->total_weight <= instance->capacity;
}

// Create an empty solution
Solution *create_solution(int item_count) {
    Solution *sol = (Solution*) malloc(sizeof(Solution));
    sol->included = (int*) calloc(item_count, sizeof(int));
    sol->total_value = 0;
    sol->total_weight = 0;
    sol->total_penalty = 0;
    sol->included_items = (int*) calloc(item_count, sizeof(int));
    sol->size_included = 0;
    sol->not_included_items = (int*) malloc(item_count * sizeof(int));
    for (int i = 0; i < item_count; i++) {
        sol->not_included_items[i] = i;
    }
    sol->size_not_included = item_count;
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

// Check if two solutions are the same
int is_same_solution(const KnapsackInstance *instance, const Solution *sol1, const Solution *sol2) {
    for (int i = 0; i < instance->item_count; ++i) {
        if (sol1->included[i] != sol2->included[i]) {
            return 0;
        }
    }
    return 1;
}

// Add item to solution list of included items
void add_item(Solution *sol, int idx) {
    // Include item
    sol->included[idx] = 1;
    // Add item in list of included items
    sol->included_items[sol->size_included] = idx;
    sol->size_included += 1;
    // Remove item from list of not included items
    sol->not_included_items[idx] = sol->not_included_items[sol->size_not_included];
    sol->size_not_included -= 1;
}

// Remove item from solution list of included items
void remove_item(Solution *sol, int idx) {
    // Remove item
    sol->included[idx] = 0;
    // Add item in list of not included items
    sol->not_included_items[sol->size_not_included] = idx;
    sol->size_not_included += 1;
    // Remove item from list of included items
    sol->included_items[idx] = sol->included_items[sol->size_included];
    sol->size_included -= 1;
}

// Acceptance criterion to choose a solution
void acceptance_criterion(const KnapsackInstance *instance, Solution *sol_star, Solution *sol, int k, int max_iterations, int no_change, int no_change_limit) {
    // Compute no_change and iteration factors
    double no_change_factor = (double) no_change / no_change_limit; // between 0 and 1
    double iter_factor = (double) (max_iterations - k) / max_iterations; // between 0 and 1
    
    // Compute a probability from factors
    double prob = no_change_factor * iter_factor;

    // Random number between 0 and 1
    double rand_num = (double) rand() / RAND_MAX;

    if (objective_value(sol_star) < objective_value(sol)) {
        // Accept better solution
        copy_solution(instance, sol, sol_star);
    } else if (rand_num <= prob) {
        // Accept worse solution with a given probability
        copy_solution(instance, sol, sol_star);
    }
}