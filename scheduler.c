//two queues for job and ready
//two threads for long and short term scheduler

/**
1. Populate job queue with jobs
  <print>
2. Use LTS to bring jobs from job queue to ready queue
    Print:
    [Kernel] LTS invoked
    LTS: <contents of both queues>
    every invokation
3. Use STS to execute jobs and then put them back in job queue
    Print:
    [Kernel] STS invoked
    STS: <contents of both queues>
    every invokation
4. Repeat until all processes executed
**/

//Hamza Saleem 215557648
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <pthread.h>

void initializeQueues();
void printreadyQueue();
void printjobQueue();
int getreadyqueueSize();
int getjobqueueSize();
void *stSched(void *ptr);
void *ltSched(void *ptr);
struct Process{

  int pid;
  int exetime;
  TAILQ_ENTRY(Process) jobqueue;
  TAILQ_ENTRY(Process) readyqueue;
};

TAILQ_HEAD(, Process) jobqueue_head;
TAILQ_HEAD(, Process) readyqueue_head;
struct Process *item;
struct Process *temp_item;
pthread_t thread1, thread2;
int lts;
int sts;

int main()
{
  srand(time(NULL));
  initializeQueues();
  char *message1 = "Thread 1";
  char *message2 = "Thread 2";
  // printjobQueue();
  printjobQueue();
  while (getjobqueueSize() > 0 || getreadyqueueSize() > 0)
  {

    printf("[Kernel] Long Term Scheduler Invoked\n");
    lts = pthread_create( &thread1, NULL, ltSched, (void*) message1);
    pthread_join(thread1, NULL); //WAIT FOR THREAD TO FINISH
    printf("[Kernel] Short Term Scheduler Invoked\n");
    sts = pthread_create( &thread2, NULL, stSched, (void*) message2);
    pthread_join(thread2, NULL);
    //after sts then go back and lts
  }
  printf("[Kernel] All processes have finished their execution.\n");

}
void *ltSched( void *ptr ) //dequeue jobs
{
  printf("[LTS] ");
  printjobQueue();
  printf("[LTS] ");
  printreadyQueue();
  if (getreadyqueueSize() == 5)
  {
    printf("[LTS] Ready Queue is Full, cannot enter more\n");
  }
  while (getreadyqueueSize() < 5 && getjobqueueSize() > 0)
  {
    temp_item = TAILQ_FIRST(&jobqueue_head);
    TAILQ_REMOVE(&jobqueue_head, temp_item, jobqueue);
    printf("[LTS] Process %d removed from the Job Queue and inserted into the Ready Queue\n", temp_item->pid );
    TAILQ_INSERT_TAIL(&readyqueue_head, temp_item, readyqueue);

  }

}
void *stSched( void *ptr ) //enqeue jobs
{
  printf("[STS] ");
  printjobQueue();
  printf("[STS] ");
  printreadyQueue();
  int counter;
  if (getreadyqueueSize() == 0)
  {

  }
  else
  {
    for (counter = 0; counter < getreadyqueueSize(); counter++) //cycle all 5 processes
    {
      temp_item = TAILQ_FIRST(&readyqueue_head);
      printf("[STS] Process %d is now executing\n",temp_item->pid );
      temp_item->exetime = (temp_item->exetime) - 2;
      if ((temp_item->exetime) <= 0)
      {
        printf("[STS] Process %d terminated\n",temp_item->pid );
        TAILQ_REMOVE(&readyqueue_head, temp_item, readyqueue);
      }
      else
      {
        TAILQ_REMOVE(&readyqueue_head, temp_item, readyqueue);
        printf("[STS] Process %d with remaining time %d enqueued to the Ready Queue\n",temp_item->pid, temp_item->exetime );
        TAILQ_INSERT_TAIL(&readyqueue_head, temp_item, readyqueue);
      }
    }
  }

}
void initializeQueues()
{
  int i;
  TAILQ_INIT(&jobqueue_head);
  TAILQ_INIT(&readyqueue_head);

  for (i = 1; i < 101; i++)
  {
    item = malloc(sizeof(*item));
    item->pid = i;
    item->exetime = rand() % 31;
    printf("[Kernel] Process %d created with Time = %d\n", item->pid, item->exetime );
    TAILQ_INSERT_TAIL(&jobqueue_head, item, jobqueue);
  }
}
void printjobQueue()
{
    printf("Job Queue: ");
  if (getjobqueueSize() == 0)
  {
    printf("EMPTY");
  }
  else
  {
    TAILQ_FOREACH(item, &jobqueue_head, jobqueue)
    {

      printf("[Process %d: Time:%d], ", item->pid, item->exetime);
    }
  }

  printf("\n" );
}
void printreadyQueue()
{
    printf("Ready Queue: ");
  if (getreadyqueueSize() == 0)
  {
    printf("EMPTY");
  }
  else
  {
    TAILQ_FOREACH(item, &readyqueue_head, readyqueue)
    {
      printf("[Process %d:Time:%d], ", item->pid, item->exetime);
    }
  }
    printf("\n" );
}

int getreadyqueueSize()
{
  int size = 0;
  TAILQ_FOREACH(item, &readyqueue_head, readyqueue)
  {
    size+=1;
  }
  return size;
}

int getjobqueueSize()
{
  int size = 0;
  TAILQ_FOREACH(item, &jobqueue_head, jobqueue)
  {
    size+=1;
  }
  return size;
}
