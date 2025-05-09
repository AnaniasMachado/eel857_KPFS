typedef struct {
    int *included;          // Array of indices of included items
    double total_value;     // Solution value
    double total_weight;    // Solution weight
    double total_penalty;   // Penalty cost based on forfeit sets
} Solution;