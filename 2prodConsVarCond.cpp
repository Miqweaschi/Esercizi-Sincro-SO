#include <pthread>
#include <stdio.h>
#include <iostream>
#include <queue>
#DEFINE MAX=10

std::queue<int> buffer;
int pieno = 0;
int item = 0;
pthread_cond_t pieno;
pthread_cond_t vuoto;
pthread_mutex_t mutex;

void* produttore (void* arg){
    pthread_mutex_lock(&mutex);
        while(pieno==MAX){
            pthread_cond_wait(&pieno,&mutex);
        }
    item++;
    buffer.push(item);
    pieno++;
    pthread_cond_signal(&vuoto);
    pthread_mutex_unlock(&mutex);
}

void* consumatore (void* arg){
    pthread_mutex_lock(&mutex);
        while(pieno==0){
           pthread_cond_wait(&vuoto,&mutex);
        }
    buffer.pop();
    pieno--;
    pthread_cond_signal(&pieno);
    pthread_mutex_unlock(&mutex);
}

