#include "header.h"
#include <stdlib.h>
#include <stdio.h>
void UpdateData(int id_person, int id_station, int daye)
{
	if (day[daye%16].person[id_person].status = 2 || day[daye%16].station[id_station].worst_affected = 2 || day[daye%16].person[id_person].status = 3 || day[daye%16].station[id_station].worst_affected = 3)
	{
		if (day[daye%16].person[id_person].status > day[daye%16].station[id_station].worst_affected)
		{
			UpdateStation(id_person, id_station, daye);
		}
		// ---> person coming to the station is more infected than whole station.
		else if (day[daye%16].person[id_person].status < day[daye%16].station[id_station].worst_affected)
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
	L = day[daye%16].station[id_station].list;
	while (L != NULL)
	{
		if(day[daye%16].person[L->person_id].status != 4) {
			day[daye%16].person[L->person_id].status = day[daye%16].person[id_person].status - 1;
			day[daye%16].person[L->person_id].cause = id_person;
			day[daye%16].person[L->person_id].days = 0;
			day[daye%16].station[id_station].danger_value += getDangerIndex(L->person_id, daye);
			L = L->next;
		}
	}
	// Changed the status of all people on the station.
	day[daye%16].station[id_station].worst_affected = day[daye%16].person[id_person].status - 1;
}
void UpdatePerson(int id_person, int id_station, int daye)
{
	_list *L;
	day[daye%16].person[id_person].status = day[daye%16].station[id_station].worst_affected - 1;
	L = day[daye%16].station[id_station].list;
	while (L != NULL)
	{
		if (day[daye%16].person[L->person_id].status == day[daye%16].station[id_station].worst_affected)
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
	day[daye%16].person[id_person].cause = L->person_id; // cause will be any arbitrary person on the station who has status same as worst affected.
	day[daye%16].person[id_person].days = 0;
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
	L = day[daye%16].station[station_from].list;
	L = DeleteNodeById(L, id_person);
	day[daye%16].station[station_from].list = L;

	// then update the station's danger value.
	day[daye%16].station[station_from].danger_value -= getDangerIndex(id_person, daye);

	// check if person moved was worst affected, in that case, update worst affected status
	if (day[daye%16].person[id_person].status == day[daye%16].station[station_from].worst_affected)
		day[daye%16].station[station_from].worst_affected = getWorstAffected(station_from, daye);
}
void AddPerson(int id_person, int station_to, int daye)
{
	// Therefore we have to update the danger value, worst affected and id_person to the list.

	_list *L = day[daye%16].station[station_to].list;
	L = AddNodeAtStart(L, id_person);
	day[daye%16].station[station_to].list = L;
	day[daye%16].station[station_to].danger_value += getDangerIndex(id_person, daye);
	if (day[daye%16].person[id_person].status > day[daye%16].station[station_to].worst_affected)
		day[daye%16].station[station_to].worst_affected += 1;

	// update the location of the person:
	day[daye%16].person[id_person].station = station_to;
}
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

void Backtrace(int start_day, int end_day, int* list, int inum_people, int num_person, int num_stations)
{
	for(int i = 0; i < inum_people; i++) {
		day[start_day%16].person[list[i]].status = 3;
		day[start_day%16].person[list[i]].days = 0;
		int cur_station = day[start_day%16].person[list[i]].station;
		day[start_day%16].station[cur_station].worst_affected = 3;
		_list* L;
		L = day[start_day%16].station[cur_station].list;
		while(L != NULL) {
			if(day[start_day%16].person[L->person_id].status != 3) {
				day[start_day%16].person[L->person_id].status = 2;
				day[start_day%16].person[L->person_id].cause = list[i];
				day[start_day%16].person[L->person_id].days = 0;
				day[start_day%16].station[cur_station].danger_value += 0.2;
			}
			L = L->next;
		}
	}

	_path *P;
	for (int i = start_day; i <= end_day;i++)
	{
		P = day[(i-1)%16].path;
		UpdateForDay(P, i);
		//PrintStationInfo(i,num_stations);
		PrintQuery1(i,list,inum_people,num_person);
		if(i >= end_day)
			break;
		//copy_day(i,num_stations, num_person); 
		DayIncrement(i+1, num_person);
	}
}

void DayIncrement(int daye, int T_ppl)
{
	int s_no;  //station number of the person
	for (int i = 0; i < T_ppl; i++)
	{
		// if person is covid +ve increase his no. of days affected
		if(day[daye%16].person[i].status == 4) {
			day[daye%16].person[i].days++;
		}
		if (day[daye%16].person[i].status == 3)
		{
			day[daye%16].person[i].days++;
		}
		// if a primary contact is present on a station where worse affected person is not present,increment day count
		else if (day[daye%16].person[i].status == 2)
		{
			s_no = day[daye%16].person[i].station;
			if (day[daye%16].station[s_no].worst_affected <= 2)
			{
				day[daye%16].person[i].days++;
			}
		}
		//if 2ndary contact is present on a station where worse affected person is not present,increment day count
		else if (day[daye%16].person[i].status == 1)
		{
			s_no = day[daye%16].person[i].station;
			if (day[daye%16].station[s_no].worst_affected <= 1)
			{
				day[daye%16].person[i].days++;
			}
		}
		//for covid -ve person,no need of changing day count

		/* if the total affected days = 14,then the person is not considered to be affected 
		   and changing the status of the person might change the danger value and 
		   the worst affected of the station on which he was present*/
		if (day[daye%16].person[i].days == 14)
		{
			s_no = day[daye%16].person[i].station;
			int temp = day[daye%16].person[i].status;
			day[daye%16].station[s_no].danger_value = day[daye%16].station[s_no].danger_value - getDangerIndex(i, daye);
			day[daye%16].person[i].days = -1;
			day[daye%16].person[i].status = 0;
			if(temp == day[daye%16].station[s_no].worst_affected)
			{
				day[daye%16].station[s_no].worst_affected = getWorstAffected(s_no,daye);
			}
		}
	}
}

void Copy(int curr_day,int T_sta,int T_ppl)
{
	for(int i=0;i<T_ppl;i++)
	{
		day[(cur_day + 1) % 16].person[i].status = day[cur_day%16].person[i].status;
		day[(cur_day + 1) % 16].person[i].days = day[cur_day%16].person[i].days;
		day[(cur_day + 1) % 16].person[i].cause = day[cur_day%16].person[i].cause;
	}
	for (int i = 0; i < T_sta; i++)
	{
		day[(cur_day + 1) % 16].station[i].worst_affected = day[cur_day%16].station[i].worst_affected;
		day[(cur_day + 1) % 16].station[i].danger_value = day[cur_day%16].station[i].danger_value;
	}
}
