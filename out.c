// threadprime finds all the prime numbers between START and END and lists them out
// call it with the number of threads, for example
//   ./threadprime 4
// will do the computation with four threads


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//range of numbers to test if prime
#define START 100000000
#define END 100001000

//when primes are found they're put in primelist
//primecount will tell how many there are
int primelist[END-START];
int primecount=0;
pthread_mutex_t lock;
pthread_mutex_t mutex;
//returns 1 if num is prime, 0 otherwise
int isprime(int num)
{
	int i;
	for(i=2; i<num-1; i++)
		if(num%i==0)
			return 0;
	return 1;
}

//when a thread is created, the parameters are placed in these global variables so that the thread can read them
int startParameter;		//beginning of the range of numbers the thread is computing
int endParameter;		//end of the range
int threadnumberParameter;	//id number for the thread: 0..N-1

//this function is run several times as separate thread
void* findprimes()
{

	//read the parameters into local variables so the global vars can be reused
	int start=startParameter;
	int end=endParameter;
	int threadnumber=threadnumberParameter;
	int p;
	pthread_mutex_lock(&lock);
	printf("\nThread %d is about to find primes from %d to %d\n",threadnumber,start,end);

	//go through the range of numbers
	for(p=start; p<end; p++)
	{
		//if number p is prime, then...
		if(isprime(p)==1)
		{
			//put it into the global array
			primelist[primecount]=p;
			primecount++;
		}
	}

	printf("Thread %d is done.\n",threadnumber);
	pthread_mutex_unlock(&lock);
}


//this sorts all the numbers in the global array that haven't yet been sorted
int lastUnsorted=0;	//the index of the last sorted number

void sortTheUnsorted()
{
	int i,j,value;
	//do an insertion sort on everybody who has been added to primelist since the last sort
	pthread_mutex_lock(&mutex);
	for(i=lastUnsorted; i<primecount; i++)
	{
//		usleep(100000);
		value=primelist[i];
		for(j=i; j>0 && value<primelist[j-1]; j--)
		{
//			usleep(100000);
			primelist[j]=primelist[j-1];
		}
		primelist[j]=value;
//		usleep(100000);
	}
	//now every prime with index below lastUnsorted is sorted

	lastUnsorted=primecount;
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[])
{
	//print out this message if no parameter for N was given
	if(argc<2)
	{
		printf("\nthis program finds all the prime numbers between %d and %d\n\nUsage: \n\t./threadprime 4\nwhere 4 is the number of threads\n",START,END);
//		usleep(100000);
		return 0;
	}

	//N is the number of threads
	int N=atoi(argv[1]);
	int t;

	//thread variables
	pthread_t thread[N];
	if (pthread_mutex_init(&lock,NULL) != 0){
		return 1;
	}
	//go through each thread
	for(t=0; t<N; t++)
	{
		//compute the range of numbers that thread "t" will handle
		//put them into global vars so thread can read them
		startParameter=START+t*(END-START)/N;
		endParameter=START+(t+1)*(END-START)/N;
		threadnumberParameter=t;
//		usleep(100000);
		if(t==N-1)
			endParameter=END;

		//now start the thread
		pthread_create(&thread[t],NULL,findprimes,NULL);
	}

//	if (pthread_mutex_init(&mutex, NULL)!=0){
//		return 1;
//	}
	//don't go further until every computing thread is done
	for(t=0; t<N; t++){
		if (pthread_mutex_init(&mutex, NULL)!=0){
		usleep(100000);
		return 1;
		}
		pthread_join(thread[t],NULL);
//		return 1;
	}
	//sort everybody before printing them out
//	pthread_mutex_destroy(&mutex);
	sortTheUnsorted();

	//print out the results
	printf("\nAll threads are done.  Here are the primes between %d and %d:\n",START,END);
//	pthread_mutex_destroy(&mutex);
	for(t=0; t<primecount; t++)
	{
		printf("%d ",primelist[t]);
	}
//	pthread_mutex_destroy(&lock);
	printf("\n\n%d primes total\n",primecount);
//	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&lock);
	pthread_mutex_destroy(&mutex);
}
