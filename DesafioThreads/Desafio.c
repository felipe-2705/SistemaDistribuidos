#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

int msg_length = 80;
char* mensagem;  ////// shared string between threads
int threads_number = 30;  ///// going to build 30 threads
sem_t* semaphore;   //////// semaphore id to the share string

int sequence = 0;  // sequence of threads execution
int flag = 0;     // sign the end of execution  



////////////////////////Begin thread ///////////////////////
void* toUp(void* num)
{
 long pos = (long) num;  // its the creation number of the thread 
 int i;



while(flag==0){
//enter critical area

sem_wait(&semaphore[pos]);


if(flag==1){
sem_post(&semaphore[(++sequence)%threads_number]); //free the next thread of sequence
pthread_exit(NULL);
}


printf("%s\n",mensagem);

//search for the first lowercase letter

for(i=0;i<msg_length;i++)
{
   if(mensagem[i]>=97 && mensagem[i]<=122){ /// 97 = a and 122 = z
       
     mensagem[i] = mensagem[i] - 32;
     flag=0;  
     break;  
   }
}
if(i>=msg_length)flag=1;



sem_post(&semaphore[(++sequence)%threads_number]); //free the next thread of sequence

 

//end critical area
sleep(1);

}


return NULL;    
}

////////////////////// end thread ///////////////////////////////////



void caracteres()
{
 srand(time(0));
  
for(int i=0;i<msg_length-1;i++)
{
  mensagem[i] = 65+(rand()%58);
}
mensagem[msg_length-1] = '\0';
return;
}

///// begin main function ///////////////////////////////////////////////
int main()
{
pthread_t thread_handles[threads_number];
                     
mensagem = (char*)malloc(msg_length*sizeof(char));
semaphore = (sem_t*)malloc(threads_number*sizeof(sem_t));                                     
caracteres();

for(int i=0;i<threads_number;i++){

sem_init(&semaphore[i],0,0);           // creating a open semaphore
}

sem_post(&semaphore[0]);  // allowing the first thread to begin
//create and execute all threads /////
for(long i = 0;i<threads_number;i++)
{
   pthread_create(&thread_handles[i],NULL,toUp,(void*)(i));
}


//wait all threads to end up 
for(int i=0;i<threads_number;i++)
{
   pthread_join(thread_handles[i],NULL);
   
}



 

for(int i=0;i<threads_number;i++)
{
sem_destroy(&semaphore[i]);
}

printf("All threads has ended sucessifuly\n");
printf("%s\n",mensagem);
return 0;
}

///////////////////// end main function ////////////////////////////////////////////
