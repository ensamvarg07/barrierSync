MISC:

-------------------------------------------------------------------------
wait queue:
A queue of processes that are waiting fot an event.

static declaration of wait queue -> DECLARE_WAIT_QUEUE_HEAD(my_queue) ;

wait_queue_head_t my_queue;
init_waitqueue_head(&my_queue);

Once the wait queue is declared and intialized, a process may use it to 
go to sleep.
