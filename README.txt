

-------------------------------------------------------------------------
-------------------------------------------------------------------------
MISC:
-------------------------------------------------------------------------
wait queue:
A queue of processes that are waiting fot an event.

static declaration of wait queue -> DECLARE_WAIT_QUEUE_HEAD(my_queue) ;

run time declaration:
->wait_queue_head_t my_queue;
->init_waitqueue_head(&my_queue);

Once the wait queue is declared and intialized, a process may use it to 
go to sleep.

Threads are put to sleep by
->wait_event_interruptible( wait_queue_head_t head ,condition);

Specific threads in a wait queue can be woken up by
-> wake_up (wait_queue_head_t *temp);
-> wake_up_interruptible(wait_queue_head_t *temp);
All threads in a wait queue can be woken up by
-> wake_up_all(wait_queue_head_t *temp);

-------------------------------------------------------------------------
kernel linked lists

struct my_struct{
    char data0;
    int data1;
    struct list_head listhead;
}

[1] Declare "struct list_head" variable inside your defined struct to form
    a linked list. Understand that struct list_head is just two pointers def-
    -ined as 
    struct list_head{
        struct list_head *next;
        struct list_head *prev;
    }.
===> Hence you have to allocate memory for the defined struct before
     you add elements to the list. Kernel will not do it for you.
     Although frustrating, it I think it's a very elegant design pattern
     and what makes C, C.

[2] LIST_HEAD(my_linked_list);
    We can refer to the head anytime using  &my_linked_list.list_head;

[3] Addition:
    struct my_struct *temp = kmalloc( sizeof(struct my_struct), GFP_KERNEL);
    temp->data0 = 'a'; temp->data1=0;
    list_add( &temp->listhead, &my_linked_list);

[4] Traversal:
    -> list_for_each(pos, &my_linked_list.listhead);
    where pos is "struct list_head *pos"
    but this only gets pointer to list_head. To get the pinter to our desired 
    struct use list_entry() which is an alias of container_of();

       struct my_struct *temp;
    -> temp = list_entry( pos, &my_struct, listhead);
    
    -> list_for_each_entry( temp, &my_linked_list, listhead);

[5] Delete:
    Use list_for_each_safe() as it preserves the temporary cursors next
    element. If we don't use safe() temporary cursor is lost once we delete
    it leaving us without a gps/maps in a forest.

       struct list_head *pos, *q;
    -> list_for_each_safe( pos, q, &my_linked_list.listhead);
    -> tmp = list_entry(pos, &my_struct, listhead);
    -> list_del(pos);
    -> free(tmp);

-------------------------------------------------------------------------
-> task_tgid_vnr(current): gets the thread group id;

From a user viewpoint, all threads have the same process id
However at a kernel level, threads have different process id, but are 
                           still related to the process using the concept
                           thread group id.

So, If a process is created with process id = 7,
    you create a new thread, for the kernel new thread's process id is 
    8 [anynumber but not 7] but it binds this new thread with the initial
    main thread using the thread group id 7.
    
    But for an user, pid is just 7.

-------------------------------------------------------------------------


