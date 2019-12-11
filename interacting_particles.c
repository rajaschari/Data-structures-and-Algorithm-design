#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>

#define PI 3.14159265358979323846

//------getting binary form of flaot value---------------

static void print_ipart(long double x)
{
    int digit = (int)(modfl(x / 2, &x) * 2.0) + '0';
    if (x)
    {
        //print_ipart(x);
    }
    //putchar(digit);
}

char *bin(long double x)
{
    // Some TBD code
    // Handle NAN with isnan()
    // Handle infinity with isfinite()

    int temp = 1;
    static char str[11];

    str[0] = (signbit(x) ? '-' : '+');
    //putchar(signbit(x) ? '-' : '+');

    long double ipart;
    long double fpart = modfl(fabsl(x), &ipart);

    print_ipart(ipart);
    //putchar('.');
    while (fpart && temp <= 10)
    {
        long double ipart;
        fpart = modfl(fpart * 2, &ipart);
        str[temp] = ((int)ipart + '0');

        //putchar((int)ipart + '0');
        temp++;
    }
    //putchar('\n');
    return str;
}

//-------------------------------------------------------

//--------generating random signed float in [-1,1]-----------
//----first geenerate random float in [0,2], then subtract 1---

double random_float()
{
    //srand((unsigned int)time(NULL));

    return (double)rand() / (double)(RAND_MAX)*2.0 - 1.0;
}

struct temp_arr
{
    double x_coord, y_coord, vx, vy;
    struct temp_arr *link;
};

struct cell
{
    struct cell *one_zero_zero;
    struct cell *two_zero_one;
    struct cell *three_one_zero;
    struct cell *four_one_one;
    struct cell *parent;

    int level;
    char *str_x, *str_y;

    struct temp_arr *particle_list;
};

//----------------------bitString generation---------------------------

struct list_bitString
{
    char *str;
    struct list_bitString *link;
};

//--------------------modified bit string generation-------------------------

char *decimal_to_binary(int n, int digit)
{
    int c, d, t;
    char *p;

    t = 0;
    p = (char *)malloc(digit + 1);

    if (p == NULL)
        exit(EXIT_FAILURE);

    for (c = digit; c >= 0; c--)
    {
        d = n >> c;

        if (d & 1)
            *(p + t) = 1 + '0';
        else
            *(p + t) = 0 + '0';

        t++;
    }
    *(p + t) = '\0';

    return p;
}

generateAllBinaryStrings(int digit, struct list_binaryString *A) //digit=number of digits
{
    struct list_bitString *temp = (struct list_bitString *)malloc(sizeof(struct list_bitString));
    temp->str = decimal_to_binary(0, digit - 1);
    temp->link = NULL;

    A = temp;
    printf("%s\n", temp->str);

    for (int j = 1; j < (int)(pow(2, (double)digit)); j++)
    {
        temp->str = decimal_to_binary(j, digit - 1);
        temp->link = NULL;

        struct list_bitString *temp1 = (struct list_bitString *)malloc(sizeof(struct list_bitString));
        temp1 = A;

        while (temp1->link != NULL)
        {
            temp1 = temp1->link;
        }
        printf("%s\n", temp1->str);
        temp1->link = temp;
    }
}
//---------------------------------------------------------------------------

//-----------------function for inserting into a linked list-----------------

void insert_temparr(struct temp_arr *A, double x_coord, double y_coord, double vx, double vy) //struct temp_arr element)
{
    struct temp_arr *temp = (struct temp_arr *)malloc(sizeof(struct temp_arr));

    temp->x_coord = x_coord;
    temp->y_coord = y_coord;
    temp->vx = vx;
    temp->vy = vy;
    temp->link = NULL;

    struct temp_arr *temp1 = A; //pinter to head node

    while (temp1->link != NULL)
    {
        temp1 = temp1->link;
    }

    temp1->link = temp; //was a mistake here
};

//-------------------------------------------------------

//-------function for binning particles into cells--------

void bin_particles(struct temp_arr *root_temp_arr, struct cell *root)
{
    //struct temp_arr *temp = root_temp_arr;
    struct cell *temp_tree = root;

    struct temp_arr *temp_bin = (struct temp_arr *)malloc(sizeof(struct temp_arr));

    temp_bin = root_temp_arr;

    while (temp_bin != NULL)
    {
        printf("inside binning loop\n");
        temp_bin = temp_bin->link; //gives pointer to struct element

        char *x = bin(temp_bin->x_coord);
        char *y = bin(temp_bin->y_coord);

        printf("%s\n", x);
        printf("*x==%c and *y==%c\n", *x, *y);

        if (*x == '+' && *y == '+') //----x=(x+0)
        {
            temp_tree = temp_tree->one_zero_zero;
        }
        else if (*x == '+' && *y == '-')
        {
            temp_tree = temp_tree->two_zero_one;
        }
        else if (*x == '-' && *y == '+')
        {
            temp_tree = temp_tree->three_one_zero;
        }
        else if (*x == '-' && *y == '-')
        {
            printf("here 219\n");
            temp_tree = temp_tree->four_one_one;
            printf("here\n");
        }
        else
        {
            printf("there is something wrong 219\n");
        }
        printf("passed sign test level 0\n");

        for (int level = 1; level < 11; level++)
        {
            printf("inside level loop\n");
            if (*(x + level) == '0' && *(y + level) == '0') //----x=(x+0)
            {
                temp_tree = temp_tree->one_zero_zero;
            }
            else if (*(x + level) == '0' && *(y + level) == '1')
            {
                temp_tree = temp_tree->two_zero_one;
            }
            else if (*(x + level) == '1' && *(y + level) == '0')
            {
                temp_tree = temp_tree->three_one_zero;
            }
            else if (*(x + level) == '1' && *(y + level) == '1')
            {
                temp_tree = temp_tree->four_one_one;
            }
            else
            {
                printf("there is something wrong 242\n");
            }
            if (level == 0) //we have reached lowermost node--time to populate
            {
                struct temp_arr *root_arr_cell = (struct temp_arr *)malloc(sizeof(struct temp_arr));
                root_arr_cell = temp_tree->particle_list;

                while (root_arr_cell != NULL)
                {
                    root_arr_cell = root_arr_cell->link;
                }

                root_arr_cell->x_coord = temp_bin->x_coord;
                root_arr_cell->y_coord = temp_bin->y_coord;
                root_arr_cell->vx = temp_bin->vx;
                root_arr_cell->vy = temp_bin->vy;
            }
        }
    }

    printf("hello\n");
}

//--------------------------------------------------------

int main()
{
    int part_numb = 100;
    double x_coord, y_coord, vx, vy, theta;

    srand((unsigned int)time(NULL));
    //seeding for pseudo random number generator rand() with time

    //char *str = bin(random_float()); --> this call gives you the useful
    //printf("%s", str);               --- binary form with the sign(no decimal point)

    //-----------initialization---------------------------

    struct temp_arr *root_temp_arr = NULL; //What A is this

    x_coord = random_float();
    y_coord = random_float();

    struct temp_arr *temp = (struct temp_arr *)malloc(sizeof(struct temp_arr));

    temp->x_coord = x_coord;
    temp->y_coord = y_coord;
    temp->vx = 0.0;
    temp->vy = 0.0;
    temp->link = NULL;

    root_temp_arr = temp;

    for (int j = 0; j < (part_numb / 2) - 1; j++)
    {
        x_coord = random_float();
        y_coord = random_float();

        insert_temparr(root_temp_arr, x_coord, y_coord, 0.0, 0.0);
    }

    for (int j = 0; j < part_numb / 2; j++)
    {
        x_coord = random_float();
        y_coord = random_float();

        theta = (double)rand() / (double)(RAND_MAX)*2.0 * PI;

        vx = cos(theta);
        vy = sin(theta);

        insert_temparr(root_temp_arr, x_coord, y_coord, vx, vy);
    }

    //----------------------------------------------------

    //------------inititalizing tree----------------------

    struct cell *root = NULL;

    for (int level = 0; level <= 11; level++)
    {
        int arr[level];

        struct list_bitString *A = NULL;

        A = generateAllBinaryStrings(level, arr, 0, A); //returns root to linked list with the bitstrings

        struct list_bitString *temp_x = (struct list_bitString *)malloc(sizeof(struct list_bitString));
        temp_x = A;

        struct list_bitString *temp_y = (struct list_bitString *)malloc(sizeof(struct list_bitString));
        temp_y = A;

        if (A != NULL)
        {
            printf("here 346\n");
            while (temp_x->link != NULL) //gives element in list
            {
                printf("here 349\n");
                while (temp_y->link != NULL)
                {
                    printf("here 352\n");
                    struct cell *temp_tree = (struct cell *)malloc(sizeof(struct cell));
                    temp_tree = root;

                    char *s_x = temp_x->str;
                    char *s_y = temp_y->str;

                    for (int j = 0; j < level; j++)
                    {
                        printf("here\n");

                        if (*(s_x + j) == '0' && *(s_y + j) == '0') //
                        {
                            if (temp_tree->one_zero_zero == NULL) //create node
                            {
                                //make temporary node
                                struct cell *trash = (struct cell *)malloc(sizeof(struct cell));
                                trash->one_zero_zero = NULL;
                                trash->two_zero_one = NULL;
                                trash->three_one_zero = NULL;
                                trash->four_one_one = NULL;

                                trash->level = j;
                                trash->str_x = s_x;
                                trash->str_y = s_y;

                                trash->parent = temp_tree;

                                temp_tree->one_zero_zero = trash;
                            }
                            else //traverse the tree
                            {
                                temp_tree = temp_tree->one_zero_zero;
                            }
                        }
                        else if (*(s_x + j) == '0' && *(s_y + j) == '1')
                        {
                            if (temp_tree->two_zero_one == NULL) //create node
                            {
                                //make temporary node
                                struct cell *trash = (struct cell *)malloc(sizeof(struct cell));
                                trash->one_zero_zero = NULL;
                                trash->two_zero_one = NULL;
                                trash->three_one_zero = NULL;
                                trash->four_one_one = NULL;

                                trash->level = j;
                                trash->str_x = s_x;
                                trash->str_y = s_y;

                                trash->parent = temp_tree;

                                temp_tree->two_zero_one = trash;
                            }
                            else //traverse the tree
                            {
                                temp_tree = temp_tree->two_zero_one;
                            }
                        }
                        else if (*(s_x + j) == '1' && *(s_y + j) == '0')
                        {
                            if (temp_tree->three_one_zero == NULL) //create node
                            {
                                //make temporary node
                                struct cell *trash = (struct cell *)malloc(sizeof(struct cell));
                                trash->one_zero_zero = NULL;
                                trash->two_zero_one = NULL;
                                trash->three_one_zero = NULL;
                                trash->four_one_one = NULL;

                                trash->level = j;
                                trash->str_x = s_x;
                                trash->str_y = s_y;

                                trash->parent = temp_tree;

                                temp_tree->three_one_zero = trash;
                            }
                            else //traverse the tree
                            {
                                temp_tree = temp_tree->three_one_zero;
                            }
                        }
                        else if (*(s_x + j) == '1' && *(s_y + j) == '0')
                        {
                            if (temp_tree->four_one_one == NULL) //create node
                            {
                                //make temporary node
                                struct cell *trash = (struct cell *)malloc(sizeof(struct cell));
                                trash->one_zero_zero = NULL;
                                trash->two_zero_one = NULL;
                                trash->three_one_zero = NULL;
                                trash->four_one_one = NULL;

                                trash->level = j;
                                trash->str_x = s_x;
                                trash->str_y = s_y;

                                trash->parent = temp_tree;

                                temp_tree->four_one_one = trash;
                            }
                            else //traverse the tree
                            {
                                temp_tree = temp_tree->four_one_one;
                            }
                        }
                        else
                        {
                            printf("something is wrong, level not defined appropriately.\n");
                        }
                    }
                    temp_y = temp_y->link;
                }
                temp_x = temp_x->link;
            }
        }
    }

    //----------------------------------------------------

    // printf("473\n");
    // struct list_bitString *A = NULL;

    // generateAllBinaryStrings(4, A); //returns root to linked list with the bitstrings

    // struct list_bitString *temp1 = (struct list_bitString *)malloc(sizeof(struct list_bitString));
    // temp1 = A;

    // // while (temp1->link != NULL)
    // // {
    // //     printf("%s\n", temp1->str);

    // //     temp1 = temp1->link;
    // // }

    // for (int j = 0; j < (int)(pow(2.0, 4.0)); j++)
    // {
    //     printf("%s\n", temp1->str);
    //     temp1 = temp1->link;
    // }

    bin_particles(root_temp_arr, root);

    return 0;
}
