/*
 * round_robin.c
 * =============
 * Implementation of CPU Scheduling Algorithm: Round Robin
 *
 * This program simulates the Round Robin (RR) scheduling algorithm.
 *
 * Key Concepts:
 * - Time Quantum: Each process gets a fixed amount of CPU time (quantum).
 * - Preemptive: If a process's burst time is longer than the quantum,
 *   it is preempted and moved to the back of the ready queue.
 * - Circular Queue: Processes are executed in a circular order.
 *
 * Performance Metrics Calculated:
 * - Completion Time (CT): The time at which a process finishes execution.
 * - Turnaround Time (TAT): CT - Arrival Time.
 * - Waiting Time (WT): TAT - Burst Time.
 */

# include<stdio.h>

int main(){
    int at[10],bt[10],wt[10],tat[10],n,rem[10],qunatum,ct[10];
    int count=0,exec,time=0;
    float a_tat=0,a_wt=0;
    
    printf("Enter no. of processes:");
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        printf("\nP%d arrival time",i+1);
        scanf("%d",&at[i]);
        printf("\nP%d burst time",i+1);
        scanf("%d",&bt[i]);
        rem[i]=bt[i];
    }
     printf("\nEnter Time Qunatum");
    scanf("%d",&qunatum);
    printf("\nProcess\tAT\tBT\tTAT\tWT");
    while(count<n){
        exec=0;
        for(int i=0;i<n;i++){
            if(at[i]<=time && rem[i]>0){
                exec=1;
                if(rem[i]>qunatum){
                    time+=qunatum;
                    rem[i]-=qunatum;
                }else{
                    time+=rem[i];
                    rem[i]=0;
                    count++;
                    ct[i]=time;
                    tat[i]=ct[i]-at[i];
                    wt[i]=tat[i]-bt[i];
                    a_tat+=tat[i];
                     a_wt+=wt[i];
                }
            }
        }
        if(!exec){
            time++;
        }
    }
    for(int i=0;i<n;i++){
         printf("\n%d\t%d\t%d\t%d\t%d",i+1,ct[i],bt[i],tat[i],wt[i]);
    }
    printf("\nAverage TAT:%f",a_tat/n);
    printf("\nAverage WT:%f",a_wt/n);
    return 0;
    
}