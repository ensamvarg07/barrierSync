#include <linux/module.h>
#include <linux/slab.h>
#include <linux/wait.h> // wait_queue
#include <linux/list.h>
#include <asm/uaccess.h>
#include <linux/sched.h> //task_tgid_vnr(current)
#include <linux/errno.h>
int errno;

static LIST_HEAD(linkedlist);
int waitq_flag=0;
int waitq_counter=0;

struct barrier_content{
    struct list_head linkedlist_head;
   
    int count;                  // number of threads already on the barrier
    int remaining_threads;      // number of threads that needs to hit the barrier
    int barrier_id;
    pid_t process_id;

    wait_queue_head_t my_q;
    spinlock_t my_spin;
};
typedef struct barrier_content barrier_content;

asmlinkage long sys_barrier_init(unsigned int count,unsigned int * barrier_id){
    barrier_content *bc_ptr, *bc_temp;          //bc stands for barrier content
    int init_flag=0;  
 
    bc_ptr = kmalloc(sizeof(barrier_content), GFP_KERNEL );
    if( !bc_ptr) printf("\nError: Kmalloc() ");
    memset(bc_ptr, 0, sizeof(barrier_content));    

    if( !list_empty(linkedlist) ){
    //list_empty() is valid when the linkedlist is empty. Hence this block executes when the
    //linkedlist is not empty
    
        if(init_flag==0)                // For the first time, we don't have to iterate the list.
            bc_ptr -> barrier_id = 1;

        list_for_each_entry( bc_temp, &linkedlist, linkedlist_head){
            if( bc_temp -> process_id == task_tgid_vnr(current) ){
                bc_ptr -> barrier_id = bc_temp->barrier_id +1;
                init_flag=1;
            }
        }   
    }
        
    else{ 
        bc_ptr -> barrier_id = 1;
    } 
        bc_ptr -> remaining_threads = count;
        bc_ptr -> process_id = task_tgid_vnr(current);
        bc_ptr -> count  = 0;
    
        init_waitqueue_head( &(bc_ptr->my_q));
        spin_lock_init( &(bc_ptr->my_spin));       

        INIT_LIST_HEAD( barrier_content_ptr -> barrier_head);
        list_add( &bc_ptr->linkedlist_head, &linkedlist);  
        
        *barrier_id = bc_ptr -> barrier_id;
    
return 0;
}

/* function: sys_barrier_wait()
 * If n threads want to hit the barrier,
 * [1] n-1 threads increment the waitq_counter and are put to sleep
 * [2] nth thread sets waitq_flag to 1 and wakes up all the sleeping threads
 * [3] Upon waking up, n-1 threads will decrement the waitq_counter essentially making it 0.
 * [4] When waitq_counter becomes zero waitq_flag is reset to zero
 *
 * waitq_flag is the CONDITION argument in wait_event_interruptible( wait_queue_head_t *, CONDITION);
 */
asmlinkage long sys_barrier_wait(unsigned int barrier_id){

    int barrier_id = barrier_id;
    int flag=0;
    barrier_content *bc_temp;
    
    list_for_each_entry(bc_temp, &linkedlist, linkedlist_head){
        if( bc_temp-> process_id == task_tgid_vnr(current) && bc_temp-> process_id == barrier_id){
            flag=1;
            break;
        }   
    }
    if(flag==0){
        printk("\nError in barrier wait");
        return -EINVAL;
    }

    spin_lock(&bc_temp->my_spin);       // <------------------------------------------------ 
    if( bc_temp-> count +1 < bc_temp->remaining_threads ){                              // | 
        bc_temp -> count ++;                                                            // | 
        waitq_counter ++;                                                               // | 
    spin_unlock(&bc_temp->my_spin);     // <------------------------------------------------  

        wait_event_interruptible( &bc_temp->my_q, waitq_flag);                          // | 

    spin_lock(&bc_temp->my_spin);                                                       // | 
        if( waitq_counter == 0 ) waitq_flag=0;                                          // | 
        else waitq_counter-- ;                                                          // | 
    spin_unlock(&bc_temp->my_spin);                                                     // | 
    }                                                                                   // | 
    
    else{                                                                               // | 
        waitq_flag = 1;
        wake_up_all( &bc_temp->my_q);                                                   // | 
        bc_temp -> count = 0;                                                           // | 
    spin_unlock(&bc_temp->my_spin);     // <------------------------------------------------
    }

return 0;
}

asmlinkage long sys_barrier_destroy(unsigned int barrier_id){
    barrier_content bc_tmp;
    struct list_head *pos, *q;

    list_for_each_safe(pos, q, &linked_list.linkedlist_head){
        bc_tmp = list_entry(pos, &barrier_content, linkedlist_head); 
        if( bc_tmp->barrier_id == barrier_id && bc_tmp->process_id == task_tgid_vnr(current)){
            list_del(pos);
            kfree(bc_tmp);
        }
    }
return 0;
}


