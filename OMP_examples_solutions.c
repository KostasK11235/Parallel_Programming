/* ParallelComputing_06.OpenMP3 */

/* Slide 47 solution */
int count_goo(item_t *item)
{
    int n = 0;
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            while(item)
            {
                #pragma omp task firstprivate(item)
                {
                    if(is_good(item))
                    {
                        #pragma omp atomic
                        n++;
                    }
                }
                item = item->next;
            }
        }
    }
    return n;
}

/* More efficient code than the above */
int count_good(item_t *item) {
    int n = 0;
    int P = omp_get_max_threads(); // Get the number of threads
    int pn[P]; // Ensure P is correctly set

    #pragma omp parallel
    {
        pn[omp_get_thread_num()] = 0;
        #pragma omp single nowait
        {
            while (item) {
                #pragma omp task firstprivate(item)
                {
                    if (is_good(item)) {
                        pn[omp_get_thread_num()]++;
                    }
                }
                item = item->next;
            }
        }
        #pragma omp atomic
        n += pn[omp_get_thread_num()];
    }
    return n;
}

/* Slide 50 solution */
void task(double *x, double *y)
{
    *y = x[0]+x[1];
}

int main(int argc, char *argv[])
{
    double result[100];

    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            for(int i=0;i<100;i++)
            {
                #pragma omp task firstprivate(i) shared(result)
                {
                    double d[2];
                    d[0] = drand48();
                    d[1] = drand48();
                    task(d, &result[i]);
                }
            }

            #pragma omp taskwait
        }        
    }

    /* print results */
    return 0;
}

/* Slide 55 solution */
int a[20], s;
#pragma omp parallel num_threads(10)
{
    #pragma omp single nowait
    for(int i=0; i<20; i++)
    {
        #pragma omp task firstprivate(i)
        {
            a[i] = func();
        }

        #pragma omp taskwait
    }

    #pragma omp barrier
    
    #pragma omp single
        s = a[0];
    #pragma omp for reduction(+:s)
    for(int j=1; j<20; j++) s += a[j];
}

/* Slide 56 solution */
/* Given parallelization */
// A: matrix of size NxN

// nested parallelism is enable
#pragma omp parallel for
for(int i=0; i<N; i++)
{
    int chunksize = 10;
    #pragma omp parallel for schedule(dynamic,chunksize)
    for(int j=0; j<N; j++)
    {
        A[i][j] = func(i,j);
    }
}

/* Solution using tasks*/
// A: matrix of size NxN

// nested parallelism is enable
#pragma omp parallel
{
    #pragma omp for
    for(int i=0; i<N; i++)
    {
        #pragma omp single nowait
        {
            for(int j=0;j<N;j++)
            {
                #pragma omp task firstprivate(i,j)
                {
                    A[i][j] = func(i,j);
                }
            }
            #pragma omp taskwait
        }
    }
}

/* Slide 57 solution */
#pragma omp parallel
{
    #pragma omp for
    for(int i=0; i<N; i++)
    {
        #pragma omp single nowait
        {
            int ntasks = N/10;
            for(int t=0; t<ntasks; t++)
            {
                #pragma omp task shared(a, N) firstprivate(i, t)
                {
                    int j0 = t * 10;
                    int j1 = (t + 1) * 10;
                    if(j1 > N) j1 = N;

                    for(int j=j0; j<j1; j++)
                        a[i][j] = func(i, j);
                }
            }
            #pragma omp taskwait    
        }
    }
}

/* OPEN MP - I */
/* SLIDE 56 */
#define N 1024

#pragma omp parallel for schedule(static)
for(int i=0;i<N;i++)
{
    a[i] = a[i] + b[i];
}

/* SLIDE 58 */
#pragma omp parallel num_threads(3)
#pragma omp sections
{
    #pragma omp section
    V = alpha();
    #pragma omp section
    W = beta();
    #pragma omp section 
    Y = delta();
} 

X = gamma(V,W);
printf("%f\n", epsilon(X,Y));

/* SLIDE 61 */
long num_steps = 100000;
double step;

void main()
{
    double x, pi, sum = 0.0;
    step = 1.0/(double) num_steps;

    #pragma omp parallel for private(x) schedule(dynamic) reduction(+:sum)
    for(int i=0;i<num_steps; i++)
    {
        x = (i+0.5)*step;
        sum = sum + 4.0/(1.0+x*x);
    }
    pi = step*sum;
    printf("Pi is %lf\n", pi);
}

/* OPEN MP - II */
/* SLIDE 49 */
int A[N], B[N];
int auxdot = 0, dot = 0;

#pragma omp parallel
{
    int loc = 0;
    #pragma omp for
    for(int i=0; i<N; i++)
        loc += A[i]*B[i]

    #pragma omp critical
        dot += loc;
}