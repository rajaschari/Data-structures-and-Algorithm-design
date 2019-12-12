#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAXSTACK 10
#define MAXEXP 2*MAXSTACK
struct stack
{
    float arr[MAXSTACK];
    int ind_arr;
};

int number(char inp)
{
    switch(inp)
    {
        case '0': return 1; break;
        case '1': return 1; break;
        case '2': return 1; break;
        case '3': return 1; break;
        case '4': return 1; break;
        case '5': return 1; break;
        case '6': return 1; break;
        case '7': return 1; break;
        case '8': return 1; break;
        case '9': return 1; break;
        default : return 0; break;
    }
}

int pass_tag(char* inp){
    
    int cnt_dt=0;
    int cnt_e=0;
    int fnd_e=0;
    int fnd_num=0;
    int length=0;
    int sign=0;
    if(((inp[0]=='+') || (inp[0]=='*') || (inp[0]=='-') || (inp[0]=='/')) && (inp[1]=='\0' || inp[1]=='\n')) return 1;
    if((inp[0]=='+' || inp[0]=='-') && number(inp[1])) sign=1;
    if((inp[0]=='e' || inp[0]=='E')) return 0;
    for(int i=0;inp[i]!='\0';i++){
        if(cnt_dt==2) return 0;
        if(inp[i]=='.') cnt_dt++;
        if(sign!=1 && i>0){
            if(!((inp[i-1]=='e' || inp[i-1]=='E') && (inp[i]=='+' || inp[i]=='-') && number(inp[i+1]))){
                if((!number(inp[i])) && (inp[i]!='e' && inp[i]!='E') && (inp[i]!='.')) return 0;
            }
        }
        if(inp[i]=='e' || inp[i]=='E'){ 
            fnd_e=1;
            cnt_e++;
        }
        if(cnt_e==2) return 0;
        if((fnd_e==1) && (inp[i]=='.')) return 0;
        if(number(inp[i])) fnd_num=1;
        if(inp[i]=='.' && (inp[i+1]=='e' || inp[i+1]=='E') && fnd_num==0) return 0;
        length++;
    }
    if(inp[length-1]=='e' || inp[length-1]=='E') return 0;
    if(fnd_num==1) return length;
    else return 0;
}

int push(float item,struct stack *inp )
 {

	 if(inp->ind_arr > MAXSTACK)
	 {
		 printf("Stack overflow\n");
         exit(0);
	 }
	 else
	 {
		 inp->ind_arr = inp->ind_arr + 1 ;
		 inp->arr[inp->ind_arr]= item;
         return 1;
	 }
 }

 float pop(struct stack *inp)
 {
	 float item;
	 if(inp->ind_arr < 0)
	 {
		printf("Cannot pop Null stack\n");
        exit(0);
	 }
	 else
	 {
		 item = inp->arr[inp->ind_arr];
		 inp->ind_arr = inp->ind_arr - 1;
		 return item;
	 }
 }

int eval(char inp[],struct stack *inp1){
    float a,b;
    int len=0;
    int er=0;
    if(len = pass_tag(inp)) 
    {
        if(!number(inp[0]) && inp[0]!='e' && inp[0]!='E' && len==1)
        {
            if(inp1->ind_arr < 1) {
                printf("Input invalid \"%c\" does not have enough operands\n",inp[0]);
                er=1;
                return er;
            }
            else {
                a = pop(inp1);
                b = pop(inp1);
                switch(inp[0]) {
                    case '+': push(b+a, inp1); break;
                    case '-': push(b-a, inp1); break;
                    case '*': push(b*a, inp1); break;
                    case '/': push(b/a, inp1); break;
                    default : break;
                }
                return 0;
            }
        }
        else {
            if(!push(atof(inp), inp1)) {
                er=1;
                return er;
            }
            else {
                return 0;
            }
        }
    }
    else {
        printf("Input invalid Illegal tag : %s\n",inp);
        er=1;
        return er;
    }
}

int main(int argc, char* argv[]){
    float result;
    // char inp[20][20];
    clock_t start,end;
    struct stack inp1;
    int error=0,result=0;
    inp1.ind_arr = -1;
    FILE *fp;
    if(argc>1) fp=fopen(argv[1], "r");
    else fp=fopen("input.dat", "r");
    if(fp==NULL) printf("File does not exist\n");
    else 
    {
        char str[300];
        fgets(str,sizeof(str),fp);
        start = clock();

        for(int x=0;x<100000;x++)
        {   
            char inp2[10][20];
            int i=0,j=0;
            result=0;
            error=0;
            int y=0;
            do 
            {
                if(str[i]!= ' ' && str[i]!='\t')
			    {					
			    	inp2[y][j]=str[i];
			    	j++;
			    }
			    else if ((str[i]== ' ' || str[i]=='\t') && (str[i-1] == ' '|| str[i-1]=='\t')) {}
			        else
			        {   
                        inp2[y][j]='\0';
                        j=0;
                        error=eval(inp2[y],&inp1);
                        if(error) break;
                        y++;
                    }
                i++;
            }while(str[i]!='\n' && str[i]!='\0' && j<20 && y<10);

            inp2[y][j-1]='\0';
            if(str[i]=='\n') error=eval(inp2[y],&inp1);
            if(!error) 
            {
                result=pop(&inp1);
                if(inp1.ind_arr==-1) result=1;
                else {
                    result=0;
                    printf("Input invalid. Too many operands\n");
                    break;
                }
            }
            else break;
        }
        if(result) printf("resultult: %f\n",result);
        fclose(fp);
        double t_elapsed;
        end = clock();
        t_elapsed=((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Time: %lf\n",t_elapsed);
    }
    return 0;
}
