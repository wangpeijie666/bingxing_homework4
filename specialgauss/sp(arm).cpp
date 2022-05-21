#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <sys/time.h>
#include <omp.h>
using namespace std;
int temp;
int numFlag;
int z;
int count_eliminated=0;
const int NUM_THREADS=4;
int numMaxCol,numEline,numElitor;
class BitSet;
BitSet *eline,*eliminator;
class BitSet
{
public:
    int bitNum;
    int *bitVector;
    int lp;
public:
    BitSet()
    {
        bitNum=0;
        lp=-1;
        bitVector=NULL;
    }
    bool isNULL()
    {

        if(this->lp==-1)
            return true;
        else
        {
            return false;
        }

    }
    void init(int n)
    {
        bitNum=n;
        bitVector=new int[bitNum];
        for(int i=0;i<bitNum;i++)
        {
            bitVector[i]=0;
        }
    }
    bool insertData(int index)
    {
        if(index>=0&&index<bitNum)
        {
			bitVector[index]=1;
			if(index>lp)
				lp=index;
			return true;
        }
        return false;
    }
    void showData()
    {
        for (int i=0;i<bitNum;i++)
        {
            if (bitVector[i])
            {
                cout<<i<<" ";
            }
        }
        cout<<endl;
    }
    void resetLp()
    {
        int tempLp=-1;
        for (int i=0;i<bitNum;i++)
        {
            if (bitVector[i])
            {
               tempLp=i;
            }
        }
        this->lp=tempLp;
    }
	bool xorBitSet(BitSet b)
    {
        if(this->bitNum!=b.bitNum)
            return false;
        for(int z=0;z<bitNum;z++)
        {
            this->bitVector[z]=this->bitVector[z]^b.bitVector[z];
        }
        return true;

    }
    int getLp()
    {
        return lp;
    }
    void equal(BitSet b)
    {
        this->bitNum=b.bitNum;
        this->lp=b.lp;
        for(int z=0;z<bitNum;z++)
        {
            this->bitVector[z]=b.bitVector[z];
        }

    }
};
void getNum()
{
	numMaxCol=8399;
	numElitor=6375;
	numEline=4535;

}
void initData()
{

    int data,linecount=0,elcount=0;
    int lpcount=0;
    eline=new BitSet[numEline];
    eliminator=new BitSet[numMaxCol];
    for(int i=0;i<numEline;i++)
        eline[i].init(numMaxCol);
    for(int i=0;i<numMaxCol;i++)
        eliminator[i].init(numMaxCol);
    ifstream elineInput;
    string elineStr;
    elineInput.open("l7.txt");
    while(getline(elineInput,elineStr))
    {
        istringstream iss(elineStr);
        while(iss>>data)
        {
            if(data>=0&data<=numMaxCol)
	  eline[linecount].insertData(data);

        }
        linecount++;
    }
    elineInput.close();
    ifstream eliminatorInput;
    string eliminatorStr;
    eliminatorInput.open("e7.txt");
    while(getline(eliminatorInput,eliminatorStr))
    {
        istringstream iss(eliminatorStr);
        while(iss>>data)
        {
			if(data>=0&data<=numMaxCol)
			{
				if(lpcount==0)
				{
					elcount=data;
				}
				eliminator[elcount].insertData(data);
				lpcount++;
			}
        }
        lpcount=0;
    }
    eliminatorInput.close();


}
void displayData(BitSet *b,int n)
{
    cout<<"Result:"<<endl;
    if((n<0)|(n>numMaxCol)|(b==NULL))
        return;
    else
    {
        for(int i=0;i<n;i++)
            b[i].showData();
    }
}
void omp_Gauss()
{
	#pragma omp parallel num_threads(NUM_THREADS) 
	while(count_eliminated<numEline)
    {
        #pragma omp for
        for(int i=count_eliminated+1;i<numEline;i++)
        {
            while(!eline[i].isNULL())
            {
                bool flag=eliminator[eline[i].getLp()].isNULL();
                if(!flag)
                {
                    eline[i].xorBitSet(eliminator[eline[i].getLp()]);
                    eline[i].resetLp();
                }
                else
                {
                    break;
                }
            }
        }
        #pragma omp single
        {
            while(!eline[count_eliminated].isNULL())
            {
                bool flag=eliminator[eline[count_eliminated].getLp()].isNULL();
                if(!flag)
                {
                    eline[count_eliminated].xorBitSet(eliminator[eline[count_eliminated].getLp()]);
                    eline[count_eliminated].resetLp();
                }
                else
                {
                    eliminator[eline[count_eliminated].getLp()].equal(eline[count_eliminated]);
                    break;
                }
            }
            ++count_eliminated;
        }
    }
}
int main()
{

    //cin>>numFlag;
    struct timeval t1,t2;
    double time=0.0;
    getNum();
    initData();
    gettimeofday(&t1,NULL);
    omp_Gauss();
    gettimeofday(&t2,NULL);
    time= (double)(t2.tv_sec-t1.tv_sec)*1000.0 + (double)(t2.tv_usec-t1.tv_usec)/1000.0;
    cout<<"time:"<<time<<endl;
    //displayData(eline,numEline);

    return 0;


}
