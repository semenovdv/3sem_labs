#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <fstream>
#include <vector>

using namespace std;

unsigned long long nProcess = 1;
sem_t semaphore;

typedef struct {
    vector<long long>::iterator l;
    vector<long long>::iterator r;
    bool inv;
} Data;


void* bitseqsort(void *thdata) {

    Data *data = new Data;
    data->l =  ((Data*)(thdata))->l;
    data->r =  ((Data*)(thdata))->r;
    data->inv =  ((Data*)(thdata))->inv;

    if (data->r - data->l < 1)
        return NULL;
 
	vector<long long>::iterator m = data->l + (data->r - data->l) / 2 ;

	for (vector<long long>::iterator i = data->l, j = m+1; i <= m && j <= data->r; i++, j++) {
		if (!data->inv){
            if (*i > *j){
                swap(*i, *j);
                
            }
        } else {
            if (*i < *j){
                swap(*i, *j);
                
            }
        } 
	}

    Data *putl = new Data;
    putl->l = data->l;
    putl->r =  m;
    putl->inv = data->inv;
    

    Data *putr = new Data;
    putr->l = m+1;
    putr->r = data->r;
    putr->inv = data->inv;
    

    int curProc;
    sem_getvalue(&semaphore, &curProc);
    //cout << "Sema: " << curProc << endl;

        if(curProc == 0){
            bitseqsort(putl);
            bitseqsort(putr);
        } else {
       
            pthread_t thread_pid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);


            sem_wait(&semaphore);

            if(pthread_create(&thread_pid, NULL, bitseqsort, putl)) {
                cout << "ERROR creating thread" << endl;
                exit(1);
            }
            
            
            bitseqsort(putr);
            
            
            if(pthread_join(thread_pid, NULL)) {
                cout << "ERROR joining thread" << endl;
                exit(1);
            }
            sem_post(&semaphore);
        }
    

    delete data;
    delete putl;
    delete putr;
    
    return NULL;
}

void* makebitonic(void* thdata);

void* make_n_sort(void *thdata) {
    Data *data = new Data;
    data->l =  ((Data*)(thdata))->l;
    data->r =  ((Data*)(thdata))->r;
    data->inv =  ((Data*)(thdata))->inv;
    

    makebitonic(data);
    
	bitseqsort(data);
    delete data;
    return NULL;
}


// делает послдовательность битонной
// дозаполняем ее элементами больше максимального, не влияет на ассимптотику
void* makebitonic(void* thdata) {
    
    Data *data = new Data;
    data->l =  ((Data*)(thdata))->l;
    data->r =  ((Data*)(thdata))->r;
    data->inv =  ((Data*)(thdata))->inv;


	if (data->r - data->l <= 1) {
        return NULL;
    }

    // находим центр
	vector<long long>::iterator m = data->l + (data->r - data->l) / 2;

    Data *putL = new Data;
    putL->l = data->l;
    putL->r = m;
    putL->inv = 0;
    
    Data *putR = new Data;
    putR->l = m+1;
    putR->r = data->r;
    putR->inv = 1;
    
    
    int curProc;
    sem_getvalue(&semaphore, &curProc);
    
    if(curProc == 0){
            make_n_sort(putL);
            make_n_sort(putR);
    } else {

        
        pthread_t thread_pid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setscope(&attr,PTHREAD_SCOPE_PROCESS);

        sem_wait(&semaphore); // отнимает, если может
        
        if(pthread_create(&thread_pid, NULL, make_n_sort, putL)) {
            cout << "ERROR creating thread" << endl;
            exit(1);
        }


        make_n_sort(putR);

        if(pthread_join(thread_pid, NULL)) {
            cout << "ERROR joining thread" << endl;
            exit(1);
        }

        sem_post(&semaphore);
        
    }
    

    delete data;
    delete putL;
    delete putR;
    return NULL;
}

void bitonicsort(vector<long long> &a) {
	
	vector<long long>::iterator max;
 
    max = std::max_element(a.begin(), a.end());
    long long nmax = *max + 1;
    
    long long n = 1;
	while (n < a.end() - a.begin()) n *= 2;
    
   
    
  

    unsigned long long N = n;

	while (a.size() < N) a.push_back(nmax);  // дозаполнение элементов

    Data *put = new Data;
    put->l = a.begin();
    put->r = a.end() - 1;
    put->inv = 0;
    
    

	makebitonic(put);
    
	bitseqsort(put);

    // чистим 
    // расчет на сортировку по возрастанию
    
    while(*(a.end()-1) == ((nmax))){
        a.pop_back();
    }

    delete put;
}

int main(int argc, char* argv[]) {

    // проверка входных данных
    if (argc <= 1) {
        fprintf(stderr, "No file detected.\n");
        exit(1); 
    } else if(argc == 2 && strcmp(argv[1], "-p") == 0) {
        fprintf(stderr, "Enter number of threads.\n");
        exit(1);
    } else if(argc == 3) {
        fprintf(stderr, "No file detected.\n");
        exit(1);
    }

    // составление кол-ва потоков и открытие файла
    nProcess = 0;
    int fil = 1; // определяет положение файла
    if(strcmp(argv[1], "-p") == 0){
        nProcess = atoi(argv[2]);
        fil = 3;
    }

    ifstream fin(argv[fil]);
    if (!fin.is_open()){
        fprintf(stderr, "Cannot open file.\n");
        exit(2);
    }
    
    long long kol= 0;
    fin >> kol;
            
    vector<long long> a(kol);
    
    // считывание значений из файла
    for(long long i = 0; i < kol; i++)
        fin >> a[i];
    
    // сортировка 
    sem_init(&semaphore, 0, nProcess-1);
    bitonicsort(a);
    sem_destroy(&semaphore);   

    ofstream fout("out.txt");
    if (!fout.is_open()){
        fprintf(stderr, "Cannot open file.\n");
        exit(2);
    }  
    
    for(unsigned int i = 0; i < a.size(); i++){
        fout << a[i] << " ";
        
        if ( i%10 == 0 ){
            fout << endl;
        }
    }
     cout << endl;

    
    
    return 0;
}