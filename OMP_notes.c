/* GENERAL INFORMATION ABOUT OMP !!! */
- Suitable for shared memory platforms
- Three primary components: 
        1. compiler directives - instruct the compiler to generate multithreaded code
        2. library calls
        3. ENVIRONMENT variables
- FORK-JOIN type of parallelism
- Threads communicate through shared variables
- 5 categories of directives:
        1. Parallel regions
        2. Worksharing
        3. Data Environment
        4. Synchronization
        5. Runtime functions & environment variables

/* Task construct in C/C++ */
/* Spawns tasks and puts them into a queue for the threads to work on */

# pragma omp task [clause[[,]clause]...] newline
    structured-block

clause: if (scalar-sxpression) /* only parallelize if true */
        final** (scalar-expression) /* if true, the task is meant to be the last task in a sequence or block of tasks */
        untied /*Ignored when final==true or task is an included task. Any thread can resume the task (might have trouble with the variables privacy) */
        default(shared|none)
        mergeable /* allows the task to be merged with others */
        private(list)
        firstprivate(list)
        shared(list)

** If the expression of a final clause if true: all child tasks will be executed sequentially in the task region, immediately by the encountering thread

ATTENTION! UNTIED tasks should not be combined with: threadprivate variables or thread numbers (ids)


/* taskwait construct */
/* The encountering task suspends its execution until all its child tasks complete */

# pragma omp taskwait newline


/* taskyield construct */
/* Specifies that the current task can be suspended in favor of the execution of a different task */

# pragma omp taskyield newline


/* parallel construct */
# pragma omp parallel [clause...] newline
                    if (scalar expression) /* Only parallelize if TRUE*/
                    num_threads (integer)
                    private (list)
                    shared (list)
                    default (shared|none)
                    firstprivate (list)
                    reduction (operator: list)  /* Perform a reduction on the thread-local variables and assign it to master thread*/
                    copyin (list) /* assign the same value to THREADPRIVATE variables for all threads in team */
structured block

/* The number of threads is determined in the following order:
1. Evaluation of if clause
2. Setting num_threads clause
3. Using omp_set_num_threads() func
4. Setting the OMP_NUM_THREADS environment variable
5. Implementation default - usually the number of CPUs on a node
*/


/* Creates a group of threads and distributes the loops among the threads */

# pragma omp parallel for  [clause[[,]clause]...]
    for-loops

clause: private(list)
        firstprivate(list)
        lastprivate(list)
        reduction(operator:list) /* variables in list must be shared in the parallel region where the reduction exists */
        schedule(kind[,chunk_size])
        collapse(n)
        ordered
        nowait

kind:
- static    : iterations size of chunk are assigned to threads (round-robin fashion)
- dynamic   : iterations are divied into chunk_size chunks and each thread requests a new chunk when done with a chunk
- guided    : like dynamic but chunks size is decreased every time until size 1 (or chunk_size if defined)
- runtime   : system reads OMP_SCHEDULE environment variable to define schedule and chunk_size
- auto      : the selection of schedule is defined by the compiler

 Tips:
 -if each loop has big run time difference then better choose DYNAMIC scheduling
 -if each loop has small run time difference then better choose STATIC scheduling
 

  /* Gives a different structured block to each thread*/
 # pragma omp sections [clause...] newline


 /* The structured block is executed by only one thread*/
 # pragma omp single [clause...] newline


/* Data Environment */
private(var): creates a private copy of var in each thread
- The value of the copy is not initialized
- The private copy is not related to the original in memory location

firstprivate(var): The private copy of each thread is initialized with the value of the original thread

lastprivate(var): Copies the value of the private variable, as assigned by the last loop iteration, to the original variable

/* Synchronization */
/* Critical Section: No two threads will simultaneously be in the critical section */
# pragma omp critical {...}

/* Atomic: Special case of critical section for simple instructions */
# pragma omp atomic

/* Barrier: All threads wait until each thread has reached the barrier */
# pragma omp barrier 

/* Master: Executed only by master thread, all other threads ignore it.*/
/* No barrier at the end of it !!! */
# pragma omp master


/* Implicit Barriers at the end of the following constructs */
- parallel
- for (except when nowait is used)
- sections (except when nowait is used)
- single (except when nowait is used)


