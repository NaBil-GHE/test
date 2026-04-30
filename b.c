#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>


sem_t s1, s2,s3,s4;

void* tf1(void* arg) {
    while(1){
 sem_wait(&s1);
 printf("A\n");
 sem_post(&s3);
    }

 return NULL;
}

void* tf2(void* arg) {
    while(1){
    sem_wait(&s2);
   printf("B\n");
   sem_post(&s3);
    }
   return NULL;
}

void* tf3(void* arg) {
    while(1){
    sem_wait(&s3);
    printf("C\n");
    sem_post(&s2);
    
    }
    return NULL;
}
void* tf4(void* arg) {
    while(1){
    sem_wait(&s3);
    printf("D\n");
    sem_post(&s1);
    }
    return NULL;
}

int main(){
    pthread_t t1, t2, t3, t4;
    int id1 = 1, id2 = 2, id3 = 3, id4 = 4;

    sem_init(&s1, 0, 1); 
    sem_init(&s2, 0, 1); 
    sem_init(&s3, 0, 0);
    sem_init(&s4, 0, 0);

    pthread_create(&t1, NULL, tf1, NULL);
    pthread_create(&t2, NULL, tf2, NULL);
    pthread_create(&t3, NULL, tf3, NULL);
    pthread_create(&t4, NULL, tf4, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    sem_destroy(&s1); 
    sem_destroy(&s2); 
    sem_destroy(&s3); 
    sem_destroy(&s4);

   
    return 0; 
}
