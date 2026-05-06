#ifndef _CELLULARAUTOMATA_H_
#define _CELLULARAUTOMATA_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "Entity.hpp"
#include "TModel.hpp"
#include "MovementSensor.hpp"
#include "ModelTypes.hpp"
#include "Grid.hpp"
#include "Configure.hpp"

/*
 *  CellularAutomata.h
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *  This class has the basic structure as well as transition rules
 *
 */

using namespace std;

class CellularAutomata
{

public:
    CellularAutomata();
    ~CellularAutomata();
    void help(void);
    void init(Configure *);
    void exec(void);
    void setVerbose(void) { mVerbose = true; };
    void finalize(void);


	shared_ptr<Grid>                   mGrid;
	shared_ptr<MovementSensor>         mSensor;
    shared_ptr<TModel>                 mRules;
    shared_ptr<Configure>              mConfigure;

    private:
        bool mVerbose;
};
#endif
