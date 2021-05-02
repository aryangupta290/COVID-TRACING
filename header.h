#ifndef __HEADER__
#define __HEADER__

#define NUM 100000
#define DAY_MAX 15

typedef struct __day _day;
typedef struct __person _person;
typedef struct __station _station;
typedef struct __path _path;
typedef struct __route _route;
typedef struct __list _list;
typedef enum __status _status;

int cur_day;
enum  __status {negative, secondary_contact, primary_contact, positive};

struct __station {
	_status worst_affected;
	_list* list;
};
struct __list {
	int person_id;
	struct __list* next;
};

struct __route {
	int station_id;
	struct __route* next_station;
};

struct __person {
	_status status;
	int days;
	int cause;
	int station;
};

struct __path {
	int person_id;
	_route* next_station;
	struct __path* next_person;
};

struct __day {
	_person person[NUM];
	_path path;
	_station station[NUM];
};

struct Node  //node for adjacency list
{
  int vertex;
  int weight;
  struct Node* next;
};

struct Graph
{
  int total_vertices;
  struct Node** adjLists;
};

struct Node* create_Node(int);//Create a node, util function 
struct Graph* create_Graph(int N);//Creates a graph of N vertices
void addEdge(struct Graph*, int s, int d , int w);//Adds an edge of length w between s and d
void print_Graph(struct Graph*);// prints the Graph, for testing 

typedef struct __day_queue
{
	int front,rear;
	_day array[DAY_MAX];
}_day_queue;

_day_queue* init_queue();
void enqueue(_day_queue* Queue, _day D);

#endif
