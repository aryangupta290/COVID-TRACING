// A utility function to create
// a new adjacency list node
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "header.h"
struct AdjListNode *newAdjListNode(
	int dest, int weight)
{
	struct AdjListNode *newNode =
		(struct AdjListNode *)
			malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;
	return newNode;
}

// A utility function that creates
// a graph of V vertices
struct Graph *createGraph(int V)
{
	struct Graph *graph = (struct Graph *)
		malloc(sizeof(struct Graph));
	graph->V = V;

	// Create an array of adjacency lists.
	// Size of array will be V
	graph->array = (struct AdjList *)
		malloc(V * sizeof(struct AdjList));

	// Initialize each adjacency list
	// as empty by making head as NULL
	for (int i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph *graph, int src,
			 int dest, int weight)
{
	// Add an edge from src to dest.
	// A new node is added to the adjacency
	// list of src.  The node is
	// added at the beginning
	struct AdjListNode *newNode =
		newAdjListNode(dest, weight);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;

	// Since graph is undirected,
	// add an edge from dest to src also
	newNode = newAdjListNode(src, weight);
	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
}

_day_queue *init_queue()
{
	_day_queue *Queue = (void *)malloc(sizeof(_day_queue));
	Queue->rear = -1;
	// for(int i=0;i<DAY_MAX;i++)  //intitalizing to empty day
	//     delete_day(Queue.array[i]);
	return Queue;
}

void enqueue(_day_queue *Queue, _day D)
{
	Queue->rear++;
	if (Queue->rear == DAY_MAX)
		Queue->rear = 0;
	//delete_day(Queue->array[Queue->rear]);
	Queue->array[Queue->rear] = D;
}
/*  
/////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
void UpdateData(int id_person, int id_station, int daye)
{
	if (day[daye].person[id_person].status >= 2 || day[daye].station[id_station].worst_affected >= 2)
	{
		if (day[daye].person[id_person].status > day[daye].station[id_station].worst_affected)
		{
			UpdateStation(id_person, id_station, daye);
		}
		// ---> person coming to the station is more infected than whole station.
		else if (day[daye].person[id_person].status < day[daye].station[id_station].worst_affected)
		{
			UpdatePerson(id_person, id_station, daye);
		}
		// ---> person coming to the station is less infected than whole station.
		// If their status are same, then do nothing
	}
}
void UpdateStation(int id_person, int id_station, int daye)
{
	_list *L;
	L = day[daye].station[id_station].list;
	while (L != NULL)
	{
		day[daye].person[L->person_id].status = day[daye].person[id_person].status - 1;
		day[daye].person[L->person_id].cause = id_person;
		day[daye].person[L->person_id].days = 0;
		L = L->next;
	}
	// Changed the status of all people on the station.
	day[daye].station[id_station].worst_affected = day[daye].person[id_person].status - 1;
}
void UpdatePerson(int id_person, int id_station, int daye)
{
	_list *L;
	day[daye].person[id_person].status = day[daye].station[id_station].worst_affected - 1;
	L = day[daye].station[id_station].list;
	while (L != NULL)
	{
		if (day[daye].person[L->person_id].status == day[daye].station[id_station].worst_affected)
			break;
		L = L->next;
	}
	//iterating through all the people on the station to find one of the people who are worst affected.
	//The first such person will be the cause.
	if (L == NULL)
	{
		fprintf(stderr, "ERROR!!, no one with worst affected status");
		return;
	}
	day[daye].person[id_person].cause = L->person_id; // cause will be any arbitrary person on the station who has status same as worst affected.
	day[daye].person[id_person].days = 0;
}
void UpdateRoute(_route *R, int id_person, int daye)
{
	while (R != NULL)
	{
		UpdateData(id_person, R->station_id, daye);
		R = R->next_station;
	}
}
void RemovePerson(int id_person, int station_from, int daye)
{
	// first check if person is at station_from, and remove it from the station linked list.

	_list *L;
	L = day[daye].station[station_from].list;
	L = DeleteNodeById(L, id_person);
	day[daye].station[station_from].list = L;

	// then update the station's danger value.
	day[daye].station[station_from].danger_value -= getDangerIndex(id_person, daye);

	// check if person moved was worst affected, in that case, update worst affected status
	if (day[daye].person[id_person].status == day[daye].station[station_from].worst_affected)
		day[daye].station[station_from].worst_affected = getWorstAffected(station_from, daye);
}
void AddPerson(int id_person, int station_to, int daye)
{
	// Therefore we have to update the danger value, worst affected and id_person to the list.

	_list *L = day[daye].station[station_to].list;
	L = AddNodeAtStart(L, id_person);
	day[daye].station[station_to].list = L;
	day[daye].station[station_to].danger_value += getDangerIndex(id_person, daye);
	if (day[daye].person[id_person].status > day[daye].station[station_to].worst_affected)
		day[daye].station[station_to].worst_affected += 1;

	// update the location of the person:
	day[daye].person[id_person].station = station_to;
}
*/
/* The functions above are to be used for mainly two purposes.
 * We have to 
	* 1. Update the status everyday a person travels from one place to another.
	* 2. BackTrace and update the existing data based on new information given in the form of query 1.
* for the first type of update, we have to work route by route.
* given a route, we should first remove the person from that particular station, update the station variable.
* then, visit all the stations given by the route linked list and update the status of the person or the stations he visited accordingly.
* finally we have to add the person to the destination station. this is done by updating the linked list for the station struct and also the location of the person.
*
*
*
* for the second type of update, we have to work day by day.
* given that a list of people were covid positive since a particular day, we must look at all the travels made by peole on each day.
* for one day, we have to traverse the path linked list of linked lists and and update the values accordingly and print the results
*/
/*  
void UpdateForPerson(_route *L, int id_person, int daye)
{
	int station_from = L->station_id;
	int station_to;
	RemovePerson(id_person, station_from, daye);
	UpdateRoute(L->next_station, id_person, daye);
	while (L != NULL)
	{
		station_to = L->station_id;
		L = L->next_station;
	}
	AddPerson(id_person, station_to, daye);
}
void UpdateForDay(_path *P, int daye)
{
	while (P != NULL)
	{
		UpdateRoute(P->next_station, P->person_id, daye);
		P = P->next_person;
	}
}
void Backtrace(int start_day, int end_day)
{
	_path *P;
	for (int i = start_day; i <= end_day; i++)
	{
		P = day[i].path;
		UpdateForDay(P, i);
	}
}
///////////////////////////////////////////////////////////////
#include "header.h"
#include <stdlib.h>
#include <stdio.h>
const char *status_mapping[] = {
	"negative", "secondary_contact", "primary_contact", "positive"};
float getDangerIndex(int id_person, int daye)
{
	int severity = day[daye].person[id_person].status;
	switch (severity)
	{
	case 0:
		return 0;
	case 1:
		return 0.1;
	case 2:
		return 0.2;
	case 3:
		return 1;
	default:
		fprintf(stderr, "Error, status out of specified range\n");
		return 0;
	}
}
int getWorstAffected(int id_station, int daye)
{
	_list *L;
	L = day[daye].station[id_station].list;
	int iworst_affected = 0;
	while (L != NULL)
	{
		if (iworst_affected < day[daye].person[L->person_id].status)
			iworst_affected = day[daye].person[L->person_id].status;
		L = L->next;
	}
	return iworst_affected;
}
_list *AddNodeAtStart(_list *L, int id_person)
{
	_list *temp;
	temp = L;
	L = (_list *)malloc(sizeof(_list));
	L->person_id = id_person;
	L->next = temp;
	return L;
}
_list *DeleteNodeById(_list *L, int id_person)
{
	_list *start;
	start = L;
	_list *temp;
	if (L->person_id == id_person)
	{
		temp = L->next;
		free(L);
		return temp;
	}
	while (L->next != NULL)
	{
		if (L->person_id == id_person)
		{
			temp = L->next->next;
			free(L->next);
			L->next = temp;
		}
	}
	return start;
}
void PrintStatus(int n)
{
	printf("%s", status_mapping[n]);
}

*/
/////////////////// Following is the code for the 3-way dijkstra implementation
#include <limits.h>
#define maxi 1000000000000000
typedef long long int ll;
// A utility function to create
// a new adjacency list node
// A utility function that creates
// a graph of V vertices
// A utility function to create a
// new Min Heap Node
struct MinHeapNode *newMinHeapNode(int v,
								   double dist)
{
	struct MinHeapNode *minHeapNode =
		(struct MinHeapNode *)
			malloc(sizeof(struct MinHeapNode));
	minHeapNode->v = v;
	minHeapNode->dist = dist;
	return minHeapNode;
}

// A utility function to create a Min Heap
struct MinHeap *createMinHeap(int capacity)
{
	struct MinHeap *minHeap =
		(struct MinHeap *)
			malloc(sizeof(struct MinHeap));
	minHeap->pos = (int *)malloc(
		capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array =
		(struct MinHeapNode **)
			malloc(capacity *
				   sizeof(struct MinHeapNode *));
	return minHeap;
}

// A utility function to swap two
// nodes of min heap.
// Needed for min heapify
void swapMinHeapNode(struct MinHeapNode **a,
					 struct MinHeapNode **b)
{
	struct MinHeapNode *t = *a;
	*a = *b;
	*b = t;
}

// A standard function to
// heapify at given idx
// This function also updates
// position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap *minHeap,
				int idx)
{
	int smallest, left, right;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size &&
		minHeap->array[left]->dist <
			minHeap->array[smallest]->dist)
		smallest = left;

	if (right < minHeap->size &&
		minHeap->array[right]->dist <
			minHeap->array[smallest]->dist)
		smallest = right;

	if (smallest != idx)
	{
		// The nodes to be swapped in min heap
		struct MinHeapNode *smallestNode =
			minHeap->array[smallest];
		struct MinHeapNode *idxNode =
			minHeap->array[idx];

		// Swap positions
		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] = smallest;

		// Swap nodes
		swapMinHeapNode(&minHeap->array[smallest],
						&minHeap->array[idx]);

		minHeapify(minHeap, smallest);
	}
}

// A utility function to check if
// the given minHeap is ampty or not
int isEmpty(struct MinHeap *minHeap)
{
	return minHeap->size == 0;
}

// Standard function to extract
// minimum node from heap
struct MinHeapNode *extractMin(struct MinHeap *
								   minHeap)
{
	if (isEmpty(minHeap))
		return NULL;

	// Store the root node
	struct MinHeapNode *root =
		minHeap->array[0];

	// Replace root node with last node
	struct MinHeapNode *lastNode =
		minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	// Update position of last node
	minHeap->pos[root->v] = minHeap->size - 1;
	minHeap->pos[lastNode->v] = 0;

	// Reduce heap size and heapify root
	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}

// Function to decreasy dist value
// of a given vertex v. This function
// uses pos[] of min heap to get the
// current index of node in min heap
void decreaseKey(struct MinHeap *minHeap,
				 int v, double dist)
{
	// Get the index of v in  heap array
	int i = minHeap->pos[v];

	// Get the node and update its dist value
	minHeap->array[i]->dist = dist;

	// Travel up while the complete
	// tree is not hepified.
	// This is a O(Logn) loop
	while (i && minHeap->array[i]->dist <
					minHeap->array[(i - 1) / 2]->dist)
	{
		// Swap this node with its parent
		minHeap->pos[minHeap->array[i]->v] =
			(i - 1) / 2;
		minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
		swapMinHeapNode(&minHeap->array[i],
						&minHeap->array[(i - 1) / 2]);

		// move to parent index
		i = (i - 1) / 2;
	}
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
int isInMinHeap(struct MinHeap *minHeap, int v)
{
	if (minHeap->pos[v] < minHeap->size)
		return 1;
	return 0;
}
// The main function that calulates
// distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
void dijkstra(struct Graph *graph, int src, double dist[], int curr_day, int parent[])
{

	// Get the number of vertices in graph
	int V = graph->V;
	//int weight[graph];
	for (int i = 0; i < V; i++)
	{
		parent[i] = -1;
	}
	// minHeap represents set E
	struct MinHeap *minHeap = createMinHeap(V);

	// Initialize min heap with all
	// vertices. dist value of all vertices
	for (int v = 0; v < V; ++v)
	{
		dist[v] = maxi;
		minHeap->array[v] = newMinHeapNode(v,
										   dist[v]);
		minHeap->pos[v] = v;
	}

	// Make dist value of src vertex
	// as 0 so that it is extracted first
	minHeap->array[src] =
		newMinHeapNode(src, dist[src]);
	minHeap->pos[src] = src;
	dist[src] = 0;
	decreaseKey(minHeap, src, dist[src]);

	// Initially size of min heap is equal to V
	minHeap->size = V;

	// In the followin loop,
	// min heap contains all nodes
	// whose shortest distance
	// is not yet finalized.
	while (!isEmpty(minHeap))
	{
		// Extract the vertex with
		// minimum distance value
		struct MinHeapNode *minHeapNode =
			extractMin(minHeap);

		// Store the extracted vertex number
		int u = minHeapNode->v;

		// Traverse through all adjacent
		// vertices of u (the extracted
		// vertex) and update
		// their distance values
		struct AdjListNode *pCrawl =
			graph->array[u].head;
		while (pCrawl != NULL)
		{
			int v = pCrawl->dest;

			// If shortest distance to v is
			// not finalized yet, and distance to v
			// through u is less than its
			// previously calculated distance
			if (isInMinHeap(minHeap, v) && dist[u] != maxi && dist[u] + day[cur_day + 1].station[pCrawl->dest].danger_value * 100000 + pCrawl->weight <= dist[v]) //day[curr_day + 1].station[temp->station_id].danger_value;
			{

				dist[v] = dist[u] + day[cur_day + 1].station[pCrawl->dest].danger_value * 100000 + pCrawl->weight; //day[curr_day + 1].station[temp->station_id].danger_value;
				parent[v] = u;
				// update distance
				// value in min heap also
				decreaseKey(minHeap, v, dist[v]);
			}
			pCrawl = pCrawl->next;
		}
	}

	// print the calculated shortest distances
	//printArr(dist, V, parent);
}

// Actual function
// This function will add the shortest path to possible_routes struct
void add_path_one(_possible_path *possible_routes, double dist[], int src, int target, int parent[])
{

	struct __route *temp = &(possible_routes->best_path);
	temp->station_id = src;
	temp->next_station = NULL;
	double *temp1 = &(possible_routes->best_path_danger_value);
	*temp1 = day[cur_day + 1].station[src].danger_value;
	int start = src;
	while (start != target)
	{
		temp->next_station = (struct __route *)malloc(sizeof(struct __route));
		temp = temp->next_station;
		temp->station_id = parent[start];
		*temp1 += day[cur_day + 1].station[parent[start]].danger_value;
		temp->next_station = NULL;
		start = parent[start];
	}
	return;
}
/// This function would allow us to trace the 2nd best path from src to target
void add_path_two(_possible_path *possible_routes, double dist[], int src, int target, int parent[], int index1, int divert1, int a)
{
	struct __route *temp;
	double *temp1;
	if (a)
	{
		temp = &(possible_routes->second_best_path);
		temp1 = &(possible_routes->second_best_path_danger_value);
		*temp1 = 0;
	}
	else
	{
		temp = &(possible_routes->third_best_path);
		temp1 = &(possible_routes->third_best_path_danger_value);
		*temp1 = 0;
	}
	temp->station_id = src;
	temp->next_station = NULL;
	*temp1 += day[cur_day + 1].station[src].danger_value;
	int start = src;
	if (index1 == start)
	{
		start = divert1;
		temp->next_station = (struct __route *)malloc(sizeof(struct __route));
		temp = temp->next_station;
		temp->station_id = divert1;
		temp->next_station = NULL;
		*temp1 += day[cur_day + 1].station[divert1].danger_value;
		if (divert1 != target)
		{
			start = divert1;
			while (start != target)
			{
				temp->next_station = (struct __route *)malloc(sizeof(struct __route));
				temp = temp->next_station;
				temp->station_id = parent[start];
				*temp1 += day[cur_day + 1].station[parent[start]].danger_value;
				temp->next_station = NULL;
				start = parent[start];
			}
			return;
		}
	}
	else
	{
		start = src;
		while (start != index1)
		{
			temp->next_station = (struct __route *)malloc(sizeof(struct __route));
			temp = temp->next_station;
			temp->station_id = parent[start];
			*temp1 += day[cur_day + 1].station[parent[start]].danger_value;
			temp->next_station = NULL;
			start = parent[start];
		}
		temp->next_station = (struct __route *)malloc(sizeof(struct __route));
		temp = temp->next_station;
		temp->station_id = divert1;
		*temp1 += day[cur_day + 1].station[divert1].danger_value;
		temp->next_station = NULL;
		start = divert1;
		if (start != target)
		{
			start = divert1;
			while (start != target)
			{
				temp->next_station = (struct __route *)malloc(sizeof(struct __route));
				temp = temp->next_station;
				temp->station_id = parent[start];
				*temp1 += day[cur_day + 1].station[parent[start]].danger_value;
				temp->next_station = NULL;
				start = parent[start];
			}
			return;
		}
	}
}
/// This function would allow us to trace the 3rd best path from src to target
void add_path_three(_possible_path *possible_routes, double dist[], int src, int target, int parent[], int index3, int divert3)
{
	struct __route *temp1;
	temp1 = &(possible_routes->second_best_path);
	temp1 = temp1->next_station;
	struct __route *temp2 = &(possible_routes->third_best_path);
	temp2->station_id = src;
	double *temp3;
	temp3 = &(possible_routes->third_best_path_danger_value);
	*temp3 += day[cur_day + 1].station[temp2->station_id].danger_value;
	temp2->next_station = NULL;
	int start = src;
	while (start != index3)
	{
		temp2->next_station = (struct __route *)malloc(sizeof(struct __route));
		temp2 = temp2->next_station;
		temp2->station_id = temp1->station_id;
		temp2->next_station = NULL;
		start = temp1->station_id;
		*temp3 += day[cur_day + 1].station[start].danger_value;
		temp1 = temp1->next_station;
	}
	temp2->next_station = (struct __route *)malloc(sizeof(struct __route));
	temp2 = temp2->next_station;
	temp2->station_id = divert3;
	*temp3 += day[cur_day + 1].station[divert3].danger_value;
	temp2->next_station = NULL;
	start = divert3;
	while (start != target)
	{
		temp2->next_station = (struct __route *)malloc(sizeof(struct __route));
		temp2 = temp2->next_station;
		temp2->station_id = parent[start];
		*temp3 += day[cur_day + 1].station[parent[start]].danger_value;
		temp2->next_station = NULL;
		start = parent[start];
	}
	return;
}

// This function will implement the 3-way dijkstra
_possible_path *three_way_dijkstra_implementation(struct Graph *graph, int src, int target, int curr_day)
{
	double dist[graph->V];																/// This array would store the our net edge weight distance from target to each vertex
	int parent[graph->V];																//// This would store the parent of each of the vertex
	_possible_path *possible_routes = (_possible_path *)malloc(sizeof(_possible_path)); //// This struct would contain our answer
	dijkstra(graph, target, dist, curr_day, parent);
	if (dist[src] == maxi)
	{
		possible_routes->no_of_possible_path = 0;
		return possible_routes;
	}
	else
	{
		add_path_one(possible_routes, dist, src, target, parent);
	}
	// Code to find the second shortest path
	double min1 = maxi, min2 = maxi;
	int index1 = -1, index2 = -1, divert1 = -1, divert2 = -1;
	// min1 stores danger value of 2nd shortest path
	// min2 stores danger value of a possible 3rd shortest path
	// index stores at which node we diverted
	// divert stores to which we diverted
	struct __route *temp = &(possible_routes->best_path);
	double suffix = 0;
	double weight = 0; // For the min2 path it gives the edge weight(combined) till they share common path
	int prev = -2;	   // stores index of the previous station
	while (temp->station_id != target)
	{
		struct AdjListNode *pCrawl =
			graph->array[temp->station_id].head;
		while (pCrawl != NULL)
		{
			int x = pCrawl->dest;
			if (pCrawl->dest == temp->next_station->station_id || dist[pCrawl->dest] == maxi || pCrawl->dest == prev || parent[pCrawl->dest] == temp->station_id)
			{
				pCrawl = pCrawl->next;
				continue;
			}
			if (dist[src] - dist[temp->station_id] + day[cur_day + 1].station[temp->station_id].danger_value * 100000 + pCrawl->weight + dist[pCrawl->dest] <= min1)
			{
				min2 = min1;
				index2 = index1;
				divert2 = divert1;
				min1 = dist[src] - dist[temp->station_id] + day[cur_day + 1].station[temp->station_id].danger_value * 100000 + pCrawl->weight + dist[pCrawl->dest];
				index1 = temp->station_id;
				divert1 = pCrawl->dest;
				weight = dist[src] - dist[temp->station_id] + 100000 * (day[cur_day + 1].station[temp->station_id].danger_value) + pCrawl->weight;
			}
			else if (dist[src] - dist[temp->station_id] + (day[cur_day + 1].station[temp->station_id].danger_value) * 100000 + pCrawl->weight + dist[pCrawl->dest] < min2)
			{
				min2 = dist[src] - dist[temp->station_id] + day[cur_day + 1].station[temp->station_id].danger_value * 100000 + pCrawl->weight + dist[pCrawl->dest];
				index2 = temp->station_id;
				divert2 = pCrawl->dest;
			}
			pCrawl = pCrawl->next;
		}
		prev = temp->station_id;
		temp = temp->next_station;
	}
	if (min1 == maxi)
	{
		possible_routes->no_of_possible_path = 1;
		return possible_routes;
	}
	else
	{
		add_path_two(possible_routes, dist, src, target, parent, index1, divert1, 1);
	}
	// Code for the third shortest path
	double min3 = INT_MAX;
	int index3 = -1, divert3 = -1;
	// min3 stores danger value of a possible contender for 3rd shortest path
	// index3 stores at which node we diverted
	// divert3 stores to which we diverted
	struct __route *temp1 = &(possible_routes->best_path);
	struct __route *temp2 = &(possible_routes->second_best_path);
	// temp1 will be used to check if we have a subgraph till the node we are at the moment
	bool evaluate = false; // checks till when we have a sub path
	prev = -2;			   // stores index of the previous station
	while (temp2->station_id != target)
	{
		if (evaluate == false && temp1->station_id == temp2->station_id)
		{
			prev = temp2->station_id;
			temp1 = temp1->next_station;
			temp2 = temp2->next_station;
			continue;
		}
		else
		{
			evaluate = true;
		}
		struct AdjListNode *pCrawl =
			graph->array[temp2->station_id].head;
		while (pCrawl != NULL)
		{
			if (pCrawl->dest == temp2->next_station->station_id || dist[pCrawl->dest] == maxi || pCrawl->dest == prev || parent[pCrawl->dest] == temp2->station_id)
			{
				pCrawl = pCrawl->next;
				continue;
			}
			if (weight + dist[divert1] - dist[temp2->station_id] + 100000 * day[cur_day + 1].station[temp2->station_id].danger_value + pCrawl->weight + dist[pCrawl->dest] < min3)
			{
				min3 = weight + dist[divert1] - dist[temp2->station_id] + 100000 * day[cur_day + 1].station[temp2->station_id].danger_value + pCrawl->weight + dist[pCrawl->dest];
				index3 = temp2->station_id;
				divert3 = pCrawl->dest;
			}
			pCrawl = pCrawl->next;
		}
		prev = temp2->station_id;
		temp2 = temp2->next_station;
	}
	if (min2 == maxi && min3 == maxi)
	{
		possible_routes->no_of_possible_path = 2;
		return possible_routes;
	}
	else
	{
		if (min2 < min3)
		{
			possible_routes->no_of_possible_path = 3;
			add_path_two(possible_routes, dist, src, target, parent, index2, divert2,0);
		}
		else if (min2 == min3)
		{
			possible_routes->no_of_possible_path = 3;
			add_path_two(possible_routes, dist, src, target, parent, index2, divert2, 0);
		}
		else if (min2 > min3)
		{
			possible_routes->no_of_possible_path = 3;
			add_path_three(possible_routes, dist, src, target, parent, index3, divert3);
		}
	}
	return possible_routes;
}
//        ASSUMPTION:
// if someone has to go from say A to B (directly connected)...so the shortest path is the path directly from A to B
// say A->C->B is the second shortest path
// From graph there exists a third shortest path which can be like A->C->D->C->B but we will not consider this as a logical path since the person would be travelling the same path two times which is not needed
// Hence a path like A->C->D->E->B is also rejected since the shortest path for the person to go from D to B is D->C->B ...Hence the person won't go through this path since he has better options

//////////////////
void add_day_struct(int person_id, int station)
{
	day[0].person[person_id].status = 0;		/// 0 means person is negative(that is neither primary nor secondary contact)
	day[0].person[person_id].cause = -1;		//since person is negative he has no cause
	day[0].person[person_id].days = -1;			// -1 indicates the person has not been in quarantine and is free to move
	day[0].person[person_id].station = station; // gives us the station no of the ith person initialyy
	struct __list *temp = (struct __list *)malloc(sizeof(struct __list));
	temp->person_id = person_id;
	temp->next = NULL;
	if (day[0].station[station].list == NULL)
	{
		day[0].station[station].list = temp;
	}
	else
	{
		temp->next = day[0].station[station].list;
		day[0].station[station].list = temp;
	}
}

void copy_day(int cur_day, int num_stations, int num_person)
{
	day[(cur_day + 1) % 16].person = (struct __person *)malloc(num_person * sizeof(struct __person));
	day[(cur_day + 1) % 16].station = (struct __station *)malloc(num_stations * sizeof(struct __station));
	day[(cur_day + 1) % 16].path = NULL;

	for (int i = 0; i < num_stations; i++)
	{
		day[(cur_day + 1) % 16].station[i].worst_affected = day[cur_day].station[i].worst_affected;
		day[(cur_day + 1) % 16].station[i].list = NULL;
		day[(cur_day + 1) % 16].station[i].danger_value = day[cur_day].station[i].danger_value;
	}
	for (int i = 0; i < num_person; i++)
	{
		day[(cur_day + 1) % 16].person[i].status = day[cur_day].person[i].status;
		day[(cur_day + 1) % 16].person[i].days = day[cur_day].person[i].days;
		day[(cur_day + 1) % 16].person[i].cause = day[cur_day].person[i].cause;
		day[(cur_day + 1) % 16].person[i].station = day[cur_day].person[i].station;
		struct __list *temp5 = (struct __list *)malloc(sizeof(struct __list));
		temp5->person_id = i;
		temp5->next = NULL;
		if (day[cur_day].station[day[cur_day].person[i].station].list == NULL)
		{
			day[cur_day].station[day[cur_day].person[i].station].list = temp5;
		}
		else
		{
			temp5->next = day[cur_day].station[day[cur_day].person[i].station].list;
			day[cur_day].station[day[cur_day].person[i].station].list = temp5;
		}
	}
};
