#include<stdio.h>
struct process
{
 int at;
 int st;
 int ft;
 int pr;
 int trt;
 int status;
}ready[10];
int n,t,com=0;
int Dispatch(int ct)
{
 int index = -1,high_at = 0, high_status = 0;
 int m;
 m = n;
 for(int i=0;i<n;i++)
 {
  if(ready[i].at > high_at)
  {
   high_at = ready[i].at;
  }
 }
 for(int i=0;i<n;i++)
 {
  if(ready[i].at <= ct)
  {
   if(ready[i].at < high_at)
   {   
    index = i;
    high_at = ready[i].at;
   }
   if(ready[i].at == high_at)
   {
    if(ready[i].status < high_status)
    {
     index=i;
     high_status = ready[i].status;
    }
    else
    {
     if(ready[i].status==high_status)
     {
      if(i<m)
      {
       index=i;
       m = i;
      }
     }
    }
   }
  }
 }
 return index;
}
int main()
{
 int i;
 printf("Enter No. Processes : \n");
 scanf("%d",&n);
 printf("Enter the Time Slice : ");
 scanf("%d",&t);
 for(int i=0;i<n;i++)
 {
  printf("Process : %d\n",i+1);
  printf("********************* \n");
  printf("Enter the Arrival Time : ");
  scanf("%d",&ready[i].at);
  printf("Enter the Burst Time : ");
  scanf("%d",&ready[i].st);
  ready[i].status =  0;
 }
 i=0;
 int at[10],st[10];
 for(int i=0;i<n;i++)
 {
  at[i] = ready[i].at;
  st[i] = ready[i].st;
 }
 int pid,cur_time = 0 ;
 while(com < n)
 {
  pid = Dispatch(cur_time);
  if(ready[pid].st <= t)
  {
   cur_time += ready[pid].st;
   ready[pid].ft = cur_time;
   ready[pid].status = 2;
   com++;
  }
  else
  {
   cur_time += t;
   ready[pid].at = cur_time;
   ready[pid].st = ready[pid].st - t;
   ready[pid].status = 1;
  }
 }
 printf("Process ID \t Arrival Time \t Service Time \t Finish Time \t Turn Around TIme \t Waiting TIme \n");
 for(int i=0;i<n;i++)
 {
  printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%n",i+1,at[i],st[i],ready[i].ft,ready[i].ft-at[i],ready[i].ft-at[i]-st[i]);
 }
 return 0;
}

  
