#include <stdlib.h>
#include <stdio.h>

#define TRUE  1
#define FALSE 0

typedef struct {
    int ID;
    int type;
    int arrival_time; // Request time 
    int termination_time; // End of job execution
    int is_emergency; // Flag which identifies emergency job 1 if it is emergency
    // you might want to add variables here!
} Job;

/* a link in the queue, holds the data and point to the next Node */
typedef struct Node_t {
    Job data;
    struct Node_t *prev;
} NODE;

/* the HEAD of the Queue, hold the amount of node's that are in the queue */
typedef struct Queue {
    NODE *head;
    NODE *tail;
    int size;
    int limit;
} Queue;

Queue *ConstructQueue(int limit);
void DestructQueue(Queue *queue);
int Enqueue(Queue *pQueue, Job j);
Job Dequeue(Queue *pQueue);
int isEmpty(Queue* pQueue);
int getTotalTime(Queue *queue);
int Enqueue_Front(Queue *pQueue, Job j);
void printQueue(Queue *queue);

// The method for printhing queues into console
void printQueue(Queue *queue){
    NODE *tmp;
    tmp = queue->head;
    while(tmp != NULL){
        if(tmp->data.is_emergency == 1){
            printf("%d(E) ",tmp->data.ID);
        }
        else{
            if(tmp->data.type == 2){
                printf("%d(L) ",tmp->data.ID);
            }
            else if(tmp->data.type == 4){
                printf("%d(D) ",tmp->data.ID);
            }
            else if(tmp->data.type == 12){
                printf("%d(A) ",tmp->data.ID);
            }
        }

        tmp = tmp->prev;
    }

}
// The method which calculates total time of jobs in the queue
int getTotalTime(Queue *queue){
    int totalTime = 0;
    NODE *tmp;
    tmp = queue->head;
    while(tmp != NULL){
        totalTime += tmp->data.type;
        tmp = tmp->prev;
    }

    return totalTime;
}
Queue *ConstructQueue(int limit) {
    Queue *queue = (Queue*) malloc(sizeof (Queue));
    if (queue == NULL) {
        return NULL;
    }
    if (limit <= 0) {
        limit = 65535;
    }
    queue->limit = limit;
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void DestructQueue(Queue *queue) {
    NODE *pN;
    while (!isEmpty(queue)) {
        Dequeue(queue);
    }
    free(queue);
}

int Enqueue(Queue *pQueue, Job j) {
    /* Bad parameter */
    NODE* item = (NODE*) malloc(sizeof (NODE));
    item->data = j;

    if ((pQueue == NULL) || (item == NULL)) {
        return FALSE;
    }
    // if(pQueue->limit != 0)
    if (pQueue->size >= pQueue->limit) {
        return FALSE;
    }
    /*the queue is empty*/
    item->prev = NULL;
    if (pQueue->size == 0) {
        pQueue->head = item;
        pQueue->tail = item;

    } else {
        /*adding item to the end of the queue*/
        pQueue->tail->prev = item;
        pQueue->tail = item;
    }
    pQueue->size++;
    return TRUE;
}

Job Dequeue(Queue *pQueue) {
    /*the queue is empty or bad param*/
    NODE *item;
    Job ret;
    if (isEmpty(pQueue))
        return ret;
    item = pQueue->head;
    pQueue->head = (pQueue->head)->prev;
    pQueue->size--;
    ret = item->data;
    free(item);
    return ret;
}

int isEmpty(Queue* pQueue) {
    if (pQueue == NULL) {
        return FALSE;
    }
    if (pQueue->size == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}
// The method which add job in front of a queue
int Enqueue_Front(Queue *pQueue, Job j) {
    /* Bad parameter */
    NODE* temp_head = (NODE*) malloc(sizeof (NODE));
    NODE* item = (NODE*) malloc(sizeof (NODE));
    item->data = j;

    if ((pQueue == NULL) || (item == NULL)) {
        return FALSE;
    }
    // if(pQueue->limit != 0)
    if (pQueue->size >= pQueue->limit) {
        return FALSE;
    }
    /*the queue is empty*/
    item->prev = NULL;
    if (pQueue->size == 0) {
        pQueue->head = item;
        pQueue->tail = item;

    } else {
        /*adding item to the front of the queue*/
        temp_head = pQueue->head;
        temp_head->prev = pQueue->head->prev;
        pQueue->head = item;
        pQueue->head->prev = temp_head;
    }
    pQueue->size++;
    return TRUE;
}