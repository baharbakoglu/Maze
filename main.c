#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stack *lastvalue=NULL;
struct stack{
    char ch;
    char value;
    struct stack *back;
};

int control_loop(char *arr);/*checking return to the same place*/
void print_stack();/*print path in path.txt file*/
char find_stack_last_element();/*finding stack's last element*/
int find_key(int i);/*searching key in the stack*/
void find_path(int row, int column, int startrow, int startcolumn, char *arr);/*find exit path*/
void take_key(int row, int column, int startrow, int startcolumn, char *arr, int lastelement);/*find door's key in maze*/
void stack_push(char c,char value);/*push value in stack*/
void stack_pop();/*pop stacks last element*/

int main()
{
    FILE *mazefile;
    char buffer[50];
    int row, column, i=0, j, k=0;

    if ((mazefile = fopen( "maze.txt", "r" ) ) == NULL ){/*open maze file*/
        printf( "File could not be opened\n" );
        exit(-1);/*if file could not be opened close program*/
    }
    fscanf(mazefile, "%d %d", &row, &column);/*read maze's row and column number*/
    char mazearr[row][column];/*define maze*/
    char backuparr[row*column];
    fgets(buffer,sizeof(buffer),mazefile);/*read mazefile's first line*/
    while(fgets(buffer,sizeof(buffer),mazefile)!=NULL){/*read maze line*/
        for(j=0;buffer[j]!='\0';j++)
        {
             mazearr[i][j]=buffer[j];/*add values in maze array*/
        }
        i++;
    }
    for(i=0;i<row;i++)
    {
        for(j=0;j<column;j++){
            backuparr[k]=mazearr[i][j];
            k++;
        }
    }
    for(i=0;i<row;i++)
    {
        for(j=0;j<column;j++){
            if(mazearr[i][j]=='S'){/*find starting place*/
                break;
            }
        }
        if(mazearr[i][j]=='S'){
            break;
        }
    }
    find_path(row,column,i,j,backuparr);
    print_stack();/*print stack in path file*/
    fclose(mazefile);/*close maze file*/
    return 0;
}

int find_key(int i)/*control key in stack*/
{
    struct stack *temp;
    for(temp=lastvalue;temp!=NULL;temp=temp->back)
    {
        if(temp->value-'0'==i){
            return 1;}/*if the key is found, return 1 else 0*/
    }
    return 0;
}
void print_stack()/*print path*/
{
    FILE *pathfile;
    char charr[100], valuearr[100];
    int i, control=0;
    struct stack *temp;
    if ((pathfile = fopen( "path.txt", "w" ) ) == NULL ){/*open path file*/
        printf( "File could not be opened\n" );
        exit(-2);/*if file could not be opened exit this program*/
    }
    for(i=0;i<100;i++){charr[i]='\0'; valuearr[i]='\0';}
    i=0;
    for(temp=lastvalue;temp!=NULL; temp=temp->back){/*for checking to exit from maze*/
        charr[i]=temp->ch;
        valuearr[i]=temp->value;
        i++;
        if(temp->value=='E'){
            control=1;
        }
    }
    if(control==0){fprintf(pathfile,"MAZE NO SOLUTION!"); exit(-3);}
    for(i=99;i>=0;i--){
        if(charr[i]!='\0'){
            fprintf(pathfile,"GO %c\n",charr[i]);/*print the steps in path file*/
        }
        if((valuearr[i]-'0'>=17)&&(valuearr[i]-'0'<=42)&&(valuearr[i]!='E')&&(valuearr[i]!='S'))
        {
            fprintf(pathfile,"OPEN DOOR %c\n",valuearr[i]);/*when door opened, print this*/
        }
        if((valuearr[i]-'0'>=49)&&(valuearr[i]-'0'<=74))
        {
            fprintf(pathfile,"TAKE KEY %c\n",valuearr[i]);/*when take key, print this*/
        }
    }
    fclose(pathfile);/*close path file*/
}
int control_loop(char *arr)/*control there is a loop or not*/
{
    if(((arr[0]=='E')&&(arr[1]=='S')&&(arr[2]=='W')&&(arr[3]=='N'))||((arr[0]=='E')&&(arr[1]=='N')&&(arr[2]=='W')&&(arr[3]=='S'))){
        return -1;
    }
    else if(((arr[0]=='W')&&(arr[1]=='S')&&(arr[2]=='E')&&(arr[3]=='N'))||((arr[0]=='W')&&(arr[1]=='N')&&(arr[2]=='E')&&(arr[3]=='S'))){
        return -1;
    }
    else if(((arr[0]=='N')&&(arr[1]=='E')&&(arr[2]=='S')&&(arr[3]=='W'))||((arr[0]=='N')&&(arr[1]=='W')&&(arr[2]=='S')&&(arr[3]=='E'))){
        return -1;
    }
    else if(((arr[0]=='S')&&(arr[1]=='E')&&(arr[2]=='N')&&(arr[3]=='W'))||((arr[0]=='S')&&(arr[1]=='W')&&(arr[2]=='N')&&(arr[3]=='E'))){
        return -1;/*if this way have a loop return -1*/
    }
    else return 1;
}

void take_key(int row, int column, int startrow, int startcolumn, char *arr, int lastelement)/*find and take key*/
{
    char mazearr[row][column], array[50], stackarr[4];
    int i,j,k=0,stackint, finalrow=startrow, finalcolumn=startcolumn,controlint=0,controlvalue=0;
    for(i=0;i<row;i++)
    {
        for(j=0;j<column;j++)
        {
            mazearr[i][j]=arr[k];/*create maze array*/
            k++;
        }
    }
    for(k=0;k<50;k++){array[k]='\0';}
    i=0;
    k=0;
    while(controlvalue!=lastelement)/*until found the key*/
    {/*find path to key*/
        controlint=0;
        if(((k==0)||(find_stack_last_element()!='W'))&&((mazearr[finalrow][finalcolumn+1]-'0'>=49)||(mazearr[finalrow][finalcolumn+1]=='0'))&&(finalcolumn+1<column)){
            finalcolumn=finalcolumn+1;
            stack_pop();
            stack_push('E',mazearr[finalrow][finalcolumn]);
            controlvalue=mazearr[finalrow][finalcolumn]-'0';
        }
        else if(((k==0)||(find_stack_last_element()!='E'))&&((mazearr[finalrow][finalcolumn-1]-'0'>=49)||(mazearr[finalrow][finalcolumn-1]=='0'))&&(finalcolumn-1>=0)){
            finalcolumn=finalcolumn-1;
            stack_push('W',mazearr[finalrow][finalcolumn]);
            controlvalue=mazearr[finalrow][finalcolumn]-'0';
        }
        else if(((k==0)||(find_stack_last_element()!='N'))&&((mazearr[finalrow+1][finalcolumn]-'0'>=49)||(mazearr[finalrow+1][finalcolumn]=='0'))&&(finalrow+1<row)){
            finalrow=finalrow+1;
            stack_push('S',mazearr[finalrow][finalcolumn]);
            controlvalue=mazearr[finalrow][finalcolumn]-'0';
        }
        else if(((k==0)||(find_stack_last_element()!='S'))&&((mazearr[finalrow-1][finalcolumn]-'0'>=49)||(mazearr[finalrow-1][finalcolumn]=='0'))&&(finalrow-1>=0)){
            finalrow=finalrow-1;
            stack_push('N',mazearr[finalrow][finalcolumn]);
            controlvalue=mazearr[finalrow][finalcolumn]-'0';
        }
        else{controlint++;}
        if(controlint==0){array[i]=find_stack_last_element(); i++;}
        if(controlint==1){/*if there is no way delete last way*/
            array[i-1]='\0';
            mazearr[finalrow][finalcolumn]='1';
            if(find_stack_last_element()=='E'){
                finalcolumn=finalcolumn-1;
            }
            else if(find_stack_last_element()=='W'){
                finalcolumn=finalcolumn+1;
            }
            else if(find_stack_last_element()=='S'){
                finalrow=finalrow-1;
            }
            else if(find_stack_last_element()=='N'){
                finalrow=finalrow+1;
            }
            stack_pop();
            i--;
            controlint=0;
        }
        k++;
        struct stack *temp;
        stackint=0;
        for(temp=lastvalue;temp!=NULL;temp=temp->back)/*control stack last four element to have loop*/
        {
            if(stackint<4){
                stackarr[stackint]=temp->ch;
                stackint++;
            }
        }
        if(control_loop(stackarr)==-1){
            mazearr[finalrow][finalcolumn]='1';
            if(find_stack_last_element()=='E'){
                finalcolumn=finalcolumn-1;
            }
            else if(find_stack_last_element()=='W'){
                finalcolumn=finalcolumn+1;
            }
            else if(find_stack_last_element()=='S'){
                finalrow=finalrow-1;
            }
            else if(find_stack_last_element()=='N'){
                finalrow=finalrow+1;
            }
            stack_pop();
        }
    }
    for(k=49;k>=0;k--)/*return door*/
    {
        if(array[k]!='\0'){
            if(array[k]=='E'){finalcolumn=finalcolumn-1; stack_push('W',mazearr[finalrow][finalcolumn]);}
            else if(array[k]=='W'){finalcolumn=finalcolumn+1; stack_push('E',mazearr[finalrow][finalcolumn]);}
            else if(array[k]=='N'){finalrow=finalrow+1; stack_push('S',mazearr[finalrow][finalcolumn]);}
            else if(array[k]=='S'){finalrow=finalrow-1; stack_push('N',mazearr[finalrow][finalcolumn]);}
        }
    }
}

void stack_push(char c, char value)/*value push in stack*/
{
    struct stack *newch;
    newch=(struct stack *)malloc(sizeof(struct stack));
    newch->ch= c;
    newch->value=value;
    newch->back=lastvalue;
    lastvalue=newch;
}
void stack_pop()/*pop stack's last element*/ /*http://www.dailyfreecode.com/code/create-dynamic-stack-2423.aspx*/
{
    struct stack *temp=lastvalue, *temp1;
    if(temp==NULL)
    {
        printf("Stack is empty!\n");
    }
    else
    {
        temp1=lastvalue->back;
        lastvalue=temp1;
    }
}
char find_stack_last_element()/*find and return stack's last element*/
{
    if(lastvalue==NULL){
        return '\0';
    }
    return lastvalue->ch;
}

void find_path(int row, int column, int startrow, int startcolumn, char *arr)/*find way to go*/
{
    char mazearr[row][column], backuparr[row*column], stackarr[4];
    int i,j,k=0,stackint, finalrow=startrow, finalcolumn=startcolumn,controlint=0,controlvalue;
    for(i=0;i<row;i++)
    {
        for(j=0;j<column;j++)
        {
            mazearr[i][j]=arr[k];
            backuparr[k]=arr[k];
            k++;
        }
    }
    k=0;
    while(mazearr[finalrow][finalcolumn]!='E')/*while finding exit in maze*/
    {/*control and add new path*/
        if(finalrow==0){/*if the first line*/
            if(finalcolumn==0)
            {
                if((mazearr[finalrow][finalcolumn+1]!='1')&&(find_stack_last_element()!='W')){
                    finalcolumn=finalcolumn+1;
                    stack_push('E',mazearr[finalrow][finalcolumn]);}
                else if((mazearr[finalrow+1][finalcolumn]!='1')&&(find_stack_last_element()!='N')){
                    finalrow=finalrow+1;
                    stack_push('S',mazearr[finalrow][finalcolumn]);}
                else{controlint++;}
            }
            else if(finalcolumn==column-1)
            {
                if((mazearr[finalrow+1][finalcolumn]!='1')&&(find_stack_last_element()!='N')){
                    finalrow=finalrow+1;
                    stack_push('S',mazearr[finalrow][finalcolumn]);}
                else if((mazearr[finalrow][finalcolumn-1]!='1')&&(find_stack_last_element()!='E')){
                    finalcolumn=finalcolumn-1;
                    stack_push('W',mazearr[finalrow][finalcolumn]);}
                else{controlint++;}
            }
            else {
                if((mazearr[finalrow][finalcolumn+1]!='1')&&(find_stack_last_element()!='W')){
                    finalcolumn=finalcolumn+1;
                    stack_push('E',mazearr[finalrow][finalcolumn]);
                }
                else if((mazearr[finalrow+1][finalcolumn]!='1')&&(find_stack_last_element()!='N')){
                    finalrow=finalrow+1;
                    stack_push('S',mazearr[finalrow][finalcolumn]);
                }
                else if((mazearr[finalrow][finalcolumn-1]!='1')&&(find_stack_last_element()!='E')){
                    finalcolumn=finalcolumn-1;
                    stack_push('W',mazearr[finalrow][finalcolumn]);
                }
                else{controlint++;}
            }
        }
        else if(finalrow==row-1)/*if the last line*/
        {
            if(finalcolumn==0){

                if((mazearr[finalrow][finalcolumn+1]!='1')&&(find_stack_last_element()!='W')){
                        finalcolumn=finalcolumn+1;
                        stack_push('E',mazearr[finalrow][finalcolumn]);}
                else if((mazearr[finalrow-1][finalcolumn]!='1')&&(find_stack_last_element()!='S')){
                        finalrow=finalrow-1;
                        stack_push('N',mazearr[finalrow][finalcolumn]);}
                else{controlint++;}
            }
            else if(finalcolumn==column-1)
            {
                if((mazearr[finalrow][finalcolumn-1]!='1')&&(find_stack_last_element()!='E')){
                        finalcolumn=finalcolumn-1;
                        stack_push('W',mazearr[finalrow][finalcolumn]);}
                else if((mazearr[finalrow-1][finalcolumn]!='1')&&(find_stack_last_element()!='S')){
                        finalrow=finalrow-1;
                        stack_push('N',mazearr[finalrow][finalcolumn]);}
                else{controlint++;}
            }
            else {
                if((mazearr[finalrow][finalcolumn-1]!='1')&&(find_stack_last_element()!='E')){
                        finalcolumn=finalcolumn-1;
                        stack_push('W',mazearr[finalrow][finalcolumn]);}
                else if((mazearr[finalrow][finalcolumn+1]!='1')&&(find_stack_last_element()!='W')){
                        finalcolumn=finalcolumn+1;
                        stack_push('E',mazearr[finalrow][finalcolumn]);}
                else if((mazearr[finalrow-1][finalcolumn]!='1')&&(find_stack_last_element()!='S')){
                        finalrow=finalrow-1;
                        stack_push('N',mazearr[finalrow][finalcolumn]);}
                else{controlint++;}
            }
        }
        else if(finalcolumn==0)/*if the first column*/
        {
                if((mazearr[finalrow-1][finalcolumn]!='1')&&(find_stack_last_element()!='S')){
                    finalrow=finalrow-1;
                    stack_push('N',mazearr[finalrow][finalcolumn]);}
                else if((mazearr[finalrow+1][finalcolumn]!='1')&&(find_stack_last_element()!='N')){
                    finalrow=finalrow+1;
                    stack_push('S',mazearr[finalrow][finalcolumn]);}
                else if((mazearr[finalrow][finalcolumn+1]!='1')&&(find_stack_last_element()!='W')){
                    finalcolumn=finalcolumn+1;
                    stack_push('E',mazearr[finalrow][finalcolumn]);}
                else{controlint++;}
        }
        else if(finalcolumn==column-1)/*if the last column*/
        {
                if((mazearr[finalrow+1][finalcolumn]!='1')&&(find_stack_last_element()!='N')){
                    finalrow=finalrow+1;
                    stack_push('S',mazearr[finalrow][finalcolumn]);}
                else if((mazearr[finalrow-1][finalcolumn]!='1')&&(find_stack_last_element()!='S')){
                    finalrow=finalrow-1;
                    stack_push('N',mazearr[finalrow][finalcolumn]);}
                else if((mazearr[finalrow][finalcolumn-1]!='1')&&(find_stack_last_element()!='E')){
                    finalcolumn=finalcolumn-1;
                    stack_push('W',mazearr[finalrow][finalcolumn]);}
                else{controlint++;}
        }
        else {
                if((mazearr[finalrow-1][finalcolumn]!='1')&&(find_stack_last_element()!='S')){
                    finalrow=finalrow-1;
                    stack_push('N',mazearr[finalrow][finalcolumn]);}
                else if((mazearr[finalrow+1][finalcolumn]!='1')&&(find_stack_last_element()!='N')){
                    finalrow=finalrow+1;
                    stack_push('S',mazearr[finalrow][finalcolumn]);}
                else if((mazearr[finalrow][finalcolumn+1]!='1')&&(find_stack_last_element()!='W')){
                    finalcolumn=finalcolumn+1;
                    stack_push('E',mazearr[finalrow][finalcolumn]);}
                else if((mazearr[finalrow][finalcolumn-1]!='1')&&(find_stack_last_element()!='E')){
                    finalcolumn=finalcolumn-1;
                    stack_push('W',mazearr[finalrow][finalcolumn]);}
                else{controlint++;}
        }
        controlvalue=mazearr[finalrow][finalcolumn]-'0';
        if((mazearr[finalrow][finalcolumn]!='E')&&(mazearr[finalrow][finalcolumn]!='S')){
            if((controlvalue>=17)&&(controlvalue<=42)&&(find_key(controlvalue+32)!=1)){/*if finding door but there is no key*/
                stack_pop();
                if(find_stack_last_element()=='E'){
                    finalcolumn=finalcolumn-1;
                }
                else if(find_stack_last_element()=='W'){
                    finalcolumn=finalcolumn+1;
                }
                else if(find_stack_last_element()=='S'){
                    finalrow=finalrow-1;
                }
                else if(find_stack_last_element()=='N'){
                    finalrow=finalrow+1;
                }
                take_key(row, column, finalrow, finalcolumn, backuparr, controlvalue+32 );/*find and tae key*/
            }
        }
        if(controlint==1){/*if there is no way go back a step*/
            mazearr[finalrow][finalcolumn]='1';
            if(find_stack_last_element()=='E'){
                finalcolumn=finalcolumn-1;
            }
            else if(find_stack_last_element()=='W'){
                finalcolumn=finalcolumn+1;
            }
            else if(find_stack_last_element()=='S'){
                finalrow=finalrow-1;
            }
            else if(find_stack_last_element()=='N'){
                finalrow=finalrow+1;
            }
            stack_pop();
            controlint=0;
        }
        struct stack *temp;
        stackint=0;
        for(temp=lastvalue;temp!=NULL;temp=temp->back)/*to control loop*/
        {
            if(stackint<4){
                stackarr[stackint]=temp->ch;
                stackint++;
            }
        }
        k++;
        if(control_loop(stackarr)==-1){/*if ther eis a loop delete last step*/
            mazearr[finalrow][finalcolumn]='1';
            if(find_stack_last_element()=='E'){
                finalcolumn=finalcolumn-1;
            }
            else if(find_stack_last_element()=='W'){
                finalcolumn=finalcolumn+1;
            }
            else if(find_stack_last_element()=='S'){
                finalrow=finalrow-1;
            }
            else if(find_stack_last_element()=='N'){
                finalrow=finalrow+1;
            }
            stack_pop();
        }
        if((find_stack_last_element()=='\0')&&(k >10)){break;}/*if maze no solution*/
    }
}





