#include "queue.c"
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Eren Berke Demirbas: 0064627
// Batuhan Yalcin: 0064274

#define LOG_FILE "events.log"
// Global Parameters
int simulationTime = 120;    // simulation time
int seed = 10;               // seed for randomness
int emergencyFrequency = 40; // frequency of emergency
float p = 0.2;               // probability of a ground job (launch & assembly)
int counter = 1; // Global counter for job ID(s)
int t = 2; // Time unit given in the description
time_t start_time; // Relative start time of the simulation
time_t end_time; // Relative end time of the simulation
time_t padB_before_sleep; // Recorded time before padB goes sleep
time_t padA_before_sleep; // Recorded time before padA goes sleep
int PadA_sleep_job_type; // PadA executing job type
int PadB_sleep_job_type; // PadB executing job type
int n; // Console printing start time


void* LandingJob(void *arg); 
void* LaunchJob(void *arg);
void* EmergencyJob(void *arg); 
void* AssemblyJob(void *arg); 
void* ControlTower(void *arg);
void* Pad_A();
void* Pad_B();
void recordLogs(Job job, char pad);
void* printQueues(void *arg);



// Queue Initialization 
Queue *landing_queue;
Queue *launching_queue;
Queue *assembly_queue;
Queue *padA_queue;
Queue *padB_queue;

//PArt 3 Emergency Queue
Queue *Emergency_queue;

// Mutex Creation for Shared Variables, Files, Counters, etc.
pthread_mutex_t landing_mutex;
pthread_mutex_t launching_mutex;
pthread_mutex_t assembly_mutex ;
pthread_mutex_t padA_mutex;
pthread_mutex_t padB_mutex;
pthread_mutex_t counter_mutex;
pthread_mutex_t file_mutex;

//PArt 3 Emergency Mutex
pthread_mutex_t Emergency_mutex;

// pthread sleeper function
int pthread_sleep (int seconds)
{
    pthread_mutex_t mutex;
    pthread_cond_t conditionvar;
    struct timespec timetoexpire;
    if(pthread_mutex_init(&mutex,NULL))
    {
        return -1;
    }
    if(pthread_cond_init(&conditionvar,NULL))
    {
        return -1;
    }
    struct timeval tp;
    //When to expire is an absolute time, so get the current time and add it to our delay time
    gettimeofday(&tp, NULL);
    timetoexpire.tv_sec = tp.tv_sec + seconds; timetoexpire.tv_nsec = tp.tv_usec * 1000;
    
    pthread_mutex_lock (&mutex);
    int res =  pthread_cond_timedwait(&conditionvar, &mutex, &timetoexpire);
    pthread_mutex_unlock (&mutex);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&conditionvar);
    
    //Upon successful completion, a value of zero shall be returned
    return res;
}

int main(int argc,char **argv){


    // -p (float) => sets p
    // -t (int) => simulation time in seconds
    // -s (int) => change the random seed
    for(int i=1; i<argc; i++){
        if(!strcmp(argv[i], "-p")) {p = atof(argv[++i]);}
        else if(!strcmp(argv[i], "-t")) {simulationTime = atoi(argv[++i]);}
        else if(!strcmp(argv[i], "-s"))  {seed = atoi(argv[++i]);}
        else if(!strcmp(argv[i], "-n"))  {n = atoi(argv[++i]);}
      
    }
    
    srand(seed); // feed the seed
    
    /* Queue usage example
        Queue *myQ = ConstructQueue(1000);
        Job j;
        j.ID = myID;
        j.type = 2;
        Enqueue(myQ, j);
        Job ret = Dequeue(myQ);
        DestructQueue(myQ);
    */

    // your code goes here





    start_time = time(NULL);
    end_time = start_time + simulationTime;
    

  // Thread Initialization
	pthread_t tower_thread;
	pthread_t padA_thread;
	pthread_t padB_thread;
	pthread_t launching_thread;
	pthread_t landing_thread;
	pthread_t assembly_thread;

	//Part 3 emergency thread 
	pthread_t emergency_thread;
	//Part 4 log thread
	pthread_t printing_thread;
  
	// Queue Initialization
	int limit = 999;
	landing_queue = ConstructQueue(limit);
  launching_queue = ConstructQueue(limit);
  assembly_queue = ConstructQueue(limit);
  padA_queue = ConstructQueue(limit);
  padB_queue = ConstructQueue(limit);
  //PArt 3 emergency que
  Emergency_queue = ConstructQueue(limit);

  // First job Initialization
  Job job;
  job.ID=counter;
  job.type = 2*t;
  job.arrival_time = start_time-time(NULL);
  Enqueue(launching_queue,job);
	FILE *fp =fopen(LOG_FILE,"w");
  fprintf(fp,"EventID   Status   Request Time   End Time   Turnaround Time   Pad\n ---------------------------------------------------------------------\n"); 
  fclose(fp);
    
	// Mutex Initialization
  pthread_mutex_init(&padA_mutex,NULL);
  pthread_mutex_init(&padB_mutex,NULL);
  pthread_mutex_init(&counter_mutex,NULL);
  pthread_mutex_init(&landing_mutex,NULL);
 	pthread_mutex_init(&launching_mutex,NULL);
 	pthread_mutex_init(&assembly_mutex,NULL);
	pthread_mutex_init(&file_mutex,NULL);
	pthread_mutex_init(&Emergency_mutex,NULL); // Emergency mutex

  // Thread Creation
  pthread_create(&tower_thread, NULL, ControlTower, NULL);
  pthread_create(&padA_thread, NULL, Pad_A, NULL);
  pthread_create(&padB_thread, NULL, Pad_B, NULL);
	pthread_create(&landing_thread, NULL, LandingJob, NULL);
  pthread_create(&launching_thread, NULL, LaunchJob, NULL);
  pthread_create(&assembly_thread, NULL, AssemblyJob, NULL);
  pthread_create(&emergency_thread, NULL, EmergencyJob, NULL); // Emergency thread
	pthread_create(&printing_thread, NULL, printQueues, NULL);

  // Joining Threads
  pthread_join(tower_thread,NULL);
  pthread_join(padA_thread,NULL);
  pthread_join(padB_thread,NULL);
  pthread_join(landing_thread,NULL);
  pthread_join(launching_thread,NULL);
  pthread_join(assembly_thread,NULL);
  pthread_join(emergency_thread,NULL);
	pthread_join(printing_thread,NULL);
	  


  // Destroying Threads
  pthread_mutex_destroy(&padA_mutex);
  pthread_mutex_destroy(&padB_mutex);
  pthread_mutex_destroy(&counter_mutex);
  pthread_mutex_destroy(&landing_mutex);
  pthread_mutex_destroy(&launching_mutex);
  pthread_mutex_destroy(&assembly_mutex);
  pthread_mutex_destroy(&Emergency_mutex);


  // Destructing Queues
  DestructQueue(landing_queue);
  DestructQueue(launching_queue);
  DestructQueue(assembly_queue);
  DestructQueue(padA_queue);
  DestructQueue(padB_queue);
  DestructQueue(Emergency_queue);



    return 0;
}

// The function that creates plane threads for landing
void* LandingJob(void *arg){
	srand(seed); // feed the seed
	time_t currentTime = time(NULL);	
	while(currentTime < end_time){
		pthread_sleep(t);

		double random = (double) rand()/RAND_MAX;
		//printf(" %f \n",random);
		if(random < (1-p)){

			pthread_mutex_lock(&counter_mutex);
			counter++;
			Job job;
			job.ID = counter;
			job.type = t;
			pthread_mutex_unlock(&counter_mutex);
			
			job.arrival_time = time(NULL) - start_time ;
			pthread_mutex_lock(&landing_mutex);
			Enqueue(landing_queue,job);
			//printf("Landing, ID = %d, Arrival Time = %d \n",job.ID,job.arrival_time);
			pthread_mutex_unlock(&landing_mutex);

		}
	 currentTime = time(NULL);
	}

	return NULL;

}

// The function that creates plane threads for (launching)departure
void* LaunchJob(void *arg){
	srand(seed); // feed the seed
	time_t currentTime = time(NULL);	
	while(currentTime < end_time){
		pthread_sleep(t);

	    double random = (double) rand()/RAND_MAX;
		if(random < (p/2)){

			pthread_mutex_lock(&counter_mutex);
			counter++;
			Job job;
			job.ID = counter;
			job.type = 2*t;
			pthread_mutex_unlock(&counter_mutex);
			
			job.arrival_time = time(NULL) - start_time;
			pthread_mutex_lock(&launching_mutex);
			Enqueue(launching_queue,job);
			//printf("Launching, ID = %d, Arrival Time = %d \n",job.ID,job.arrival_time);
			pthread_mutex_unlock(&launching_mutex);

		}
	 currentTime = time(NULL);
	}

	return NULL;


}

// The function that creates plane threads for emergency landing
void* EmergencyJob(void *arg){

	time_t currentTime = time(NULL);	
	while(currentTime < end_time){

		pthread_sleep(t);
		int time_for_emergency = (time(NULL)-start_time)%(emergencyFrequency*t);

		if(time_for_emergency==0){

			pthread_mutex_lock(&counter_mutex);

			counter++;
			Job job;
			job.ID = counter;
			job.type = t;
			job.is_emergency = 1;
			job.arrival_time = time(NULL) - start_time;

			pthread_mutex_lock(&Emergency_mutex);
			Enqueue(Emergency_queue,job);
		//	printf("Emergency, ID = %d, Arrival Time = %d \n",job.ID,job.arrival_time);
			pthread_mutex_unlock(&Emergency_mutex);

			counter++;
			Job job2;
			job2.ID = counter;
			job2.type = t;
			job2.is_emergency = 1;
			job2.arrival_time = time(NULL) - start_time;

			pthread_mutex_unlock(&counter_mutex);
			
			job2.arrival_time = time(NULL) - start_time;

			pthread_mutex_lock(&Emergency_mutex);
			Enqueue(Emergency_queue,job2);
			//printf("Emergency, ID = %d, Arrival Time = %d \n",job2.ID,job2.arrival_time);
			pthread_mutex_unlock(&Emergency_mutex);

		}
	 currentTime = time(NULL);
	}


	return NULL;
}

// The function that creates plane threads for emergency landing
void* AssemblyJob(void *arg){
	srand(seed); // feed the seed
	time_t currentTime = time(NULL);
	while(currentTime < end_time){
		pthread_sleep(t);

	    double random = (double) rand()/RAND_MAX;

		if(random < (p/2)){

			pthread_mutex_lock(&counter_mutex);
			counter++;
			Job job;
			job.ID = counter;
			job.type = 6*t;
			pthread_mutex_unlock(&counter_mutex);
			
			job.arrival_time = time(NULL) - start_time ;
			pthread_mutex_lock(&assembly_mutex);
			Enqueue(assembly_queue,job);
		//	printf("Assembly, ID = %d Arrival Time = %d \n",job.ID,job.arrival_time);
			pthread_mutex_unlock(&assembly_mutex);

		}
	 currentTime = time(NULL);
	}


	return NULL;
}

// The control tower method
void* ControlTower(void *arg){

	time_t current_time = time(NULL);
    while(current_time < end_time){

      // Emergency Job Handling
    	pthread_mutex_lock(&Emergency_mutex);
    	while((Emergency_queue->size>0)){
    	
            
            pthread_mutex_lock(&padA_mutex);
            pthread_mutex_lock(&padB_mutex);

            int Pad_A_sleep_rest = (padA_before_sleep + PadA_sleep_job_type) - time(NULL);
            int Pad_B_sleep_rest = (padB_before_sleep + PadB_sleep_job_type) - time(NULL);

            if((Pad_B_sleep_rest<Pad_A_sleep_rest+t)){
            
            Enqueue_Front(padA_queue, Dequeue(Emergency_queue));
            Enqueue_Front(padB_queue, Dequeue(Emergency_queue));
        	
        	}else{

        		Enqueue_Front(padA_queue, Dequeue(Emergency_queue));
        		Enqueue_Front(padA_queue, Dequeue(Emergency_queue));
        	} 
        
            // unlock padAQueue and padBQueue
            pthread_mutex_unlock(&padA_mutex);
            pthread_mutex_unlock(&padB_mutex);
        }
        pthread_mutex_unlock(&Emergency_mutex);
        
      pthread_mutex_lock(&launching_mutex);
    	pthread_mutex_lock(&assembly_mutex);
      // Part 2 Handling
       	while (launching_queue->size>2 || assembly_queue->size>2){
        	pthread_mutex_unlock(&launching_mutex);
        	pthread_mutex_unlock(&assembly_mutex);
          // Checking emergency jobs while handling part 2
        	pthread_mutex_lock(&Emergency_mutex);
            if ((Emergency_queue->size>0)){
            	pthread_mutex_unlock(&Emergency_mutex);
            	break;
            }
            pthread_mutex_unlock(&Emergency_mutex);

          // Launching Job Handling
        	pthread_mutex_lock(&launching_mutex);
        	if(launching_queue->size>2){
        		pthread_mutex_lock(&padA_mutex);
        		Enqueue(padA_queue, Dequeue(launching_queue));
        		pthread_mutex_unlock(&padA_mutex);
        		pthread_mutex_unlock(&launching_mutex);
        	}
        	pthread_mutex_unlock(&launching_mutex);
          // Assembly Job Handling
        	pthread_mutex_lock(&assembly_mutex);
        	if(assembly_queue->size>2){
        		pthread_mutex_lock(&padB_mutex);
        		Enqueue(padB_queue, Dequeue(assembly_queue));
        		pthread_mutex_unlock(&padB_mutex);
        	}
        	pthread_mutex_unlock(&assembly_mutex);

	        pthread_mutex_lock(&launching_mutex);
	    	pthread_mutex_lock(&assembly_mutex);
        }

		  pthread_mutex_unlock(&launching_mutex);
    	pthread_mutex_unlock(&assembly_mutex);
        // Landing Queue Handling Part 1
        pthread_mutex_lock(&landing_mutex);
        while(!isEmpty(landing_queue)){
        	pthread_mutex_lock(&launching_mutex);
        	pthread_mutex_lock(&assembly_mutex);
        	pthread_mutex_lock(&Emergency_mutex);
            if (launching_queue->size>2 || assembly_queue->size>2 || (Emergency_queue->size>0)){
            	pthread_mutex_unlock(&Emergency_mutex);
            	pthread_mutex_unlock(&launching_mutex);
            	pthread_mutex_unlock(&assembly_mutex);
            	break;
            }
            pthread_mutex_unlock(&Emergency_mutex);
            pthread_mutex_unlock(&launching_mutex);
        	pthread_mutex_unlock(&assembly_mutex);


            pthread_mutex_lock(&padA_mutex);
            pthread_mutex_lock(&padB_mutex);
            // Determining which pad should take the job
            if(getTotalTime(padA_queue) <= getTotalTime(padB_queue)){
                Enqueue(padA_queue, Dequeue(landing_queue));
            }
            else{
                Enqueue(padB_queue, Dequeue(landing_queue));
            }
            // Unlocking padA mutex and padB mutex
            pthread_mutex_unlock(&padA_mutex);
            pthread_mutex_unlock(&padB_mutex);
        }
        // Unlocking the landing queue
        pthread_mutex_unlock(&landing_mutex);

        // Unlocking the padA mutex and launch mutex and emergency mutex
        pthread_mutex_lock(&padA_mutex);
        pthread_mutex_lock(&launching_mutex);
        pthread_mutex_lock(&Emergency_mutex);

        if (isEmpty(padA_queue) && !isEmpty(launching_queue) && (Emergency_queue->size<=0)) {
            Enqueue(padA_queue, Dequeue(launching_queue));
        }

        // Unlocking the padA mutex and launch mutex and emergency mutex
        pthread_mutex_unlock(&Emergency_mutex);
        pthread_mutex_unlock(&padA_mutex);
        pthread_mutex_unlock(&launching_mutex);

        // Locking the padB mutex and assembly mutex and emergency mutex
        pthread_mutex_lock(&padB_mutex);
        pthread_mutex_lock(&assembly_mutex);
        pthread_mutex_lock(&Emergency_mutex);

        if (isEmpty(padB_queue) && !isEmpty(assembly_queue) && (Emergency_queue->size<=0) ) {
            Enqueue(padB_queue, Dequeue(assembly_queue));
        }

        // Unlock the padB mutex and assembly mutex
        pthread_mutex_unlock(&Emergency_mutex);
        pthread_mutex_unlock(&padB_mutex);
        pthread_mutex_unlock(&assembly_mutex);
		current_time = time(NULL);
    }
    return NULL;
}


// The function that controls the Pad_A
void* Pad_A(){

	time_t currentTime = time(NULL);
		
	while(currentTime < end_time){

		pthread_mutex_lock(&padA_mutex);

		if (padA_queue->size <= 0){

			pthread_mutex_unlock(&padA_mutex);
			pthread_sleep(t);

		}

		else{
      
			Job job=Dequeue(padA_queue);
			pthread_mutex_unlock(&padA_mutex);

			padA_before_sleep = time(NULL);
			PadA_sleep_job_type = job.type;

			pthread_sleep(job.type); //this is the only new line

			padA_before_sleep = time(NULL);

			job.termination_time = time(NULL) - start_time;
			int turnaround = job.termination_time-job.arrival_time;
    			
			recordLogs(job, 'A');
			


		}

		currentTime = time(NULL);
	}

	

	return NULL;	

}


// the function that controls the Pad_B
void* Pad_B(){

	time_t currentTime = time(NULL);
		
	while(currentTime < end_time){

		pthread_mutex_lock(&padB_mutex);

		if (padB_queue->size <= 0){

			pthread_mutex_unlock(&padB_mutex);
			pthread_sleep(t);

		}

		else{

			Job job=Dequeue(padB_queue);
			pthread_mutex_unlock(&padB_mutex);

			padB_before_sleep = time(NULL);
			PadB_sleep_job_type = job.type;

			pthread_sleep(job.type);
			padB_before_sleep = time(NULL);

			job.termination_time = time(NULL) - start_time;
			int turnaround = job.termination_time-job.arrival_time;

			recordLogs(job,'B');
			
		}

		currentTime = time(NULL);
	}


	return NULL;

}

// The method which prints log statements into file called events.log
void recordLogs(Job job, char pad){
	pthread_mutex_lock(&file_mutex);
	FILE* file;
	file = fopen(LOG_FILE, "a+");
	char status;
	
	if(job.type == t && (job.is_emergency == 1)){
		status = 'E';
	}
	else if(job.type == t){
		status = 'L';
	}
	else if(job.type == 2*t){
		status = 'D';
	}
	else{
		status = 'A';
	}
	
	char buffer[150];
	sprintf(buffer, "   %-3d%9c%10d%12d%15d%13c \n",job.ID, status, job.arrival_time, job.termination_time, job.termination_time-job.arrival_time, pad);
    fprintf(file, "%s", buffer);

    fclose(file);
	pthread_mutex_unlock(&file_mutex);
}

// The method which prints queues at current time into the console
void* printQueues(void* arg){

	time_t current_time = time(NULL);
	while(current_time < end_time){
		
		pthread_sleep(1);
    current_time = time(NULL);
		int time_s = current_time - start_time;

		if(time_s >= n){
			
			pthread_mutex_lock(&landing_mutex);
			printf("At %d sec landing : ", time_s);
			printQueue(landing_queue);
			printf("\n");
			pthread_mutex_unlock(&landing_mutex);

			pthread_mutex_lock(&launching_mutex);
			printf("At %d sec launching : ", time_s);
			printQueue(launching_queue);
			printf("\n");
			pthread_mutex_unlock(&launching_mutex);

			pthread_mutex_lock(&assembly_mutex);
			printf("At %d sec assembly : ", time_s);
			printQueue(assembly_queue);
			printf("\n");
			pthread_mutex_unlock(&assembly_mutex);

			pthread_mutex_lock(&padA_mutex);
			printf("At %d sec padA : ", time_s);
			printQueue(padA_queue);
			printf("\n");
			pthread_mutex_unlock(&padA_mutex);

			pthread_mutex_lock(&padB_mutex);
			printf("At %d sec padB : ", time_s);
			printQueue(padB_queue);
			printf("\n\n");
			pthread_mutex_unlock(&padB_mutex);



		}
		
	}
}
