#include "header.h"
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

void DayIncrement(int daye, int T_ppl)
{
	
	for (int i = 0; i < T_ppl; i++)
	{
		// if person is covid +ve increase his no. of days affected
		if (day[daye].person[i].status == 3)
		{
			day[daye].person[i].days++;
		}
		// if a primary contact is present on a station where worse affected person is not present,increment day count
		else if (day[daye].person[i].status == 2)
		{
			int s_no = day[daye].person[i].station;
			if (day[daye].station[s_no].worst_affected <= 2)
			{
				day[daye].person[i].days++;
			}
		}
		//if 2ndary contact is present on a station where worse affected person is not present,increment day count
		else if (day[daye].person[i].status == 1)
		{
			int s_no = day[daye].person[i].station;
			if (day[daye].station[s_no].worst_affected <= 1)
			{
				day[daye].person[i].days++;
			}
		}
		//for covid -ve person,no need of changing day count
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
void Backtrace(int start_day, int end_day)
{
	_path *P;
	for (int i = start_day; i <= end_day; i++)
	{
		P = day[i].path;
		UpdateForDay(P, i);
	}
}
