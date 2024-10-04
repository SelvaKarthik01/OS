// To Implement FCFS CPU Scheduling Algortihm 
#include<stdio.h>
struct Process
{
 int at;
 int bt;
}process[100],temp;
int main()
{
 printf("\t ************************** FCFS Scheduling **********************\n");
 int n;
 printf("Enter the Total No. of Processor : \n");
 scanf("%d\n",&n);
 for(int i=0;i<n;i++)
 {
  printf("Processor %d : \n",i+1);
  printf("\n");
  printf("Enter the Arrival Time : ");
  scanf("%d",&process[i].at);
  printf("\n");
  printf("Enter Burst Time : ");
  scanf("%d",&process[i].bt);
  printf("\n");
 }
 for(int i=0;i<n;i++)
 {
  for(int j=i+1;j<n;j++)
  {
   if (process[i].at > process[j].at)
   {
    temp = process[i];
    process[i] = process[j];
    process[j] = temp;
   }
  }
 }
 int CT,WT,TAT;
 for(int i=0;i<n;i++)
 {
  CT = CT + process[i].at;
  printf("The Completion TIme of the Process %d is %d\n",i+1,CT);
  TAT = CT - process[i].at;
  printf("The Turn Around Time of Process %d is %d\n",i+1,TAT);
  WT = TAT - process[i].bt;
  printf("The Waiting Time of Process %d is %d\n",i+1,WT);
  printf("\n");
 }
 return 0;
}

