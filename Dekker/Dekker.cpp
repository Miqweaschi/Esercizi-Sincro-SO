#include <iostream>
#include <pthread.h>

int turn = 1;
int c1 = 1;
int c2 = 1;

void* process1(void* arg){

    int id = *(int*)arg;
    while(true){
    c1 = 0;
    while(c2 == 0){
        if(turn == 2){
            c1 = 1;
            while (turn == 2){
                }
        c1 = 0;
        }
   
    std::cout<<"sezione critica: "<<id<<std::endl;

    turn = 2;
    
    c1 = 1;
    }
    }
    return nullptr;
}


void* process2(void* arg){

    int id = *(int*) arg;
    while(true){
    c2 = 0;
    while(c1 == 0){
        if(turn == 1) {
           c2 = 1;
           while ( turn == 1){
           }
        c2 = 0;
        }
    
    std::cout<<"sezione critica: "<<id<<std::endl;

    turn = 1;

    c2 = 1;
    }
    }
    return nullptr;
}


int main(){


pthread_t processo1;
int process1ID = 1;

pthread_t processo2;
int process2ID = 2;

pthread_create(&processo1,nullptr,process1,&process1ID);
pthread_create(&processo2,nullptr,process2,&process2ID);


pthread_join(processo1, nullptr);
pthread_join(processo2, nullptr);

return 0;
}


