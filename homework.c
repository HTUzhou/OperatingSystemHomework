#include<stdio.h>
#include<malloc.h>

#define max 50

struct PCB
{
	int name;
	int arrivetime; //到达时间
	int servicetime; //服务时间
	//int starttime[max]; //开始时间
    int finishtime; //完成/结束时间
    int turntime; //周转时间
    int average_turntime; //带权周转时间
	int sign; //标志进程是否完成
	int remain_time; //剩余时间
	int priority;  //优先级
}pcb[max];

void init(int n)  //初始化
{ int i;
	for(i=0;i<=n;i++)
	{
		pcb[i].arrivetime=0; 
		pcb[i].servicetime=0; 
	    //pcb[i].starttime=0; 
	    pcb[i].finishtime=0; 
	    pcb[i].turntime=0;
	    pcb[i].average_turntime=0;
		pcb[i].remain_time=0;
		pcb[i].sign=0;
		pcb[i].priority=0;
	}
}
void creat(int n) //创建PCB
{
	int i;
	for(i=1;i<=n;i++)
	{
		printf("\n%d:\n input the information of process\n input processID:",i);
		scanf("%d",&pcb[i].name);
		printf("input the arrivetime:");
		scanf("%d",&pcb[i].arrivetime);
		printf("input the servicetime:");
		scanf("%d",&pcb[i].servicetime);
		printf("input the priority:");
		scanf("%d",&pcb[i].priority);
		pcb[i].remain_time=pcb[i].servicetime;  //初始化剩余时间为服务时间
	}
}

void FCFS(int n) //先来先服务
{
	int starttime;
	printf("input the start time：\n");
	scanf("%d",&starttime);
	if(starttime>=pcb[1].arrivetime)
	{
		pcb[1].finishtime=pcb[1].servicetime+starttime;
	}
	else
	{
		// pcb[0].finishtime=pcb[0].finishtime+pcb[0].servicetime;
		pcb[1].finishtime=pcb[1].arrivetime+pcb[1].servicetime;
	}
	// 为第一个pcb的turntime和average_turntime赋值
	pcb[1].turntime=pcb[1].finishtime-pcb[1].arrivetime;
	pcb[1].average_turntime=pcb[1].turntime/pcb[1].servicetime;
    int i;
	for(i=2;i<=n;i++)
	{
		if(pcb[i-1].finishtime>pcb[i].arrivetime)
			pcb[i].finishtime=pcb[i-1].finishtime+pcb[i].servicetime;
		else
			pcb[i].finishtime=pcb[i].arrivetime+pcb[i].servicetime;
		pcb[i].turntime=pcb[i].finishtime-pcb[i].arrivetime;
		pcb[i].average_turntime=pcb[i].turntime/pcb[i].servicetime;
	}
}

void print_FCFS(int n)
{
		//printf("ProcessID, arrivetime\t Servicetime\t finishtime\t turntime\t:,,%s\t%d\t%d\t%d\t%d\t%d\t"); //进程名,到达时间,服务时间,完成时间,周转时间,周转时间
	printf("process ID  arrivetime servicetime finishtime turntime , averageturntime  .\n");
         int i;
	for(i=1;i<=n;i++)
	{
		printf("%d  ,%d  ,%d  ,%d  ,%d  ,%d  ",pcb[i].name  ,pcb[i].arrivetime  ,pcb[i].servicetime  ,pcb[i].finishtime  ,pcb[i].turntime  ,pcb[i].average_turntime);
		printf("\n");
	}

}
void time_segment(int n) //时间片轮转
{
	int i,j;
	int T;   //时间片
	int flag=1;   //就绪队列中是否还有进程
	//int time=pcb[0].arrivetime;   //当前的时间
	int time=0;
	int sum=0;   //已经完成的进程数

	//按各进程的arrivetime进行升序排列
	for(i=1;i<=n;i++)
	for(j=i+1;j<=n;j++)
	{
		if(pcb[j].arrivetime<pcb[i].arrivetime)
		{
			pcb[0]=pcb[j];
			pcb[j]=pcb[i];
			pcb[i]=pcb[0];
		}
	}
	    //printf("output the sort result:\n");
		//for(i=1;i<=n;i++)    //检查排序是否正确
     	//printf("%d\t",pcb[i].name);
	printf("input the slicetime:\n");
	scanf("%d",&T);
	//printf("\n running processID runtime resttime finishtime\n”) //  开始运行时间   运行时间   剩余服务时间   结束时间
	time=pcb[1].arrivetime;  //减少一次循环 

	while(sum<n)
	{
		flag=0;   //当前就绪队列中没有进程
		int i;
        for(i=1;i<=n;i++)
		{
			if(pcb[i].sign==1) continue; //表示该进程已完成
			else
			{
				if(time>=pcb[i].arrivetime) {	//当pcb到达时，开始进行运算，否则不进行 
					//没有完成的进程需要的时间大于一个时间片
					if(pcb[i].remain_time > T)
					{
						flag=1;
						time=time+T;
						pcb[i].remain_time=pcb[i].remain_time-T;
						//printf("%10d%16d%12d%12d%12d\n",pcb[i].name,time-T,T,pcb[i].remain_time,time);
					}
					//没有完成的进程需要的时间小于或等于一个时间片
					else if(pcb[i].remain_time <= T)
					{
						flag=1;  //加入就绪队列
						time=time+pcb[i].remain_time;
						pcb[i].finishtime=time;
	                    pcb[i].sign=1;
						//printf("%10d%16d%12d%12d%12d\n",pcb[i].name,time-pcb[i].remain_time,pcb[i].servicetime,0,time); 
			 	        pcb[i].remain_time=0;
					}
					if(pcb[i].sign==1) sum++;
				}
			}
		}//for
		if(flag==0&&sum<n)   // 还有没执行的进程，且没进入就就绪队列 
		{
	        int i;
			for(i=1;i<=n;i++)
			if(pcb[i].sign==0) 
			{
				time=pcb[i].arrivetime;
				break;
			}
		}
	}//while

}
void print_time(int n)
{   
	int i;
	for(i=1;i<=n;i++)
	{
	printf("\n processID servicetime finishtime\n");  //进程名   服务时间   完成时间
	printf("%6d%10d%10d",pcb[i].name,pcb[i].servicetime,pcb[i].finishtime);
	printf("\n");
	}
}

void Priority_print(int n)
{
	int i, j;
	for(i=1;i<=n;i++)
	{
		for(j=i+1;j<=n;j++)  
		{
			if(pcb[j].name < pcb[i].name)
			{
				pcb[0]=pcb[j];
				pcb[j]=pcb[i];
				pcb[i]=pcb[0];	
			}
		}
	}
	for(i=1;i<=n;i++)
	{
		printf("\n processID servicetime priority finishtime\n");  //进程名   服务时间    优先级  完成时间
		printf("%6d%10d%10d%10d",pcb[i].name,pcb[i].servicetime,pcb[i].priority,pcb[i].finishtime);
		printf("\n");
	}
}

void SortByPriorityOrArrivetime(int flag, int n)
{
	int i, j;
	if(flag == 1)
	{
		for(i=1;i<=n;i++) 
		{
			for(j=i+1;j<=n;j++)
			{
				if(pcb[j].priority > pcb[i].priority)
				{
					pcb[0]=pcb[j];
					pcb[j]=pcb[i];
					pcb[i]=pcb[0];	
				}
			}	
		}	
	}
	else
	{
		for(i=1;i<=n;i++)
		{
			for(j=i+1;j<=n;j++)
			{
				if(pcb[j].arrivetime < pcb[i].arrivetime)
				{
					pcb[0]=pcb[j];
					pcb[j]=pcb[i];
					pcb[i]=pcb[0];	
				}
			}	
		}
	}
}

void Priority(int n)
{
	int sum=0;
	int i,j;
	int time = 0;
	printf("\n processID runtime priority fihishtime \n");//进程名   服务时间   优先级  完成时间
	int arrNum=0;
	int k;
	while(sum < n)
	{
		arrNum=0;
		k=0;
		for(i=1;i<=n;i++)  //判断是否arrivetime<=time的 
		{
			if(pcb[i].sign == 1)
			{
				continue;
			}
			if(pcb[i].arrivetime <= time)
			{
				arrNum++;
			}
		}
		if(arrNum > 0)
		{
			//按优先级排序 
			SortByPriorityOrArrivetime(1, n);
			for(i=1;i<=n;i++) //获取优先级最大且 arrivetime<=time 
			{
				if(pcb[i].sign==0&&pcb[i].arrivetime<=time)  //进行赋值操作 
				{
					pcb[i].finishtime = time + pcb[i].servicetime;
					pcb[i].sign = 1;
					sum++;
					time = time + pcb[i].servicetime;
					break;
				}
			}
		}
		else
		{
			//按arrivetime顺序排序 
			SortByPriorityOrArrivetime(0, n);
			//获取arrivetime最小的，且优先级最大
			for(i=1;i<=n;i++) 
			{
				if(pcb[i].sign == 0)
				{
					k = i;
					break;
				}
			}
			for(i=1;i<=n;i++)
			{
				if(pcb[i].arrivetime == pcb[k].arrivetime && pcb[i].priority > pcb[k].priority)
				{
					k = i;
				}
			}
			pcb[k].finishtime = pcb[k].arrivetime + pcb[k].servicetime;
			pcb[k].sign = 1;
			sum++;
			time=pcb[k].arrivetime + pcb[k].servicetime;			
		}
	}
	//开始进行输出
	Priority_print(n);
}//void

void sortByTimeSlice(int n)
{
	int i, j;
	for(i=1;i<=n;i++)   
	{
		for(j=i+1;j<=n;j++)
		{
			if(pcb[j].servicetime < pcb[i].servicetime)
			{
				pcb[0]=pcb[j];
				pcb[j]=pcb[i];
				pcb[i]=pcb[0];	
			}
		}	
	}
}

void shortPrior(int n)
{
	int i, j;
	int sum = 0;
	int time = 0;
	int arrNum=0;
	int k;
	while(sum < n)
	{
		k = 0;
		arrNum=0;
		for(i=1;i<=n;i++)  //判断有几个arrivetime<=time的 
		{
			if(pcb[i].sign == 1)
			{
				continue;
			}
			if(pcb[i].arrivetime <= time)
			{
				arrNum++;
				break;
			}
		}
		if(arrNum > 0)
		{
			//按作业长短排序
			sortByTimeSlice(n);
			for(i=1;i<=n;i++) //获取作业最短且 arrivetime<=time 
			{
				if(pcb[i].sign==0&&pcb[i].arrivetime<=time)  //进行赋值操作 
				{
					pcb[i].finishtime = time + pcb[i].servicetime;
					pcb[i].sign = 1;
					sum++;
					time = time + pcb[i].servicetime;
					break;
				}
			}
		}
		else
		{
			for(i=1;i<=n;i++)  //按arrivetime顺序排序 
			{
				for(j=i+1;j<=n;j++)
				{
					if(pcb[j].arrivetime < pcb[i].arrivetime)
					{
						pcb[0]=pcb[j];
						pcb[j]=pcb[i];
						pcb[i]=pcb[0];	
					}
				}
			}
			//获取arrivetime最小的，且时间片最短 
			for(i=1;i<=n;i++) 
			{
				if(pcb[i].sign == 0)
				{
					k = i;
					break;
				}
			}
			for(i=1;i<=n;i++)
			{
				if(pcb[i].arrivetime == pcb[k].arrivetime && pcb[i].servicetime > pcb[k].servicetime)
				{
					k = i;
				}
			}
			pcb[k].finishtime = pcb[k].arrivetime + pcb[k].servicetime;
			pcb[k].sign = 1;
			sum++;
			time=pcb[k].arrivetime + pcb[k].servicetime;		
		}
	}
	//开始进行输出
	Priority_print(n); 
}

void layout(int n)
{
	int ch=0;
	printf("\t\t************schedule algorithm************\n");
	printf("\t\t1.FCFS\n");
	printf("\t\t2.timesegment\n");
	printf("\t\t3.priority\n");
	printf("\t\t4.ShortPrior\n");
	printf(" choose the algorithm:\n");
	scanf("%10d",&ch);
	switch(ch)
	{
	case 1:
		    FCFS(n);
		    print_FCFS(n); break;
	case 2:
		    time_segment(n);
		    print_time(n); break;
	case 3:
		    Priority(n); break;
	case 4:
			shortPrior(n); break;
	default:printf("enter error data!\n");
	//P:int类型的变量,case后面不要加''
	}
}

int main()
{
	int n;
	printf("input the number of process\n");
	scanf("%d",&n);
	init(n);
	creat(n);
	layout(n);
	//FCFS(n);
	//print_FCFS(n);
	//time_segment(n);
	//print_time(n);
	//Priority(n);
	return 0;
}
