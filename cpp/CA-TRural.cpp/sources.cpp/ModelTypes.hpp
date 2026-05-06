#ifndef _MODELTYPES_HPP_
#define _MODELTYPES_HPP_
#include <cstring>
#include <thread>
#include <string>

using namespace std;

#define EMPTY    NULL

#define  STRINGSIZE        256
#define VEHICLEPERKM      133
#define PERCENT2VEHICLES  (1000.0f / 7.5f)

#define NOTCHANGE    0
#define RIGHT2LEFT  -1
#define LEFT2RIGHT   1


#define TPVEHICLESTYPESIZE (8 + STRINGSIZE + (sizeof(float) * 3) + (sizeof(int) * 3) + (sizeof(double) * 6))

/*
 * Vehicles parameters
 */
struct tpVehiclesType
{
    char       description[STRINGSIZE]; //       description[STRINGSIZE];
    double      percent;
    int        size;
    int        vMax;
    double     param[5];
    double     inc;
    int        desc;
    double      left_p;
    double    right_p;
    int       aheadInt;
    int       safeDist;

};



union unVehiclesType{
    tpVehiclesType VehiclesType;
    unsigned char bytes[TPVEHICLESTYPESIZE];
};






#endif
