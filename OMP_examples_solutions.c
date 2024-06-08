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
    }

    #pragma omp taskwait
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
        #pragma omp single
        {
            for(int j=0;j<N;j++)
            {
                #pragma omp task firstprivate(i,j)
                {
                    A[i][j] = func(i,j);
                }
            }
        }
        #pragma omp taskwait
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
        }
        #pragma omp taskwait
    }
}