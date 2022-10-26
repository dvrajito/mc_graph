/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Authors: Dana Vrajitoru
            Jason DeBoni
   File:    GraphEval.cc
   Updated: October 2022

   Functions for evaluating graphs for the purpose of
   genetic algorithms.

*********************************************************************/

#include <iostream>
#include "GraphEval.h"
#include <cstdlib>
#include "List.h"
#include "Point.h"
#include <cmath>
using namespace std;

//#include <fstream>
//#include <strstream>
//#include <cstring>
//#include "Graph.h"
//#include "name_table.h"
//#include "GraphExtra.h"
#include "GraphPos.h"

//Calculates the standard deviation of the points in the graph
//Returns a Point with the stddev for x,y and z.
void VolumeSwag(Point* Points, int vertexNr, Point& StdDev)
{
    int i;
    //double SumX, SumY, SumZ;

    Point PointSum(0, 0, 0, 0);
    Point Average(0, 0, 0, 0);
    StdDev.Init(0.0);

    PointSum.x = PointSum.y = PointSum.z = 0;
    Average.x = Average.y = Average.z = 0;
    //StdDev.x = StdDev.y = StdDev.z = 0;

    for (i = 0; i < vertexNr; i++)
    {
        PointSum.x += Points[i].x;
        PointSum.y += Points[i].y;
        PointSum.z += Points[i].z;
    }

    Average.x = PointSum.x / vertexNr;
    Average.y = PointSum.y / vertexNr;
    Average.z = PointSum.z / vertexNr;

    for (i = 0; i < vertexNr; i++)
    {
        StdDev.x += (Points[i].x - Average.x) * (Points[i].x - Average.x)
            / vertexNr;
        StdDev.y += (Points[i].y - Average.y) * (Points[i].y - Average.x)
            / vertexNr;
        StdDev.z += (Points[i].z - Average.z) * (Points[i].z - Average.x)
            / vertexNr;
    }
    StdDev.x = sqrt(StdDev.x) / vertexNr;
    StdDev.y = sqrt(StdDev.y) / vertexNr;
    StdDev.z = sqrt(StdDev.z) / vertexNr;
}

//Determines the separation of edges formed between the specified point
//and all of its neighbors.  The 3D graph is projected onto a 2D graph 
//in order for this calculation to be completed.  The variable Projection
//determines which axis (y or z) will be projected to the 2D graph.  
//Default value is 0 (y-axis), a value of one causes the function 
//to use the z-axis
double AngleSwag(Point* Points, Graph* aGraph, int StartPoint, int Projection)
{
    ListW* EdgeList = aGraph->edgeList;
    ListNodeW* AdjacentNode;
    int Neighbor, AngleCount = 0, i;
    double Slope, Angle, TotalAngle, Average, X1, X2, Y1, Y2, Sum, Diff;
    double* AngleList = new double[aGraph->vertexNr];
    double* AngleDiff = new double[aGraph->vertexNr];

    TotalAngle = 0;

    for (AdjacentNode = EdgeList[StartPoint].head; AdjacentNode != NULL;
         AdjacentNode = AdjacentNode->next)
    {
        Neighbor = AdjacentNode->value;
        if (Projection == 0)
        {
            X1 = Points[StartPoint].x;
            Y1 = Points[StartPoint].y;
            X2 = Points[Neighbor].x;
            Y2 = Points[Neighbor].y;
        }
        else
        {
            X1 = Points[StartPoint].x;
            Y1 = Points[StartPoint].z;
            X2 = Points[Neighbor].x;
            Y2 = Points[Neighbor].z;
        }

        //First step is to move the line so that the StartPoint (X1,Y1) 
        //is at the origin (0,0).  This requires that the Neighbor point
        //be moved and equivalent distance.
        if (X1 < 0)
            X2 += abs(X1);
        else
            X2 -= X1;

        if (Y1 < 0)
            Y2 += abs(Y1);
        else
            Y2 -= Y1;

        X1 = Y1 = 0;

        //KISS, all values will be converted to positive, in 
        //order to place the line in the first quadrant of a 2D graph.  
        //After that happens the angle of the resulting line will be 
        //found and the angle of the line in its original 
        //quadrant will be determined.  
        if (X2 < 0 && Y2 > 0)
        {
            //For lines from quadrant II
            X2 *= -1;
            Slope = (Y2 - Y1) / (X2 - X1); //** could be /X2 since X1=0
            Angle = ((atan(Slope) * 180) / PI);
            Angle = 180 - Angle; //** I think this is the correct value
        }
        else if (X2 < 0 && Y2 < 0)
        {
            //Lines from quadrant III
            X2 *= -1;
            Y2 *= -1;
            Slope = (Y2 - Y1) / (X2 - X1); //** same as before
            Angle = ((atan(Slope) * 180) / PI);
            Angle = 180 + Angle; //** I think this is the correct value
        }
        else if (X2 > 0 && Y2 < 0)
        {
            //Lines from quadrant IV
            Y2 *= -1;
            Slope = (Y2 - Y1) / (X2 - X1);
            Angle = ((atan(Slope) * 180) / PI);
            Angle = (360 - Angle); //** You had this one right
        }
        else if (X2 == 0) //** Added this case to avoid a division by 0
            if (Y2 > 0)
                Angle = 90;
            else
                Angle = 270;
        else
        {
            //Lines already in quadrant I
            Slope = (Y2 - Y1) / (X2 - X1);
            Angle = ((atan(Slope) * 180) / PI);
        }
        //** I moved these here since they are the same in every case.
        AngleList[AngleCount] = Angle;
        AngleCount++;
    }

    //The angles are calculated and stored in the graph in an essentially 
    //random order, they need to be sorted before the separation between
    //the angles can actually be determined.  
    SortAngles(AngleList, AngleCount);

    //Calculate the difference between each angle, and store the 
    //resulting values 
    for (i = 1; i < AngleCount; i++)
        AngleDiff[i] = AngleList[i] - AngleList[i - 1];

    //The first angle value needs to be adjusted, the separation between
    //this angle and the last one needs to be determined.
    AngleDiff[0] = ((360 - AngleList[i]) + AngleList[0]);

    //Value that should be found if each edge is evenly spaced through 360 deg
    Diff = 360.0 / AngleCount;

    Sum = 0;
    for (i = 0; i < AngleCount; i++)
        Sum += (AngleDiff[i] - Diff) * (AngleDiff[i] - Diff);

    //Average of the separation between edges as calculated from the graph
    Average = sqrt(Sum) / AngleCount;

    return Average;

}

void SortAngles(double* AngleList, int AngleCount)
{
    double TempAngle;
    int SwapMade = 1, Count = 1, i;

    while (SwapMade == 1)
    {
        SwapMade = 0;
        for (i = 0; i < AngleCount - 1; i++)
        {
            if (AngleList[i] > AngleList[i + 1])
            {
                TempAngle = AngleList[i];
                AngleList[i] = AngleList[i + 1];
                AngleList[i + 1] = TempAngle;
                SwapMade = 1;
            }
        }
    }
}

//Uses the AngleSwag function to determine an average for all the 
//points in the specified graph
//Calculates the average for both the y and z axes and averages
//the results together.
double GraphAngleEvaluation(Point* Points, Graph* aGraph)
{
    int PointCount = aGraph->vertexNr;
    int i;
    double GraphSumY, GraphSumZ, GraphAverage;
    double GraphAverageY, GraphAverageZ;
    GraphSumY = GraphSumZ = 0;

    //Calculate the deviation for each point in both the y and z axes
    //These values will be averaged together later.
    for (i = 0; i < PointCount; i++)
    {
        GraphSumY += AngleSwag(Points, aGraph, i, 0);
        GraphSumZ += AngleSwag(Points, aGraph, i, 1);
    }

    //Determine the average of the Y and X axes and then average those
    //results together for an overall average for the graph.  
    GraphAverageY = GraphSumY / PointCount;
    GraphAverageZ = GraphSumZ / PointCount;
    GraphAverage = ((GraphAverageY + GraphAverageZ) / 2);

    return GraphAverage;

}

double PointDistance(Point PointA, Point PointB)
{
    double Distance;
    Distance = sqrt((pow(PointA.x - PointB.x, 2) + pow(PointA.y - PointB.y, 2)));
    return Distance;
}

double GraphSurfaceArea(Point* Points, Graph* aGraph)
{
    ListW* EdgeList = aGraph->edgeList;
    ListNodeW* AdjacentNode;
    Point PointA, PointB, PointC;
    int Neighbor, AngleCount = 0, h, i;
    int PointCount = aGraph->vertexNr;
    double Slope, Angle, X1, X2, Y1, Y2;
    double TotalArea = 0.0;
    double* AngleList = new double[PointCount];
    double* AngleDiff = new double[PointCount];
    int* NeighborList = new int[PointCount];

    for (h = 0; h < PointCount; h++)
    {
        AngleCount = 0;
        for (AdjacentNode = EdgeList[h].head; AdjacentNode != NULL;
            AdjacentNode = AdjacentNode->next)
        {

            Neighbor = AdjacentNode->value;
            NeighborList[AngleCount] = Neighbor;

            X1 = Points[h].x;
            Y1 = Points[h].y;
            X2 = Points[Neighbor].x;
            Y2 = Points[Neighbor].y;

            //First step is to move the line so that the StartPoint (X1,Y1) 
            //is at the origin (0,0).  This requires that the Neighbor point
            //be moved and equivalent distance.
            if (X1 < 0)
                X2 += abs(X1);
            else
                X2 -= X1;

            if (Y1 < 0)
                Y2 += abs(Y1);
            else
                Y2 -= Y1;

            X1 = Y1 = 0;

            //KISS, all values will be converted to positive, in 
            //order to place the line in the first quadrant of a 2D graph.  
            //After that happens the angle of the resulting line will be 
            //found and the angle of the line in its original 
            //quadrant will be determined.  
            if (X2 < 0 && Y2 > 0)
            {
                //For lines from quadrant II
                X2 *= -1;
                Slope = (Y2 - Y1) / (X2 - X1); //** could be /X2 since X1=0
                Angle = ((atan(Slope) * 180) / PI);
                Angle = 180 - Angle; //** I think this is the correct value
            }
            else if (X2 < 0 && Y2 < 0)
            {
                //Lines from quadrant III
                X2 *= -1;
                Y2 *= -1;
                Slope = (Y2 - Y1) / (X2 - X1); //** same as before
                Angle = ((atan(Slope) * 180) / PI);
                Angle = 180 + Angle; //** I think this is the correct value
            }
            else if (X2 > 0 && Y2 < 0)
            {
                //Lines from quadrant IV
                Y2 *= -1;
                Slope = (Y2 - Y1) / (X2 - X1);
                Angle = ((atan(Slope) * 180) / PI);
                Angle = (360 - Angle); //** You had this one right
            }
            else if (X2 == 0) //** Added this case to avoid a division by 0
                if (Y2 > 0)
                    Angle = 90;
                else
                    Angle = 270;
            else
            {
                //Lines already in quadrant I
                Slope = (Y2 - Y1) / (X2 - X1);
                Angle = ((atan(Slope) * 180) / PI);
            }
            //** I moved these here since they are the same in every case.
            AngleList[AngleCount] = Angle;
            AngleCount++;
        }

        //The angles are calculated and stored in the graph in an essentially 
        //random order, they need to be sorted before the separation between
        //the angles can actually be determined.  
        SortNeighborAngles(AngleList, NeighborList, AngleCount);

        PointA = Points[h];
        PointB = Points[NeighborList[0]];
        PointC = Points[NeighborList[AngleCount - 1]];

        TotalArea += SurfaceTriangle(PointA, PointB, PointC);

        for (i = 0; i < AngleCount - 1; i++)
        {
            PointA = Points[h];
            PointB = Points[NeighborList[i]];
            PointC = Points[NeighborList[i + 1]];

            TotalArea += SurfaceTriangle(PointA, PointB, PointC);
        }

    }

    TotalArea /= 3.0;

    return TotalArea;
}

// Computes the area of a triangle specified as 3 points.
double SurfaceTriangle(Point& A, Point& B, Point& C)
{
    double distAB = Distance(A, B);
    double distAC = Distance(A, C);
    Point AB = B;
    Point AC = C;
    AB -= A;
    AC -= A;
    double cosA = ScalarProd(AB, AC) / (distAB * distAC);
    double sinA = sqrt(1 - cosA * cosA);
    double area = distAB * distAC * sinA / 2;
    return area;
}

void SortNeighborAngles(double* AngleList, int* NeighborList, int AngleCount)
{
    double TempAngle;
    int SwapMade = 1, Count = 1, i, TempVal;

    while (SwapMade == 1)
    {
        SwapMade = 0;
        for (i = 0; i < AngleCount - 1; i++)
        {
            if (AngleList[i] > AngleList[i + 1])
            {
                TempAngle = AngleList[i];
                TempVal = NeighborList[i];
                AngleList[i] = AngleList[i + 1];
                NeighborList[i] = NeighborList[i + 1];
                AngleList[i + 1] = TempAngle;
                NeighborList[i + 1] = TempVal;
                SwapMade = 1;
            }
        }
    }
}

