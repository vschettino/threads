#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#define NUM_THREADS     5

void *PrintHello(void *threadid)
{
   pid_t child_pid;
   
   long tid;
   tid = (long)threadid;
   printf("Hello World! It's me, thread #%ld!\n", tid);
   printf ("And my process ID is %d\n", (int) getpid ());
   printf ("And my parent has the process ID %d\n", (int) getppid ());
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{

   pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   for(t=0; t<NUM_THREADS; t++){
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }
   pthread_exit(NULL);
}
