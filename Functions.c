#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include "custom_datatype.h"

//mix method for FastHash
#define mix_fh(h)                                                                 \
({                                                                           \
    (h) ^= (h) >> 23;                                                          \
    (h) *= 0x2127599bf4325c37ULL;                                              \
    (h) ^= (h) >> 47;                                                          \
})

int data_from_file(char* filename,int no_words,char** Arr){
    int max_line_length = 50;
    char* line = (char*)malloc(max_line_length * sizeof(char));
    FILE *f = fopen(filename, "r");
        int iter=0;
        while(fgets(line, max_line_length, f)){
            Arr[iter] = (char*)malloc(max_line_length* sizeof(line));
            line[strcspn(line, "\r\n")] = 0;
            strcpy(Arr[iter],line);
            iter++;
        }
        printf("No of Words : %d",iter);
    fclose(f);  
    return iter;
}

Bucket** create_Heavy_Keeper(int depth, int width){
    Bucket** heavyKeeper = (Bucket**)calloc(depth, sizeof(Bucket*));
    for(int count = 0; count < width; count++){
        heavyKeeper[count] = (Bucket*)calloc(width, sizeof(Bucket));
    }
    return heavyKeeper;
}

uint64_t key_gen_64_bit(){
    uint64_t key = (uint64_t)rand();
    return key;
}

int number_from_64_bit_hash(uint64_t hash_value, int bucket_size) {
    int num_bits = (int)ceil(log2(bucket_size));
    int shift_amount = 64 - num_bits + 1;
    uint64_t shifted_hash = hash_value >> shift_amount;
    uint64_t masked_bits = shifted_hash & ((1 << num_bits) - 1);
    int bucket_index = (int)masked_bits;
    return bucket_index;
}

uint64_t fasthash64(const void *buf, size_t len, uint64_t seed) {
    const uint64_t m = 0x880355f21e6d1965ULL;
    const uint64_t *pos = (const uint64_t *)buf;
    const uint64_t *end = pos + (len / 8);
    const unsigned char *pos2;
    uint64_t h = seed ^ (len * m);
    uint64_t v;

    while (pos != end) {
        v = *pos++;
        h ^= mix_fh(v);
        h *= m;
    }

    pos2 = (const unsigned char *)pos;
    v = 0;

    switch (len & 7) {
    case 7:
        v ^= (uint64_t)pos2[6] << 48; 
    case 6:
        v ^= (uint64_t)pos2[5] << 40; 
    case 5:
        v ^= (uint64_t)pos2[4] << 32; 
    case 4:
        v ^= (uint64_t)pos2[3] << 24; 
    case 3:
        v ^= (uint64_t)pos2[2] << 16; 
    case 2:
        v ^= (uint64_t)pos2[1] << 8; 
    case 1:
        v ^= (uint64_t)pos2[0];
        h ^= mix_fh(v);
        h *= m;
    }

    return mix_fh(h);
}

int parent(int i) { return (i - 1) / 2; }

int left_child(int i) { return (2*i + 1); }

int right_child(int i) { return (2*i + 2); }

Node get_min(MinHeap* heap){ return heap->arr[0]; }

MinHeap* init_minheap(int capacity) {
    MinHeap* minheap = (MinHeap*) malloc(sizeof(MinHeap));
    minheap->arr = (Node*) calloc (capacity, sizeof(Node));
    minheap->capacity = capacity;
    minheap->size = 0;
    return minheap;
}

MinHeap* insert_minheap(MinHeap* heap, Node element) {
    if (heap->size == heap->capacity) {
        fprintf(stderr, "Cannot insert %s. Heap is already full!\n", element.element);
        return heap;
    }
    heap->size++;
    heap->arr[heap->size - 1] = element;
    int curr = heap->size - 1;
    while (curr > 0 && heap->arr[parent(curr)].size > heap->arr[curr].size) {
        Node temp = heap->arr[parent(curr)];
        heap->arr[parent(curr)] = heap->arr[curr];
        heap->arr[curr] = temp;
        curr = parent(curr);
    }
    return heap; 
}

MinHeap* heapify(MinHeap* heap, int index) {
    if (heap->size <= 1) return heap;
    
    int left = left_child(index); 
    int right = right_child(index); 

    int smallest = index; 
    
    if (left < heap->size && heap->arr[left].size < heap->arr[index].size) 
        smallest = left; 
    
    if (right < heap->size && heap->arr[right].size < heap->arr[smallest].size) 
        smallest = right; 

    if (smallest != index) { 
        Node temp = heap->arr[index];
        heap->arr[index] = heap->arr[smallest];
        heap->arr[smallest] = temp;
        heap = heapify(heap, smallest); 
    }
    return heap;
}

MinHeap* delete_minimum(MinHeap* heap) {
    if (!heap || heap->size == 0)
        return heap;

    int size = heap->size;
    Node last_element = heap->arr[size-1];

    heap->arr[0] = last_element;

    heap->size--;
    size--;

    heap = heapify(heap, 0);
    return heap;
}

void print_heap(MinHeap* heap) {
    printf("\nMin Heap:\n");
    for (int i=0; i<heap->size; i++) {
        printf("%s , %d \n", heap->arr[i].element, heap->arr[i].size);
    }
    printf("\n");
}

void free_minheap(MinHeap* heap) {
    if (!heap)
        return;
    free(heap->arr);
    free(heap);
}

int djb_hash(const char* cp) {
    int hash = 5381;
    while (*cp)
        hash = 33 * hash ^ (unsigned char) *cp++;
    return hash;
}

void TopKHitter(char** Arr, int k, int count_of_elements){
    int depth = 0;
    int width = 0;

    if(k <= 5){ //k value should be a minimum of 8
        depth = 7;
        width = 8;
    }
    else{
        depth = log(k);
        width = k*log(k);
    }

    printf("\nThe width is %d and depth is %d\n",width,depth);
    printf("\n");
    // 'd' no. of keys
    uint64_t* Keys = (uint64_t*)malloc(depth * sizeof(uint64_t));
    for(int iter = 0; iter < depth; iter++){
        Keys[iter] = key_gen_64_bit();
    }

    // 2-d array of buckets initialised to 0
    Bucket** heavyKeeper = (Bucket**)calloc(depth, sizeof(Bucket*));
    for(int count = 0; count < depth; count++){
        heavyKeeper[count] = (Bucket*)calloc(width, sizeof(Bucket));
    }
    
    for(int i = 0; i<depth; i++){
        for(int j = 0; j<width; j++){
            heavyKeeper[i][j].counter = 0;
            heavyKeeper[i][j].fingerprint = 0;
        }
    }

    //initializing heap
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->arr = (Node*) calloc (k, sizeof(Node));
    heap->capacity = k;
    heap->size = 0;
    

    //Top-k 
    for(int word_count = 0; word_count < count_of_elements; word_count++){
        int est_size = 0;
        int element_index = 0;
        bool flag = false;

        //check if element in heap
        for(int ind = 0; ind<heap->size; ind++){

            //if element in heap, mark its location adn flag true
            if(strcmp(heap->arr[ind].element, Arr[word_count]) == 0){
                flag = true;
                element_index = ind;
                break;
            }
        }

        //compute for 'd' hash functions
        for(int j = 0; j < depth; j++){
            uint64_t key = Keys[j];
            int hash_val = number_from_64_bit_hash(fasthash64(Arr[word_count], strlen(Arr[word_count]), Keys[j]),width);
            int finger_print = djb_hash(Arr[word_count]);
            
            //case I --> first encounter at heavy keeper
            if(heavyKeeper[j][hash_val].counter == 0){
                heavyKeeper[j][hash_val].counter = 1;
                heavyKeeper[j][hash_val].fingerprint = finger_print;
                est_size = 1;
            }

            //case II --> element already in heavy keeper
            else if(heavyKeeper[j][hash_val].counter > 0 && heavyKeeper[j][hash_val].fingerprint == finger_print){
                heavyKeeper[j][hash_val].counter++;
                est_size = fmax(est_size, heavyKeeper[j][hash_val].counter);
            }

            //case III
            else if(heavyKeeper[j][hash_val].counter > 0 && heavyKeeper[j][hash_val].fingerprint != finger_print){
                int c = heavyKeeper[j][hash_val].counter;
                float r = (float)rand()/(float)RAND_MAX;
                float prob = pow(1.08,-c);
                if( r < prob){
                    heavyKeeper[j][hash_val].counter--;
                    if(heavyKeeper[j][hash_val].counter == 0){
                        heavyKeeper[j][hash_val].fingerprint = finger_print;
                        heavyKeeper[j][hash_val].counter = 1;
                        est_size = fmax(est_size,1);
                    }
                }
            }
        }

        if(flag == true){ //element already in heap (identifies as one of top 'k' flows)
            heap->arr[element_index].size = fmax(est_size, heap->arr[element_index].size);
            heapify(heap,heap->size-1);
            
        }
        else{ //Element not in min heap
            Node heap_node = {Arr[word_count], est_size};

            //if any bucket is empty in heap
            if(heap->size < heap->capacity){
                insert_minheap(heap, heap_node);             
            }
            //check if it is really an elephant flow
            else if(est_size - get_min(heap).size == 1){
                delete_minimum(heap);
                insert_minheap(heap,heap_node);
                
            }
        }
        heapify(heap, 0);
              
    }

    print_heap(heap);

    //Free the Heavy Keeper
    for(int i = 0 ; i < depth; i++){ free(heavyKeeper[i]); }
    free(heavyKeeper);

    //Free Keys
    free(Keys);

    //free heap
    free_minheap(heap);  
}




