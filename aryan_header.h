#ifndef __HEADER__
#define __HEADER__
#define NUM 100000

int cur_day;
extern const char *status_mapping[];
//mapping from integer to its corresponding status in string
struct __route
{
	int station_id;
	struct __route *next_station;
};
struct __list
{
	int person_id;
	struct __list *next;
};
struct __person
{
	int status;
	int days;	 // number of days person has been affected
	int cause;	 // id of the person who could have been cause of infection/// if cause not exist then -1
	int station; // id of the station
};

struct __station
{
	int worst_affected;	 //indicates the status of the worst affected person on the station///0=negative,1-seconday///2-primary///3-covid///
	struct __list *list; // linked list of all the people on the station
	float danger_value;
};

struct __path
{
	int person_id;
	struct __route *next_station; // pointer to linked list listing the person's route
	struct __path *next_person;	  // pointer to the same struct, denoting the path of the next person
};

struct __day
{
	struct __person *person;
	struct __path *path;
	struct __station *station;
}; // we maintain the status of the all the stations and persons for 15 days
/* Uncomment the following line to test with arbitrary day array */
struct __day day[15];

typedef struct __day _day;
typedef struct __person _person;
typedef struct __station _station;
typedef struct __path _path;
typedef struct __route _route;
typedef struct __list _list;

struct AdjListNode
{
	int dest;
	int weight;
	struct AdjListNode *next;
};

// A structure to represent
// an adjacency list
struct AdjList
{

	// Pointer to head node of list
	struct AdjListNode *head;
};
struct _possible_path
{
	int no_of_possible_path;
	struct __route best_path;
	struct __route second_best_path;
	struct __route third_best_path;
	double best_path_danger_value;
	double second_best_path_danger_value;
	double third_best_path_danger_value;
};
typedef struct _possible_path _possible_path;

// A structure to represent a graph.
// A graph is an array of adjacency lists.
// Size of array will be V (number of
// vertices in graph)
struct Graph
{
	int V;
	struct AdjList *array;
};

/// MINHEAP
// Structure to represent a min heap node
struct MinHeapNode
{
	int v;
	double dist;
};

// Structure to represent a min heap
struct MinHeap
{

	// Number of heap nodes present currently
	int size;

	// Capacity of min heap
	int capacity;

	// This is needed for decreaseKey()
	int *pos;
	struct MinHeapNode **array;
};
/////

struct AdjListNode *newAdjListNode(int dest, int weight);		  //Create a node, util function
struct Graph *createGraph(int V);								  //Creates a graph of N vertices
void addEdge(struct Graph *graph, int src, int dest, int weight); //Adds an edge of length w between s and d
void print_Graph(struct Graph *);								  // prints the Graph, for testing

/***** IMPLEMENTED ******/
/* present in update-functions.c */
void UpdateData(int id_person, int id_station, int daye);
void UpdateStation(int id_person, int id_station, int daye);
void UpdatePerson(int id_person, int id_station, int daye);
void UpdateRoute(_route *R, int id_person, int daye);
void RemovePerson(int id_person, int station_from, int daye);
void AddPerson(int id_person, int station_to, int daye);
void UpdateForDay(_path *P, int daye);
void UpdateForPerson(_route *L, int id_person, int daye);
void Backtrace(int start_day, int end_day);

/* present in general-functions.c */
float getDangerIndex(int id_person, int daye);
int getWorstAffected(int id_station, int daye);
_list *DeleteNodeById(_list *L, int id_person);
_list *AddNodeAtStart(_list *L, int id_person);
void PrintStatus(int n);

//// Used in main.c
void copy_day(int cur_day, int num_statoins, int num_person);
void add_day_struct(int person_id, int station);
///3-WAY dijkstra
_possible_path *three_way_dijkstra_implementation(struct Graph *graph, int src, int target, int curr_day);
void add_path_three(_possible_path *possible_routes, double dist[], int src, int target, int parent[], int index3, int divert3);	   // This function will add 3rd the shortest path to possible_routes struct
void add_path_two(_possible_path *possible_routes, double dist[], int src, int target, int parent[], int index1, int divert1, bool a); // This function will add the 2nd shortest path to possible_routes struct
void add_path_one(_possible_path *possible_routes, double dist[], int src, int target, int parent[]);								   // This function will add the shortest path to possible_routes struct
void dijkstra(struct Graph *graph, int src, double dist[], int curr_day, int parent[]);
int isInMinHeap(struct MinHeap *minHeap, int v);			   // A utility function to check if a given vertex 'v' is in min heap or not
void decreaseKey(struct MinHeap *minHeap, int v, double dist); //
struct MinHeapNode *extractMin(struct MinHeap *minHeap);
int isEmpty(struct MinHeap *minHeap);
void minHeapify(struct MinHeap *minHeap, int idx);
void swapMinHeapNode(struct MinHeapNode **a, struct MinHeapNode **b);
struct MinHeap *createMinHeap(int capacity);
struct MinHeapNode *newMinHeapNode(int v, double dist);
void addEdge(struct Graph *graph, int src, int dest, int weight);
struct Graph *createGraph(int V);
struct AdjListNode *newAdjListNode(int dest, int weight);
/////////

#endif
