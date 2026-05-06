#ifndef _CONFIGURE_HPP_
#define _CONFIGURE_HPP_
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include "ModelTypes.hpp"

using namespace std;


/*
 *  configure.hpp
 *
 *  Created by Marcelo Zamith on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *  This class has the basic structure as well as transition rules
 *
 */

using namespace std;

class Configure
{

public:
	const static int SINGLE_LANE      = 1;
	const static int MULTI_LANE_ASYM  = 2;
	const static int MULTI_LANE_SYM   = 3;

    Configure();
    Configure(Configure *config);
    ~Configure();
    void loadConfigFile(const string& configFile);
    //void saveConfigFile(const string& configFile = "");
    void setProfiles(int);
    void setProfile(int, char *, float, int, int, double, double, double, int, int, int, float, float);
    void print(void);

    

    string mModelName;
    int mTypeModel;
    int mVMax;      //Max velocity
    int mCellX;   //Space in X
    int mCellY;   //Space in Y --> number of the roads
    float mDeltaH;
    float mDefaultSize; //default size of one vehicle - it means 7.5mts

    int mSTime;   //Simulation time
    int mDTime;   //Discarde time
    int mStTime;   //Statistic time

    float mDensity;

    bool mFixed;
    bool mPhoto;
    bool mLogCluster;
    //bool mLog;

    int mDeceleration;
    float mTimePerception;

    
    
    tpVehiclesType *mProfile;
    int mTypeIndex;

    void clear(void);

};
#endif
