#include<stdio.h>
#include<pthread.h>

int varA = 0;
  int i;
int write = 0;

void* ecr(void* arg) {
    int thread_id = *(int*)arg;

    varA = 42; 
    printf("Thread %d is writing to var\n", thread_id);
 
    write = 1;
    return NULL;
}
void* ar(void* arg) {
    int thread_id = *(int*)arg;

   

    printf("Thread %d is reading var: %d\n", thread_id, varA*varA);
    return NULL;
}


int main(){
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    pthread_create(&t1, NULL,ecr , &id1);
    pthread_create(&t2, NULL, ar, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

   
    return 0; 
}