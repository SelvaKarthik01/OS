// Code for Priority Scheduling in Non-Preemptive Mode..

#include<stdio.h>
struct process
{
	char name[3];
    	int at;
    	int bt;
    	int tat;
    	int ct;
    	int wt;
    	int pr;
}pro[50],temp;

void main()
{
	int i,n,j;
	float totalwt,totaltat,avgwt,avgtat;

	printf("\nEnter the n value : ");
	scanf("%d",&n);

	printf("\nEnter the process name : ");
	for(i=0;i<n;i++)
	{
		scanf("%s",&pro[i].name);
	}

	printf("\nEnter the arrival time : ");
	for(i=0;i<n;i++)
	{
		scanf("%d",&pro[i].at);
	}
	
	printf("\nEnter the burst time : ");
	for(i=0;i<n;i++)
	{
		scanf("%d",&pro[i].bt);
	}

	printf("\nEnter the priority number : ");
	for(i=0;i<n;i++)
	{
		scanf("%d",&pro[i].pr);
	}

//sorting
	for(i=0;i<n;i++)
	{
	    	for(j=i+1;j<n;j++)
    		{
			if(pro[i].at>pro[j].at)
			{
				temp=pro[i];
				pro[i]=pro[j];
				pro[j]=temp;
			}
		}
	}
//sort according to priority
	for(i=1;i<n;i++)
	{
		for(j=i+1;j<n;j++)
    		{
        		if(pro[i].pr<pro[j].pr)
        		{
            			temp=pro[i];
            			pro[i]=pro[j];
            			pro[j]=temp;
        		}
    		}
	}

	printf("\nPROCESS\t CT\t TAT\t WT");
//calculation
	
	pro[0].ct=pro[0].at+pro[0].bt;
	pro[0].tat=pro[0].ct-pro[0].at;
	pro[0].wt=pro[0].tat-pro[0].bt;

	totalwt=pro[0].wt;
	totaltat=pro[0].tat;

	printf("\n    %s\t %d\t  %d\t %d",pro[0].name,pro[0].ct,pro[0].tat,pro[0].wt);

	for(i=1;i<n;i++)
	{
		pro[i].ct=pro[i-1].ct+pro[i].bt;
		pro[i].tat=pro[i].ct-pro[i].at;
		pro[i].wt=pro[i].tat-pro[i].bt;

		totalwt+=pro[i].wt;
		totaltat+=pro[i].tat;
		
		printf("\n    %s\t %d\t  %d\t %d",pro[i].name,pro[i].ct,pro[i].tat,pro[i].wt);
	}
	
	avgtat=totaltat/n;
	avgwt=totalwt/n;
	printf("\nthe average turnaround time and waiting time is %f and %f",avgtat,avgwt);
	printf("\n");
}
