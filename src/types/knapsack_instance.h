typedef struct {
    Item *items;                   // Array of all items
    int item_count;                // Total number of items
    ForfeitSet *forfeit_sets;      // Array of forfeit sets
    // int **F;                       // Forfeit set membership matrix (nS x nI)
    // int *forfeit_sizes;            // Array of size nS with number of items in each set
    int forfeit_count;             // Number of forfeit sets
    // int *thresholds;               // Allowance threshold for the set
    // int *penalties;                // Array of penalties for each forfeit set
    int capacity;                  // Knapsack capacity
} KnapsackInstance;