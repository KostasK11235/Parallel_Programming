/* GENERAL INFORMATION */
A process consists of: 1. Address space: text segment, data segment, heap and stack
                       2. Information maintained by the operation system (process state,
                            priority, resources, statistics)

Process: memory, instructors, program counter, stack pointer, registers, file descriptors
Thread : program counter, stack pointer and stack registers


/* Thread creation */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*routine)(void *), void *arg);

thread  : unique identifier for the new thread returned by the subroutine
attr    : used to set thread attributes. If NULL, the default values are used.
routine : the C routine that the thread will execute once it is created
arg     : single argument that may be passed to start_routine. It must be passed by reference as a pointer  
            cast of type void. NULL may be used if no argument is to be passed.

/* Thread joining */
/* Blocks the calling thread until the specified thread terminates. The value returned by the thread function
is stored in the memory location specified by status */
int pthread_join(pthread_t thread, void **status);


/* Thread Attributes */
pthread_attr_t attr;
int pthread_attr_init(&attr);
int pthread_attr_destroy(&attr);

/* Detach state of thread */
int pthread_attr_getdetachstate(&attr, (int *) &ds);
int pthread_attr_setdetachstate(&attr, (int) ds);

PTHREAD_CREATE_JOINABLE: joinable thread
PTHREAD_CREATE_DETACHED: daemon thread

/* Stack size of the thread, minimum is PTHREAD_STACK_MIN */
int pthread_attr_getstacksize(&attr, (size_t *) *ss);
int pthread_attr_setstacksize(&attr, (size_t) ss);

/* Stack of a thread, allocated explicitly (e.g. malloc) */
int pthread_attr_getstackaddr(&attr, (void **) &sa);
int pthread_attr_setstacksize(&attr, (void *) sa);


/* Mutext Management */
/* Declaration and static initialization of a mutex: */
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

/* Declaration and dynamic intialization of a mutex: */
pthread_mutex_t mymutex;
pthread_mutex_init(&mymutex, NULL);

/* Dynamic allocation of Mutex: */
/* Pointer to a mutex: */
pthread_mutex_t *mymutex;

/* Memory allocation: */
mymutex  = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));

/* Mutex intialization: */
pthread_mutex_init(mymutex, NULL);

/* Destroying mutex: */
pthread_mutex_destroy(mymutex);
free(dynamic_mutex);

/* Locking the mutex: */
pthread_mutex_lock(&mymutex);

/* Unlocking the mutex: */
pthread_mutex_unlock(&mymutex);

/* Implementation of spinlock: */
int pthread_mutex_trylock(pthread_mutex_t *m);
/* e.g. while(pthread_mutex_trylock(&mutex) == EBUSY ) sched_yield(); */

/* Mutex Attributes */
pthread_mutexattr_t attr;
int pthread_mutexattr_init(&attr);
int pthread_mutexattr_destroy(&attr);

/* Mutex types */
int pthread_mutexattr_gettype(&attr, (int *)&type);
int pthread_mutexattr_settype(&attr, (int) type);

PTHREAD_MUTEX_NORMAL     : faster implementation 
PTHREAD_MUTEX_RECURSIVE  : can be locked several times by the same thread
PTHREAD_MUTEX_ERRORCHECK : for debugging
PTHREAD_MUTEX_DEFAULT    : usually the first one above

/* Barriers */
int pthread_barrier_init(pthread_barrier_t *bar, const pthread_barrierattr_t *attr, unsigned int count);
int pthread_barrier_wait(pthread_barrier_t *bar);
int pthread_barrier_destroy(pthread_barrier_t *bar);

/* Condition Variables */
/* Synchronization mechanism that allows multiple threads to wait on a condition 
and resume their execution after some time. They protect a more general operation.
A thread waits on a condition variable until it is informed (by the variable) that
it can continue. Another (running) thread signals the condition variable, allowing 
other threads to continue.*/
int pthread_cond_init(pthread_cond_t *cond, const_pthread_condattr_t *attr);

/* Static initialization: */
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

/* Dynamic initialization: */
pthread_cond_init(&condition, NULL);

/* They must be ALWAYS an associated mutex!!!*/

int pthread_cond_wait(pthread_cond_t *condition, pthread_mutex_t *mutex);

/* A thread informs a single thread that waits on the condition variable */
int pthread_cond_signal(pthread_cond_t *cond);

/* A thread informs all the threads that wait on the condition variable */
int pthread_cond_broadcast(pthread_cond_t *cond);

/* Waits on a condition variable but with time constraint. If timeout occurs, the function
returns ETIMEDOUT */
int pthread_cond_timedwait(&condition, &mutex, (struct timespec *)expiration);
