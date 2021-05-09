# COVID-Tracing

We would be submitting 4 files on moodle with the names aryan_main.c,aryan_fucntion.c,general_functions.c,update_functions.c .

Compilation instruction: Use  gcc aryan_main.c  aryan_function.c general-functions.c update-functions.c


// BASIC INFO:

a) For status -> 0 denotes normal person,1 denotes secondary contact,2 denotes primary contact , 3 denotes covid positive person 



How to run test cases:

1. On the first line give the no of stations(num_stations),no of roads(num_roads),
no of persons(num_person).

2. On the next num_roads lines, Give three values on each line - from ,to distance which denotes the ith road starting from station "from" to station "to" and the distance between these two stations being "distance"

3. On the next num_preson lines ,give one value on each line "station" which would denote the station at the which the ith person is present.

4.Now we would from day 1:

We can perform four queries

a)  : Entering (1) would mean query 1 which is that given the list of covid positive people on some day,we backtrack the last D days and print the list of people who got infected due to those peoples (basically printing list of all people who became primary or secondary contact due to those people) .

Input instructions:

first enter the number of people on the list, followed by the value of X. Then give a list of all the people who are declared to be covid positive
The output will be a list of all the primary and secondary contacts of the list L on each day

b)  : Entering (2) would mean query no 2 which is that given a person id and the station to where he wants to go ,we would give him the three(if possible) most optimal paths for doing this and then he would have the option to decide if he wants to traverse through this path.

Input instructions:

Just enter three values "person_id" ,"from_where" and "to_where" which which denote the person who wants to travel, from where he wants to travel and to where he wants to travel.
Now we will print the most optimal paths 
If the person wants to traverse through any of the paths say path 2,then he should enter 1 first to signify that he wants to travel and then enter the "path no".
If the person decides not to through through any of the paths then he just has to print 0.

c)  : Entering (3) would print the status of the asked person like on which station that person is present currently , what is his status , if he is primary or secondary contact then who affected him and since how many days he has been in this condition.

Input Instructions:

Just enter the id of the person whose info u want to print

d)   : Entering (4) would print the condition of the asked station like the list of all the people in that station and their condition like whether they are covid positive or primary contact or secondary contact or negative.

//////////////////

e)   : Entering (5) would mean that we are done for all the queries on this day and now we can move on to the next day.

f)   : Entering (-1) would exit us from the program

Also if we give query 1, then after we process it, we would directly goto the next day so we don't need to give 5 again

////////////




Assumptions:

1) We assumed that the primary and secondary contact would also remain in that condition for 14 days like covid positive people but they won't be quarantined.

2) For the 3-way shortest path we won't consider any path if it has a 2-way cycle although cycles of length greater than 2 would be considered.

3) Also say from some node A , the shortest path to say B involves going through C such that it involves a 2-way cycle ,then also we won't consider that path.

4) At each day we assume that we are provided with only one list(query 1) that comprises of all the person  found to be covid positive at the end of the day that is as soon as query 1 is provided ,we assume that no more people will travel on that day.

5) While backracking , given value of X ,we assume that the given list of people have been covid positive for the past X days.So that means that we have considered the given list of people since the beginning of day D-X.

6) We assume that after 14 days,the person is not affected any more.

7) If there are more than one say primary contact and a person becomes secondary contact due to them ,then we randomly considr any one of the primary contacts as cause for the secondary contact of the new person who arrived.



