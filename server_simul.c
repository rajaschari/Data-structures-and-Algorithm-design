#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int front_dp = -1, rear_dp = -1;
int front_vod = -1, rear_vod = -1;
int front_tmp = -1, rear_tmp = -1;

//drop if the packets are retransmitted;
//accept when the packets are dispatched(even if retransmitted)
int drop_vod = 0, sent_vod = 0;
int drop_dp = 0, sent_dp = 0;
int drop_vod_step = 0, sent_vod_step = 0;
int drop_dp_step = 0, sent_dp_step = 0; //every time step
int N_tot = 0;                          //total number of packages
int t;                                  //time

float perc_pack_drop = 0.0f;
float avg_delay = 0.0f;
int tot_delay_time = 0;
float time_per_pack = 0.0;

typedef struct
{
    int id;       // packet id
    float t0;     // arrival time of packet
    int priority; // higher means more important
    double size;
    char contents[4];    // contents of packet
    double retrans_time; //time accumulated due to retransmission
    double trans_time;   //time accumulated due to trans. packets ahead in queue
} PACKET;

int isFull(int SIZE, int tag)
{
    if (tag == 0)
    {
        if ((front_dp == rear_dp + 1) || (front_dp == 0 && rear_dp == SIZE - 1))
            return 1;
        return 0;
    }
    else if (tag == 1)
    {
        if ((front_vod == rear_vod + 1) || (front_vod == 0 && rear_vod == SIZE - 1))
            return 1;
        return 0;
    }
    else if (tag == -1)
    {
        if ((front_tmp == rear_tmp + 1) || (front_tmp == 0 && rear_tmp == SIZE - 1))
            return 1;
        return 0;
    }
}
int isEmpty(int tag)
{
    if (tag == 0)
    {
        if (front_dp == -1)
            return 1;
        return 0;
    }
    else if (tag == 1)
    {
        if (front_vod == -1)
            return 1;
        return 0;
    }
}

// ------- tag=0 --> data packet ---- tag=1 --> video packet ---------

void enQueue(PACKET *arr[], int SIZE, PACKET *element, int tag)
{
    if (tag == 0)
    {
        if (isFull(SIZE, tag))
        {
            printf("data Queue is full!! Package dropped.\n");
            //drop += 1;
        }
        else
        {
            if (front_dp == -1)
                front_dp = 0;

            rear_dp = (rear_dp + 1) % SIZE;
            arr[rear_dp] = element;

            //printf("Package Inserted -> %s\n", arr[rear_dp]->contents);
            //accept += 1;
        }
    }
    else if (tag == 1)
    {
        if (isFull(SIZE, tag))
        {
            printf("vod Queue is full!! Package dropped.\n");
            //drop += 1;
        }
        else
        {
            if (front_vod == -1)
                front_vod = 0;

            rear_vod = (rear_vod + 1) % SIZE;
            arr[rear_vod] = element;

            //printf("Package Inserted -> %s\n", arr[rear_vod]->contents);
            //accept += 1;
        }
    }
    else if (tag == -1)
    {
        if (isFull(SIZE, tag))
        {
            printf("Queue is full!! Package dropped.\n");
            //drop += 1;
        }
        else
        {
            if (front_tmp == -1)
                front_tmp = 0;

            rear_tmp = (rear_tmp + 1) % SIZE;
            arr[rear_tmp] = element;

            //printf("Package Inserted -> %s\n", arr[rear_vod]->contents);
            //accept += 1;
        }
    }
}
float deQueue(PACKET *arr[], int SIZE, float t, int tag)
{
    PACKET *element;
    float t_in_queue;

    if (tag == 0)
    {
        element = arr[front_dp];
        if (isEmpty(tag))
        {
            printf("Queue is empty !! \n");
            return (0.0);
        }
        else
        {

            if (front_dp == rear_dp)
            {
                front_dp = -1;
                rear_dp = -1;
            } /* Q has only one element, so we reset the queue after dequeing it. ? */
            else
            {
                front_dp = (front_dp + 1) % SIZE;
            }
            t_in_queue = t - element->t0;

            //printf("Deleted element -> %s, Time in queue=%f\n", element->contents, t_in_queue);
            tot_delay_time = tot_delay_time + t_in_queue;
            return element->size;
        }
    }
    else if (tag == 1)
    {
        element = arr[front_vod];
        if (isEmpty(tag))
        {
            printf("Queue is empty !! \n");
            return (0.0);
        }
        else
        {
            if (front_vod == rear_vod)
            {
                front_vod = -1;
                rear_vod = -1;
            } /* Q has only one element, so we reset the queue after dequeing it. ? */
            else
            {
                front_vod = (front_vod + 1) % SIZE;
            }
            t_in_queue = t - element->t0;
            //printf("Deleted element -> %s, Time in queue=%f\n", element->contents, t_in_queue);
            tot_delay_time = tot_delay_time + t_in_queue;
            return element->size;
        }
    }
    else if (tag == -1)
    {
        element = arr[front_tmp];
        if (isEmpty(tag))
        {
            //printf("Queue is empty !! \n");
            return (0.0);
        }
        else
        {
            if (front_tmp == rear_tmp)
            {
                front_tmp = -1;
                rear_tmp = -1;
            } /* Q has only one element, so we reset the queue after dequeing it. ? */
            else
            {
                front_tmp = (front_tmp + 1) % SIZE;
            }
            t_in_queue = t - element->t0;
            //printf("Deleted element -> %s, Time in queue=%f\n", element->contents, t_in_queue);
            tot_delay_time = tot_delay_time + t_in_queue;
            return element->size;
        }
    }
}

int check_delay(PACKET *p, float t, int tag)
{
    if (tag == 0 && p->id == 0 && (t - p->t0 >= 30.0))
    {
        drop_dp_step += 1;
        return 1;
    }
    else if (tag == 1 && p->id == 1 && (t - (p->t0) >= 1.0)) //condtion to check for dropping VonD
    {
        drop_vod_step += 1;
        return 1;
    }
    else
    {
        return 0;
    }
}
//----------------------------------middle_delete----------------------------------------

void delay_delete(PACKET *arr[], int SIZE, float t, int tag, PACKET *arr_tmp[])
{
    //printf("-----delay_delete------\n");
    PACKET *pop = (PACKET *)malloc(sizeof(PACKET));

    if (isEmpty(tag))
    {
        //printf("Queue is empty !! \n");
    }
    else if (tag == 0)
    {
        int front1 = front_dp;
        int rear1 = rear_dp;
        //for (int tmp = front1; tmp < rear1; tmp++)
        while (front1 != rear1)
        {
            int tmp = front1;
            //printf("check_delay=%d\n", check_delay(arr[tmp], t));
            if (check_delay(arr[tmp], t, tag) == 1)
            {
                strcpy(pop->contents, arr[tmp]->contents);
                pop->id = arr[tmp]->id;
                pop->size = arr[tmp]->size;
                pop->retrans_time = t - arr[tmp]->t0;
                //pop->t0 = t - (t - arr[tmp]->t0); //which is just arr[tmp]->t0
                pop->t0 = t;

                //printf("%s at index %d is being retransmitted due to delay\n", pop->contents, tmp);

                //now, we have to shift each element betn rear and tmp one step
                for (int x = tmp + 1; x <= rear_dp; x++)
                { //save info in pop1    ...[]-[info]...-->pop
                    //[left]-->[right],  [pop]--> []-[rear]-[]-[]...
                    //move info from left to right element

                    strcpy(arr[(x - 1) % SIZE]->contents, arr[x]->contents);
                    arr[(x - 1) % SIZE]->id = arr[x]->id;
                    arr[(x - 1) % SIZE]->retrans_time = arr[x]->retrans_time;
                    arr[(x - 1) % SIZE]->size = arr[x]->size;
                    arr[(x - 1) % SIZE]->t0 = arr[x]->t0;
                }

                enQueue(arr_tmp, SIZE, pop, -1);

                front1 = tmp;
                rear1 -= 1;
            }
            else
            {
                front1 += 1;
            }
        }
    }
    else if (tag == 1)
    {
        int front1 = front_vod;
        int rear1 = rear_vod;
        //for (int tmp = front1; tmp < rear1; tmp++)
        while (front1 != rear1)
        {
            int tmp = front1;
            //printf("check_delay=%d\n", check_delay(arr[tmp], t));
            if (check_delay(arr[tmp], t, tag) == 1)
            {
                strcpy(pop->contents, arr[tmp]->contents);
                pop->id = arr[tmp]->id;
                pop->size = arr[tmp]->size;
                pop->retrans_time = t - arr[tmp]->t0;
                //pop->t0 = t - (t - arr[tmp]->t0); //which is just arr[tmp]->t0
                pop->t0 = t;

                //printf("%s at index %d is being retransmitted due to delay\n", pop->contents, tmp);

                //now, we have to shift each element betn rear and tmp one step
                for (int x = tmp + 1; x <= rear_dp; x++)
                { //save info in pop1    ...[]-[info]...-->pop
                    //[left]-->[right],  [pop]--> []-[rear]-[]-[]...
                    //move info from left to right element

                    strcpy(arr[(x - 1) % SIZE]->contents, arr[x]->contents);
                    arr[(x - 1) % SIZE]->id = arr[x]->id;
                    arr[(x - 1) % SIZE]->retrans_time = arr[x]->retrans_time;
                    arr[(x - 1) % SIZE]->size = arr[x]->size;
                    arr[(x - 1) % SIZE]->t0 = arr[x]->t0;
                }

                enQueue(arr_tmp, SIZE, pop, -1);

                front1 = tmp;
                rear1 -= 1;
            }
            else
            {
                front1 += 1;
            }
        }
    }
}

int main()
{

    double t;
    double t_max = 120.0;
    int N = 102400;
    PACKET *arr_dp[N];
    PACKET *arr_vod[N];
    PACKET *arr_tmp[N];

    double time_step = 1 / (128.0 * 3.0);

    for (int i = 0; i < 512; i++)
    {
        PACKET *p = (PACKET *)malloc(sizeof(PACKET));

        strcpy(p->contents, "data");
        p->id = 0;
        p->t0 = 0;
        p->trans_time = 0.0;
        p->retrans_time = 0.0;
        p->size = 1;
        enQueue(arr_dp, N, p, 0);
    }
    printf("Inserted first burst of data.\n");

    //-------------- Magic Number = 1/128*3-------1/96=4*(1/128*3)-----1/64=6*(1/128*3)-----

    int count;
    double tmp_size = 0.0;
    double wait_time = 0.0;
    double tmp_time = 0.0;

    for (count = 0; count < (128 * 3 * (int)t_max); count++)
    {
        t = count * time_step;

        //printf("time=%f\n", t);

        // while adding packets are added simultaneously due to high(infinite) bandwidth
        if (count != 0 && count % (64 * 3) == 0 && count % (128 * 3) != 0) //every half-sec add 6 DP
        {
            for (int i = 0; i < 6; i++) //addition of 6 units of dp
            {
                PACKET *p = (PACKET *)malloc(sizeof(PACKET));

                strcpy(p->contents, "data");
                p->id = 0;
                p->t0 = t;
                p->trans_time = 0.0;
                p->retrans_time = 0.0;
                p->size = 1.0;
                enQueue(arr_dp, N, p, 0);
            }

            for (int i = front_tmp; i <= rear_tmp; i++)
            {
                PACKET *p = (PACKET *)malloc(sizeof(PACKET));

                p = arr_tmp[i];

                if (p->id == 0)
                {
                    enQueue(arr_dp, N, p, 0);
                }
                else if (p->id == 1)
                {
                    enQueue(arr_vod, N, p, 1);
                }

                deQueue(arr_tmp, N, t, -1);
            }
            //printf("Inserted 6 data packets.\n");
        }

        //printf("front_tmp=%d\n", front_tmp);

        //--------------------------transmission---------------------------------------

        if (t - tmp_time >= wait_time) // && (isEmpty() == 0))
        {
            //tmp_size = arr[front]->size;
            tmp_time = t;

            if (isEmpty(1) == 0)
            {
                for (int tmp = front_vod; tmp <= rear_vod; tmp++) //all elements in queue add trans. time
                {
                    arr_vod[tmp]->trans_time += (1.0 / 128.0);
                }
                sent_vod_step += 1;
                wait_time = (1.0 / 128.0);
                deQueue(arr_vod, N, t, 1);
            }
            else if (isEmpty(0) == 0)
            {
                for (int tmp = front_dp; tmp <= rear_dp; tmp++) //all elements in queue add trans. time
                {
                    arr_dp[tmp]->trans_time += (1.0 / 64.0);
                }
                sent_dp_step += 1;
                wait_time = (1.0 / 64.0);

                deQueue(arr_dp, N, t, 0);
            }
            else
            {
                //printf("Trying to delete when both queues are empty.\n");
            }

            //always use deQueue at the end ---> because it changes front and rear
            // on call
        }
        //-----------------------------------------------------------------------------

        if (count % (128 * 3) == 0) //print results every second
        {
            printf("time= %f , HTTP = %d; Video = %d; HTTP Drops = %d; Video Drops = %d.\n",
                   t, sent_dp_step, sent_vod_step, drop_dp_step, drop_vod_step);

            drop_dp += drop_dp_step;
            drop_vod += drop_vod_step;
            sent_dp += sent_dp_step;
            sent_vod += sent_vod_step;
            //reinitialize step variables
            sent_dp_step = 0;
            sent_vod_step = 0;
            drop_dp_step = 0;
            drop_vod_step = 0;
        }

        if (count % 4 == 0) //time is a multiple of 1/96 add VonD
        {
            PACKET *p = (PACKET *)malloc(sizeof(PACKET));

            strcpy(p->contents, "VonD");
            p->id = 1;
            p->t0 = t;
            p->trans_time = 0.0;
            p->retrans_time = 0.0;
            p->size = 0.5;
            enQueue(arr_vod, N, p, 1);
            //printf("VonD packet added at count=%d.\n", count);
        }

        delay_delete(arr_vod, N, t, 1, arr_tmp);
        delay_delete(arr_dp, N, t, 0, arr_tmp);
    }

    printf("percentage drop for dp=%f, percentage drop for vod=%f\n",
           (float)drop_dp / (drop_dp + sent_dp), (float)drop_vod / (drop_vod + sent_vod));

    return 0;
}