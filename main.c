/**
    Author : Terra Mule Team (Gauthier MARCELIN)
    Date: 18/06/2023
    Function: Rover State Machine implementation
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "InternalFunctions.h"

//#define LIMIT_SIZE 10000
#define DEBUG

int main()
{
    //STATES OF THE STATE MACHINE
    enum structure {STOP, MOVE, TEAR, SELECT_TREE, PLANT, COVER};
    enum structure rover = STOP;

    //STATE MACHINE STRUCTURE
    switch (rover)
    {
    case STOP:
        break;
    case MOVE:
        break;
    case TEAR:
        break;
    case SELECT_TREE:
        break;
    case PLANT:
        break;
    case COVER:
        break;
    default:
        exit(EXIT_FAILURE);
    }

     /*Debugging phase*/
     #ifdef DEBUG

    //GPS Files
    FILE* myfile = NULL;
    char* chaine = "";
    Way *myWay;
    myfile = fopen("geo", "r+");

    // reading of data transmitted by the GPS
    chaine = readGPSData(myfile);

    myWay = BuildRoverWay(chaine);
    printTrajectory(myWay);


    #endif // DEBUG
}
