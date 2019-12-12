#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int front=-1,rear=-1;
int drop=0,accept=0;
int N_tot=0;//total number of packages
int t;//time
 
float perc_pack_drop=0.0f;
float avg_delay=0.0f;
int tot_delay_time=0.0d;
float time_per_pack=0.0d;

typedef struct{
int id; // packet id
int t0; // arrival time of packet
int priority; // higher means more important
char contents[6]; // contents of packet
} PACKET;


int isFull(int SIZE)
{
    if( (front == rear + 1) || (front == 0 && rear == SIZE-1)) return 1;
    return 0;
}
int isEmpty()
{
    if(front == -1) return 1;
    return 0;
}


void enQueue(PACKET* arr[],int SIZE,PACKET* element)
{
    if(isFull(SIZE)){ 
		printf("Queue is full!! Package dropped.\n");
		drop+=1;
	}
    else
    {
        if(front == -1) front = 0;

        rear = (rear + 1) % SIZE;
	arr[rear]=element;	

        printf("Package Inserted -> %s\n", arr[rear]->contents);
	accept+=1;
    }

}
int deQueue(PACKET* arr[],int SIZE)
{
    PACKET* element;
	float t_in_queue;
	element=arr[front];

    if(isEmpty())
	{
        printf("Queue is empty !! \n");
        return(-1);
    }
	else
	{
        
        if (front == rear)
		{
            front = -1;
            rear = -1;
        } /* Q has only one element, so we reset the queue after dequeing it. ? */
        else
		{
            front = (front + 1) % SIZE;
    	}
		//t_in_queue=(float)clock()/CLOCKS_PER_SEC-element->t0;
		t_in_queue=t-element->t0;
	    printf("Deleted element -> %s, Time in queue=%f\n", element->contents,t_in_queue);
		tot_delay_time=tot_delay_time+t_in_queue;
		return 1;
    }
}



int nextTime(float rateParameter)
{
	float x=RAND_MAX;
	float x1=x+1.0f;

return (int)(-logf(1.0f - (float) rand() / x1) / rateParameter);
}

int main()
{
    int N=20;
//    clock_t time_req_in,time_req_out;

    float lambda=0.3f,mu=0.5f;
    int nti=nextTime(lambda);
//    int nto=nextTime(mu);
	int nto=(int)(1/mu);
	PACKET arr[N];

/*test starts here

	for(int j=0;j<100;j++)
	{
		for(int i=0;i<100000000;i++)
		{};
		printf("%d\n",(int)clock()/CLOCKS_PER_SEC);
	};
test ends here
*/

/*code starts here
*/


    for(t=0;t<1000000;t++)
    {
        while(t==nti)//(int)(clock()/CLOCKS_PER_SEC)==nti)
        {
            //add next packet to queue
            PACKET *p;
            p = (PACKET *)malloc(sizeof(PACKET));

            strcpy(p->contents, "elemet");
	    	//p->t0=(float)clock()/CLOCKS_PER_SEC;
			p->t0=t;
            N_tot+=1;
            enQueue(arr,N,p);
            nti=nti+nextTime(lambda);
        }
        while(t==nto)//(int)clock()/CLOCKS_PER_SEC==nto)
        {
            deQueue(arr,N);//delete element in head
	    	nto=nto+(int)(1/mu);
        }
    }
/*
code ends here
*/

    time_per_pack=(float)t/accept;
	perc_pack_drop=(float)(drop)/(float)(drop+accept);
    avg_delay=tot_delay_time/accept;

    printf("accept=%d,drop=%d\nAvg. Delay=%f, Percentage of packets dropped=%f, Time per packet=%f.\n",accept,drop,avg_delay,perc_pack_drop,time_per_pack);

    return 0;
}
