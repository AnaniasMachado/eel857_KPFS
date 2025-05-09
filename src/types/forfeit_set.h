typedef struct {
    int *items;       // Array of item IDs in the set
    int size;         // Number of items in the set
    int threshold;    // Allowance threshold for the set
    int penalty;   // Penalty cost for exceeding threshold
} ForfeitSet;