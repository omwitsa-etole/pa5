#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>

// Rutgers University 2021 Spring CS 211 Computer Architecture
// Assignment based on CS:APP Cache Lab

// 64-bit memory address
typedef unsigned long long int mem_addr_t;

// cache properties
unsigned char s = 0; // set index bits
unsigned short S = 1<<0; // set count

unsigned char E = 16; // way associative cache; lines per set

unsigned char b = 4; // block offset bits
// unsigned short B = 1<<4; // block size in bytes

typedef enum queue_ret
{
    QUEUE_RET_Success,
    QUEUE_RET_FailedMemoryAlloc,
    QUEUE_RET_QueueIsEmpty,
} queue_ret_t;


// fully associative cache line
typedef struct cache_line {
    mem_addr_t tag;
    struct cache_line* next_line;
} cache_line_t;

typedef struct node
{
    void* data;
    struct node* next;
} node_t;

typedef struct fifo_cache_set {
    cache_line_t* front; // front (head) of the queue
    cache_line_t* back; // back (tail) of the queue
    unsigned char size;
} fully_associative_fifo_cache_t;

unsigned char freeList(fully_associative_fifo_cache_t* , void* , size_t );
unsigned char addList(fully_associative_fifo_cache_t* , void*);

// accessData - Access data at memory address addr.
void accessData (
    mem_addr_t addr,
    fully_associative_fifo_cache_t* cache,
    unsigned int* hit_count, // If it is already in cache, increase hit_count
    unsigned int* miss_count, // If it is not in cache, bring it in cache, increase miss_count
    unsigned int* eviction_count // Also increase eviction_count if a line is evicted
) {
    //printf("hit: %d, addr: %d\n",miss_count,addr);
    //printf("%d\n",addr);
    // Cache indices for this address
    mem_addr_t tag = addr >> (s+b);
    int hit = 0;
    // Cache hit
    cache_line_t* curr_line = cache->front;
    printf("\n%d",cache->size);
	printf("\n%d :: %d\n",curr_line,tag);
    while ( curr_line != NULL ) {
        if ( curr_line->tag == tag ) {
            hit = 1;
	    hit_count = hit_count+1;
            return;
        }
	
        curr_line = curr_line->next_line;
    }
    printf("\n%d",cache->size);
	printf("\n%d :: %d\n\n\n",curr_line,tag);
    // Otherwise, record a cache miss
    /* ... */
    if(hit == 0){

	miss_count = miss_count+1;
    }

    // If cache is full, evict oldest line due to FIFO cache replacement policy
    if ( cache->size== E ) {
        // dequeue from front of FIFO, update occupancy, and record an eviction
        /* ... */
        freeList(cache,cache->back,sizeof(cache->back));
	eviction_count = eviction_count+1;
    }

    // Due to cache miss, enqueue cache line, and update occupancy
    /* ... */
    printf("%c\n",curr_line);
    addList(cache,curr_line);
    
    

}

int main(int argc, char* argv[]) {

    // path to memory trace
    if ( argc!= 2 ) {
        printf( "Usage: ./fullyAssociative <mem_trace_file>\n" );
        exit( EXIT_FAILURE );
    }
    char* mem_trace_file = argv[1];
    FILE *fp = fopen(mem_trace_file, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file '%s'\n", mem_trace_file);
        exit( EXIT_FAILURE );
    }

    fully_associative_fifo_cache_t cache = { .front=NULL, .back=NULL, .size=0 };
    // cache simulation statistics
    unsigned int* miss_count = 0;
    unsigned int* hit_count = 0;
    unsigned int* eviction_count = 0;

    // Loop through until we are done with the file.
    size_t line_buf_size = 256;
    char line_buf[line_buf_size];

    while ( fgets(line_buf, line_buf_size, fp) != NULL ) {

        // replay the given trace file against the cache
        if ( line_buf[1]=='L' || line_buf[1]=='S' || line_buf[1]=='M' ) {
            char access_type = '\0';
            mem_addr_t addr = 0;
            unsigned int len = 0;
            sscanf ( line_buf, " %c %llx,%u", &access_type, &addr, &len );
	    
            if ( access_type=='L' || access_type=='S' || access_type=='M') {
                accessData(addr, &cache, hit_count, miss_count, eviction_count);
            }

            // If the instruction is M indicating L followed by S then access again
            if(access_type=='M'){
                accessData(addr, &cache, hit_count, miss_count, eviction_count);
	    }
        }
    }

    cache_line_t* curr_line = cache.front;
    while ( curr_line != NULL ) {
        cache_line_t* temp = curr_line;
        curr_line = curr_line->next_line;
        free(temp);
    }
    fclose(fp);

    /* Output the hit and miss statistics for the autograder */
    printf("hits:%d misses:%d evictions:%d\n", hit_count, miss_count, eviction_count);

    exit( EXIT_SUCCESS );
}


unsigned char QUEUE_peek(fully_associative_fifo_cache_t* queue, void* data, size_t size)
{
    if (queue->size == 0)
    {
        return QUEUE_RET_QueueIsEmpty;
    }

    memcpy(data, queue->front, size);

    return QUEUE_RET_Success;
}

unsigned char freeList(fully_associative_fifo_cache_t* queue, void* data, size_t size){
    
    queue_ret_t ret = QUEUE_peek(queue, data, size);

    if (ret != QUEUE_RET_Success)
    {
        return ret;
    }

    if (queue->front == queue->back)
    {
        free(queue->front);
        queue->front = NULL;
        queue->back = NULL;
    }
    else
    {
        node_t* temp = queue->front;
        queue->front = temp->next;
        free(temp);
    }

    (queue->size)--;

    return QUEUE_RET_Success;
}
unsigned char addList(fully_associative_fifo_cache_t* queue,void* data)
{
    
    node_t* node = (node_t*)calloc(1, sizeof(node_t));

    if (node == NULL)
    {
        return QUEUE_RET_FailedMemoryAlloc;
    }

    node->data = data;
    node->next = NULL;

    if (queue->size == 0)
    {
        queue->front = node;
        queue->back = node;
    }
    else
    {
        queue->back->next_line = node;
        queue->back = node;
    }

    (queue->size)++;
    return QUEUE_RET_Success;
}
