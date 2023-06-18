#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Structures: POINTS OF THE ROVER'S WAY
    typedef struct Coordinates
    {
        double latitude;
        double longitude;
        struct Coordinates *next;
        struct Coordinates *prev;
        double distanceFromLastPt;
    } Coordinates;

    typedef struct Way
    {
        int nbPoints;
        Coordinates *firstPoint;
        Coordinates *lastPoint;

    } Way;



//Functions
double distance (double latitudeA, double longitudeA, double latitudeB, double longitudeB);
char* readGPSData (FILE* file);
Way* BuildRoverWay (char* readFileContent);
Way *newWay ();
Way *dlist_append(Way *p_list, double latitude, double longitude);
void printTrajectory (Way *trajectory);
