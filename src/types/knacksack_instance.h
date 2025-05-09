typedef struct {
    Item *items;                // Array of all items
    int item_count;             // Total number of items
    ForfeitSet *forfeit_sets;   // Array of forfeit sets
    int forfeit_count;          // Number of forfeit sets
    int capacity;            // Knapsack capacity
} KnapsackInstance;