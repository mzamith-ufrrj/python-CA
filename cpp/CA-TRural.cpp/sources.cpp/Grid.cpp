/*
 *  Grid.cpp
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/16/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

/*
 * */

#include "Grid.hpp"
#include <cassert>
#include <iostream>
Grid::Grid():
entities(NULL),
grid(NULL),
vehiclesType(NULL),
iID(0),
cellX(-1),
cellY(-1){};

Grid::~Grid()
{
#ifdef _DEBUG
	std::cerr << __FILE__ << ":" << __FUNCTION__ << std::endl;
#endif



};


void Grid::finalize(void){


   if (grid != NULL)
      free(grid);

   grid = NULL;

   clearVehiclesTypes();

}




/**
 *  Allocs the lattice
 *  @param x: x size
 *  @param y: y size
 */
void Grid::allocGrid(int x, int y){
   cellX = x; cellY = y;
   if (grid != NULL)
      free(grid);
   assert(posix_memalign((void**) &grid, ALIGN, cellX * cellY *  sizeof(uintptr_t)) == 0);
   //assert(grid != NULL);
};



/**
 *  Sets the lattice
 *  @param x: x position
 *  @param y: y position
 *  @param v: Vehicle's postition
 */
void Grid::setGrid(int x, int y, Entity *e){
    int p = y * cellX + x;
    grid[p] = reinterpret_cast<uintptr_t> (e);


};

/**
 *  Retruns the vehicle information
 *  @param x: x position
 *  @param y: y position
 *  @return Vehicle postion
 */

Entity* Grid::getGrid(int x, int y){
    int p = y * cellX + x;
    return reinterpret_cast<Entity*> (grid[p]) ;

};



/**
 *  Adds a new vehicle in data structure
 *  @param v: v is vehicle
 *  @param idx: idx is index of vector
 */

void Grid::addEntity (Entity *e){
    if (entities == NULL){
        entities = e;
        return;
    }

    Entity *ptr = entities;
    while (ptr->next != NULL)
        ptr = ptr->next;

    ptr->next = e;
    e->prev = ptr;

};



void Grid::allocVehicleType(int s){
    sVehiclesType = s;

    if (vehiclesType != NULL){
        clearVehiclesTypes();
    }

   assert(posix_memalign((void**) &vehiclesType, ALIGN, sVehiclesType *  sizeof(tpVehiclesType)) == 0);
   //assert(vehiclesType != NULL);
};

/**
 *  Adds a new vehicle type in data structure
 *  @param v: v is vehicle type
 *  @param idx: idx is index of vector
 */

void Grid::addVehicleType(tpVehiclesType v, int idx){
    assert(idx < sVehiclesType);
    vehiclesType[idx] = v;

};

/**
 *  Adds a new vehicle type in data structure
 *  @param v: v is vehicle
 *  @param idx: idx is index of vector
 */

tpVehiclesType *Grid::getVehicleType(int idx){
    assert(idx < sVehiclesType);
    return &vehiclesType[idx];
};


/**
 *  Clear the lattice, setting the default value as -1
 *  @param
 */
void Grid::clearGrid(void){

	int size = cellX * cellY;

    memset(grid, 0x00, size * sizeof(Entity*));

}

void Grid::clearMemories(void){
    clearGrid();

    Entity *entity = entities;
    while (entity != NULL){
        Entity *ptr = entity;
        entity = entity->next;
        free(ptr);
        ptr = NULL;
    }
    entities = NULL;

}
/**
 *  Clear the vehicles type vector
 *  @param
 */
void Grid::clearVehiclesTypes(void){
	if (vehiclesType != NULL)
		free(vehiclesType);
	vehiclesType = NULL;
}
