/*******************************************************************************
 * include/Logger.h
 * Implementation of operations to log the various parameters of the algorithm. 
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include<cstdio>
#include"Tree.h"
#include"error.h"
#include<vector>
#include<cmath>

using namespace std;

#ifndef LOGGER
#define LOGGER

class Logger
{
private:
static long long edgProbed;
static long long crossEdg;
static int sizeUpd;		//	Size of Updated subtree
static int broomS;
static int broomS2;
static int tail_bristles;
static int wavg_bristles;
static int max_bristles;
static int max_bristleH;
static vector<int> bristles;
static vector<int> components;
static double LATime;  		//Time taken in LCA/LA data structure
static double EdgMTime;  	//Time taken in edgMap data structure in Tree/Graph


/* 
Calculates the size of each subtree in <Tmp> and stores it in sizeSub
Param: <Tmp> Given Tree, <root_tmp> Root of subtree to be evaluated, <sizeSub> Required Array
*/

static void calcSizeSub(int sizeSub[], const Tree& Tmp, int root_tmp);

public:
/////////////////////
/// EDG_M DS TIME ///
/////////////////////

/*
Resets the time taken in EdgM data structure
*/
static void resetEdgMTime();

/*
Increments the time taken in EdgM data structure
by <time>
*/
static void incrEdgMTime(double time);

/*
Reports the total time taken in EdgM data structure
Return: double <EdgMTime>
*/
static double getEdgMTime();


////////////////////////
/// LA / LCA DS TIME ///
////////////////////////

/*
Resets the time taken in LCA/LA data structure
*/
static void resetLATime();

/*
Increments the time taken in LCA/LA data structure
by <time>
*/
static void incrLATime(double time);

/*
Reports the total time taken in LCA/LA data structure
Return: double <LATime>
*/
static double getLATime();



//////////////////////////////
// SIZE OF UPDATED SUBTREE ///
//////////////////////////////



/*
Resets Count of SizeUpd
*/
static void resetSizeUpd();

/*
Increments Count of SizeUpd
*/
static void incrSizeUpd();

/*
Reports size of SizeUpd 
Return: <sizeUpd>
*/
static int getSizeUpd();



//////////////////
// CROSS EDGES ///
//////////////////
/*
Resets Count of Cross Edges
*/
static void resetCE();

/*
Increments Count of Cross Edges
*/
static void incrCE();

/*
Increments Count of Cross Edges  by <count>
Param: <count> Number of increments
*/
static void incrCE(long long count);

/*
Reports Count of Cross Edges 
Return: <crossEdg>
*/
static long long getCE();

//////////////////
// EDGES PROBED //
//////////////////
/*
Resets Count of Edges Probed
*/
static void resetEP();

/*
Increments Count of Edges Probed
*/
static void incrEP();

/*
Increments Count of Edges Probed by <count>
Param: <count> Number of increments
*/
static void incrEP(long long count);

/*
Reports Count of Edges Probed
Return: <edgProbed>
*/
static long long getEP();

////////////////////////
// BROOM STICK LENGTH //
////////////////////////
/*
Find the length and end of broom stick of 
the given tree <T> starting from root <root>
Also calculate the length of broomS2 and max_bristles
Param: <T> given tree, <root> root from where stick is measured. 
*/
static void calcBStick(const Tree& T, int root);

/*
Reports length of Broom Stick
Return: <broomS>
*/
static int getBS();


/*
Reports length of secondary Broom Stick
Return: <broomS2>
*/
static int getBS2();

/*
Reports height of max bristles
Return: <max_bristleH>
*/
static int getMBH();


/*
Reports max size of bristles
Return: <max_bristles>
*/
static int getMB();

/*
Reports weighted average size of bristles
Return: <wavg_bristles>
*/
static int getWAB();

/*
Reports size of tail bristles
Return: <tail_bristles>
*/
static int getTB();

/*
Reports set of bristles
Return: <bristles>
*/
static vector<int> getBristles();

/*
Reports set of components
Return: <components>
*/
static vector<int> getComponents();


};


#endif
