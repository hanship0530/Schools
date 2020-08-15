/*
   quadThread.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

const static int num = 70000000;
long S[70000000];
struct points {
   int low;
   int high;
};
void swap(long *a, long *b) {
        long temp = *a;
        *a = *b;
        *b = temp;
}
void DualPivotQuickSort(int low, int high) 
{ 
    if (high - low > 16) { 
        // lp means left pivot, and rp means right pivot. 
        int lp, rp;  
        rp = partition2(low, high, &lp); 
        DualPivotQuickSort(low, lp - 1); 
        DualPivotQuickSort(lp + 1, rp - 1); 
        DualPivotQuickSort(rp + 1, high); 
    } else {
      int x, y, key;
       for(x = low+1; x< high+1; x++){
               key = S[x];
               for(y=x-1;y>=low&&S[y]>key;y--) 
                  S[y+1] = S[y];
               S[y+1]=key;
       }      
    }
}
int partition (int low, int high, int pivotpoint) {
   int i,j;
   long pivotitem;
   pivotitem = S[low];
   j = low;
   for(i=low+1; i<=high; i++){
      if(S[i] < pivotitem) {
         j++;
         long temp = S[i];
         S[i] = S[j];
         S[j] = temp;
      }
   }
   pivotpoint = j;
   long temp = S[low];
   S[low] = S[pivotpoint];
   S[pivotpoint] = temp;
   return pivotpoint;
}

int partition2(int low, int high, int* lp) 
{ 
    if (S[low] > S[high]) 
        swap(&S[low], &S[high]); 
    // p is the left pivot, and q is the right pivot. 
    int j = low + 1; 
    int g = high - 1, k = low + 1;
    long p = S[low], q = S[high]; 
    while (k <= g) { 
        // if elements are less than the left pivot 
        if (S[k] < p) { 
            swap(&S[k], &S[j]); 
            j++; 
        } 
        // if elements are greater than or equal  
        // to the right pivot 
        else if (S[k] >= q) { 
            while (S[g] > q && k < g) 
                g--; 
            swap(&S[k], &S[g]); 
            g--; 
            if (S[k] < p) { 
                swap(&S[k], &S[j]); 
                j++; 
            } 
        } 
        k++; 
    } 
    j--; 
    g++; 
    // bring pivots to their appropriate positions. 
    swap(&S[low], &S[j]); 
    swap(&S[high], &S[g]); 
  
    // returning the indeces of the pivots. 
    *lp = j; // because we cannot return two elements  
             // from a function. 
    return g; 
} 
void *quickSortT(void* index) {
   struct points *p = index;
   DualPivotQuickSort(p->low, p->high);
}
void *threadQuickSort3(void* index){
   struct points *ppp = index;
   pthread_t t5, t6;
   int pivotpoint;
   struct points p5;
   struct points p6;   
   pivotpoint = partition(ppp->low, ppp->high, pivotpoint);
   p5.low = ppp->low;
   p5.high = pivotpoint - 1;
   p6.low = pivotpoint + 1;
   p6.high = ppp->high;
   if(pthread_create(&t5, NULL, quickSortT, &p5)) {
      fprintf(stderr, "Error creating thread5\n");
      exit(1);
   }
   if(pthread_create(&t6, NULL, quickSortT, &p6)) {
      fprintf(stderr, "Error creating thread6\n");
      exit(1);
   }   
   pthread_join(t5, NULL);
   pthread_join(t6, NULL);   
}
void *threadQuickSort2(void* index){
   struct points *pp = index;
   pthread_t t3, t4;
   int pivotpoint;
   struct points p3;
   struct points p4;   
   pivotpoint = partition(pp->low, pp->high, pivotpoint);
   p3.low = pp->low;
   p3.high = pivotpoint - 1;
   p4.low = pivotpoint + 1;
   p4.high = pp->high;
   if(pthread_create(&t3, NULL, threadQuickSort3, &p3)) {
      fprintf(stderr, "Error creating thread3\n");
      exit(1);
   }
   if(pthread_create(&t4, NULL, threadQuickSort3, &p4)) {
      fprintf(stderr, "Error creating thread4\n");
      exit(1);
   }   
   pthread_join(t3, NULL);
   pthread_join(t4, NULL);   
}
void threadQuickSort(int low, int high){
   pthread_t t1, t2;
   int pivotpoint;
   struct points p1;
   struct points p2;
   pivotpoint = partition(low, high, pivotpoint);
   p1.low = low;
   p1.high = pivotpoint - 1;
   p2.low = pivotpoint + 1;
   p2.high = high;
   if(pthread_create(&t1, NULL, threadQuickSort2, &p1)) {
      fprintf(stderr, "Error creating thread1\n");
      exit(1);
   }
   if(pthread_create(&t2, NULL, threadQuickSort2, &p2)) {
      fprintf(stderr, "Error creating thread2\n");
      exit(1);
   }   
   pthread_join(t1, NULL);
   pthread_join(t2, NULL);
}
int main(int argc, char* argv[]) {
     char* file_name;
     long number;
   int i=0;
   struct timeval startTime, endTime;
   double diffTime;
   FILE *fpRead, *fpWrite;
   file_name = argv[1];

   fpRead = fopen(file_name, "r");
   if(fpRead == NULL){
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);      
   }
   while ( fscanf(fpRead, "%ld", & number ) == 1 ){
      S[i++] = number;
   }
   fclose(fpRead);
   printf("Start watching time -quadThread.c-\n");
   gettimeofday(&startTime, NULL);
   threadQuickSort(0,num-1);
   gettimeofday(&endTime, NULL);
   diffTime = ( endTime.tv_sec - startTime.tv_sec ) + (( endTime.tv_usec - startTime.tv_usec ) / 1000000);
   printf("Execution time: %f s\n", diffTime);

   fpWrite = fopen("result_quicksort","w");
   for(i=0; i<num; i+=5) {
      fprintf(fpWrite,"%ld %ld %ld %ld %ld\n",S[i],S[i+1],S[i+2],S[i+3],S[i+4]);
   }
   fclose(fpWrite);

   long prev = S[0];
   int isSorted = 1;
   printf("Sort Checking\n");
   for(i=1; i<num;i++){
      if(prev>S[i]){
         printf("Unsorted... %d\n",i);
         isSorted = 0;
         break;
      }
      prev = S[i];
   }
   if(isSorted == 1){
      printf("Sorted...\n");
   }
   return 0;
}