#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PROD 3

std::queue<int> buffer;
//ci serviranno 2 semafori per gestire i produttori ed i consumatori.

sem_t vuoto;  //semaforo usato per controllare se il buffer è vuoto.
sem_t pieno;  //semaforo usato per controllare se il buffer è pieno.

pthread_mutex_t mutex;   // viene creato questo mutex perchè è necessario 
                         // quando viene prodotto o consumato un prodotto
                         // quindi aggiunto/eliminato dal buffer
                         // che sia fatto in mutua esclusione, i semafori da soli
                         // non impediscono che più thread accedano contemporaneamente
                         // al buffer. ES. immagina che due produttori (o cons)
                         // partano contemporanenamente, se ci sono due posti 
                         // liberi/pieni entrambi possono entrare e rischierebbero
                         // di scrivere sul buffer nello stesso momento.


void* produttore(void* arg){  // utilizziamo questa firma strana per poter utilizzare
                              // la funzione produttore con i thread.
                              
   int id = *(int*)arg;         // facciamo il cast dell arg per ricavare
                                // l'id del thread.
   
   int item = 0;
   while(true){
        sleep(1);  //simuliamo il tempo di produzione
        item++;
        
        sem_wait(&vuoto);      // controlliamo se il buffer è vuoto, inizialmente
                               // il buffer secondo la logica di questa simulazione
                               // sarà vuoto, quindi avrà valore 5. 


        pthread_mutex_lock(&mutex);  //chiudiamo il mutex
            buffer.push(item);
            std::cout<<"[produttore " << id << "] ha prodotto: " << item << std::endl;
        pthread_mutex_unlock(&mutex); //apriamo il mutex
        

        sem_post(&pieno);      // qui segnaliamo la creazione di uno spazio pieno 
                               // all'interno dell buffer e quindi nel caso ci fosse
                               // un consumatore in attesa di uno spazio pieno
                               // verrà risvegliato, all'inizio questo semaforo avrà
                               // valore 0 visto che il buffer è vuoto.
    }
    return nullptr;  //in questo caso il thread non restituisce nulla quindi nullptr.
}


void* consumatore(void* arg){
int id = *(int*) arg;
     while(true){
        sem_wait(&pieno);        // in modo specchiato alla funzione produttore, il 
                                // consumatore controlla inizialmente se c'è uno
                                // spazio prodotto all'interno del buffer.


        pthread_mutex_lock(&mutex);

            int item = buffer.front();      // consuma l'item nell buffer.
            buffer.pop();
            std::cout<<"[consumatore " << id <<"] ha consumato: " << item << std::endl;
        pthread_mutex_unlock(&mutex);

        sem_post(&vuoto);       // segnalo che all'interno del buffer c'è un posto
                                // vuoto in più


        sleep(2);  //simula il consumo.
    }
    return nullptr;
}


int main(){
    pthread_t prod[NUM_PROD];            // inizializziamo un array di threadproduttori 
     
    int prodID[NUM_PROD];           // inizializziamo un array che andranno ad indicare
                                    // gli ID dei singoli thread.
                                    
    pthread_t cons;             //inizializziamo un singolo consumatore.
    int consID = 1;


    sem_init(&vuoto,0,BUFFER_SIZE);
    sem_init(&pieno,0,0);                   //inizializziamo i semafori ed il mutex
    pthread_mutex_init(&mutex,nullptr);


    for(int i=0; i<NUM_PROD;++i){
        prodID[i]= i+1;
        pthread_create(&prod[i],nullptr,produttore,&prodID[i]);  // la funzione
                                            // pthread_create() serve per creare
                                            // un thread e richiede 4 paramtri:
                                            // 1: l'identificatore del thread
                                            // 2: gli attributi che sono opzionali
                                            // 3: la funzione da eseguire
                                            // 4: argomento da passare alla funzione.
    }


    pthread_create(&cons,nullptr,consumatore,&consID);

    for (int i=0;i<NUM_PROD;++i){       
    
        pthread_join(prod[i],nullptr);       // pthread_join serve per bloccare il
                                             // main finchè il thread chiamato non
                                             // finisce (in questa simulazione mai).
    }
    pthread_join(cons,nullptr);

return 0;
}
