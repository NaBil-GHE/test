#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h> 

sem_t e, f, m;
#define SIZE 5 
int tab[SIZE], i=0, j=0;


void* prod(void* arg) {
    while(1) {
        sem_wait(&e);
        sem_wait(&m);
        tab[i] = i;
        printf("ecrite: %d \n", tab[i]);
        i = (i+1) % SIZE;
        sem_post(&m);
        sem_post(&f);
        // sleep(1);

    }
    return NULL;
}

void* consm(void* arg) {
  
    while(1) {
        sem_wait(&f);
        sem_wait(&m);
        printf("lire: %d \n", tab[j]);
        printf("-------------------\n");
        j = (j+1) % SIZE;
        sem_post(&m);
        sem_post(&e);
        // sleep(1);
    }
    return NULL;
}



int main(){

    pthread_t t1, t2;

    sem_init(&e, 0, SIZE);
    sem_init(&f, 0, 0);
    sem_init(&m, 0, 1);


    pthread_create(&t1, NULL, prod, NULL);
    pthread_create(&t2, NULL, consm, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);





    return 0;
}