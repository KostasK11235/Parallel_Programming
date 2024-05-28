/* ParallelComputing_06.OpenMP3 */

/* Slide 55 solution */
int a[20], s;
#pragma omp parallel num_threads(10)
{
    #pragma omp single nowait
    for(int i=0; i<20; i++)
    {
        #pragma omp task
        {
            a[i] = func();
        }
    }

    #pragma omp taskwait
    #pragma omp barrier
    s = a[0];
    #pragma omp for firstprivate(s)
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
