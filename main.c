#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>   //wait()
#include <sys/wait.h>    //wait()
#include <errno.h>
#include <pthreads.h>

#define ITERATIONS 100

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

// barrier_init(unisigned int count, unsigned int *barried_id)    
    syscall( ,5, barrier_id_set1);
    bid1 = *barrier_id_set1;

    for(i=0; i<5; i++){
        args -> i = i;
        args -> barrier_id = bid1;
        pthread_create( &thread_set1[i], NULL, process1, (void *)args);
    }

// barrier_init(unsigned int count, unsigned int *barrier_id)
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

//barrier_destroy(unsigned int barrier_id)
    syscall( ,barrier_id_set1);
    sleep(1);
//barrier_destroy(unsigned int barrier_id)
    syscall( ,barrier_id_set2);
}

void * process1(void *pargs){   
    process_args *args = (process_args *)malloc(sizeof(process_args));
   
    for(int i=0; i< ITERATIONS; i++){
        args = (process_args *)pargs;
//barrier_wait(unsigned int barrier_id)
        syscall( , args-> barrier_id);

    }
    pthread_exit(0);
}


void * process2(void *pargs){
    process_args *args = (process_args *)malloc(sizeof(process_args));
   
    for(int i=0; i< ITERATIONS; i++){
        args = (process_args *)pargs;
//barrier_wait(unsigned int barrier_id)
       syscall( , args->barrier_id);

    }
    pthread_exit(0);
}


