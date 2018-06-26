
static LIST_HEAD(linkedlist);

struct barrier_content{
    struct list_head linkedlist_head;
   
    int count;
    int remaining_threads;
    int barrier_id;
    pid_t process_id;
};
typedef struct barrier_content barrier_content;

int barrier_init(int count, int * barrier_id){
    barrier_content *bc_ptr, *bc_temp;          //bc stands for barrier content
   
    bc_ptr = kmalloc(sizeof(barrier_content), GFP_KERNEL );
    if( !bc_ptr) printf("\nError: Kmalloc() ");
    memset(bc_ptr, 0, sizeof(barrier_content));    

    if( !list_empty(linkedlist) ){
    //list_empty() is valid when the linkedlist is empty. Hence this block executes when the
    //linkedlist is not empty
        list_for_each_entry( bc_temp, &linkedlist, linkedlist_head){
            if( bc_temp -> process_id == task_tgid_vnr(current) )
                bc_ptr -> barrier_id = bc_temp->barrier_id +1;
        }
    }
        
    else{ 
        bc_ptr -> barrier_id = 1;
    } 
        bc_ptr -> remaining_threads = count;
        bc_ptr -> process_id = task_tgid_vnr(current);
        bc_ptr -> count  = 0;
        
        INIT_LIST_HEAD( barrier_content_ptr -> barrier_head);
        list_add( &bc_ptr->barrier_head, &linkedlist);  
        
        *barrier_id = bc_ptr -> barrier_id;
    
return 0;
}
