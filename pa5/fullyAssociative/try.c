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

typedef struct cache_line {
    mem_addr_t tag;
    struct cache_line* next_line;
} cache_line_t;

typedef struct fifo_cache_set {
    //struct fifo_cache_set *t = (struct fifo_cache_set *)malloc(sizeof(t));
    cache_line_t* front; // front (head) of the queue
    cache_line_t* back; // back (tail) of the queue
    //cache_line_t* t->front;
    //cache_line_t* t->back;
    unsigned char size;
    //return t;
} fifo_cache_t;



void deQue(fifo_cache_t* cache){
	printf("cache size: %d\n",cache->size);
	cache->size-=1; 
	for(int i=0;i<cache->size;i++){
		//printf("size: %d\n",cache->front[i].tag);
		//printf("size: %d\n",cache->front->tag);
	}
	
}

void addData(fifo_cache_t* cache,cache_line_t* curr_line,mem_addr_t addr){
	//curr_line->tag = addr;
	if(cache->size < E){
		
		cache->size+=1; 
	}
	cache[(cache->size)+1].front = (long) addr;
	//cache[(cache->size)+1].back = cache[(cache->size)].front;
	//curr_line->tag=addr;
	//cache->back->tag = addr;
	
}



void accessData (
    mem_addr_t addr,
    fifo_cache_t* cache,
    unsigned int* hit_count, // If it is already in cache, increase hit_count
    unsigned int* miss_count, // If it is not in cache, bring it in cache, increase miss_count
    unsigned int* eviction_count // Also increase eviction_count if a line is evicted
) {
	int hit = 0;
	printf("%lu\n",cache->back);
	//printf("%d\n",cache->back.tag);
	//printf("%d\n",addr);
	cache_line_t* curr_line = cache->front;
        while ( curr_line != NULL ) {
		
		if ( curr_line->tag == addr ) {
			hit = 1;
			hit_count = hit_count+1;
			return;
		}
		curr_line = curr_line->next_line;
	}
	
	if(hit == 0){
		miss_count = miss_count+1;
	}
	if ( cache->size== E ) {
		// dequeue from front of FIFO, update occupancy, and record an eviction
		/* ... */
		//deQue(cache);
	}
	addData(cache,curr_line,addr);
	//cache_line_t* curr_line = addr;
	
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
    cache_line_t* cache_line; cache_line->tag=12;

    fifo_cache_t cache = { .front=cache_line, .back=cache_line, .size=0 };
    //printf("%d",cache.back);
    //fifo_cache_t* cache[E];
    //cache[0]->front=NULL; cache[0]->back=NULL; cache[0]->size=0 ;
    // cache simulation statistics
    unsigned int* miss_count = 0;
    unsigned int* hit_count = 0;
    unsigned int* eviction_count = 0;

    // Loop through until we are done with the file.
    size_t line_buf_size = 256;
    char line_buf[line_buf_size];
    int count = 0;
    while ( fgets(line_buf, line_buf_size, fp) != NULL ) {
	
        // replay the given trace file against the cache
        if ( line_buf[1]=='L' || line_buf[1]=='S' || line_buf[1]=='M' ) {
            char access_type = '\0';
            mem_addr_t addr = 0;
            unsigned int len = 0;
            sscanf ( line_buf, " %c %llx,%u", &access_type, &addr, &len );
	    printf("%c\n",access_type);
            if ( access_type=='L' || access_type=='S' || access_type=='M') {
                accessData(addr, &cache, hit_count, miss_count, eviction_count);
            }

            // If the instruction is M indicating L followed by S then access again
            if(access_type=='M'){
                //accessData(addr, &cache, hit_count, miss_count, eviction_count);
	    }
        }
	//if(count > 2){break;}
    }

    //cache_line_t* curr_line = cache[0].front;
    //while ( curr_line != NULL ) {
        //cache_line_t* temp = curr_line;
        //curr_line = curr_line->next_line;
        //free(temp);
    //}
    fclose(fp);

    /* Output the hit and miss statistics for the autograder */
    printf("hits:%d misses:%d evictions:%d size: %d\n", hit_count, miss_count, eviction_count,cache.back);

    exit( EXIT_SUCCESS );
}