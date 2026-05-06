/*
 * File:   Vehicle.cpp
 * Author: marcelozamith
 *
 * Created on October 20, 2010, 10:28 AM
 */

#include "Entity.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <climits>
#include "MovementSensor.hpp"
using namespace std;


Entity::Entity():
 ID(0),
 x(0),
 y(0),
 lg(0),
 next(NULL),
 prev(NULL)
{}

Entity::~Entity(){}
string Entity::whoAmI(void){ return "Entity"; }
//--------------------------------------------------------------------------------------------

Obstacle::Obstacle():
Entity()
{}

Obstacle::~Obstacle(){}

string Obstacle::whoAmI(void){ return "Obstacle"; }

void Obstacle::clear(void){

}

 
//--------------------------------------------------------------------------------------------

Vehicle::Vehicle():
Entity(),
 mCellX(0),
 mCellY(0),
 vx(0),
 vxNew(0),
 vy(0),
 myDensity(0.0f),
 mStates(0),
 type(NULL),
 yChange(false),
 mLights(false),
 mSensor(NULL),
 mCount(0),
 mDV(0),
 mDIST(0)
 {};// #registrar o numero de celulas que deveria desacelerar!



Vehicle::~Vehicle(){};

//Updating in (Y)
void Vehicle::updateY(bool statistic, int time){
   if (vy != NOTCHANGE)
       yChange = true;
   y += vy;
   vy = NOTCHANGE;


};



void Vehicle::update(bool statistic, int time){

   vx = vxNew;
   int   ivl = vx,
         x1,
         x2;



    x1 = x + ivl;

    x += ivl;

    yChange = false;

    //Updating vehicle position
    if (x >= mCellX)
        x %= mCellX;

    vxNew = 0;



    if(!statistic)
        return;




    if ((x1 >= mCellX) && (mStates == 0)){
        mStates = 1;
    }else  if (mStates == 1)
        x1 = mCellX + x1;



    x2 = x1 - lg + 1;

    if (mStates == 0) return;


    if (x2 < mCellX)
        x2 = mCellX;

    int passed = x1 + 1 - x2 - mCount;

    if (passed > 0){
        //oid MovementSensor::setCellInMovement(int cells, int velocity, int y){
        assert(passed >= 1);
        assert(passed <= lg);
        mSensor->setCellInMovement(passed, ivl, y, this);
        mCount += passed;


    }



    if (mCount == lg){ //Passou!!!
        mStates  = 0;
        mCount  = 0;
    }



};


void Vehicle::clear(void){
   mCount = 0;
   mStates = 0;
}




void Vehicle::defineLattice(int lx, int ly) { mCellX = lx; mCellY = ly; };

string Vehicle::whoAmI(void) { return "Vehicle"; }


