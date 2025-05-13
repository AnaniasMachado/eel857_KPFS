typedef struct {
    int *included;          // Array of 0 or 1 indicating presence of item i
    int total_value;        // Solution value
    int total_weight;       // Solution weight
    int total_penalty;      // Penalty cost based on forfeit sets
} Solution;