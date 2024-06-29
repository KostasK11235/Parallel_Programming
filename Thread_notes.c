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
