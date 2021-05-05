// C / C++ program for Dijkstra's
// shortest path algorithm for adjacency
// list representation of graph
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
double danger[] = {4, 8, 5, 0, 3, 1, 9, 3, 5}; // later this will be commented
// A structure to represent a
// node in adjacency list
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

// A structure to represent a graph.
// A graph is an array of adjacency lists.
// Size of array will be V (number of
// vertices in graph)
struct Graph
{
    int V;
    struct AdjList *array;
};

// A utility function to create
// a new adjacency list node
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
bool isInMinHeap(struct MinHeap *minHeap, int v)
{
    if (minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}

// A utility function used to print the solution
void printArr(double dist[], int n, int parent[])
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %lf   %d\n", i, dist[i], parent[i]);
}

// The main function that calulates
// distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
void dijkstra(struct Graph *graph, int src, double dist[], int curr_day, int parent[], int weight[])
{

    // Get the number of vertices in graph
    int V = graph->V;
    //int weight[graph];
    for (int i = 0; i < V; i++)
    {
        parent[i] = -1;
    }

    weight[src] = 0;

    // minHeap represents set E
    struct MinHeap *minHeap = createMinHeap(V);

    // Initialize min heap with all
    // vertices. dist value of all vertices
    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
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
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && dist[u] + danger[v] <= dist[v]) //day[curr_day + 1].station[temp->station_id].danger_value;
            {
                if (dist[v] == INT_MAX)
                {
                    dist[v] = dist[u] + danger[v]; //day[curr_day + 1].station[temp->station_id].danger_value;
                    parent[v] = u;

                    // update distance
                    // value in min heap also
                    decreaseKey(minHeap, v, dist[v]);
                    // weight checks that if danger value of two path is same then the path with shorter distance is selected
                    weight[v] = weight[u] + pCrawl->weight;
                }
                else if (dist[u] + danger[v] < dist[v])
                {
                    dist[v] = dist[u] + danger[v]; //day[curr_day + 1].station[temp->station_id].danger_value;
                    parent[v] = u;

                    // update distance
                    // value in min heap also
                    decreaseKey(minHeap, v, dist[v]);
                    weight[v] = weight[u] + pCrawl->weight;
                }
                else if (weight[v] > weight[u] + pCrawl->weight)
                {
                    dist[v] = dist[u] + danger[v]; //day[curr_day + 1].station[temp->station_id].danger_value;
                    parent[v] = u;

                    // update distance
                    // value in min heap also
                    decreaseKey(minHeap, v, dist[v]);
                    weight[v] = weight[u] + pCrawl->weight;
                }
            }

            //  else if(sInMinHeap(minHeap, v) && dist[u] != INT_MAX && dist[u] + danger[v] == dist[v])
            pCrawl = pCrawl->next;
        }
    }

    // print the calculated shortest distances
    printArr(dist, V, parent);
}

// Actual function
struct __route
{
    int station_id;
    struct __route *next_station;
};
// This struct will return us the best 3 possible paths
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
// This function will add the shortest path to possible_routes struct
void add_path_one(_possible_path *possible_routes, double dist[], int src, int target, int parent[])
{

    struct __route *temp = &(possible_routes->best_path);
    possible_routes->best_path_danger_value = dist[src];
    temp->station_id = src;
    temp->next_station = NULL;
    int start = src;
    while (start != target)
    {
        temp->next_station = (struct __route *)malloc(sizeof(struct __route));
        temp = temp->next_station;
        temp->station_id = parent[start];
        temp->next_station = NULL;
        //possible_routes->best_path_danger_value = dist[parent[start]];
        start = parent[start];
    }
    /*temp->next_station = (struct __route *)malloc(sizeof(struct __route));
    temp = temp->next_station;
    temp->station_id = parent[start];
    temp->next_station = NULL;*/
    return;
}
void add_path_two(_possible_path *possible_routes, double dist[], int src, int target, int parent[], int index1, int divert1, bool a)
{
    struct __route *temp;
    if (a)
    {
        temp = &(possible_routes->second_best_path);
    }
    else
    {
        temp = &(possible_routes->third_best_path);
    }
    temp->station_id = src;
    temp->next_station = NULL;
    int start = src;
    if (index1 == start)
    {
        start = divert1;
        temp->next_station = (struct __route *)malloc(sizeof(struct __route));
        temp = temp->next_station;
        temp->station_id = divert1;
        temp->next_station = NULL;
        if (divert1 != target)
        {
            start = divert1;
            while (start != target)
            {
                temp->next_station = (struct __route *)malloc(sizeof(struct __route));
                temp = temp->next_station;
                temp->station_id = parent[start];
                temp->next_station = NULL;
                //possible_routes->best_path_danger_value = dist[parent[start]];
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
            temp->next_station = NULL;
            //possible_routes->best_path_danger_value = dist[parent[start]];
            start = parent[start];
        }
        temp->next_station = (struct __route *)malloc(sizeof(struct __route));
        temp = temp->next_station;
        temp->station_id = divert1;
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
                temp->next_station = NULL;
                //possible_routes->best_path_danger_value = dist[parent[start]];
                start = parent[start];
            }
            return;
        }
    }
}
void add_path_three(_possible_path *possible_routes, double dist[], int src, int target, int parent[], int index3, int divert3)
{
    struct __route *temp1;
    temp1 = &(possible_routes->second_best_path);
    temp1 = temp1->next_station;
    struct __route *temp2 = &(possible_routes->third_best_path);
    temp2->station_id = src;
    temp2->next_station = NULL;
    int start = src;
    while (start != index3)
    {
        temp2->next_station = (struct __route *)malloc(sizeof(struct __route));
        temp2 = temp2->next_station;
        temp2->station_id = temp1->station_id;
        temp2->next_station = NULL;
        start = temp1->station_id;
        temp1 = temp1->next_station;
    }
    temp2->next_station = (struct __route *)malloc(sizeof(struct __route));
    temp2 = temp2->next_station;
    temp2->station_id = divert3;
    temp2->next_station = NULL;
    start = divert3;
    while (start != target)
    {
        temp2->next_station = (struct __route *)malloc(sizeof(struct __route));
        temp2 = temp2->next_station;
        temp2->station_id = parent[start];
        temp2->next_station = NULL;
        start = parent[start];
    }
    return;
}

// This function will implement the 3-way dijkstra
_possible_path *three_way_dijkstra_implementation(struct Graph *graph, int src, int target, int curr_day)
{
    double dist[graph->V];
    int parent[graph->V];
    int weight[graph->V];
    _possible_path *possible_routes = (_possible_path *)malloc(sizeof(_possible_path));
    dijkstra(graph, target, dist, curr_day, parent, weight);
    if (dist[src] == INT_MAX)
    {
        possible_routes->no_of_possible_path = 0;
        return possible_routes;
    }
    else
    {
        add_path_one(possible_routes, dist, src, target, parent);

        /*struct __route *temp = &(possible_routes->best_path);
        do
        {
            printf("%d -> ", temp->station_id);
            temp = temp->next_station;
        } while (temp != NULL);
        printf("Destination Reached\n");
        // return possible_routes;*/
    }
    // Code to find the second shortest path

    double min1 = INT_MAX, min2 = INT_MAX;
    int index1 = -1, index2 = -1, divert1 = -1, divert2 = -1;
    int weight2 = -1; //to store the path distance for the min2 path so that we can compare it with the min3 path
    // min1 stores danger value of 2nd shortest path
    // min2 stores danger value of a possible 3rd shortest path
    // index stores at which node we diverted
    // divert stores to which we diverted
    struct __route *temp = &(possible_routes->best_path);
    double suffix = 0;
    int distance[graph->V];            // stores suffic weight(distance btwn two stations)
    for (int i = 0; i < graph->V; i++) //day[curr_day + 1].station[temp->station_id].danger_value; i++)
    {
        distance[i] = 0;
    }
    bool a = false;
    int prev = -1; // stores index of the previous station
    int val;       //stores distance between the two shortest path on track
    while (temp->station_id != target)
    {
        if (a == true)
        {
            distance[temp->station_id] = distance[prev] + val;
        }
        a = true;
        suffix += danger[temp->station_id]; //day[curr_day + 1].station[temp->station_id].danger_value;
        struct AdjListNode *pCrawl =
            graph->array[temp->station_id].head;
        while (pCrawl != NULL)
        {
            if (pCrawl->dest == temp->next_station->station_id || dist[pCrawl->dest] == INT_MAX || pCrawl->dest == prev || parent[pCrawl->dest] == temp->station_id)
            {
                if (temp->next_station->station_id == pCrawl->dest)
                {
                    val = pCrawl->weight;
                }
                pCrawl = pCrawl->next;
                continue;
            }
            int new_dist = distance[temp->station_id] + weight[pCrawl->dest];

            if (suffix + dist[pCrawl->dest] < min1)
            {
                min2 = min1; // remember to include the danger value of src at the end
                index2 = index1;
                divert2 = divert1;
                weight2 = new_dist;
                min1 = suffix + dist[pCrawl->dest]; // remember to include the danger value of src at the end
                index1 = temp->station_id;
                divert1 = pCrawl->dest;
            }
            else if (suffix + dist[pCrawl->weight] == min1 && new_dist < distance[index1] + weight[divert1])
            {
                min2 = min1; // remember to include the danger value of src at the end
                index2 = index1;
                divert2 = divert1;
                weight2 = new_dist;
                min1 = suffix + dist[pCrawl->dest]; // remember to include the danger value of src at the end
                index1 = temp->station_id;
                divert1 = pCrawl->dest;
            }
            else if (suffix + dist[pCrawl->weight] == min1 && new_dist >= distance[index1] + weight[divert1])
            {
                min2 = suffix + dist[pCrawl->dest]; // remember to include the danger value of src at the end
                index2 = temp->station_id;
                divert2 = pCrawl->dest;
                weight2 = new_dist;
            }
            else if (suffix + dist[pCrawl->dest] < min2)
            {
                min2 = suffix + dist[pCrawl->dest]; // remember to include the danger value of src at the end
                index2 = temp->station_id;
                divert2 = pCrawl->dest;
                weight2 = new_dist;
            }

            else if (suffix + dist[pCrawl->weight] == min2 && new_dist < distance[index2] + weight[divert2])
            {
                min2 = suffix + dist[pCrawl->dest]; // remember to include the danger value of src at the end
                index2 = temp->station_id;
                divert2 = pCrawl->dest;
                weight2 = new_dist;
            }
            pCrawl = pCrawl->next;
        }
        prev = temp->station_id;
        temp = temp->next_station;
    }
    ////debug code
    // printf("%lf  %d   %d\n%lf   %d   %d\n",min1,index1,divert1,min2,index2,divert2);
    if (min1 == INT_MAX)
    {
        possible_routes->no_of_possible_path = 1;
        return possible_routes;
    }
    else
    {
        add_path_two(possible_routes, dist, src, target, parent, index1, divert1, true);
        possible_routes->second_best_path_danger_value = min1;
    }
    // printing the shortest and second shortest path
    temp = &(possible_routes->best_path);
    do
    {
        printf("%d -> ", temp->station_id);
        temp = temp->next_station;
    } while (temp != NULL);
    printf("Destination Reached %lf \n", possible_routes->best_path_danger_value);
    temp = &(possible_routes->second_best_path);
    do
    {
        printf("%d -> ", temp->station_id);
        temp = temp->next_station;
    } while (temp != NULL);
    printf("Destination Reached %lf \n", possible_routes->second_best_path_danger_value);
    /* for (int i = 0; i < graph->V; i++)
    {
        printf("%d ", weight[i]);
    }*/

    // Code for the third shortest path
    double min3 = INT_MAX;
    int index3 = -1, divert3 = -1;
    int weight3 = -1;
    // min3 stores danger value of a possible contender for 3rd shortest path
    // index3 stores at which node we diverted
    // divert3 stores to which we diverted
    struct __route *temp1 = &(possible_routes->best_path);
    struct __route *temp2 = &(possible_routes->second_best_path);
    // temp1 will be used to check if we have a subgraph till the node we are at the moment
    suffix = 0;                        // stores suffix weight(distance btwn two stations)
    for (int i = 0; i < graph->V; i++) //day[curr_day + 1].station[temp->station_id].danger_value; i++)
    {
        distance[i] = 0;
    }
    bool evaluate = false; // checks till when we have a sub path
    a = false;
    prev = -1; // stores index of the previous station
    val;       //stores distance between the two shortest path on track
    while (temp2->station_id != target)
    {
        if (a == true)
        {
            distance[temp2->station_id] = distance[prev] + val;
        }
        a = true;
        suffix += danger[temp2->station_id];
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
            if (pCrawl->dest == temp2->next_station->station_id || dist[pCrawl->dest] == INT_MAX || pCrawl->dest == prev || parent[pCrawl->dest] == temp2->station_id)
            {
                if (temp2->next_station->station_id == pCrawl->dest)
                {
                    val = pCrawl->weight;
                }
                pCrawl = pCrawl->next;
                continue;
            }
            int new_dist = distance[temp2->station_id] + weight[pCrawl->dest];

            if (suffix + dist[pCrawl->dest] < min3)
            {
                min3 = suffix + dist[pCrawl->dest]; // remember to include the danger value of src at the end
                index3 = temp2->station_id;
                divert3 = pCrawl->dest;
                weight3 = new_dist;
            }
            else if (suffix + dist[pCrawl->weight] == min3 && new_dist < distance[index3] + weight[divert3])
            {
                min3 = suffix + dist[pCrawl->dest]; // remember to include the danger value of src at the end
                index3 = temp2->station_id;
                divert3 = pCrawl->dest;
                weight3 = new_dist;
            }
            pCrawl = pCrawl->next;
        }
        prev = temp2->station_id;
        temp2 = temp2->next_station;
    }
    //debuf code
    printf("min2=%lf,index2= %d,divert2= %d,min3= %lf,index3 =%d,divert3= %d\n", min2, index2, divert2, min3, index3, divert3);
    if (min2 == INT_MAX && min3 == INT_MAX)
    {
        possible_routes->no_of_possible_path = 2;
        return possible_routes;
    }
    else
    {
        if (min2 < min3)
        {
            possible_routes->no_of_possible_path = 3;
            possible_routes->third_best_path_danger_value = min2;
            add_path_two(possible_routes, dist, src, target, parent, index2, divert2, false);
        }
        else if (min2 == min3 && weight2 <= weight3)
        {
            possible_routes->no_of_possible_path = 3;
            possible_routes->third_best_path_danger_value = min2;
            add_path_two(possible_routes, dist, src, target, parent, index2, divert2, false);
        }
        else if (min2 == min3 && weight3 < weight2)
        {
            possible_routes->no_of_possible_path = 3;
            possible_routes->third_best_path_danger_value = min3;
            add_path_three(possible_routes, dist, src, target, parent, index3, divert3);
        }
        else if (min2 > min3)
        {
            possible_routes->no_of_possible_path = 3;
            possible_routes->third_best_path_danger_value = min3;
            add_path_three(possible_routes, dist, src, target, parent, index3, divert3);
        }
    }
    temp = &(possible_routes->third_best_path);
    do
    {
        printf("%d -> ", temp->station_id);
        temp = temp->next_station;
    } while (temp != NULL);
    printf("Destination Reached %lf \n", possible_routes->third_best_path_danger_value);

    return possible_routes;
}

// Driver program to test above functions
int main()
{
    // create the graph given in above fugure
    int V = 9;
    struct Graph *graph = createGraph(V);
    addEdge(graph, 0, 1, 1);
    addEdge(graph, 0, 7, 1);
    addEdge(graph, 1, 2, 1);
    addEdge(graph, 1, 7, 1);
    addEdge(graph, 7, 8, 1);
    addEdge(graph, 7, 6, 1);
    addEdge(graph, 6, 5, 1);
    addEdge(graph, 6, 8, 9);
    addEdge(graph, 2, 8, 1);
    addEdge(graph, 2, 5, 1);
    addEdge(graph, 2, 3, 2);
    addEdge(graph, 5, 4, 1);
    addEdge(graph, 3, 4, 6);
    addEdge(graph, 3, 5, 7);
    int curr_day = 8;
    three_way_dijkstra_implementation(graph, 7, 6, curr_day);
    return 0;
}
//double danger[] = {4, 8, 5, 2, 3, 1,9,3,5};

//        ASSUMPTION:
// if someone has to go from say A to B (directly connected)...so the shortest path is the path directly from A to B
// say A->C->B is the second shortest path
// From graph there exists a third shortest path which can be like A->C->D->C->B but we will not consider this as a logical path since the person would be travelling the same path two times which is not needed
// Hence a path like A->C->D->E->B is also rejected since the shortest path for the person to go from D to B is D->C->B ...Hence the person won't go through this path since he has better options
// sure about the shortest path but if there are a lot of paths with the same shortest path then the order might be wrong because i cannot check for all the shortest paths