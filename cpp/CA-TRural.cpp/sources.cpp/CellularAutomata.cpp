   /*
 *  CellularAutomata.cpp
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CellularAutomata.hpp"
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cassert>
#include <ctime>
#include <climits>
#include <thread>
#include <chrono>
using namespace std;

/*
 *
 */
CellularAutomata::CellularAutomata():
mRules(NULL),
mConfigure(NULL),
mVerbose(false)
{
   
}

/*
 *
 */
CellularAutomata::~CellularAutomata(){
    


    if (mRules != NULL){
      mRules->mGrid->clearMemories();
      mRules->finalizer();
      mRules.reset();
      
    }
    mGrid.reset();
    mSensor.reset();



}




/*
 * Clear memories
 */
//void CellularAutomata::clear(void) { mRules->getGrid()->allocGrid(mParam.cellX, mRules->getParam()->cellY); };

void CellularAutomata::help(void){
   cout << endl;
   cout << endl;
   cout << "HELP" << endl;
   cout << "\tApplication: " << _APPLICATION << endl;
   cout << "\t  Execution: ./<" << _APPLICATION << "> <config file>" << endl;
   cout << "\t     Author: Marcelo Zamith" << endl;
   cout << "\t     e-mail: zamith.marcelo@gmail.com " << endl;
   cout << "\t    Website: http://www.dcc.ufrrj.br/~marcelo" << endl;
   cout << endl;

}


/**
 *  inicialize
 *  @param fileName: config file name
 */
void CellularAutomata::init(Configure *configure)
{

	//mConfigure = new Configure(configure);
    mConfigure = make_shared<Configure>(configure);
	if (mConfigure == NULL)
	    return;
	//if (mRules != NULL) delete mRules;
	//mRules = NULL;
    if (mConfigure->mTypeModel == Configure::SINGLE_LANE){
        if (mVerbose)
            cout << "\tModel: SINGLE_LANE" << endl;
        mRules = make_shared<TModel>();
        //mRules = new TModel();
    }else if (mConfigure->mTypeModel == Configure::MULTI_LANE_SYM){
        if (mVerbose)
            cout << "\tModel: MULTI_LANE_SYM" << endl;
        mRules = make_shared<TModel_ML_SYM>();
        //mRules = new TModel_ML_SYM();
    }else if (mConfigure->mTypeModel == Configure::MULTI_LANE_ASYM){
        if (mVerbose)
            cout << "\tModel: MULTI_LANE_ASYM" << endl;
        
        //mRules = new TModel_ML_ASYM();
        mRules = make_shared<TModel_ML_ASYM>();
    }else {
        cerr << "Error: no model defined" << endl;
        exit(-1);
    }
    cout.flush();
    mSensor = make_shared<MovementSensor>();
    mGrid   = make_shared<Grid>();


    mRules->mConfigure = mConfigure;
    mRules->mSensor = mSensor;
    mRules->mGrid = mGrid;

    mGrid->allocGrid(mConfigure->mCellX,mConfigure->mCellY);
    mGrid->allocVehicleType(mConfigure->mTypeIndex);
    for (int i = 0; i < mConfigure->mTypeIndex; i++){
        tpVehiclesType profile = mConfigure->mProfile[i];
        profile.param[2] = gamaFunction(profile.param[0]);
        profile.param[3] = gamaFunction(profile.param[1]);
        profile.param[4] = gamaFunction(profile.param[0]+profile.param[1]);

    	mGrid->addVehicleType(profile, i);
    	
    }
    //Update profiles above
    mConfigure->clear();


    stringstream ss;
    ss << fixed << setprecision(5) << mConfigure->mDensity;

    mSensor->setFileName(mConfigure->mModelName, ss.str());
    mSensor->mGrid = mGrid;
    mSensor->reset();
    mSensor->setSaveFiles(mConfigure->mPhoto, mConfigure->mFixed);
    mSensor->clear();




};


/*
 * Running the simulation
 */

void CellularAutomata::exec(void){
    auto start_time_s = std::chrono::high_resolution_clock::now();
	int mpStep   = mConfigure->mSTime / 20;

    if (mpStep == 0) mpStep = 1;

    
    mRules->initialCondition(mConfigure->mDensity);
    mRules->mSensor->clear();
    mRules->mSensor->setAverageSpeed(mConfigure->mVMax);
    mRules->update(0, false, true);// update(0, dGlobal);
    
    if (mVerbose){
        cout << "\t-Starting simulation-" << endl;
        cout.flush();
    }
    cout << "D(" << fixed << setw(4) << mConfigure->mDensity << ") ";
    for (int step = 1; step < mConfigure->mSTime; step++){
    	if ((step % mpStep) == 0){
			std::cout << ".";
			std::cout.flush();
		}//end-if ((mStep % mpStep) == 0){

		if (step >= mConfigure->mDTime){
			if (step == mConfigure->mDTime){
				std::cout << "|";

				mRules->setStatistic(true);
			}//end-if (mStep == mRules->getParam()->dTime){

		}//end-if (mStep >= mRules->getParam()->dTime){
		mRules->applyRules(step);
    }//end-for (int step = 1; step < mConfigure->mSTime; step++){


    mRules->finalCondition();
    auto stop_time_s = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_time = stop_time_s - start_time_s;
    if (mVerbose)
        cout << "\t\tTotal simulation: " << fixed << setprecision(2) << total_time.count() << " seconds" << endl;
    else 
        cout << endl;
    

};
