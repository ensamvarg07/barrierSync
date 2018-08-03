#include <linux/module.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <linux/errno.h>

static LIST_HEAD(linkedlist);
int waitq_flag=0;
int waitq_temp=0;

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

asmlinkage int barrier_init(int count, int * barrier_id){
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
                init_flag=1;
                bc_ptr -> barrier_id = bc_temp->barrier_id +1;
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

asmlinkage int barrier_wait(int barrier_id){

    int barrier_id = barrier_id;
    int flag=0;
    barrier_content *bc_temp;
    
    list_for_each_entry(bc_temp, &linkedlist, linkedlist_head){
        if( bc_temp-> process_id == task_tgid_vnr(current) && bc_temp-> process_id == barrier_id){
            flag=1;
            break;
        }   
    }
    if(flag==0)
        return -EINVAL;

    spin_lock(&bc_temp->my_spin);       /* <---------------------------------------- */
    if( bc_temp-> count < bc_temp->remaining_threads ){                         // | 
        bc_temp -> count ++;                                                    // | 
        waitq_temp ++;                                                          // | 
    spin_unlock(&bc_temp->my_spin);     /* <---------------------------------------- */ 

        wait_event_interruptible( &bc_temp->my_q, waitq_flag);                  // | 

    spin_lock(&bc_temp->my_spin);                                               // | 
        if( waitq_temp == 0 ) waitq_flag=0;                                     // | 
        else waitq_temp-- ;                                                     // | 
    spin_unlock(&bc_temp->my_spin);                                             // | 
    }                                                                           // | 
    
    else{                                                                       // | 
        waitq_flag = 1;
        wake_up_all( &bc_temp->my_q);                                           // | 
        bc_temp -> count = 0;                                                   // | 
    spin_unlock(&bc_temp->my_spin);     /* <---------------------------------------- */
    }

return 0;
}

asmlinkage int barrier_destroy(int barrier_id){
    barrier_content bc_tmp;
    struct list_head *pos, *q;

    list_for_each_safe(pos, q, linked_list){
        bc_tmp = list_entry()    
        list_del();
    }

return 0;
}


