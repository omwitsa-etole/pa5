#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

volatile char MARKER_START, MARKER_END;

int B = 4;
int cache_size = 16;


void blockMultiply(int *a,int* b,int* c, int n){
	//int first = (n/16) + n;
	
	int i,j,k,ii,jj,kk;
	for(i=0;i<n;i+=B){
		for(j=0;j<n;j+=B){
			
			for(ii=i;ii<i+B;ii++){
				for(jj=j;jj<j+B;jj++){
					c[ii*n+jj] = 0;
					for(k=0;k<n;k++){
						c[ii*n+jj] += a[ii*n+k]*b[k*n+jj];	
					}
				//x[i*n]
				}
			}		
			
		}
	}
	//int totalmisses = first*(n*n);
	//printf("total: misses: %d\n",totalmisses);

}

int main(int argc, char* argv[]){
	 /* Record marker addresses */
    FILE* marker_fp = fopen(".marker","w");
    assert(marker_fp);
    fprintf(marker_fp, "%llx\n%llx",
        (unsigned long long int) &MARKER_START,
        (unsigned long long int) &MARKER_END );
    fclose(marker_fp);

    if (argc!=3) {
        printf("Usage: ./cacheBlocking <matrix_a_file> <matrix_b_file>\n");
        exit(EXIT_FAILURE);
    }

    FILE* matrix_a_fp = fopen(argv[1], "r");
    if (!matrix_a_fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    size_t n;
    int ret = fscanf(matrix_a_fp, "%ld\n", &n);
    assert (ret==1);
    int* a = calloc( n*n, sizeof(int) );
    for ( size_t i=0; i<n; i++ ) {
        for ( size_t k=0; k<n; k++ ) {
            ret = fscanf(matrix_a_fp, "%d ", &a[i*n+k]);
            assert (ret==1);
        }
        ret = fscanf(matrix_a_fp, "\n");
    }
    fclose(matrix_a_fp);

    FILE* matrix_b_fp = fopen(argv[2], "r");
    if (!matrix_b_fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    size_t m;
    ret = fscanf(matrix_b_fp, "%ld\n", &m);
    assert (ret==1);
    assert( n==m );
    int* b = calloc( n*n, sizeof(int) );
    for ( size_t k=0; k<n; k++ ) {
        for ( size_t j=0; j<n; j++ ) {
            ret = fscanf(matrix_b_fp, "%d ", &b[k*n+j]);
            assert (ret==1);
        }
        ret = fscanf(matrix_b_fp, "\n");
    }
    fclose(matrix_b_fp);

    int* c = calloc( n*n, sizeof(int) );
	MARKER_START = 211;
	blockMultiply(a,b,c,n);
	MARKER_END = 211;
	
	for ( size_t i=0; i<n; i++ ) {
		for ( size_t k=0; k<n; k++ ) {
			printf("%d ", c[i*n+k]);
		}
		printf("\n");
	}
	free(c);
	free(b);
	free(a);
	exit(EXIT_SUCCESS);
	
}


