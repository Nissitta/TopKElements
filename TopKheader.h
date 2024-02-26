#include <stdint.h>
#include "custom_datatype.h"

//Read data from file
int data_from_file(char* filename,int no_words,char** Arr);

//Creatr Heavy Keeper
Bucket** create_Heavy_Keeper(int depth, int width);

//Key Generator
uint64_t key_gen_64_bit();

//Convert 64 bit hash to number
int number_from_64_bit_hash(uint64_t hash_value, int bucket_size);

//Hash function
uint64_t fasthash64(const void *buf, size_t len, uint64_t seed);

//ceratin hash function
int djb_hash(const char* cp) ;

//Top K Hitter identifier
void TopKHitter(char** Arr, int k, int count_of_elements);


// -------------------- MIN HEAP FUNCTIONS -----------------------

int parent(int i);

int left_child(int i);

int right_child(int i);

//Get minimum of heap
Node get_min(MinHeap* heap);

//Initialize min_heap
MinHeap* init_minheap(int capacity);

//Insert element into min heap
MinHeap* insert_minheap(MinHeap* heap, Node element);

//Rearrange the min heap (Heapify)
MinHeap* heapify(MinHeap* heap, int index);

//Delete the root element
MinHeap* delete_minimum(MinHeap* heap);

//Print the tree
void print_heap(MinHeap* heap);

//Free the tree
void free_minheap(MinHeap* heap);
