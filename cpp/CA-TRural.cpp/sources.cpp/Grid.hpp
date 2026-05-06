/*
 *  Grid.h
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/16/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _GRID_HPP_
#define _GRID_HPP_
#include "ModelTypes.hpp"
#include "Entity.hpp"
#include <string>
#include <vector>
#include <cstdint>
using namespace std;
/**
 * @file
 * @author  Marcelo Zamith <mzamith.prof@gmail.com>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Grid class has information about each cell of cellular automata applied for TCA
 */
class Grid
{
public:
    Grid();
    ~Grid();

    void finalize(void);
    void allocGrid(int, int);
    void setGrid(int , int , Entity *);
    Entity* getGrid(int, int);

    void addEntity (Entity*);
    void allocVehicleType(int);
    void addVehicleType(tpVehiclesType, int);
    tpVehiclesType *getVehicleType(int);
    void clearGrid(void);
    void clearMemories(void);


    Entity *getEntities(void) { return entities; };
    int getVehiclesTypeSize(void) { return sVehiclesType; };
    int getCellXi(void){ return cellX; }
    int getCellYi(void){ return cellY; }
    float getCellXf(void){ return static_cast <float> (cellX); }
    float getCellYf(void){ return static_cast <float> (cellY); }
    int getID(void) { return iID++; }
     uintptr_t * getGrid(void){ return grid; }


public:
    void clearVehiclesTypes(void);
    Entity *entities;
    uintptr_t *grid;

    int     cellX,      /* <! X represention > */
            cellY;      /* <! Y represention > */

    int  sVehiclesType;

    int  iID;  /* Vehicle ID */
    tpVehiclesType *vehiclesType;  /* <! Different kind of vehicles - data strucutre > */





};
#endif
