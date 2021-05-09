#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header.h"

int main(void)
{
    //initial basic input
    system("clear");

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
    printf("\n");

    //Allocating memory for global variable day in header
    day[0].person = (struct __person *)malloc(num_person * sizeof(struct __person));
    day[0].station = (struct __station *)malloc(num_stations * sizeof(struct __station));
    day[0].path = NULL;

    //Setting up all stations as negative
    for (int i = 0; i < num_stations; i++)
    {
        day[0].station[i].danger_value = 0;
        day[0].station[i].list = NULL;
        day[0].station[i].worst_affected = 0; //as initially everyone is covid negative
    }

    //Inputting location data for each person
    for (int i = 0; i < num_person; i++)
    {
        int station;
        printf("Give station of person %d: ", i);
        scanf("%d", &station);
        // inserting this person in the initial day struct

        add_day_struct(i, station);
    }
    printf("\n");
    ///////NOW we have done the initial requirements for our code

    cur_day = 0;
    // we are currently on the 0th day(starting day)

    //// We will entertain query 1 only at the end of the query
    //// input 1 means query1
    //// input 2 means query2
    //// input 3 means given the id of a person we will print what is his current situation like status,which station he is present,etc
    //// input 4 means given the station no ..we will print the list of covid positive ,primary,secondary,negative people in that station
    //// input 5 means we are done for that day
    //// input -1 means get out of the program
    printf("(1)->Query 1 /// (2)->Query 2 /// (3)->Query 3 /// (4)->Query 4\n\n");

    while (1)
    {
        bool if_added = false;

        printf("We are currently at day %d\n\n", cur_day + 1);

        struct __path *temp; // stores the latest path that we get from query 2 to reduce traversal time
        copy_day(cur_day, num_stations, num_person);
        DayIncrement((cur_day + 1) % 16, num_person);

        while (1)
        {
            int query;
            printf("Give query: ");
            scanf("%d", &query);

            if (query == 2)
            {
                int person_id;
                printf("Give the person's id who wants to travel: ");
                scanf("%d", &person_id);

                if (day[(cur_day + 1) % 16].person[person_id].status == 4)
                {
                    fprintf(stderr, "quarantined person cannot move");
                    break;
                }

                int from_where, to_where;
                printf("Give the id of the source and target stations: ");
                scanf("%d %d", &from_where, &to_where);
                if (day[(cur_day + 1) % 16].person[person_id].station != from_where)
                {
                    fprintf(stderr, "Person not present on current station");
                    break;
                }
                printf("\n");

                _possible_path *temp1 = (_possible_path *)malloc(sizeof(_possible_path)); ///// TEMP1 points to possible routes
                temp1 = three_way_dijkstra_implementation(graph, from_where, to_where, (cur_day) % 16);

                if (temp1->no_of_possible_path == 0)
                {
                    printf("There are no possible paths from %d to %d\n\n", from_where, to_where);
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

                    printf("The corresponding danger value for this path is %lf\n\n", temp1->best_path_danger_value);
                    int may_accept;
                    // enter 1 if u want to go through this path else 0
                    printf("Do you want to traverse through this path: ");
                    scanf("%d", &may_accept);

                    if (may_accept)
                    {
                        if (!if_added)
                        {
                            if_added = true;
                            day[(cur_day) % 16].path = (struct __path *)malloc(sizeof(struct __path));
                            day[(cur_day) % 16].path->person_id = person_id;
                            day[(cur_day) % 16].path->next_station = temp2;
                            day[(cur_day) % 16].path->next_person = NULL;
                            temp = day[(cur_day) % 16].path;
                            UpdateForPerson(temp2, person_id, cur_day + 1);
                        }
                        else
                        {
                            struct __path *temp3 = (struct __path *)malloc(sizeof(struct __path));
                            temp3->person_id = person_id;
                            temp3->next_station = temp2;
                            temp3->next_person = NULL;
                            temp->next_person = temp3;
                            temp = temp3;
                            UpdateForPerson(temp2, person_id, cur_day + 1);
                        }
                    }
                    else //path not accepted
                        continue;
                }

                else if (temp1->no_of_possible_path == 2)
                {
                    printf("There are only two possible path as follows: \n\n");
                    struct __route *temp2 = &(temp1->best_path);

                    do
                    {
                        printf("%d -> ", temp2->station_id);
                        temp2 = temp2->next_station;
                    } while (temp2 != NULL);

                    temp2 = &(temp1->best_path);
                    printf("Destination Reached\n");

                    printf("The corresponding danger value for this path is %lf\n\n", temp1->best_path_danger_value);
                    temp2 = &(temp1->second_best_path);

                    do
                    {
                        printf("%d -> ", temp2->station_id);
                        temp2 = temp2->next_station;
                    } while (temp2 != NULL);

                    printf("Destination Reached\n");

                    printf("The corresponding danger value for this path is %lf\n\n", temp1->second_best_path_danger_value);

                    int may_accept;
                    // enter 1 if u want to go through this path else 0
                    printf("Do you want to traverse through any of the above paths: ");
                    scanf("%d", &may_accept);

                    if (may_accept)
                    {
                        printf("Which path do you want to go through: ");
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
                        day[(cur_day) % 16].path = (struct __path *)malloc(sizeof(struct __path));
                        day[(cur_day) % 16].path->person_id = person_id;
                        day[(cur_day) % 16].path->next_station = temp2;
                        day[(cur_day) % 16].path->next_person = NULL;
                        temp = day[(cur_day) % 16].path;
                        UpdateForPerson(temp2, person_id, cur_day + 1);
                    }
                    else
                    {
                        struct __path *temp3 = (struct __path *)malloc(sizeof(struct __path));
                        temp3->person_id = person_id;
                        temp3->next_station = temp2;
                        temp3->next_person = NULL;
                        temp->next_person = temp3;
                        temp = temp3;
                        UpdateForPerson(temp2, person_id, cur_day + 1);
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

                    printf("The corresponding danger value for this path is %lf\n\n", temp1->best_path_danger_value);
                    temp2 = &(temp1->second_best_path);

                    do
                    {
                        printf("%d -> ", temp2->station_id);
                        temp2 = temp2->next_station;
                    } while (temp2 != NULL);

                    temp2 = &(temp1->second_best_path);
                    printf("Destination Reached\n");

                    printf("The corresponding danger value for this path is %lf\n\n", temp1->second_best_path_danger_value);
                    temp2 = &(temp1->third_best_path);

                    do
                    {
                        printf("%d -> ", temp2->station_id);
                        temp2 = temp2->next_station;
                    } while (temp2 != NULL);

                    printf("Destination Reached\n");
                    printf("The corresponding danger value for this path is %lf\n\n", temp1->third_best_path_danger_value);

                    int may_accept;
                    // enter 1 if u want to go through this path else 0
                    printf("Do you want to traverse through any of the above paths: ");
                    scanf("%d", &may_accept);

                    if (may_accept)
                    {
                        printf("Which path do you want to go through: ");
                        int which_path; // either 1 or 2 or 3
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
                        day[(cur_day) % 16].path = (struct __path *)malloc(sizeof(struct __path));
                        day[(cur_day) % 16].path->person_id = person_id;
                        day[(cur_day) % 16].path->next_station = temp2;
                        day[(cur_day) % 16].path->next_person = NULL;
                        temp = day[(cur_day) % 16].path;
                        UpdateForPerson(temp2, person_id, cur_day + 1);
                    }
                    else
                    {
                        struct __path *temp3 = (struct __path *)malloc(sizeof(struct __path));
                        temp3->person_id = person_id;
                        temp3->next_station = temp2;
                        temp3->next_person = NULL;
                        temp->next_person = temp3;
                        temp = temp3;
                        UpdateForPerson(temp2, person_id, cur_day + 1);
                    }
                }
            }

            else if (query == 1)
            {
                int no_input_people;
                printf("How many people are covid positive ");
                scanf("%d", &no_input_people);
                printf("Enter value of X: ");
                int X;
                scanf("%d", &X);
                int *list_no_people;
                list_no_people = (int *)calloc(no_input_people, sizeof(int));
                printf("Enter list of all covid positive people: ");
                for (int i = 0; i < no_input_people; i++)
                    scanf("%d", list_no_people + i);
                Backtrace(cur_day - X + 1, cur_day + 1, list_no_people, no_input_people, num_person, num_stations);
                free(list_no_people);
                cur_day++;
                break;
                //6th day --> day[7];
            }

            else if (query == 3)
            {
                printf("Give the id of the person whose status u want to print: ");
                int temp1;
                scanf("%d", &temp1);
                printf("Person id:  %d\n\nStatus:  %d\n\nCurrently on which station:  %d\n\n", temp1, day[(cur_day + 1) % 16].person[temp1].status, day[(cur_day + 1) % 16].person[temp1].station);
                if (day[(cur_day + 1) % 16].person[temp1].status && day[(cur_day + 1) % 16].person[temp1].status < 5)
                {
                    printf("Cause: %d\nNumber of days of infection: %d\n", day[(cur_day + 1) % 16].person[temp1].cause, day[(cur_day + 1) % 16].person[temp1].days);
                }
                printf("\n");
            }
            else if (query == 4)
            {
                printf("GIve the station no whose details u want to print: ");
                int temp1;
                scanf("%d", &temp1);
                printf("Station id:  %d \n\nWorst affected person in this station:   %d\n\nDanger value of this station:  %f\n\n", temp1, day[(cur_day + 1) % 16].station[temp1].worst_affected, day[(cur_day + 1) % 16].station[temp1].danger_value);
                printf("List of people in this station: \n");
                struct __list *temp = day[(cur_day + 1) % 16].station[temp1].list;
                if(temp==NULL)
                {
                    printf("Station is Empty.\n");
                    continue;
                }
                while (temp != NULL)
                {
                    printf("Person_id: %d     Person_status: %d\n\n", temp->person_id, day[(cur_day + 1) % 16].person[temp->person_id].status);
                    temp = temp->next;
                }
                printf("\n\n");
            }
            else if (query == 5)
            {
                cur_day = cur_day + 1;
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
