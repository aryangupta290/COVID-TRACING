#ifndef __HEADER__
#define __HEADER__

#define NUM 100000
int cur_day;
extern const char* status_mapping[]; 
//mapping from integer to its corresponding status in string
struct __route {
	int station_id;
	struct __route* next_station;
};
struct __list {
	int person_id;
	struct __list* next;
};
struct __person {
	int status;
	int days; // number of days person has been 
	int cause; // id of the person who could have been cause of infection
	int station; // id of the station
};

struct __station {
	int worst_affected; //indicates the status of the worst affected person on the station
	struct __list* list; // linked list of all the people on the station
	float danger_value; 
};
struct __path {
	int person_id;
	struct __route* next_station; // pointer to linked list listing the person's route
	struct __path* next_person; // pointer to the same struct, denoting the path of the next person
};

struct __day {
	struct __person person[NUM];
	struct __path* path;
	struct __station station[NUM];
}; // we maintain the status of the all the stations and persons for 15 days

struct __day day[15];

typedef struct __day _day;
typedef struct __person _person;
typedef struct __station _station;
typedef struct __path _path;
typedef struct __route _route;
typedef struct __list _list;

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

/***** IMPLEMENTED ******/
/* present in update-functions.c */
void UpdateData(int id_person, int id_station, int daye);
void UpdateStation(int id_person, int id_station, int daye);
void UpdatePerson(int id_person, int id_station, int daye);
void UpdateRoute(_route* R, int id_person, int daye);
void RemovePerson(int id_person, int station_from, int daye);
void AddPerson(int id_person, int station_to, int daye);
void UpdateForDay(_path*P, int daye);
void UpdateForPerson(_route* L, int id_person, int daye);
void Backtrace(int start_day, int end_day);

/* present in general-functions.c */
float getDangerIndex(int id_person, int daye);
int getWorstAffected(int id_station, int daye);
_list* DeleteNodeById(_list* L, int id_person);
_list* AddNodeAtStart(_list* L, int id_person);
void PrintStatus(int n);
#endif
