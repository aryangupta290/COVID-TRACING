#include "header.h"
#include <stdio.h>
#include <stdlib.h>


struct Node* create_Node(int v) 
{
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}


struct Graph* create_Graph(int N) 
{
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->total_vertices = N;
    graph->adjLists = malloc(N * sizeof(struct Node*)); 
    int i;
    for (i = 0; i < N; i++)
        graph->adjLists[i] = NULL;
    return graph;
}

void addEdge(struct Graph* graph, int s, int d, int w) 
{
    struct Node* newNode = create_Node(d);
    newNode->next = graph->adjLists[s];
    graph->adjLists[s] = newNode;
    graph->adjLists[s]->weight=w;
  
    newNode = create_Node(s);
    newNode->next = graph->adjLists[d];
    graph->adjLists[d] = newNode;
    graph->adjLists[d]->weight=w;
}

void print_Graph(struct Graph* graph) 
{
    int v;
    for (v = 0; v < graph->total_vertices; v++) 
    {
        struct Node* temp = graph->adjLists[v];
        printf("\n Vertex %d: ", v);
        while (temp) 
        {
            printf("%d -> %d(%d)   ", v,temp->vertex,temp->weight);
            temp = temp->next;
        }
    printf("\n");
    }
}

_day_queue* init_queue()
{
    _day_queue* Queue=(void *)malloc(sizeof(_day_queue));
    Queue->rear=-1;
    // for(int i=0;i<DAY_MAX;i++)  //intitalizing to empty day
    //     delete_day(Queue.array[i]);  
    return Queue;
}

void enqueue(_day_queue* Queue, _day D)
{
    Queue->rear++;
    if(Queue->rear==DAY_MAX)
        Queue->rear=0;
    //delete_day(Queue->array[Queue->rear]); 
    Queue->array[Queue->rear]= D;
}
