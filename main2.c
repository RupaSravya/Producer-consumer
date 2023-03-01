#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>
#include<sys/wait.h>
#include<stdlib.h>
#define BUFFER 5 //MAXIMUM SIZE OF THE BUFFER 5

int empty, full, mutex;

typedef struct node{
	int data;
	struct node *next;
}node; //USER DEFINED DATATYPE (LINKED LIST)

node *head = NULL, *tail = NULL;
int empty, full;

int initialize(){
	empty = BUFFER;
	full = 0;	
	mutex = 0;
}

void printNode(){
	node *ptr = head;
	printf("current list: ");
	if(ptr != NULL){
		while(ptr->next != NULL){
			printf("%d->", ptr->data);
			ptr = ptr->next;
		}
		printf("%d\n", ptr->data);
	}
	else{
		printf("Empty\n");
	}
	printf("\n");
	
}

void clear(){
	node *ptr = head;
	while(ptr != NULL){
		head = head->next;
		free(ptr);
		ptr = head;
	}
}

void *producer(){
	mutex = 1;
	--empty;
	++full;
	node *newNode = (node*)malloc(sizeof(node));

	newNode->data = rand() / 1000000;
	newNode->next = NULL;
	
	if(head == NULL){
		head = newNode;
		tail = newNode;
	}
	else{
		tail->next = newNode;
		tail = newNode;
	}
	
	printf("\nProduced: %d\n", newNode->data);
	printNode();
	mutex = 0;
	return NULL;
}

void *consumer(){
	mutex = 1;
	if(head == NULL){
		printf("\nBuffer is Empty\n");
		printNode();
		return NULL;
	}
	
	++empty;
	--full;
	
	if(head->next == NULL){
		printf("\nConsumed: %d\n", head->data);
		free(head);
		head = NULL;
		mutex = 0;
		printNode();
		return NULL;
	}
	
	node *ptr = head;
	head = head->next;
	printf("\nConsumed: %d\n", ptr->data);
	free(ptr);
	printNode();

	mutex = 0;
	return NULL;
}

void pro1con1(){
	initialize();
	printf("\nInitiating Single Producer Single Consumer Problem..\n");
	sleep(2);
	int ele, i = 0;
	
	pthread_t producer_t, consumer_t;
	
	srand(time(NULL));
	while(i < 20){
		ele = rand();
		if(ele % 2 == 0){
			if(mutex == 0 && empty != 0){
				pthread_create(&producer_t, NULL, producer, NULL);
				pthread_join(producer_t, NULL);
			}
			else{
				printf("Buffer is Full\n");
			}
		}
		else{
			if(mutex == 0 && full != 0){
				pthread_create(&consumer_t, NULL, consumer, NULL);
				pthread_join(consumer_t, NULL);
			}
			else{
				printf("Buffer is Empty\n");
			}
		}
		i++;
	}
	clear();
}

void pro2con1(){
	initialize();
	printf("\nInitiating Double Producers Single Consumer Problem..\n");
	sleep(2);
	pthread_t producer1, producer2, consumer1;
	int ele, i = 0;
	srand(time(NULL));
	while(i < 20){
		ele = rand();
		if(ele % 2 == 0){
			if(mutex == 0 && empty != 0){
				pthread_create(&producer1, NULL, producer, NULL);
				pthread_join(producer1, NULL);
				if(empty!=0){
					pthread_create(&producer2, NULL, producer, NULL);
					pthread_join(producer2, NULL);
				}
				else{
					printf("Buffer is Full\n");
				}
			}
			
			else{
				printf("Buffer is Full\n");
			}
		}
		else{
			if(mutex == 0 && full != 0){
				pthread_create(&consumer1, NULL, consumer, NULL);
				pthread_join(consumer1, NULL);
			}
			else{
				printf("Buffer is Empty\n");
			}
		}
		i++;
	}
	clear();	
}

void pro1con2(){
	initialize();
	printf("\nInitiating Single Producer Double Consumer Problem..\n");
	sleep(2);
	int ele, i = 0;
	
	pthread_t producer1, consumer1, consumer2;
	
	srand(time(NULL));
	while(i < 20){
		ele = rand();
		if(ele % 2 == 0){
			if(mutex == 0 && empty != 0){
				pthread_create(&producer1, NULL, producer, NULL);
				pthread_join(producer1, NULL);
			}
		}
		else{
			if(mutex == 0 && full != 0){
				pthread_create(&consumer1, NULL, consumer, NULL);
				pthread_join(consumer1, NULL);
				if(full != 0){
					pthread_create(&consumer2, NULL, consumer, NULL);
					pthread_join(consumer2, NULL);
				}
				else{
					printf("Buffer is Empty\n");
				}
			}
			else{
				printf("Buffer is Empty\n");
			}
		}
		i++;
	}
	clear();
}


int options(){
	int option;	
	printf("\n1. 1 Producer 1 Consumer\n");
	printf("2. 2 Producer 1 Consumer\n");
	printf("3. 1 Producer 2 Consumer\n");
	printf("4. Exit\n");
	printf("\nEnter the Choice: ");
	scanf("%d", &option);
	printf("\n");
	initialize();
	return option;
}

int main(){
	int n = 0, choice;
	printf("###################################### PRODUCER CONSUMER PROBLEM ######################################\n");
	printf("################################# SIMPLE ILLUSTRATION USING LINKED LIST ###############################\n");
	do{
		choice = options();
		switch(choice){
			case 1: pro1con1();
				break;
			case 2: pro2con1();
				break;
			case 3: pro1con2();
				break;
			case 4: n = 4;
				printf("Exiting ILLUSTRATION!\n");
				break;
			default: printf("Enter the valid option\n");
				break;
		}
	
	}while(n != 4);
}
