#include<stdio.h>

// colnode[] is a list of nodes which are being filled for each color, 
// after checking that they can be part of the list.
// colorednodes[] is a list of already colored nodes.
// incidence[][] is the incidence matrix


int colorfunction(int N, int colnode[N], int colorednode[N], int incidence[N][N])
{
	int flag;// set to once if a connection is encountered.
	int i,j,v,w,len_colnode=0;

	for(i=0;i<N;i++){
		flag=0;
		if(colorednode[i]==0)
		{
			v=i;
			for(j=0;j<len_colnode;j++)
			{
				w=colnode[j];

				if(incidence[v][w]==1)
				{
					flag=1;//node not added to colnode list
					break;
				}
			}
			if(flag==0)
			{
				colorednode[v] +=1;//assigning color to the node.
				colnode[len_colnode]=v;//adding node to the respective color list
				len_colnode +=1;//increasing the length of the colnode list
			}
			
		}
	}

// part of code dealing with extras
// similar logic as above

	int flag_colored;

	for(i=0;i<N;i++)
	{
		flag_colored=0;
		if(colorednode[i]==2)
		{
			v=i;
			for(j=0;j<len_colnode;j++)
			{
				w=colnode[j];
				if(incidence[v][w]==1)
				{
					flag_colored=1;
					break;
				}
			}

			if((flag_colored==0) && (len_colnode>0))
			{
				colnode[len_colnode]=v;
				len_colnode+=1;
			}
		}
		if(colorednode[i]==1)
		{
			colorednode[i]+=1;
		}
	}


	return len_colnode;
}



int main()
{	
	int N;//number of nodes
	FILE *fpointer;
	fpointer=fopen("input1.dat","r");

	fscanf(fpointer,"%d",&N);

	int i,j,incidence[N][N], colnode[N], colorednode[N];
	char nodes[N][32];
	

	for(i=0;i<N;i++)
	{
		fscanf(fpointer,"%s",&nodes[i][0]);
	}

	for(i=0;i<N;i++)
	{
		colorednode[i]=0;
		for(j=0;j<N;j++)
		{
			fscanf(fpointer,"%d",&incidence[i][j]);
		}
	}

	printf("Input data:\n");
	printf("Number of nodes = %d.\n", N);
	for(i=0;i<N;i++)
	{
		printf("%s\n",nodes[i]);
	}
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			printf("%d ",incidence[i][j]);
		}
		printf("\n");
	}

	printf("Solution:\n");
	int len,pop=1;
	while((len=colorfunction(N,colnode,colorednode,incidence))!=0)
	{
		printf("Color %d includes the following nodes:",pop);
		for(i=0;i<len;i++)
		{	
			printf("%s ",nodes[colnode[i]]);
		}
		pop+=1;
		printf("\n");
	};


	fclose(fpointer);

	return 0;
}
