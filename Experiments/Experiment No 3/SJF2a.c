// SJF CPU Scheduling 
#include<stdio.h>
#include<math.h>
#include<stdbool.h>
struct Process
{
 int id;
 int at;
 int bt;
 int tat;
 int ct;
 int wt;
 bool status;
}process[100],temp,curr_less;
int main()
{ 
 int n;
 printf("Enter the No. of Processors : ");
 scanf("%d",&n);
 for(int i=0;i<n;i++)
 {
  printf("\n");
  printf("Enter Process ID : ");
  scanf("%d",&process[i].id);
  
  printf("Enter the Arrival Time : ");
  scanf("%d",&process[i].at);
  printf("Enter the Burst Time : ");
  scanf("%d",&process[i].bt);
  process[i].status = false;
 }
 int min;
 for(int i=0;i<n;i++)
 {
  min = i;
  for(int j=i+1;j<n;j++)
  {
   if (process[j].at < process[min].at)
   {
    min = j;
   }
  }
  temp = process[i];
  process[i] = process[min];
  process[min] = temp;
 }
 int Ct = 0;
 int completed_processes = 0;
 while(completed_processes < n)
 {
  int min = -1;
  for (int i=0;i<n;i++)
  {
   if (process[i].at <= Ct && process[i].status == false)
   {
    if (min == -1)
    {
     min = i;
    }
    else if (process[i].bt < process[min].bt)
    {
     min = i;
    }
   }
  }
  if (min == -1)
  {
   printf("IDLE @ %d\n",Ct);
   Ct += 1;
  }
  else
  {
   Ct += process[min].bt;
   process[min].status = true;
   process[min].ct = Ct;
   completed_processes += 1;
  }
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
  printf("%d \t\t %d \t %d \t %d \t %d \t %d \n",process[i].id,process[i].at,process[i].bt,process[i].ct,process[i].tat,process[i].wt);
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
 
