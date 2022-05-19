#include<iostream>
#include<math.h>
#include<cstdlib>
#include<stdlib.h>
#include<fstream>
#include<pthread.h>
#include <semaphore.h>
#include <omp.h>
#include <sys/time.h>
#include<time.h>
using namespace std;
int NUM_THREADS=7;
void openMP(float** matrix,int N)
//串行高斯消去
{
    #pragma omp parallel num_threads(NUM_THREADS)
    for (int k = 0; k < N; k++)
    {
        #pragma omp for
            for (int j = k; j < N; j++)
            {
                matrix[k][j] = matrix[k][j] /matrix[k][k];
            }
        #pragma omp for
        for (int i = k + 1; i < N; i++)
        {
            for (int j = k + 1; j < N; j++)
            {
                matrix[i][j] = matrix[i][j] - matrix[i][k] * matrix[k][j];
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
            openMP(matrix,N[p]);
        gettimeofday(&end, NULL);
        cout<<"thread="<<NUM_THREADS<<" ";
        cout<<"N="<<N[p]<<" ";
        diff = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        cout << diff/3000 << "ms" << endl;
    }
    //system("pause");
    return 0;

}

