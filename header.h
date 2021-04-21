#ifndef __HEADER__
#define __HEADER__

#define NUM 100000

enum  _status {negative, secondary_contact, primary_contact, positive};
struct __day {
	_person person[NUM];
	_path path;
	_station station[NUM];
};

struct __person {
	_status status;
	int days;
	int cause;
	int station;
};

struct __station {
	_status worst_affected;
	_list list;
};
struct __list {
	int person_id;
	struct __list* next;
};
struct __path {
	int person_id;
	_route* next_station;
	struct __path* next_person;
};
struct __route {
	int station_id;
	struct __route* next_station;
};
typedef struct __day _day;
typedef struct __person _person;
typedef struct __station _station;
typedef struct __path _path;
typedef struct __route _route;
typedef struct __list _list;

#endif
