#include <stdio.h>
#include <math.h>

int max(int a, int b) { return (a > b) ? a : b; }

// Returns the maximum value that can be put in a knapsack of capacity W

double knapsack(int W, int wt[], double val[], int n)

{

    //printf("inside knapsack\n");
    int i, j;
    double temp, temp_k, temp_omega;
    double w[n + 1][W + 1][3];

    // Build table w[][] in bottom up manner

    for (i = 0; i <= n; i++)

    {

        for (j = 0; j <= W; j++)

        {
            //double cnstr = (w[i - 1][j - wt[i - 1]][0] + wt[i - 1]) / sqrtf(w[i - 1][j - wt[i - 1]][1] + 1);

            //printf("Calculation for i=%d, j=%d\n\n", i, j);

            if (i == 0 || j == 0)
            {
                w[i][j][0] = 0.0;
                w[i][j][1] = 0.0;
                w[i][j][2] = 0.0;
            }
            else
            {
                temp = w[i - 1][j][2];
                temp_k = w[i - 1][j][1];
                temp_omega = w[i][j][0];

                for (int p = j; p >= 0; p--)
                {
                    //comparing new added wt plus any other additions from upper row

                    double cnstr = (w[i - 1][p][0] + wt[i - 1]) / sqrt(w[i - 1][p][1] + 1);
                    //printf("cnstr=%f, i=%d, p=%d\n", cnstr, i, p);
                    if (cnstr <= (double)j)
                    {
                        //printf("constraint satisfied\n");
                        //printf("old utility=%f; new utility=%f\n", temp, val[i - 1] + w[i - 1][p][2]);
                        if (temp > val[i - 1] + w[i - 1][p][2])
                        {
                            w[i][j][0] = temp_omega; //w[i - 1][j][0];
                            w[i][j][1] = temp_k;     //w[i - 1][j][1];
                            w[i][j][2] = temp;       //w[i - 1][j][2];
                        }
                        else //(temp<val[i - 1] + w[i - 1][p][2])
                        {
                            temp_omega = w[i - 1][p][0] + wt[i - 1];
                            temp_k = w[i - 1][p][1] + 1;
                            temp = w[i - 1][p][2] + val[i - 1];
                        }
                        //temp = max(temp, val[i - 1] + w[i - 1][p][2]);
                    }
                }

                w[i][j][0] = temp_omega; //w[i - 1][j][0];
                w[i][j][1] = temp_k;     //w[i - 1][j][1];
                w[i][j][2] = temp;       //w[i - 1][j][2];
            }

            //printf("utility for i=%d,j=%d is %f\n\n", i, j, w[i][j][2]);
        }
    }

    return w[n][W][2];
}

int main()

{

    //double val[] = {60, 100, 120};
    //int wt[] = {1, 2, 3};
    int n = 908;
    char str[60];
    int W = 10000;

    int wt[n];
    double val[n];

    FILE *fp = fopen("input1.txt", "r");
    int i, t = 0;

    while (!feof(fp))
    {
        fgets(str, 60, fp);
        if (str != "\n")
        {
            //printf("%d\n", atoi(str));
            wt[t] = atoi(str);
            t++;
            val[t] = logbf(atoi(str)) / logb(100);
            //printf("%d, ", atoi(str));
        }
    }
    fclose(fp);

    printf("\nValue = %f", knapsack(W, wt, val, n));

    return 0;
}