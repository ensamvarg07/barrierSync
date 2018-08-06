#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>   //wait()
#include <sys/wait.h>    //wait()
#include <errno.h>
#include <pthreads.h>

struct process_args{
    int i;
    unsigned int barrier_id;
};
typedef struct process_args process_args;

void childProcess();

int main(int argc, char **argv){

pid_t pid0,pid1;

pid0 = fork();
if(pid0<0)
    printf("\nFork Error for zeroth child");

else if(pid0==0)
    childProcess();

else if(pid>0){

    pid1 = fork();
    if(pid1<0)
        printf("\nFork Error for first child");
    else if(pid==0)
        childProcess();
    
    else if(pid>0)
        wait(NULL);

    wait(NULL);
}

return 0;
}

void childProcess(){

unsigned int *barrier_id_set1, *barrier_id_set2;
barrier_id_set1 = malloc(sizeof(unsigned int));
barrier_id_set2 = malloc(sizeof(unsigned int));
unsigned int bid1, bid2;
int i;

pthread_t thread_set1[5], thread_set2[20];
process_args *args = (process_args *)malloc(sizeof(process_args));

syscall( ,5, barrier_id_set1);
bid1 = *barrier_id_set1;

for(i=0; i<5; i++){
    args -> i = i;
    args -> barrier_id = bid1;
    pthread_create( &thread_set1[i], NULL, process1, (void *)args);
}

syscall( ,20, barrier_id_set2);
bid2 = *barrier_id_set2;

for(i=0; i<20; i++){
    args -> i = i;
    args -> barrier_id = bid2;
    pthread_create( &thread_set1[i], NULL, process2, (void *)args);
}

for(i=0; i<5; i++)
    pthread_join(thread_set1[i], NULL);

for(i=0; i<20; i++)
    pthread_join(thread_set2[i], NULL);



syscall( ,barrier_id_set1);
syscall( ,barrier_id_set2);
}


void * process1(void *pargs){


// barrier_wait

}


void * process2(void *pargs){

//barrier_wait

}




}
