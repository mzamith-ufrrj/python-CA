/*
 * File:   Vehicle.h
 * Author: marcelozamith
 *
 * Created on October 20, 2010, 10:28 AM
 */

#ifndef _VEHICLE_HPP_
#define _VEHICLE_HPP_
#include "ModelTypes.hpp"
#include <string>
#include <memory>
using namespace std;
class Entity{
public:

   Entity();
  ~Entity();

    unsigned int ID;       /*<! Vehicle's ID */


    int x;                 /*<! Vehicle's x position on the road */
    int y;                 /*<! Vehicle's y position. It is the lane */
    int lg;                /*<! Vehicle's length */
    virtual string whoAmI(void);
    virtual void clear(void) = 0;
    
    Entity *next;
    Entity *prev;

};

class Obstacle: public Entity{
public:

   Obstacle();
  ~Obstacle();
   string whoAmI(void);
    virtual void clear(void);
    
    Entity *next;
    Entity *prev;

};


class MovementSensor;
class Vehicle: public Entity {
public:
   Vehicle();
   ~Vehicle();

   void apply(void);
   void update(bool, int);
   void updateY(bool, int = -1); //Updating in (y)

   void defineLattice(int, int);


   void clearParameters(void);

   string whoAmI(void);
   virtual void clear(void);
  

  

public:
    int mCellX;            /*<! Number of cells in latticle (x) */
    int mCellY;            /*<! Number of cells in latticle (y) */
    int mStates;           /*<! Informs whether the vehicle passed or not */
    int mCount;
    bool mLights;          /*<! Indicates if vehicle breaks or not */

    int vx;                     /*<! Vehicle's velocity in [0:25] */
    int vxNew;                  /*<! Vehicle's velocity in the current time (t)*/
    int vy;                     /*<! Vehicle's lane change [-1 is left] [1 is right] [0 is current] */
    //int vyNew;                /*<! Vehicle's lane in the current time (t)*/
    bool yChange;               /*<! Indicates if vehicle change the lane or not (false = not change / true = change); */
    tpVehiclesType *type;       /*<! Vehicle's behavior   */
    float myDensity;            /*<! Used to identify which density this vehicle is moving */


    int mDV;              /*<! Velocity difference between this vehicle and his ahead one */
    int mDIST;            /*<! Distance between this vehicle and his ahead one */
    shared_ptr<MovementSensor> mSensor;
    
    
};

#endif	/* VEHICLE_H */
