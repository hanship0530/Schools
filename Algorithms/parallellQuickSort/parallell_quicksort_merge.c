#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct param_quick_s{
    int *origin;
    int left;
    int right;
}param_quick;

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int getMedian(int *origin, int left, int right){
    int mid;
    mid = (left+right)/2;

    if(origin[left]>origin[mid]) swap(&origin[left],&origin[mid]);
    if(origin[left]>origin[right]) swap(&origin[left],&origin[right]);
    if(origin[mid]>origin[right]) swap(&origin[mid],&origin[right]);
    return mid;
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
void naive_quickSort(int *origin, int left, int right){
    int pivot, j, temp, i, mid;
    if(right - left < 68){
        insertionSort(origin, left, right);
    return;
    }
    if(left < right){
        pivot = getMedian(origin,left,right);
        swap(&origin[left],&origin[pivot]);
        pivot = left;
        i = left;
        j = right;

        while(i<j){
            while(origin[i] <= origin[pivot] && i<right) i++;
            while(origin[pivot] < origin[j]) j--;
            if(i<j){
                temp = origin[i];
                origin[i] = origin[j];
                origin[j] = temp;
            }
        }
        temp = origin[pivot];
        origin[pivot] = origin[j];
        origin[j] = temp;
        naive_quickSort(origin, left, j-1);
        naive_quickSort(origin, j+1, right);
    }
}

void *quickSort_t(void *param) {
    param_quick *p = param;
    int i = p->left, j = p->right;
    int tmp;
    int pivot = getMedian(p->origin, p->left, p->right);
    /* partition */
    while (i <= j) {
        while (p->origin[i] < pivot)
            i++;
        while (p->origin[j] > pivot)
            j--;
        if (i <= j) {
            tmp = p->origin[i];
            p->origin[i] = p->origin[j];
            p->origin[j] = tmp;
            i++;
            j--;
        }
    };
    /* recursion */
    if (p->left < j)
        naive_quickSort(p->origin, p->left, j);
    if (i < p->right)
        naive_quickSort(p->origin, i, p->right);
}

int main(int argc, char* argv[]){
    FILE *fp;
    const int thread_num = 8;
    char *filename, *k;
    int *origin, i = 0, arr_size = 0, number, temp, *merged = NULL;
    struct timeval start, end;
    double duration;    
    pthread_t tib_quick[thread_num];
    param_quick param_q[thread_num];

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
    printf("%s is successfully opened!\n", filename);
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

    printf("Sorting Time : %fs\n", duration);

    printf("=====Now writing file=====\n");
    fp = fopen("result_quick","w");
    if(fp != NULL){
        for(i=1;i<=arr_size;i++) {
            fprintf(fp,"%10d ",origin[i-1]);
            if(i != 0 && i%5 == 0) fprintf(fp,"\n");
        }
    }
    printf("=====Writing file is finished=====\n");
    printf("Check if file is sorted\n");
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
        printf("It's Sorted!\n");
    }
    free(origin);
    free(merged);
    fclose(fp);
}
