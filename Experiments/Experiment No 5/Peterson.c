#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>

int flag[2] = {0,0};
int turn;
int seats = 100;

int update_seats(char* action)
{
 int n;
 if(strcmp(action,"Book")==0)
 {
  printf("Enter No. of Seats to be Booked : ");
   scanf("%d",&n);
   
  if(seats-n > 0)
  {
   seats -= n;
   printf("Seats Booked Sucessfully !\n");
  }
  else
  {
   printf("No More Seats Available !!\n");
  }
 }
 else
 {
  printf("Enter the No. of Seats to Cancel : ");
  scanf("%d",&n);
  seats += n;
  printf("Seats Cancelled Successfully !\n");
 }
}
void *server(void *args)
{
 while(1)
 {
  flag[0] = 1;
  turn = 1;
  while(flag[1] == 1 && turn == 1)
  {
  }
  printf("Total Seats Available : %d \n",seats);
  flag[0] = 0;
  sleep(2);
 }
 return NULL;
}
void *client(void *args)
{
 char action[100];
 while(1)
 {
  flag[1] = 1;
  turn = 0;
  while(flag[0] == 1 && turn == 0)
  {   
  }
  printf("Do You want to Book/Cancel a Seat : ");
  scanf("%s",action);
  update_seats(action);
  flag[1]=0;
  sleep(2);
 }
 return NULL;
}
int main()
{
 pthread_t server_thread,client_thread;
 int server_id = 0,client_id = 1;
 flag[0] = 0;
 flag[1] = 0;
 turn = 0;
 pthread_create(&server_thread,NULL,server,server_id);
 pthread_create(&client_thread,NULL,client,client_id);
 pthread_join(server_thread,NULL);
 pthread_join(client_thread,NULL);
 return 0;
}
  
