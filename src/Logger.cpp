/*******************************************************************************
 * src/Logger.cpp
 * Implementation of operations to log the various parameters of the algorithm. 
 *
 * Part of Project: Incremental-DFS-Experimental: 
 *                  https://github.com/shahbazk/IncDFS-Experimental
 *
 * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include"../include/Logger.h"

long long Logger::edgProbed=0;
long long Logger::crossEdg=0;
int Logger::sizeUpd=0;
int Logger::broomS=0;
int Logger::broomS2=0;
int Logger::tail_bristles=0;
int Logger::wavg_bristles=0;
int Logger::max_bristles=0;
int Logger::max_bristleH=0;
vector<int> Logger::bristles;
vector<int> Logger::components;
double Logger::LATime;
double Logger::EdgMTime;

/* 
Calculates the size of each subtree in <Tmp> and stores it in sizeSub
Param: <Tmp> Given Tree, <root_tmp> Root of subtree to be evaluated, <sizeSub> Required Array
*/

void Logger::calcSizeSub(int sizeSub[], const Tree& Tmp, int root_tmp)
{
list<int> chld = Tmp.child(root_tmp);
list<int> :: iterator it;
int size=0;
for(it = chld.begin(); it!= chld.end(); it++)
	{
	calcSizeSub(sizeSub,Tmp,*it);
	size += sizeSub[*it];
	}
sizeSub[root_tmp] = size+1;
}


/************************/
/*** PUBLIC FUNCTIONS ***/
/************************/
/////////////////////
/// EDG_M DS TIME ///
/////////////////////

/*
Resets the time taken in EdgM data structure
*/
void Logger::resetEdgMTime()
	{
	EdgMTime = 0;
	}

/*
Increments the time taken in EdgM data structure
by <time>
*/
void Logger::incrEdgMTime(double time)
	{
	#ifndef FAST
	if(time < 0)
		ERR_MSG("Logger::incrLATime Invalid time\n");
	#endif
	EdgMTime += time;
	}

/*
Reports the total time taken in EdgM data structure
Return: double <EdgMTime>
*/
double Logger::getEdgMTime()
	{
	return EdgMTime;
	}



////////////////////////
/// LA / LCA DS TIME ///
////////////////////////

/*
Resets the time taken in LCA/LA data structure
*/
void Logger::resetLATime()
	{
	LATime = 0;
	}

/*
Increments the time taken in LCA/LA data structure
by <time>
*/
void Logger::incrLATime(double time)
	{
	#ifndef FAST
	if(time < 0)
		ERR_MSG("Logger::incrLATime Invalid time\n");
	#endif
	LATime += time;
	}

/*
Reports the total time taken in LCA/LA data structure
Return: double <LATime>
*/
double Logger::getLATime()
	{
	return LATime;
	}


//////////////////////////////
// SIZE OF UPDATED SUBTREE ///
//////////////////////////////
/*
Resets Count of SizeUpd
*/
void Logger::resetSizeUpd()
	{
	sizeUpd=0;
	}
/*
Increments Count of SizeUpd
*/
void Logger::incrSizeUpd()
	{
	sizeUpd++;
	}

/*
Reports size of SizeUpd 
Return: <sizeUpd>
*/
int Logger::getSizeUpd()
	{
	return sizeUpd;
	}


//////////////////
// CROSS EDGES ///
//////////////////
/*
Resets Count of Cross Edges
*/
void Logger::resetCE()
{
	crossEdg = 0;
}

/*
Increments Count of Cross Edges
*/
void Logger::incrCE()
{
	incrCE(1);
}
/*
Increments Count of Cross Edges  by <count>
Param: <count> Number of increments
*/
void Logger::incrCE(long long count)
{
	#ifndef FAST
	if(count< 0)
		ERR_MSG("Logger::incrCE: Invalid count to increment");
	#endif
	crossEdg+= count;
}

/*
Reports Count of Cross Edges 
Return: <crossEdg>
*/
long long Logger::getCE()
{
	return crossEdg;
}

////////////////////
/// EDGES PROBED ///
////////////////////

/*
Resets Count of Edges Probed
*/
void Logger::resetEP()
{
edgProbed = 0;
}

/*
Increments Count of Edges Probed
*/
void Logger::incrEP()
{
incrEP(1);
}

/*
Increments Count of Edges Probed by <count>
Param: <count> Number of increments
*/
void Logger::incrEP(long long count)
{
#ifndef FAST
if(count< 0)
	ERR_MSG("Logger::incrEP: Invalid count to increment");
#endif
edgProbed+= count;
}

/*
Reports Count of Edges Probed
Return: <edgProbed>
*/
long long Logger::getEP()
{
return edgProbed;
}


////////////////////////
// BROOM STICK LENGTH //
////////////////////////

/*
Find the length and end of broom stick of 
the given tree <T> starting from root <root>
Also calculate the length of broomS2 and max_bristles
Param: <T> given tree, <root> root from where stick is measured. 
*/
void Logger::calcBStick(const Tree& Tmp, int root_tmp)
{
int sizeSub[Tmp.size()];
list<int> chld;
list<int> :: iterator it,mit;

calcSizeSub(sizeSub,Tmp,root_tmp);

bristles.clear();
components.clear();

if(!Tmp.validV(root_tmp))
	ERR_MSG("Tree::calcBStick Invoked with invalid root");

chld = Tmp.child(root_tmp);
for(it=chld.begin();it!=chld.end();it++)
	components.push_back(sizeSub[*it]);

int length=0;
while(Tmp.child(root_tmp).size() == 1)
	{
	length++;
	root_tmp = Tmp.child(root_tmp).front();	
	}
broomS = length;

int maxS, maxS2;
long long weighted_sum=0;
max_bristles = 0;
max_bristleH = 0;


while(Tmp.child(root_tmp).size() != 0)
	{
	chld = Tmp.child(root_tmp);
	maxS=0; maxS2=0; tail_bristles=0;
	for(it = chld.begin(); it!= chld.end(); it++)
		{
		tail_bristles+= sizeSub[*it];
		weighted_sum+= ((long long) sizeSub[*it])*sizeSub[*it];
		bristles.push_back(sizeSub[*it]);

		if(maxS<sizeSub[*it]) 
			{
			bristles.pop_back();
			bristles.push_back(maxS);
			maxS2 = maxS;
			maxS = sizeSub[*it];
			mit = it;
			} 
		else if(maxS2 < sizeSub[*it])
			maxS2 = sizeSub[*it];
		}

	if(maxS <= pow(sizeSub[root_tmp],2.0/3)) 
//	if(maxS < max_bristles) 
//	if(maxS < (weighted_sum-((long long) maxS)*maxS)/(Tmp.size()-length-maxS) ) 
		{bristles.push_back(maxS);break;}
	if(maxS2 > max_bristles) 
		{
		max_bristles=maxS2;
	 	max_bristleH=length;	
		}
	length++;
	weighted_sum -= ((long long) maxS)*maxS;
	root_tmp = *mit;
	}
//printf("TEST:: %d %d %d\n",maxS,maxS2,max_bristles);

wavg_bristles = weighted_sum/(Tmp.size()-length);
broomS2 = length;
}


/*
Reports length of Broom Stick
Return: <broomS>
*/
int Logger::getBS()
{
return broomS;
}

/*
Reports length of secondary Broom Stick
Return: <broomS2>
*/
int Logger::getBS2()
{
return broomS2;
}

/*
Reports max size of bristles
Return: <broomS>
*/
int Logger::getMB()
{
return max_bristles;
}

/*
Reports height of max bristles
Return: <broomS>
*/
int Logger::getMBH()
{
return max_bristleH;
}

/*
Reports weighted average size of bristles
Return: <wavg_bristles>
*/
int Logger::getWAB()
{
return wavg_bristles;
}

/*
Reports size of tail bristles
Return: <tail_bristles>
*/
int Logger::getTB()
{
return tail_bristles;
}

/*
Reports set of bristles
Return: <bristles>
*/
vector<int> Logger::getBristles()
{
return bristles;
}

/*
Reports set of components
Return: <components>
*/
vector<int> Logger::getComponents()
{
return components;
}




