// FCFS Scheduling 
#include<stdio.h>
#include<math.h>
struct Process
{
 int at;
 int bt;
 int ct;
 int wt;
 int tat;
}process[100],temp;
int main()
{
 int ct,wt,tat,n;
 printf("Enter the o. of Process for FCFS Scheduling : ");
 scanf("%d",&n);
 for(int i=0;i<n;i++)
 {
  printf("Process %d : \n",i+1);
  printf("\n");
  printf("Enter The Arrival Time : ");
  scanf("%d",&process[i].at);
  printf("Enter the Burst Time : ");
  scanf("%d",&process[i].bt);
 }
 // Sort based on Arrival time 
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
 float Ct=INFINITY;
 for(int i=0;i<n;i++)
 {
  if (process[i].at < Ct)
  {
   Ct = process[i].at;
  }
 }
 for(int i=0;i<n;i++)
 {
  process[i].ct = Ct + process[i].bt;
  Ct = process[i].ct;
 }
 for(int i=0;i<n;i++)
 {
  process[i].tat = process[i].ct - process[i].at;
  process[i].wt = process[i].tat - process[i].bt;
 } 
 printf("The Table Chart of all the Necessary Informastion : \n ");
 printf("Process ID \t AT \t BT \t CT \t TAT \t WT \n");
 for(int i=0;i<n;i++)
 {
  printf("%d \t\t %d \t %d \t %d \t %d \t %d \n",i+1,process[i].at,process[i].bt,process[i].ct,process[i].tat,process[i].wt);
 }
 float avg_tat=0;
 float avg_wt = 0;
 for(int i=0;i<n;i++)
 {
  avg_tat += process[i].tat;
  avg_wt += process[i].wt;
 }
 avg_tat = avg_tat/n;
 avg_wt = avg_wt/n;
 printf("The Average Turn Around Time is :  %f \n",avg_tat);
 printf("The Average Waiting Time is : %f \n",avg_wt);

 return 0;
}
