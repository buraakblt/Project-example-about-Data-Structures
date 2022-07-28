/*
DATA STRUCTURES THAT I USED IN THIS PROJECT:
Queue:	I used a queue to replicate a real life queue situtation, queue's as data structures has a principle called FIFO(First in first out)
		basically queue can store many type of data(int,char,structures etc.) and when returning it turns the or pops/deletes first item that added to the queue just like real life queues.
		
Stack:	I used a stack while storing the information that is going to get writen on the daily report because Stacks use a principle called LIFO(last in first out)/FILO(first in last out) 
		When a data comes to a stack its stored in the top of it and when you need to pop a data it will come out from the top too, this mechanicsm creates the FILO/LIFO pricinple.
		I specificly used stack to store daily report lines because it was stated in the midterm pdf that last served will be first writen in the report and first served will be writen last.
		
Array:	Arrays are used to store multiple data of the same type with a indexes to access them. Arrays are most basic data structure with no special usage  (I said no special usage but nearly every data structure
		uses array for its base so it is most basic and most important i  think). I used array for reading the input file and i used a array in creation of stack too.
		Name-Surname: Burak Bulut
		Student ID: 191180754
		Data Structures Midterm
         
		
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
#include<ctype.h>
#include <locale.h>

typedef struct queue
{
    int count;
    char *customer;
    struct queue *link;
}queue;
typedef struct Stack {
    int top;
    int capacity;
    char** array;
}Stack;
typedef struct customer
{
    struct queue* adultq;
    struct queue* elderq;
    struct queue* childq;

}customer;
queue *insert(queue *front,char *customer)
{
    queue *tmp,*q;
    tmp = (queue *)malloc(sizeof(queue));
    tmp->customer = customer;
    tmp->count =  tmp->count+1;
    /*Queue is empty or item to be added has priority more than first item*/
    if( front == NULL)
    {
        tmp->link = front;
        front = tmp;

    }
    else
    {	if(front->count<10){

    	front->count=front->count+1;
        q = front;
        while( q->link != NULL )
            q=q->link;
        tmp->link = q->link;
        q->link = tmp;
    }
    }
    return front;
}
void push(struct Stack* stack, char* item) {
    if (stack->top == stack->capacity ) {

        return;
    } else {
        stack->top++;
        stack->array[stack->top] = malloc(sizeof(char) * (strlen(item)+1));
        stack->array[stack->top] = item;

    }
}
void pop(struct Stack* stack,FILE* out)
{
    int val;
    if(stack->top == -1)
    {

        return ;
    }
    else
    {
		fprintf(out,"%s\n",stack->array[stack->top]);
        stack->top--;

    }
}
queue *del(queue *front,Stack *report,char* type)
{
    queue *tmp;
    if(front == NULL)
{		char* tmper=malloc(sizeof(int)*strlen(type));
		strcpy(tmper,type);
		strcat(tmper,"*****");
		push(report,tmper);
        return 0;
    }
    else
    {
  
        tmp = front;
        front = front->link;
        push(report,type);
      
    }
    return front;
}

bool stringIsOnlyWhitespace(const char * line) { // function to learn whether aline is empty or not.
    int i;
    for (i=0; line[i] != '\0'; ++i)
        if (!isspace(line[i]))
            return false;
    return true;
}

void enqueue(customer *customers,char* line){
    char* token;
    char* param1;
    char* param2;

    token=strtok(line,"\t");
    
    param1=strtok(NULL,"\t");
    param2=strtok(NULL,"\t");
    if(strcmp(param1,"E")==0)
    {
    	customers->elderq=insert(customers->elderq,param2);
	}
	else if(strcmp(param1,"A")==0)
	{
		customers->adultq=insert(customers->adultq,param2);
	
	}	
	else if(strcmp(param1,"C")==0){
		customers->childq=insert(customers->childq,param2);
	}
	
 

}


int main(){
	
	setlocale(LC_ALL, ""); 
	
	customer *customerqueue= malloc(sizeof(struct customer));
	customerqueue->adultq=NULL;
	customerqueue->childq=NULL;
	customerqueue->elderq=NULL;
	struct Stack* Report=malloc(sizeof(struct Stack));
	Report->array=malloc(sizeof(char*)*100);
	Report->capacity=100;
	FILE *fp;
    FILE *fout;
    fp = fopen("input.txt", "r"); //opening the command file.
    // an algoritm for learning size information about the command file later will be used for dynamic memory allocation
    char ch;
    fout=fopen("output.txt","w");
    int line = 0, word = 1, maxchar = 0, icount = 0, maxword = 0;
    int last = '\n';
    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    while ((ch = fgetc(fp)) != EOF) {
        ++icount;
        if (ch == ' ') {
            word++;
            if (word > maxword) {
                maxword = word;
            }

        }
        if ((ch == '\n' || ch == '\r') && last != '\n') {
            word = 1;
            ++line;
            --icount;
            if (icount > maxchar) {
                maxchar = icount;
            }
            icount = 0;
        }
        last = ch;
    }
    // creating struct array dynamically to the command number

    // creating char* and char** to hold the information in the text file later will be used to call functions
    char *commandtemp = ((char *) malloc(sizeof(char*) * 1000));


    char *command;

    char **arr;

    arr = (char **) malloc(sizeof(char *) * line);
    int a;
    for (a = 0; a < line; a++) {
        arr[a]=malloc(sizeof(char)*maxchar);
    }
    int i = 0;
    rewind(fp);


    while (fgets(commandtemp, fileSize, fp) != NULL) {
        // not including empty lines from text;
        if (!(stringIsOnlyWhitespace(commandtemp))) {
            command = strtok(commandtemp, "\n,\r");

            arr[i] = strdup(command);

            i++;
        } else {

            continue;
        }
    }


    char *token;
    char *temp = ((char *) malloc(sizeof(char) * maxchar));
    int loop;
	int count =0;
	int k;
    for(loop=0;loop<i;loop++){
        strcpy(temp, arr[loop]);
        token = strtok(temp,"\t");
        while (token != NULL) {
       
            if(strcmp(token,"NewCustomer")==0){
            
              	enqueue(customerqueue,arr[loop]);

			}
			else if(strcmp(token,"ServeCustomers")==0){

				token=strtok(NULL,"\t");
				if(strcmp(token,"E")==0){
					token=strtok(NULL,"\t");

					for(k=0;k<atoi(token);k++){
						
						if(customerqueue->elderq!=NULL){
										char* tmp=malloc(1000);
						strcpy(tmp,"E\t");
						strcat(tmp,customerqueue->elderq->customer)	;	
						customerqueue->elderq=del(customerqueue->elderq,Report,tmp);
							}
						else{
							push(Report,"E\t*****");
						}
					}
				}
				else if(strcmp(token,"A")==0){
					token=strtok(NULL,"\t");
					for(k=0;k<atoi(token);k++){
					
						if(customerqueue->adultq!=NULL){
						char* tmp=malloc(1000);
						strcpy(tmp,"A\t");
						strcat(tmp,customerqueue->adultq->customer)	;	
						customerqueue->adultq=del(customerqueue->adultq,Report,tmp);
							}
						else{
							push(Report,"A\t*****");
						}
					}
				}
				else if(strcmp(token,"C")==0){
					token=strtok(NULL,"\t");
					for(k=0;k<atoi(token);k++){
							if(customerqueue->childq!=NULL){
						char* tmp=malloc(1000);
						strcpy(tmp,"C\t");
						strcat(tmp,customerqueue->childq->customer)	;	
						customerqueue->childq=del(customerqueue->childq,Report,tmp);
							}
							else{
						
							push(Report,"C\t*****");}
					}
				}	
			
			}
			   	token=strtok(NULL,"\t");        
		}
		
    }
    
	while(Report->top!=0){
		pop(Report,fout);
	}
	return 0;
}

