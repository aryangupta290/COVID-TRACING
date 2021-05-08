#include "header.h"
#include <stdlib.h>
#include <stdio.h>
const char* status_mapping[] = {
	"negative", "secondary_contact", "primary_contact", "positive"
};
float getDangerIndex(int id_person, int daye)
{
	int severity = day[daye].person[id_person].status;
	switch(severity) {
		case 0:
			return 0;
		case 1:
			return 0.1;
		case 2:
			return 0.2;
		case 3: 
			return 0;
		default:
		fprintf(stderr, "Error, status out of specified range\n");
		return 0;
	}
	//case 3: covid +ve person contributes 1 to the danger value but since it doesn't affect anything according to our assumptions we take the danger value to be 0
}
int getWorstAffected (int id_station, int daye)
{
	_list* L;
	L = day[daye].station[id_station].list;
	int iworst_affected = 0;
	while(L != NULL) {
		if(iworst_affected < day[daye].person[L->person_id].status)
			iworst_affected = day[daye].person[L->person_id].status;
		L = L->next;
	}
	return iworst_affected;
}
_list* AddNodeAtStart(_list* L, int id_person)
{
	_list* temp;
	temp = L;
	L = (_list*)malloc(sizeof(_list));
	L->person_id = id_person;
	L->next = temp;
	return L;
}
_list* DeleteNodeById(_list* L, int id_person)
{
	_list* start;
	start = L;
	_list* temp;
	if(L->person_id == id_person) {
		temp = L->next;
		free(L);
		return temp;
	}
	while(L->next != NULL) {
		if(L->person_id == id_person){
			temp = L->next->next;
			free(L->next);
			L->next = temp;
		}
		L = L->next;
	}
	return start; 
}
void PrintStatus(int n)
{
	printf("%s\n",status_mapping[n]);
}
void PrintQuery1(int daye, int* list, int inum_people, int num_person)
{
	printf("For day %d:\n",daye);
	for(int id_person = 0; id_person < num_person; id_person++) {
		if(IsOnList(daye, id_person, list, inum_people)) 
			PrintPersonStatus(daye, id_person);
	}
}
int IsOnList(int daye, int id_person, int* list, int inum_people) 
{
	for(int i = 0; i < inum_people; i++) {
		if(day[daye].person[id_person].cause == list[i])
			return 1;
	}
	return 0;
}
void PrintPersonStatus(int daye, int id_person)
{
	printf("\n");
	_person* per = &(day[daye].person[id_person]);
	printf("Person ID is %d\n",id_person);
	printf("Person cause is %d\n",per->cause);
	printf("Person is on station %d\n",per->station);
	printf("Person status is %d, which corresponds to ",per->status);
	PrintStatus(per->status);
}
void PrintStationDetails(int daye, int id_station)
{
	printf("\n");
	printf("worst affected = %d\n", day[daye].station[id_station].worst_affected);
	printf("danger value = %lf\n", day[daye].station[id_station].danger_value);
	_list* L;
	L = day[daye].station[id_station].list;
	printf("list of people on the station:\n");
	while(L != NULL) {
		printf("%d\n", L->person_id);
		L = L->next;
	}
}
void PrintStationInfo(int daye, int num_stations) 
{
	for(int id_station = 0; id_station < num_stations; id_station++) {
		PrintStationDetails(daye, id_station);
	}
}
