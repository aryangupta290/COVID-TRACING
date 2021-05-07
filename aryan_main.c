// have to declare day struct
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header.h"
int main(void)
{
    //initial basic input
    int num_stations, num_roads, num_person;
    printf("Give the no of stations,no of roads,no of people: ");
    scanf("%d %d %d", &num_stations, &num_roads, &num_person);
    struct Graph *graph = createGraph(num_stations);
    for (int i = 0; i < num_roads; i++)
    {
        int from, to, distance;
        printf("Input the values for edge no %d: ", i + 1);
        scanf("%d %d %d", &from, &to, &distance);
        addEdge(graph, from, to, distance); //assuming vertices from 0 to N-1
    }
    day[0].person = (struct __person *)malloc(num_person * sizeof(struct __person));
    day[0].station = (struct __station *)malloc(num_stations * sizeof(struct __station));
    day[0].path = NULL;
    for (int i = 0; i < num_stations; i++)
    {
        day[0].station[i].danger_value = 0;
        day[0].station[i].list = NULL;
        day[0].station[i].worst_affected = 0; // initially everyone is covid negative
    }
    for (int i = 0; i < num_person; i++)
    {
        int station;
        scanf("%d", &station);
        printf("Give station of person %d: ",i);
        // inserting this person in the initial day struct
        add_day_struct(i, station);
    }
    ///////NOW we have done the initial requirements for our code
    cur_day = 0; // we are currently on the 0th day(starting day)
    //// We will entertain query 1 only at the end of the query
    //// input 1 means query1
    /// input 2 means query2
    /// input 3 means given the id of a person we will print what is his current situation like status,which station he is present,etc
    /// input 4 means given the station no ..we will print the list of covid positive ,primary,secondary,negative people in that station
    //// input 5 means we are done for that day
    /// input -1 means get out of the program
    printf("(1)->Query 1 /// (1)->Query 1 /// (2)->Query 2 /// (3)->Query 3 /// (4)->Query 4\n");
    while (1)
    {
        bool if_added = false;
        printf("We are currently at day %d\n", cur_day);
        struct __path *temp; // stores the latest path that we get from query 2 to reduce traversal time
        copy_day(cur_day, num_stations, num_person);
        while (1)
        {
            int query;
            printf("Give query: ");
            scanf("%d", &query);
            if (query == 2)
            {
                int person_id;
                printf("Give the preson's id who wants to travel: ");
                scanf("%d", &person_id);
                int from_where, to_where;
                printf("Give the id of the to and from stations: ");
                scanf("%d %d", &from_where, &to_where);
                _possible_path *temp1 = (_possible_path *)malloc(sizeof(_possible_path));
                temp1 = three_way_dijkstra_implementation(graph, from_where, to_where, cur_day);
                if (temp1->no_of_possible_path == 0)
                {
                    printf("There are no possible paths from %d to %d: \n", from_where, to_where);
                }
                else if (temp1->no_of_possible_path == 1)
                {
                    printf("There is only one possible path as follows: \n");
                    struct __route *temp2 = &(temp1->best_path); // maintains which path the person is eventually gonna decide to go from
                    do
                    {
                        printf("%d -> ", temp2->station_id);
                        temp2 = temp2->next_station;
                    } while (temp2 != NULL);
                    temp2 = &(temp1->best_path);
                    printf("Destination Reached\n");
                    printf("The corresponding danger value for this path is %lf\n", temp1->best_path_danger_value);
                    int may_accept;
                    // enter 1 if u want to go through this path else 0
                    printf("Do u want to traverse through this path: ");
                    scanf("%d", &may_accept);
                    if (may_accept)
                    {
                        if (!if_added)
                        {
                            if_added = true;
                            day[cur_day].path = (struct __path *)malloc(sizeof(struct __path));
                            day[cur_day].path->person_id = person_id;
                            day[cur_day].path->next_station = temp2;
                            day[cur_day].path->next_person = NULL;
                            temp = day[cur_day].path;
                            UpdateForPerson(temp2, person_id, cur_day+1);
                        }
                        else
                        {
                            struct __path *temp3 = (struct __path *)malloc(sizeof(struct __path));
                            temp3->person_id = person_id;
                            temp3->next_station = temp2;
                            temp3->next_person = NULL;
                            temp->next_person = temp3;
                            temp = temp3;
                            UpdateForPerson(temp2, person_id, cur_day+1);
                        }
                    }
                }
                else if (temp1->no_of_possible_path == 2)
                {
                    printf("There are only two possible path as follows: \n");
                    struct __route *temp2 = &(temp1->best_path);
                    do
                    {
                        printf("%d -> ", temp2->station_id);
                        temp2 = temp2->next_station;
                    } while (temp2 != NULL);
                    temp2 = &(temp1->best_path);
                    printf("Destination Reached\n");
                    printf("The corresponding danger value for this path is %lf\n", temp1->best_path_danger_value);
                    temp2 = &(temp1->second_best_path);
                    do
                    {
                        printf("%d -> ", temp2->station_id);
                        temp2 = temp2->next_station;
                    } while (temp2 != NULL);
                    printf("Destination Reached\n");
                    printf("The corresponding danger value for this path is %lf\n", temp1->second_best_path_danger_value);

                    int may_accept;
                    // enter 1 if u want to go through this path else 0
                    printf("Do u want to traverse through this path");
                    scanf("%d", &may_accept);
                    if (may_accept)
                    {
                        printf("Which path do u want to go through: ");
                        int which_path; // either 1 or 2
                        scanf("%d", &which_path);
                        if (which_path == 1)
                        {
                            temp2 = &(temp1->best_path);
                        }
                        else
                        {
                            temp2 = &(temp1->second_best_path);
                        }
                    }
                    else
                        continue;
                    if (!if_added)
                    {

                        if_added = true;
                        day[cur_day].path = (struct __path *)malloc(sizeof(struct __path));
                        day[cur_day].path->person_id = person_id;
                        day[cur_day].path->next_station = temp2;
                        day[cur_day].path->next_person = NULL;
                        temp = day[cur_day].path;
                        UpdateForPerson(temp2, person_id, cur_day+1);
                    }
                    else
                    {
                        struct __path *temp3 = (struct __path *)malloc(sizeof(struct __path));
                        temp3->person_id = person_id;
                        temp3->next_station = temp2;
                        temp3->next_person = NULL;
                        temp->next_person = temp3;
                        temp = temp3;
                        UpdateForPerson(temp2, person_id, cur_day+1);
                    }
                }
                else
                {
                    printf("There are all three possible path as follows: \n");
                    struct __route *temp2 = &(temp1->best_path);
                    do
                    {
                        printf("%d -> ", temp2->station_id);
                        temp2 = temp2->next_station;
                    } while (temp2 != NULL);
                    temp2 = &(temp1->best_path);
                    printf("Destination Reached\n");
                    printf("The corresponding danger value for this path is %lf\n", temp1->best_path_danger_value);
                    temp2 = &(temp1->second_best_path);
                    do
                    {
                        printf("%d -> ", temp2->station_id);
                        temp2 = temp2->next_station;
                    } while (temp2 != NULL);
                    temp2 = &(temp1->second_best_path);
                    printf("Destination Reached\n");
                    printf("The corresponding danger value for this path is %lf\n", temp1->second_best_path_danger_value);
                    temp2 = &(temp1->third_best_path);
                    do
                    {
                        printf("%d -> ", temp2->station_id);
                        temp2 = temp2->next_station;
                    } while (temp2 != NULL);
                    printf("Destination Reached\n");
                    printf("The corresponding danger value for this path is %lf\n", temp1->third_best_path_danger_value);

                    int may_accept;
                    // enter 1 if u want to go through this path else 0
                    printf("Do u want to traverse through this path");
                    scanf("%d", &may_accept);
                    if (may_accept)
                    {
                        printf("Which path do u want to go through: ");
                        int which_path; // either 1 or 2
                        scanf("%d", &which_path);
                        if (which_path == 1)
                        {
                            temp2 = &(temp1->best_path);
                        }
                        else if (which_path == 2)
                        {
                            temp2 = &(temp1->second_best_path);
                        }
                        else if (which_path == 3)
                        {
                            temp2 = &(temp1->third_best_path);
                        }
                    }
                    else
                    {
                        continue;
                    }
                    if (!if_added)
                    {

                        if_added = true;
                        day[cur_day].path = (struct __path *)malloc(sizeof(struct __path));
                        day[cur_day].path->person_id = person_id;
                        day[cur_day].path->next_station = temp2;
                        day[cur_day].path->next_person = NULL;
                        temp = day[cur_day].path;
                        UpdateForPerson(temp2, person_id, cur_day+1);
                    }
                    else
                    {
                        struct __path *temp3 = (struct __path *)malloc(sizeof(struct __path));
                        temp3->person_id = person_id;
                        temp3->next_station = temp2;
                        temp3->next_person = NULL;
                        temp->next_person = temp3;
                        temp = temp3;
                        UpdateForPerson(temp2, person_id, cur_day+1);
                    }
                }
            }
            else if (query == 1)
            {
                int no_input_people;
                printf("How many people are covid positive ");
                scanf("%d", &no_input_people);
                int temp[no_input_people];
            }
            else if (query == 3)
            {
                printf("Give the id of the person whose status u want to print: ");
                int temp1;
                scanf("%d", &temp1);
                printf("Person id:%d\nStatus:%d\nCurrently on which station:%d\n", temp1, day[cur_day + 1].person[temp1].status, day[cur_day + 1].person[temp1].station);
                if (day[cur_day + 1].person[temp1].status && day[cur_day + 1].person[temp1].status < 4)
                {
                    printf("Cause: %d\n Station:%d\n", day[cur_day].person[temp1].cause, day[cur_day].person[temp1].days);
                }
            }
            else if (query == 4)
            {
                printf("GIve the station no whose details u want to print: ");
                int temp1;
                scanf("%d", &temp1);
                printf("Station id:%d \nWorst affected person in this station:%d\nDanger value of this station:%f\n", temp1, day[cur_day + 1].station[temp1].worst_affected, day[cur_day + 1].station[temp1].danger_value);
                printf("List of people in this station: \n");
                struct __list *temp = day[cur_day + 1].station[temp1].list;
                while (temp != NULL)
                {
                    printf("Person_id: %d     Person_status: %d", temp->person_id, day[cur_day + 1].person[temp->person_id].status);
                    temp = temp->next;
                }
                printf("\n");
            }
            else if (query == 5)
            {
                cur_day++;
                break;
            }
            else if (query == -1)
            {
                exit(0);
            }
        }
    }
    return (0);
}
