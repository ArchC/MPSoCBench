#include <stdio.h>

#ifndef acPthread_H_
#include "../../acPthread.h"
#endif
extern FILE *fileout_dijkstra;
extern FILE *filein_dijkstra;

#define malloc acPthread_malloc
#define realloc acPthread_realloc
#define free acPthread_free

extern pthread_mutex_t mutex_print;
//extern pthread_mutex_t mutex_malloc;

#include <stdio.h>

#define NUM_NODES                          70
#define NONE                               9999

struct _NODE
{
  int iDist;
  int iPrev;
};
typedef struct _NODE NODE;

struct _QITEM
{
  int iNode;
  int iDist;
  int iPrev;
  //struct _QITEM *qNext;
};

typedef struct _QITEM QITEM;

#define qSize 500

QITEM static_queue[qSize];
int qBegin = -1;
int qTail = -1;


//QITEM *qHead = NULL;

             
             
             
int AdjMatrix[NUM_NODES][NUM_NODES];

int g_qCount = 0;
NODE rgnNodes[NUM_NODES];
int ch;
int iPrev, iNode;
int i, iCost, iDist;


void print_path (NODE *rgnNodes, int chNode)
{
  if (rgnNodes[chNode].iPrev != NONE)
    {
      print_path(rgnNodes, rgnNodes[chNode].iPrev);
    }
  fprintf (fileout_dijkstra," %d", chNode);
  //fflush(stdout);
}


void enqueue_static (int iNode, int iDist, int iPrev)
{
  //QITEM *qNew = (QITEM *) malloc(sizeof(QITEM));
  //QITEM *qLast = qHead;


  qTail = (qTail + 1)%qSize;

  if (qBegin == qTail)
  {
   	fprintf(stderr, "The queue is full.\n");
        //_exit(0); 
        exit(0); // avoiding warnings
  }
    
  static_queue[qTail].iNode = iNode;
  static_queue[qTail].iDist = iDist;
  static_queue[qTail].iPrev = iPrev;


  //qNew->iNode = iNode;
  //qNew->iDist = iDist;
  //qNew->iPrev = iPrev;
  //qNew->qNext = NULL;


  if (qBegin == -1) qBegin = 0;
  
  //if (!qLast) 
  //  {
  //    qHead = qNew;
  //  }
  //else
  //  {
  //    while (qLast->qNext) qLast = qLast->qNext;
  //    qLast->qNext = qNew;
  //  }

  g_qCount++;

  //               ASSERT(g_qCount);
}


void dequeue_static (int *piNode, int *piDist, int *piPrev)
{
  //QITEM *qKill = qHead;
  
  if(qBegin == -1) 
  {
	fprintf(stderr, "The queue is empty.\n");
        //_exit(0); 
        exit(0); // avoiding warnings
  }

  *piNode = static_queue[qBegin].iNode;
  *piDist = static_queue[qBegin].iDist;
  *piPrev = static_queue[qBegin].iPrev;
  
  if (qBegin == qTail) qBegin = qTail = -1;
  else qBegin = (qBegin+1)%qSize;

  g_qCount--;

}



int qcount (void)
{
  return(g_qCount);
}


int dijkstra(int chStart, int chEnd) 
{
  

  
  for (ch = 0; ch < NUM_NODES; ch++)
    {
      rgnNodes[ch].iDist = NONE;
      rgnNodes[ch].iPrev = NONE;
    }

  if (chStart == chEnd) 
    {
	pthread_mutex_lock(&mutex_print);      
	printf("Shortest path is 0 in cost. Just stay where you are.\n");
	pthread_mutex_unlock(&mutex_print);
    }
  else
    {
      rgnNodes[chStart].iDist = 0;
      rgnNodes[chStart].iPrev = NONE;
      
      enqueue_static (chStart, 0, NONE);
      
     while (qcount() > 0)
	{
	  dequeue_static (&iNode, &iDist, &iPrev);
	  for (i = 0; i < NUM_NODES; i++)
	    {
	      if ((iCost = AdjMatrix[iNode][i]) != NONE)
		{
		  if ((NONE == rgnNodes[i].iDist) || 
		      (rgnNodes[i].iDist > (iCost + iDist)))
		    {
		      rgnNodes[i].iDist = iDist + iCost;
		      rgnNodes[i].iPrev = iNode;
		      enqueue_static (i, iDist + iCost, iNode);
		    }
		}
	    }
	}

      pthread_mutex_lock(&mutex_print);
      fprintf(fileout_dijkstra,"Shortest path is %d in cost. ", rgnNodes[chEnd].iDist);
      fprintf(fileout_dijkstra,"Path is: ");
      print_path(rgnNodes, chEnd);
      fprintf(fileout_dijkstra,"\n");
      pthread_mutex_unlock(&mutex_print);
      
    }
    return 1;
}

int main_dijkstra(int argc, char *argv[]) {

 int i,j,k;

 for (i=0;i<NUM_NODES;i++) 
    for (j=0;j<NUM_NODES;j++) 
        AdjMatrix[i][j]=NONE;

 
 /* make a fully connected matrix */
  for (i=0;i<NUM_NODES;i++) {
    for (j=i+1;j<NUM_NODES;j++) {
      fscanf(filein_dijkstra,"%d",&k);
      AdjMatrix[i][j] = k;
      AdjMatrix[j][i] = k;
      //printf("%d ",AdjMatrix[i][j]);
    }
      //printf("\n ");
  }



//  /* make a fully connected matrix */
//  for (i=0;i<NUM_NODES;i++) {
//    for (j=i+1;j<NUM_NODES;j++) {
//      /* make it more sparce */
//      fscanf(filein_dijkstra,"%d",&k);
//      AdjMatrix[i][j] = k;
//    }
//      //printf("\n ");
//  }

   /*for (i=0;i<NUM_NODES;i++) {
    for (j=0;j<NUM_NODES;j++) {
     
      printf("%d ",AdjMatrix[i][j]);
    }
    printf("\n ");
  }*/

  /* finds 10 shortest paths between nodes */
 for (i=0,j=NUM_NODES/2;i<NUM_NODES;i++,j++) {
      j=j%NUM_NODES;
      dijkstra(i,j);
  }


  /* finds 10 shortest paths between nodes */
//  for (i=0,j=NUM_NODES/2;i<NUM_NODES;i++,j++) {
//      j=j%NUM_NODES;
//      dijkstra(i,j);
//  }
  //exit(0);
}








