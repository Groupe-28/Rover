#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LIMIT_SIZE 10000
#define EARTH_RADIUS 6371

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

//FUNCTIONS

//distance calculation between two consecutive points
double distance (double latitudeA, double longitudeA, double latitudeB, double longitudeB)
{
    //We use the Haversine formula
    double latDistance = latitudeB - latitudeA;
    double longDistance = longitudeB - longitudeA;
    double average = (sin (latDistance/2) * sin(latDistance/2)) + (cos(latitudeA)*cos(latitudeB)*sin(longDistance/2)*sin(longDistance/2));
    double ceil = atan(sqrt(average)/sqrt(1-average));
    double distance = EARTH_RADIUS*ceil;
    return distance;

}

/*double degToRad(double degAngle)
{
    return degAngle*(M_PI/180);
}*/

//read GPS data from HMI
char* readGPSData (FILE* file)
{
     char* chaine = malloc (LIMIT_SIZE*sizeof(char));

     if (file != NULL)
     {
         fgets(chaine, LIMIT_SIZE, file);
         printf("LECTURE DU FICHIER GPS:\n");
         printf("%s\n\n", chaine);

         fclose(file);
     }

     return chaine;

}

Way *newWay ()
{
    Way *p_new = malloc(sizeof *p_new);
    if (p_new != NULL)
    {
        p_new->nbPoints = 0;
        p_new->firstPoint = NULL;
        p_new->lastPoint = NULL;
    }
    return p_new;
}

// add a new point on the robot trajectory
Way *dlist_append(Way *p_list, double latitude, double longitude)
{
    if (p_list != NULL) /* On vérifie si notre liste a été allouée */
    {
        Coordinates *p_new = malloc(sizeof *p_new); /* Création d'un nouveau node */
        if (p_new != NULL) /* On vérifie si le malloc n'a pas échoué */
        {
            p_new->latitude = latitude; /* On 'enregistre' notre donnée */
            p_new->longitude = longitude; /* On fait pointer p_next vers NULL */
            p_new ->next = NULL;
            if (p_list->lastPoint == NULL) /* Cas où notre liste est vide (pointeur vers fin de liste à  NULL) */
            {
                p_new->prev = NULL; /* On fait pointer p_prev vers NULL */
                p_new ->distanceFromLastPt = 0;
                p_list->firstPoint = p_new; /* On fait pointer la tête de liste vers le nouvel élément */
                p_list->lastPoint = p_new; /* On fait pointer la fin de liste vers le nouvel élément */
            }
            else /* Cas où des éléments sont déjà présents dans notre liste */
            {
                p_list->lastPoint->next = p_new; /* On relie le dernier élément de la liste vers notre nouvel élément (début du chaînage) */
                p_new->prev = p_list->lastPoint; /* On fait pointer p_prev vers le dernier élément de la liste */
                p_new -> distanceFromLastPt = distance(p_new->prev->latitude, p_new->prev->longitude, p_new ->latitude,p_new->longitude);
                p_list->lastPoint = p_new; /* On fait pointer la fin de liste vers notre nouvel élément (fin du chaînage: 3 étapes) */
            }
            p_list->nbPoints++; /* Incrémentation de la taille de la liste */
        }
    }
    return p_list; /* on retourne notre nouvelle liste */
}

void printTrajectory (Way *trajectory)
{
    int countPoints = 0;
    Coordinates *actualPoint = trajectory -> firstPoint;
    while (actualPoint != NULL)
        {
            countPoints++;
            printf ("Point numero %d\n", countPoints);
            printf ("Latitude: %lf\n", actualPoint -> latitude);
            printf ("Longitude: %lf\n", actualPoint -> longitude);
            printf ("Distance du pt precedent: %lf\n\n", actualPoint -> distanceFromLastPt);
            actualPoint = actualPoint-> next;
        }

}

// robot trajectory definition
Way* BuildRoverWay (char* readFileContent)
{
        int valueFlag = 0;
        int valueFlagLat = 0;
        int valueFlagLong = 0;
        double latValue;
        double longValue;
        double value;

    // Separators definition => simplification of the file reading
    const char * separators = " ,:[{]}\"-!";

    // Analysis of each term in the string chaine without the separators
    char * strToken = strtok ( readFileContent, separators );
    Way* direction = newWay();

    printf("LISTE DES POINTS RELEVES PAR LE GPS:\n");
    while ( strToken != NULL ) {
        if (strcmp (strToken,"latitude") == 0)
        {
            valueFlag = 1;
            valueFlagLat = 1;

        }
        else if (strcmp(strToken, "longitude") == 0)
        {
            valueFlag = 1;
            valueFlagLong = 1;

        }
        else if (valueFlag == 1)
        {
            if (valueFlagLat == 1){
                // we take the latitude value of the point
                value = atof(strToken);
                latValue = value;
                valueFlagLat = 0;
                valueFlag = 0;
            }
            else if (valueFlagLong== 1){
                // we take the longitude value of the point
                value = atof(strToken);
                longValue = value;
                direction = dlist_append(direction, latValue, longValue);
                valueFlagLong = 0;
                valueFlag = 0;
        }

        }

        // We choose the next term (on the GPS data file)
        strToken = strtok ( NULL, separators );

    }
    return direction;
    //printTrajectory(direction);
    //distanceToNextPoint(direction);

}
