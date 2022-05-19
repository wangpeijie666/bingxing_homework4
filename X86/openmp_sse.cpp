#include<iostream>
#include<math.h>
#include<unistd.h>
#include<cstdlib>
#include<stdlib.h>
#include<xmmintrin.h>
#include<fstream>
#include<immintrin.h>
#include<pthread.h>
#include <semaphore.h>
#include <omp.h>
#include <sys/time.h>
#include<time.h>
#include <pmmintrin.h>
using namespace std;
int NUM_THREADS=5;
void openMPsse(float** matrix,int N)
//串行高斯消去
{
    #pragma omp parallel num_threads(NUM_THREADS)
    for (int k = 0; k < N; k++)
    {
        #pragma omp single
        {
            for (int j = k; j < N; j++)
            {
                matrix[k][j] = matrix[k][j] /matrix[k][k];
            }
        }
        #pragma omp for
        for (int i = k + 1; i < N; i++)
        {
            __m128 t1,t2,t3,t4;
            float tmpt[4]={matrix[i][k],matrix[i][k],matrix[i][k],matrix[i][k]};
            t1=_mm_loadu_ps(tmpt);
            int j;
            for (j = k + 1; j <=N-4; j+=4)
            {
                t2=_mm_loadu_ps(matrix[i]+j);
                t3=_mm_loadu_ps(matrix[k]+j);
                t4=_mm_sub_ps(t2,_mm_mul_ps(t1,t3));
                _mm_storeu_ps(matrix[i]+j,t4);
            }
             if(j<N)
            {
                for(;j<N;j++)
                {
                    matrix[i][j]=matrix[i][j]-tmpt[0]*matrix[k][j];
                }
            }
            matrix[i][k] = 0;
        }
    }
}


int main()
{
    int N[9] = {8,32,128,256,512,1024,2048,4096,6000};
    for(int p=0;p<9;p++)
    {
        long long head, tail, freq;
        //初始化矩阵并生成一个随机数矩阵
        float** matrix = new float* [N[p]];
        for (int i = 0; i < N[p]; i++)
        {
            matrix[i] = new float[N[p]];
        }
    srand((unsigned)time(0));
    for(int i=0;i<N[p];i++)
    {
        for(int j=0;j<i;j++)
        {
            matrix[i][j]=0.0;
        }
        matrix[i][i]=1.0;
        for(int j=i+1;j<N[p];j++)
        {
            matrix[i][j]=rand()%100;
        }
    }
        struct  timeval start;
        struct  timeval end;
        unsigned  long diff;
        gettimeofday(&start, NULL);
        for(int u=0;u<3;u++)
            openMPsse(matrix,N[p]);
        gettimeofday(&end, NULL);
        cout<<"thread="<<NUM_THREADS<<" ";
        cout<<"N="<<N[p]<<" ";
        diff = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        cout << diff/3000 << "ms" << endl;
    }
    //system("pause");
    return 0;

}
