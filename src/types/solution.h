typedef struct {
    int *included;            // Array of 0 or 1 indicating presence of item i
    int total_value;          // Solution value
    int total_weight;         // Solution weight
    int total_penalty;        // Penalty cost based on forfeit sets
    int *included_items;      // Array of indices of included items
    int size_included;        // Number of included items
    int *not_included_items;  // Array of indices of not included items
    int size_not_included;    // Number of not included items
} Solution;