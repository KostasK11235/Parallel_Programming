/*
To run on server:
moduel load mpi/mpich-x86_64

mpicc -o [filename]

mpiexec -n number_of_processes executable [options]
*/

/*
#1. The number of processes is usually static (e.g. one process per core), numbered from 0 to p-1
#2. All data is local to some processor and in the protected memory space of process. No race conditions!
#3. Access to the data of other processes need to be explicitly managed by message passing!!

Disadvantages:
#1. explicit management of communication is cumbersome
#2. harder to program than OMP parallelization

Advantages:
#1. explicit management of communication allows optimization of the time-consuming communications
#2. portable to many different types of machines
*/

MPI_Init(&argc, &argv);     /* Initializes the environment */
MPI_Finalize();             /* Clean up and terminate the environment */

/* Can be synchronous or buffered, depending on message size */
/* Thread safe but not interrupt safe */
/* Returns an error message */
int MPI_Send(void* buf, int count, MPI_Datatype type, int dest, int tag, MPI_Comm comm);

/* Synchronous send: returns when the destination has started to receive the message */
int MPI_Ssend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)

/* Buffered send: returns after making a copy of the buffer. The destination might not yet 
                  have started to receive the message */
int MPI_Bsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)

/* Ready send: an optimized send if the user can guarantee that the destination has already
               posted the matching receive */
int MPI_Rsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)


/* blocking receive: returns once the message has been received.
the status object can be queried for more information about the message */
/* MPI_Recv matches a message sent by MPI_Send if tag, source and dest match */
/* Thread safe but not interrupt safe */
/* returns an error message */
int MPI_Recv(void* buf, int count, MPI_Datatype_type, int source, int tag, MPI_Comm comm, MPI_Status* status)

#1. MPI_ANY_TAG can be used as wildcard for MPI_Recv
#2. It only matches on the rank specified by dest
#3. Source has to be the rank of the sending process. 
    MPI_ANY_SOURCE can be used as wildcard.
#4. The buffer size on the receiving side is the allocated memory, 
    and thus the maximum message size that can be received. Not necessarily the actual size.


/* Stores the number of processes in the communicator MPI_COMM_WORLD in the variable size */
MPI_Comm_size(MPI_COMM_WORLD, &size)

/* Stores the rank (ID) of the calling process in the communicator MPI_COMM_WORLD in the variable rank */
MPI_Comm_rank(MPI_COMM_WORLD, &rank)



A communicator specifies the communication context for a communication operation.
Each communication context provides a separate “communication universe”: messages are
always received within the context they were sent, and messages sent in different contexts
do not interfere.
The communicator also specifies the set of processes that share this communication
context. This process group is ordered and processes are identified by their rank within
this group. Thus, the range of valid values for dest is 0, ..., n-1∪{MPI_PROC_NULL}, where
n is the number of processes in the group. (If the communicator is an inter-communicator,
then destinations are identified by their rank in the remote group. See Chapter 6.)
A predefined communicator MPI_COMM_WORLD is provided by MPI. It allows communication with all processes that are accessible after MPI initialization and processes are
identified by their rank in the group of MPI_COMM_WORLD.


/* performs a reduction using the operation op on the data in sendbuf and places the */
/* results in recvbuf on the root rank. */
/* if MPI_IN_PLACE is specified as sendbuf then the data to be reduced is assumed to */
/* be in the recvbuf and will be replaced on the root rank */
int MPI_Reduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, 
                MPI_Op op, int root, MPI_Comm comm);

// performs a reduction using the operation op on the data in sendbuf and places the 
// results in recvbuf on all ranks
// if MPI_IN_PLACE is specified as sendbuf then the data to be reduced is assumed to
// be in the recvbuf and will be replaced by the reduction
int MPI_Allreduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
                MPI_Op op, MPI_Comm comm); 

op          description
MPI_MAX     Maximum
MPI_MIN     Minimum
MPI_SUM     Sum
MPI_PROD    Product
MPI_MAXLOC  Maximum and location
MPI_MINLOC  Minimum and location
MPI_LAND    Logical AND
MPI_BAND    Bitwise AND
MPI_LOR     Logical OR
MPI_BOR     Bitwise OR
MPI_LXOR    Logical exclusive OR
MPI_BXOR    Bitwise exclusive OR


/* SCATTER OPERATIONS */
/* 
#1. There are two versions of scatter operations
#2. the sizes on each rank can be the same or different 
#3. MPI_IN_PLACE can be used for the recvbuf 
*/

// scatters data from the sendbuf buffer on the root rank into recvbuf buffers on the
// other ranks. Each rank gets a corresponding junk of the data
// sendbuf, sendcnt and sendtype are significant only on the root rank
// Note: recvcnt needs to be the same on all ranks
int MPI_Scatter(void *sendbuf, int sendcnt, MPI_Datatype sendtype, 
                void *recvbuf, int recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm)

// similar to MPI_Scatter but the sendcnt values can differ from rank to rank
// the root node thus spcifies an array of recvcnts and of displacements displs
// The displacements specify where the data for each rank starts in the buffer
int MPI_Scatterv( void *sendbuf, int *sendcnts, int *displs, 
                MPI_Datatype sendtype, void *recvbuf, int recvcnt,
                MPI_Datatype recvtype, int root, MPI_Comm comm)

// optimized version of an MPI_Reduce followed by an MPI_Scatter
int MPI_Reduce_scatter(void *sendbuf, void *recvbuf, int *recvcnts, 
                    MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)


/* GATHER OPERATIONS */
/* !!! IT GATHERS THE DATA, NOT SUM IT !!! */
/*
#1. There are four versions of gather operations
#2. Either just one root rank gathers the data or all ranks gather
#3. The sizes on each rank can be the same of different
#4. MPI_IN_PLACE can be used for the sendbuf
*/

// gathers data from the sendbuf buffers into a recvbuf buffer on the root rank
// recvbuf, recvcnt and recvtype are significant only on the root rank
// Note: the sendcnt needs to be the same on all ranks
int MPI_Gather(void *sendbuf, int sendcnt, MPI_Datatype sendtype, 
            void *recvbuf, int recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm)

// similar to MPI_Gather but the sendcnt values can differ from rank to rank
// the root node thus gets an array of recvcnts and of displacements displs
// The displacements specify where the data from each rank starts in the buffer
int MPI_Gatherv(void *sendbuf, int sendcnt, MPI_Datatype sendtype, 
                void *recvbuf, int *recvcnts, int *displs, 
                MPI_Datatype recvtype, int root, MPI_Comm comm)

// similar to MPI_Gather, but the data is gathered at all ranks and not just a root
// it is semantically the same as an MPI_Gather followed by MPI_Bcast
int MPI_Allgather(void *sendbuf, int sendcnt, MPI_Datatype sendtype, 
                void *recvbuf, int recvcnt, MPI_Datatype recvtype, MPI_Comm comm)

// similar to MPI_Gatherv, but the data is gathered at all ranks and not just a root
// it is semantically the same as an MPI_Gatherv followed by MPI_Bcast
int MPI_Allgatherv(void *sendbuf, int sendcount, MPI_Datatype sendtype, 
                void *recvbuf, int *recvcounts, int *displs, MPI_Datatype recvtype, MPI_Comm comm)


// broadcast the data from the root rank to all others
int MPI_Bcast( void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm )

// MPI_Barrier waits for all ranks to call it
int MPI_Barrier(MPI_Comm comm)


/* Probing for messages */
// wait for a matching message to arrive
int MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status)

// check if a message has arrived. 
// flag is nonzero if there is a message waiting
int MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status)

// gets the number of elements in the message waiting to be received
int MPI_Get_count(MPI_Status *status, MPI_Datatype datatype, int* count)


/* Nonblocking send and receive */
/* These functions return immediately while communication is still going 
#1. They behave the same way as the corresponding blocking versions but perform the communications asynchronously
#2. They fill in an MPI_Request object that can be used to test for competition
*/

int MPI_Issend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, 
            MPI_Comm comm, MPI_Request *request)

int MPI_Ibsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, 
            MPI_Comm comm, MPI_Request *request)

int MPI_Isend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, 
            MPI_Comm comm, MPI_Request *request)

int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
            MPI_Comm comm, MPI_Request *request)


/* Waiting for completion */
/* We can wait for one, some or all communication requests to finish */
int MPI_Wait(MPI_Request *request, MPI_Status *status)
// waits for the communication to finish and fills in the status

int MPI_Waitall(int count, MPI_Request array_of_requests[], 
                MPI_Status array_of_statuses[])
// waits for all given communications to finish and fills in the statuses

int MPI_Waitany(int count, MPI_Request array_of_requests[], int *index, 
                MPI_Status *status)
// waits for one of the given communications to finish, sets the index to indicate
// which one and fills in the status

int MPI_Waitsome(int incount, MPI_Request array_of_requests[], 
                int *outcount, int array_of_indices[], MPI_Status array_of_statuses[])
// waits for at least one of the given communications to finish, sets the number
// of communication requests that have finished, their indices and status

/* Testing for completion and cancellation */
/* Instead of waiting we can just test whether they have finished */
int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status)
// tests if the communication is finished. Sets flag to 1 and fills in the status if
// finished or sets the flag to 0 if not finished.

int MPI_Testall(int count, MPI_Request array_of_requests[], int *flag, 
                MPI_Status array_of_statuses[])
// test whether all given communications are finished. Sts flag to 1 and fills in 
// the status aray if all are finished or sets the flag to 0 if not all are finished.

int MPI_Testany(int count, MPI_Request array_of_requests[], int *index, 
                int *flag, MPI_Status *status)
// test whether one of the given communications is finished. Sets flag to 1 and fills 
// in the index and status if one finished or sets the flag to 0 if none is  finished.

int MPI_Testsome(int incount, MPI_Request array_of_requests[], int *outcount, 
                int array_of_indices[], MPI_Status array_of_statuses[])
// tests whether some of the given communications is finished, sets the number
// of communication requests that have finished, their indices and statuses.

/* Cancel a request */
int MPI_Cancel(MPI_Request *request)

/* 
Allocate a sufficiently large buffer and then pack the data into it 
Send/Receive the packed buffer with type MPI_PACKED 
Finally unpack it on the receiving side 
*/
int MPI_Pack(void *inbuf, int incount, MPI_Datatype datatype,
void *outbuf, int outcount, int *position, MPI_Comm comm)
// packs the data given as input into the outbuf buffer starting at a given position.
// outcount is the size of the buffer and position gets updated to point to the first
// free byte after packing in the data. 
// An error is returned if the buffer is too small. 

int MPI_Unpack(void *inbuf, int insize, int *position,
void *outbuf, int outcount, MPI_Datatype datatype, MPI_Comm comm)
// unpack data from the buffer starting at given position into the buffer outbuf.
// position is updated to point to the location after the last byte read

int MPI_Pack_size(int incount, MPI_Datatype datatype, MPI_Comm comm, int *size)
// returns in size an upper bound for the number of bytes needed to pack incount
// values of type datatype. This can be used to determine the required buffer size 