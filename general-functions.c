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
			return 1;
		default:
		fprintf(stderr, "Error, status out of specified range\n");
		return 0;
	}
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
	}
	return start; 
}
void PrintStatus(int n)
{
	printf("%s",status_mapping[n]);
}
