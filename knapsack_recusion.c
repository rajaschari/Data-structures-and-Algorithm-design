#include<stdio.h>
#include<stdlib.h>

int count=0;

int knapsack(int W, int i, int w[], int N)
{
	//count=*(val+1);
	count+=1;

	if(W==0)
	{
		return 1;
	}
	else if(W<0 || i>=N)
	{
		return 0;
	}
	else if(knapsack(W-w[i],i+1,w,N)==1)
	{
		//printf("%d \n",w[i]);
		return 1;
	}
	else
	{
		return knapsack(W,i+1,w,N);
	}
}


int rand_new(int min,int max)
{
	int range = max - min + 1;

	// Largest value that when multiplied by "range"
	// is less than or equal to RAND_MAX
	int chunkSize = (RAND_MAX) / range+ 1/ range; 
	int endOfLastChunk = chunkSize * range;

	int r = rand();
	while(r >= endOfLastChunk){
    	r = rand();
	}
	return min + r / chunkSize;
}


int main()
{	
	int N,W;

	for(int i=0;i<10000;i++)
	{
		count=0;

		N=rand_new(1,20);
//		printf("%d\n",N*N);
		if((N*N)%2==0)
		{
			W=rand_new(0,N*N/2);
		}
		else{
			W=rand_new(0,(N*N+1)/2);
		}

		int w[N];
	
		for(int i=0;i<N;i++)
		{
			w[i]=rand_new(0,N);
		}

		knapsack(W,0,w,N);
		
		printf("%d %d\n",N,count);

	}


	return 0;
}
