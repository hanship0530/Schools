#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct param_quick_s{
        int *origin;
        int left;
        int right;
}param_quick;

typedef struct param_merge_s{
        int *merged;
        int *origin;
        int left;
        int mid;
        int right;
}param_merge;

void swap(int* a, int* b){
        int temp = *a;
        *a = *b;
        *b = temp;
}
void insertionSort(int origin[], const int left, const int right) 
{ 
    int i, key, j; 
    for (i = left+1; i < right + 1; i++) { 
        key = origin[i]; 
        j = i - 1; 
        while (j >= left && origin[j] > key) { 
            origin[j + 1] = origin[j]; 
            j = j - 1; 
        } 
        origin[j + 1] = key; 
    } 
} 
int partition(int* arr, int low, int high, int* lp) 
{ 
    if (arr[low] > arr[high]) 
        swap(&arr[low], &arr[high]); 
    // p is the left pivot, and q is the right pivot. 
    int j = low + 1; 
    int g = high - 1, k = low + 1, p = arr[low], q = arr[high]; 
    while (k <= g) { 
  
        // if elements are less than the left pivot 
        if (arr[k] < p) { 
            swap(&arr[k], &arr[j]); 
            j++; 
        } 
  
        // if elements are greater than or equal  
        // to the right pivot 
        else if (arr[k] >= q) { 
            while (arr[g] > q && k < g) 
                g--; 
            swap(&arr[k], &arr[g]); 
            g--; 
            if (arr[k] < p) { 
                swap(&arr[k], &arr[j]); 
                j++; 
            } 
        } 
        k++; 
    } 
    j--; 
    g++; 
  
    // bring pivots to their appropriate positions. 
    swap(&arr[low], &arr[j]); 
    swap(&arr[high], &arr[g]); 
  
    // returning the indeces of the pivots. 
    *lp = j; // because we cannot return two elements  
             // from a function. 
  
    return g; 
} 
void DualPivotQuickSort(int* origin, int low, int high) 
{ 
    if (high - low > 16) { 
        // lp means left pivot, and rp means right pivot. 
        int lp, rp;  
        rp = partition(origin, low, high, &lp); 
        DualPivotQuickSort(origin, low, lp - 1); 
        DualPivotQuickSort(origin, lp + 1, rp - 1); 
        DualPivotQuickSort(origin, rp + 1, high); 
    } else {
        insertionSort(origin, low, high);
        return;
    }
} 

void naive_merge(int merged[], int origin[], const int left, const int mid, const int right){
        int i,j,k,l;
        i = left; j=mid+1; k=left;

        while(i<=mid && j<=right){
                if(origin[i]<=origin[j]) merged[k++] = origin[i++];
                else merged[k++] = origin[j++];
        }
        if(i>mid) for(l = j; l<=right; l++) merged[k++] = origin[l];
        else for(l=i;l<=mid;l++) merged[k++] = origin[l];
        for(l = left; l<=right; l++) origin[l] = merged[l];
}
void *quickSort_t(void *param) {
    param_quick *p = param;
    if (p->right > p->left) { 
        // lp means left pivot, and rp means right pivot. 
        int lp, rp;  
        rp = partition(p->origin, p->left, p->right, &lp); 
        DualPivotQuickSort(p->origin, p->left, lp - 1); 
        DualPivotQuickSort(p->origin, lp + 1, rp - 1); 
        DualPivotQuickSort(p->origin, rp + 1, p->right); 
    }
}    
int main(int argc, char* argv[]){
        FILE *fp;
        const int thread_num = 8;
        char *filename, *k;
        int *origin, i = 0, arr_size = 0, number, temp, *merged = NULL;
        struct timeval start, end;
        double duration;
        pthread_t tib_quick[thread_num];
        pthread_t tib_merge[thread_num];
        param_quick param_q[thread_num];
        param_merge param_m[thread_num];

        filename = argv[1];

        fp = fopen(filename,"r");
        if(fp == NULL){
                printf("Fail: open file\n");
                return 0;
        }
        printf("Opening %s...\n", filename);

        origin = NULL;
        merged = NULL;

        while(!feof(fp)){
                if(fscanf(fp,"%d",&number) != EOF){
                        origin = (int *)realloc(origin,sizeof(int)*(arr_size+1));
                        merged = (int *)realloc(merged,sizeof(int)*(arr_size+1));
                        origin[arr_size] = number;
                        merged[arr_size] = number;
                        arr_size++;
                }
        }
        printf("%s is successfuly opend!\n", filename);
        fclose(fp);

        int index;

        param_q[index].origin = origin;
        param_q[index].left = 0;
        param_q[index].right = arr_size/thread_num;        
        for(index = 1; index<thread_num-1;index++){
            param_q[index].origin = origin;
            param_q[index].left = arr_size*(index)/thread_num + 1;
            param_q[index].right = arr_size*(index+1)/thread_num;
        }
        param_q[index].origin = origin;
        param_q[index].left = arr_size*(thread_num-1)/thread_num + 1;
        param_q[index].right = arr_size-1;        
    
        for(index = 1; index<thread_num-1;index++){
            param_m[index].merged = merged;
            param_m[index].origin = origin;
            param_m[index].left = arr_size*(index-1)/thread_num + 1;
            param_m[index].mid = arr_size*(index)/thread_num;
            param_m[index].right = arr_size*(index+1)/thread_num;
        }    

        gettimeofday(&start,NULL);

        for(index=0; index<thread_num; index++){
            pthread_create(&tib_quick[index],NULL,quickSort_t, &param_q[index]);
        }

        pthread_join(tib_quick[0],NULL);
        pthread_join(tib_quick[1],NULL);

        naive_merge(merged,origin,0,arr_size/thread_num,arr_size*2/thread_num);

        pthread_join(tib_quick[2],NULL);
        pthread_join(tib_quick[3],NULL);

        naive_merge(merged,origin,arr_size*2/thread_num+1,arr_size*3/thread_num,arr_size*4/thread_num);
        naive_merge(merged,origin,0,arr_size*2/thread_num,arr_size*4/thread_num);

        pthread_join(tib_quick[4],NULL);
        pthread_join(tib_quick[5],NULL);

        naive_merge(merged,origin,arr_size*4/thread_num+1,arr_size*5/thread_num,arr_size*6/thread_num);
        
        pthread_join(tib_quick[6],NULL);
        pthread_join(tib_quick[7],NULL); 

        naive_merge(merged,origin,arr_size*6/thread_num+1,arr_size*7/thread_num,arr_size-1);
        naive_merge(merged,origin,arr_size*4/thread_num+1,arr_size*6/thread_num,arr_size-1);
        naive_merge(merged,origin,0,arr_size/2,arr_size-1);

        gettimeofday(&end,NULL);

        duration = (double)(end.tv_sec)+(double)(end.tv_usec)/1000000.0-(double)(start.tv_sec)-(double)(start.tv_usec)/1000000.0;

        printf("Sorting Time : %f\n", duration);

        printf("=====Now writing file=====\n");
        fp = fopen("result_quick","w");
        if(fp != NULL){
                for(i=1;i<=arr_size;i++) {
                        fprintf(fp,"%10d ",origin[i-1]);
                        if(i != 0 && i%5 == 0) fprintf(fp,"\n");
                }
        }
        printf("=====Writing file is finished=====\n");
        printf("Check file is sorted\n");
        int isSorted = 1;
        int prev = origin[0];
        for(i=1; i<arr_size; i++){
                if(prev>origin[i]){
                    printf("The file is not sorted at %d\n",i);
                    isSorted = 0; 
                    break;   
                }
                prev = origin[i];
        }
        if(isSorted == 1){
                printf("The file is sorted\n");
        }
        free(origin);
        free(merged);
        fclose(fp);
}
