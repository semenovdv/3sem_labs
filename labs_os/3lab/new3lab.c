#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>


#define MAX 8
#define SWAP(x,y) t = x; x = y; y = t;

void compare();
void bitonicmerge(int, int, int);
void *recbitonic(void *arg);
void sort();

int data[MAX];
int up = 1;
int down = 0;

struct info {
    int low;
    int c;
    int dir;
};

int main() {

    int i;
    printf("\nEnter the data");
    for (i = 0;i < MAX ;i++) {
        scanf("%d", &data[i]);
    }

    sort();

    for (i = 0;i < MAX;i++) {
        printf("%d ", data[i]);
    }
}

void compare(int i, int j, int dir){

    int t;
    if (dir == (data[i] > data[j])){
        SWAP(data[i], data[j]);
    }
}

void bitonicmerge(int low, int c, int dir){

    int k, i;
    if (c > 1){
        k = c / 2;
        for (i = low;i < low+k ;i++)
            compare(i, i+k, dir);    
        bitonicmerge(low, k, dir);
        bitonicmerge(low+k, k, dir);    
    }
}

void *recbitonic(void *arg){
    struct info *inf = arg;
    //int low, int c, int dir

    int k;
    if (inf->c > 1) {
        k = inf->c / 2;

        pthread_t thread_pid;
        
        struct info *infl;
        infl->low = inf->low;
        infl->c = k;
        infl->dir = up;
        
        pthread_create(&thread_pid, NULL, &recbitonic, &infl);
    
        struct info *infr;
        infr->low = inf->low + k;
        infr->c = k;
        infr->dir = down;
        recbitonic(infr);
        pthread_join(thread_pid, NULL);
        
        bitonicmerge(inf->low, inf->c, inf->dir);
    }
}

void sort(){
    struct info *inf;
    inf->low = 0; 
    inf->c = MAX;
    inf->dir = up;
    recbitonic(inf);
}