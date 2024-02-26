struct bucket{
    int fingerprint;
    int counter;
};
typedef struct bucket Bucket;

struct Node{
    char* element;
    int size;
};
typedef struct Node Node;

struct MinHeap {
    Node* arr;
    int size;
    int capacity;
};
typedef struct MinHeap MinHeap;