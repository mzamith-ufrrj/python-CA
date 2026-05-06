#ifndef _TMODEL_HPP_
#define _TMODEL_HPP_
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <memory>
#include <random>
#include "Statistic.hpp"
#include "Grid.hpp"
#include "Configure.hpp"
#include "MovementSensor.hpp"
struct tpCluster{
  //int ID;
  Vehicle *vehicle;
  tpCluster *next;
};


using namespace std;

double gamaFunction (double n);
class TModel
{
public:
    TModel();
    virtual ~TModel();
    virtual void finalizer (void);
    void addONEObstables(void);
    void initialCondition(float);
    void finalCondition(void);
    virtual void applyRules(int);
    void update(int, bool=false, bool=false);
    void executeRules(Entity *, bool = false);
    void movementRules(Vehicle*, bool = false);
    void  getDistanceAndVelocity(Vehicle *, int *, int *, bool = false);
    Entity * getBack(int *,  int, int,  int);
    void debug(int);
    void setStatistic(bool t)                       { mStatistic = t; };

    shared_ptr<Configure> mConfigure;
	//Configure                        *mConfigure;
    shared_ptr<MovementSensor>       mSensor;
    shared_ptr<Grid>                 mGrid;
    shared_ptr<Statistic>            mStat;

	
protected:
    int truncCurve3(double);
    void saveLog(string);
    void buildCluster(Vehicle *vehicle);
    void solveCluster(Vehicle *vehicle);
    void insertIntoCluster( Vehicle *vehicle );
    void insertIntoRedLights(Vehicle *vehicle);
    double betaFunction (double, double, double, double, double, double );
    double betaFunction (double *);
    double getRandom(void);


   tpCluster                        *mRedLights,
                                    *mClusters;
    
   bool mStatistic;
   
   
   
};

class TModel_ML_SYM : public TModel
{
public:
    TModel_ML_SYM();
    virtual ~TModel_ML_SYM();
    virtual void applyRules(int);
    virtual void finalizer(void);
    virtual void changeLaneRules(int, Vehicle **, int);
protected:

    bool rule_Safe(Vehicle **, int);
    bool rule_MovingFaster(Vehicle **);
    bool rule_BestFlow(Vehicle **, int);

};


class TModel_ML_ASYM : public TModel_ML_SYM
{
public:

    TModel_ML_ASYM();
    virtual ~TModel_ML_ASYM();

    virtual void  changeLaneRules(int, Vehicle **ppVehicle, int);
protected:

    bool rule_FasterAhead(Vehicle **ppVehicle);
    bool rule_FasterBehind(Vehicle **ppVehicle);

};

#endif
