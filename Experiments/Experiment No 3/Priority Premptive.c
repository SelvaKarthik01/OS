#include<stdio.h>

struct process
{
	int at ;
	int bt ;
	int pr ;
	int status ;
	int ft ;
}readylist[10] ;

int n ;

int dispatcher(int time)
{
	int i , highprio , index ;
	highprio = 0 ;	
	index = -1 ;
	for(i=0 ; i<n ; i++)
	{
		if(readylist[i].status != 1)
		{
			if(readylist[i].at <= time)
			{
				if(readylist[i].pr > highprio)
				{
					highprio = readylist[i].pr ;
					index = i ;
				}
			}
		}
	}

	return index ;
}

int main()
{
	int i , currtime , pid ;
	printf("Enter the Number of Processes : ") ;
	scanf("%d",&n) ;

	for(i=0 ; i<n ; i++)
	{
		printf("\nProcess %d\n" , i+1) ;
		printf("Enter Arrival Time : ") ;
		scanf("%d" , &readylist[i].at) ;
		printf("Enter Burst Time : ") ;
		scanf("%d" , &readylist[i].bt) ;
		printf("Enter Priority (1-10) : ") ;
		scanf("%d" , &readylist[i].pr) ;
		readylist[i].status = 0 ;
	}

	i=0 ;
	currtime = 0 ;

	while(i<n)
	{
		pid = dispatcher(currtime) ;
		readylist[pid].ft = currtime + readylist[pid].bt ;
		readylist[pid].status = 1 ;
		currtime += readylist[pid].bt ;
		i++ ;
	}

	printf("Process\t Arrival Time \t Service Time \t Finish Time \n\n") ;
	for(i=0 ; i<n ; i++)
	{
		printf("%d \t\t %d \t\t %d \t\t %d \n", i , readylist[i].at , readylist[i].bt , readylist[i].ft) ;
	}
}






	

	
		
