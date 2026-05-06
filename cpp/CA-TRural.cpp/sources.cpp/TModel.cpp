#include "TModel.hpp"
#include <string>
#include <sstream> 
#include <cassert>
#include <random>


#define PI      (float) 3.1415926535897932384626433832795
#define PI2     (float) 2.0f*PI
#define SQRT2PI (float) sqrt(PI2)
#define ERROR   (double) 1E-10

//#define getParam()->decelearation 10

//Calcular o horizonte de observação. Ou seja, quanto tempo o Follower leva para
//atingir o Leader. Carros fora do horizonte, aplicar regra padrão. Caso contrário,
//aplicar regra de desaceleração.

using namespace std;

TModel::TModel():
mRedLights(NULL),
mClusters(NULL),
mStat(NULL){
     mStat = make_shared<Statistic>();
     srand ( time(NULL) );
     
}
TModel::~TModel(){}

void TModel::finalizer(void){
  mStat.reset();
  mStat = NULL;
  mGrid = NULL;

}

void TModel::addONEObstables(void){
    Obstacle *obstacle = new Obstacle();

    obstacle->ID = mGrid->getID();

    obstacle->x  = mConfigure->mCellX - 1;
    obstacle->y  = mConfigure->mCellY - 1;
    obstacle->lg = 1;
    mGrid->addEntity(obstacle);

}

void TModel::initialCondition(float dGlobal){
    //Define how many vehicles will be on the road
   
    mStat->setConfigure(mConfigure);
    mStat->setGrid(mGrid);

    Vehicle vehicle;
/*
 * Observation 2017, March 18th
 * For each vehicle Verifying if it is in a list of log vehicles vehicleLog
 * To do this: Verifying if there is a substring with the number.
 *             Set true vehicle parameter
 *             Put in Vehicle.cpp code to log the vehicle performance
 *
 */
    int totalVehicles = 0,
        *vehiclesTypes = new int[mGrid->getVehiclesTypeSize()];
    mStatistic = false;

    for (int i = 0; i < mGrid->getVehiclesTypeSize(); i++){
        tpVehiclesType *type  = mGrid->getVehicleType(i);

        float dVehicle  = type->percent * dGlobal / static_cast<float> (type->size);

        int   nVehicles = static_cast <int> (dVehicle * static_cast <float> (mConfigure->mCellX));

        if (nVehicles == 0) nVehicles = 1;

        totalVehicles += nVehicles;

        vehiclesTypes[i] = nVehicles;
    }
    totalVehicles *= mConfigure->mCellY;

    mGrid->clearMemories();

    //if (mConfigure->mLogVehicles > 0)  assert(posix_memalign((void**) &mLastVehicles, ALIGN, mConfigure->mLogVehicles *  sizeof(Vehicle*)) == 0);

    
    for (int road = 0; road < mConfigure->mCellY; road++){
        int cell = 0;
        for (int i = 0; i < mGrid->getVehiclesTypeSize(); i++){
            tpVehiclesType *type  = mGrid->getVehicleType(i);
            int length    = type->size;
            int nVehicles = vehiclesTypes[i];
            for(int iveicles = 0; iveicles < nVehicles; iveicles++){
                Vehicle *vehicle = new Vehicle();

                vehicle->ID = mGrid->getID();

                vehicle->x = cell + length - 1;
                vehicle->vx = 0;
                vehicle->vxNew = 0;
                vehicle->y = road;
                vehicle->vy = 0;
                vehicle->yChange = false;
                vehicle->mLights = false;
                vehicle->lg = type->size;
                vehicle->myDensity = dGlobal;
                vehicle->mSensor = mSensor;
                vehicle->next = NULL;
                vehicle->prev = NULL;
                vehicle->type = mGrid->getVehicleType(i);
                vehicle->defineLattice(mConfigure->mCellX, mConfigure->mCellY);
                vehicle->mDV = 0;
                vehicle->mDIST = 0;
                mGrid->addEntity(vehicle);
                cell += length;

            }

        }
    }

    stringstream ss;
    ss << fixed << setprecision(5) << mConfigure->mDensity;
    mStat->setDensity(ss.str());
    mStat->setConfigure(mConfigure);
    mStat->setGrid(mGrid);

    //mVave = mParam->vMax;
    delete[] vehiclesTypes;
    vehiclesTypes = NULL;


  

}

void TModel::finalCondition(void){

   if (!mConfigure->mLogCluster)
      return;

   mStat->saveStatistic();
   

}


void TModel::applyRules(int step){



    Entity *ptr = mGrid->getEntities();

    //Applying movement rules
    while (ptr != NULL){
        executeRules(ptr);
        ptr = ptr->next;
    }


    do{
         tpCluster *list = NULL;
         Vehicle *vehicle = NULL;


        // sprintf(line, "\t<mRedLights> %p </mRedLights>", mRedLights);

         while (mRedLights != NULL){
            list = mRedLights;
            vehicle = list->vehicle;
            vehicle->mLights = false;



            buildCluster(vehicle);
            mRedLights = mRedLights->next;
            delete list;
            list = NULL;
         }//end-if (list != NULL){

         list = NULL;
         vehicle = NULL;


         while (mClusters != NULL){
            list = mClusters;
            vehicle = list->vehicle;
            solveCluster(vehicle);
            mClusters = mClusters->next;
            delete list;
            list = NULL;
         }


    }while(mRedLights != NULL);




   update(step,  false);



}


void TModel::update(int step,  bool changing, bool initial){

   std::fstream output;

   //Updating all vehicles
   bool  statistic = (step > mConfigure->mDTime) ;
   mGrid->clearGrid();

    Entity *pEntity = mGrid->getEntities();
    while (pEntity != NULL){


      if (pEntity->whoAmI().compare("Vehicle") == 0){
         if ((step % mConfigure->mStTime) == 0){//Whether save statistic time step, clear
            pEntity->clear();

         }

         Vehicle **ppVehicle = (Vehicle**) (&pEntity);
//-----------------------------------------------------------------------------
         assert(ppVehicle != NULL);
         if (!initial){
            if (changing)
                (*ppVehicle)->updateY(statistic, step);
            else
                (*ppVehicle)->update(statistic, step);
            
         }



         int y = pEntity->y;
         for (int k = 0 ; k <   pEntity->lg; k++){
            int x  =  pEntity->x - k;
            if (x < 0)
               x = mConfigure->mCellX + x;

           // assert(!changing);

            if ((mGrid->getGrid(x, y) != EMPTY) && (!changing)){
               std::cerr << "\t [ERROR] " << __FILE__ << " at " << __LINE__ << "\t Time:" << step <<  std::endl ;

               Vehicle *v = static_cast<Vehicle*>(pEntity) ; //mGrid->getGrid(x, y));

               // v.mBreakForeseenForeseenLight = true;
               std::cerr << "V1(ID= " << v->ID << ",X= "
               <<  v->x  << ",Y= "
               <<  v->y  << ", VX="
               <<  v->vx << ", VNEW="
               <<  v->vxNew <<  ")" << std::endl;


               v = static_cast<Vehicle*>(mGrid->getGrid(x, y));
               std::cerr << "V2(ID= " <<  v->ID << ",X= "
               <<  v->x  << ",Y= "
               <<  v->y  << ", VX="
               <<  v->vx << ", VNEW="
               <<  v->vxNew << ")" << std::endl;
               exit(-1);

            }

            mGrid->setGrid(x, y, pEntity);

         }//end-for (int k = 0 ; k <  static_cast <int> ( pVehicle->lg); k++){
//-----------------------------------------------------------------------------
      }else  if (pEntity->whoAmI().compare("Obstacle") == 0){//end-      if (pEntity->whoAmI().compare("Vehicle") == 0){
            int y = pEntity->y;
            for (int k = 0 ; k <   pEntity->lg; k++){
                int x  =  pEntity->x - k;
                if (x < 0)
                    x = mConfigure->mCellX + x;

                mGrid->setGrid(x, y, pEntity);
            }
      }
      pEntity = pEntity->next;
    }//while (pEntity != NULL){

   if ((!changing) && (step > 0)){
      //Verifying if there is vehicle stopped on moviment detector cell.
      if (statistic){
         for (int j = 0; j < mConfigure->mCellY; j++){

            Entity *e = mGrid->getGrid(mConfigure->mCellX - 1, j);
            if (e != NULL){
               if (e->whoAmI().compare("Vehicle") == 0){
                  Vehicle *v = reinterpret_cast<Vehicle*> (e);
                  if (v->vx == 0)
                     mSensor->setCellInStopped(j);
               }

            }



         }//end- for (int j = 0; j < mRules->getParam()->cellY; j++){

         mSensor->update();
         if ((step % mConfigure->mStTime) == 0){
            mSensor->saveStatistic(step);
         }

//--------------------------------------------------
      }//end- if (statistic){
   }//end- if ((!changing) && (step > 0)){
}

void TModel::executeRules(Entity *entity, bool changing){

   if (entity->whoAmI().compare("Vehicle") == 0){
       movementRules(static_cast<Vehicle *> (entity));
   }

}

void TModel::movementRules(Vehicle *pVehicle, bool isCluster){
   int iVel = 0,
       iDist = 0,
       iDistAux = 0,
       iAheadVel = 0,
       iOldV = 0;

   Vehicle **ppVehicle  = &pVehicle;
   tpVehiclesType *type = (*ppVehicle)->type;
    
   double    beta = betaFunction(type->param);
   double   alpha = (1.0f - beta);
   iVel = (*ppVehicle)->vx;
   
   getDistanceAndVelocity(pVehicle, &iDistAux, &iAheadVel, isCluster);

    //2022-02-12
    double dAheadVel = static_cast<double>(iAheadVel) * alpha;
    


   iDist = iDistAux + truncCurve3(dAheadVel);

   bool breaking = false;
   int DV = (iVel - iAheadVel);
   if (!isCluster){
        iOldV = iVel;
        double acc = (type->inc * alpha) - static_cast<double>(type->desc);

        double dDist = static_cast<double>(iVel *  mConfigure->mTimePerception) * beta;
        int iADist = truncCurve3(dDist);
        
        if ((iDist < iADist) && (DV > 0)){// && (iDist > 0)){
            //try to decelerate as smooth as possible
        
            
           double acc = static_cast<double>(DV);
           assert(acc >= 0.0);
           
           if (acc > mConfigure->mDeceleration) acc = mConfigure->mDeceleration;

           iVel = iVel - (truncCurve3(acc));
           
           breaking = true;
        }else{
         
            iVel += truncCurve3(acc);
        
        }

        iVel = max(iVel, 0);
        iVel = min (iVel,  min(mConfigure->mVMax, (*ppVehicle)->type->vMax));
   }
   else{
       iOldV = iVel;
       iVel = (*ppVehicle)->vxNew;
         
       if (iDist > 0){
           double acc = static_cast<double>(DV);
           if (acc < 0.0) acc = 0.0f;
           if (acc > mConfigure->mDeceleration)
               acc = mConfigure->mDeceleration;


           iVel = (*ppVehicle)->vxNew - (truncCurve3(acc));
           iVel = max(iVel, 0);
           breaking = true;
           int iDiffV = iVel - iOldV;
           if (iDiffV < 0){
            unsigned int aux = static_cast<unsigned int> (iDiffV * -1);
            mStat->setVelocity(aux);
           }
       }//end-if (iDist >0){

   }

   if (iVel > iDist){
      iVel = iDist;
      breaking = true;
   }

   if ((iVel < iOldV) && (!breaking))
      breaking = true;

   (*ppVehicle)->mLights =  breaking;

   if (breaking){
      insertIntoRedLights(pVehicle);
   }

   (*ppVehicle)->vxNew = iVel;

}



void TModel::getDistanceAndVelocity(Vehicle *v, int *dist, int *vel, bool rule){


   int x = v->x,
       y = v->y,
       iDist = 0;
   Entity *cell = NULL;

   do
   {
      x++;
      iDist++;
      if (x == mConfigure->mCellX)
         x = 0;
      cell = mGrid->getGrid(x, y);

   } while (cell == EMPTY); //((cell == EMPTY) && (iDist <= mParam->vMax));


	 if (cell->whoAmI().compare("Vehicle") == 0){
		Vehicle *v = static_cast<Vehicle *> (cell);
		if (!rule){
			*vel = v->vx;
		}else{
		   *vel = v->vxNew;
		}

	 }else{
		*vel = 0;
	 }


   iDist--;
   if (iDist < 0)
      iDist = 0;

   *dist = iDist;

}

Entity * TModel::getBack(int *out,
                          int myX,
                          int myY,
                          int myLg){

   int x = myX - myLg,
       y = myY,
       iDist = 0;
   Entity *entity = NULL;

   do{

      if (x < 0)
         x = mConfigure->mCellX + x;


      entity = mGrid->getGrid(x, y);
      if (entity == EMPTY)
      {
          iDist++;
          x--;
      }
   } while  (entity == EMPTY);


   *out = iDist;

   return entity;



}

/*
 * It is a method that depends on what king of information I would show.
 * It means: I can see on the screen or saved into a file.
 */
void TModel::debug(int time){
   /*
   cerr << "Time = " << time << endl;
   for (int j = 0; j < getParam()->cellY; j++){
      for (int i = 0; i < getParam()->cellX;  i++){
               int idx = getGrid()->getGrid(i, j);
         if (idx > EMPTY){
               Vehicle *pVehicle    = getGrid()->getVehiclePointer(idx);
               cerr << pVehicle->ID << "," << pVehicle->x << " , " << (int) pVehicle->y << endl;

         }
      }
   }
   cerr << "--------------------------------------------------------------------------------------------";
   cerr << endl;
   */
}


int TModel::truncCurve3(double v){
    int result = static_cast<int> (floor(v+0.5f));


    return result;

}






void TModel::buildCluster(Vehicle *vehicle){

        Vehicle *pVehicle = vehicle;
        Vehicle *bVehicle = NULL;
        Entity *backPtr = NULL;

        int iDist = 0,
            iVel  = 0;

        do{
         backPtr = getBack(&iDist, pVehicle->x, pVehicle->y, pVehicle->lg);



         if (backPtr->whoAmI().compare("Vehicle") != 0)
            return;

         bVehicle = static_cast<Vehicle *> (backPtr);

         iVel = bVehicle->vxNew;
         iDist += pVehicle->vxNew;

         if (iVel > iDist){

            insertIntoCluster(bVehicle);
         }

         pVehicle = bVehicle;
         bVehicle = NULL;

      }while (iVel > iDist);



}





void TModel::solveCluster(Vehicle *vehicle){
  movementRules(vehicle, true);

  //if (mStatistic)
  //    mReAjusted++;
}

void TModel::insertIntoCluster(Vehicle *vehicle){
    if (mStatistic){
        mStat->setVAdusted();
    } 
    
   tpCluster *cluster =  new tpCluster();

   cluster->vehicle = vehicle;
   cluster->next = NULL;

   tpCluster *ptr = mClusters;

   if (mClusters == NULL){
      mClusters = cluster;
      return;
   }

   while (ptr->next != NULL){
      ptr = ptr->next;
   }

   ptr->next = cluster;

}





void TModel::insertIntoRedLights(Vehicle *vehicle){
   tpCluster *cluster =  new tpCluster();
   cluster->vehicle = vehicle;
   cluster->next = NULL;

   tpCluster *ptr = mRedLights;

   if (mRedLights == NULL){
      mRedLights = cluster;
      return;
   }

   while (ptr->next != NULL){
      ptr = ptr->next;
   }

   ptr->next = cluster;

}





void TModel::saveLog(string msg){
    fstream log;
    log.open("Log.txt", fstream::out | fstream::app);
    log << msg << endl;

    log.close();

}

double  TModel::betaFunction (double gamaAlpha,
                                    double gamaBeta,
                                    double gamaAlphaBeta,
                                    double alpha,
                                    double beta,
                                    double x){


	double  xalpha = pow(x, (alpha-1.0f)),
                xbeta  = pow((1.0f - x), (beta-1.0f));

	double ret = gamaAlphaBeta / (gamaAlpha * gamaBeta);

    ret *= xalpha * xbeta;

    return ret;
}

double  TModel::betaFunction (double *vet){

	double x, y, y1;
    do{
        x  = getRandom();
        y1 = getRandom();

        y = betaFunction( vet[2],
                         vet[3],
                         vet[4],
                         vet[0],
                         vet[1],
                         x);


    }while (y1 > y);

    if (x < 0.0f)
        x = 0.0f;

    if (x > 1.0f)
        x = 1.0f;

    return x;

}

double gamaFunction (double n){

    unsigned long in1 = static_cast <unsigned long> (n-1),
    iacc = in1;

    if (n <= 2.0f) return 1.0f;

    for (unsigned long i = in1-1 ; i > 0 ; i--)
        iacc *= i;

    return static_cast <double> (iacc);
}

double TModel::getRandom(void){ return static_cast <double> (rand() % 10000 + 1) / 10000.0f;};
//------------------------------------------------------------------------------
/*
    //////////////////////////////////
    // Highway direction
    // Initial --> Final = 0 is left and 1 is right
    // 0 - left
    // 1 - right
    //////////////////////////////////
*/
TModel_ML_SYM::TModel_ML_SYM():
TModel(){
    //mIndexDensity(0){
};

TModel_ML_SYM::~TModel_ML_SYM(){

};




/**
 * Apply symmetric rules or simple ahead movement based on T_UFRRJ
 */
void TModel_ML_SYM::applyRules(int step){


      Entity *pEntity = mGrid->getEntities();
      while (pEntity != NULL){
           if (pEntity->whoAmI().compare("Vehicle") == 0){
             changeLaneRules(step, (Vehicle**) (&pEntity), RIGHT2LEFT);
           }
          pEntity = pEntity->next;
      }
      update(step,  true);

      pEntity = mGrid->getEntities();
      while (pEntity != NULL){
           if (pEntity->whoAmI().compare("Vehicle") == 0){
             changeLaneRules(step, (Vehicle**) (&pEntity), LEFT2RIGHT);
           }
          pEntity = pEntity->next;
      }
      update(step,  true);
      TModel::applyRules(step);
}

void TModel_ML_SYM::changeLaneRules(int step, Vehicle **ppVehicle, int iDir){

    tpVehiclesType *type = (*ppVehicle)->type;

    bool changed = false;
    bool bestFlow = false;
    
    if (iDir == RIGHT2LEFT){
        bestFlow = rule_BestFlow(ppVehicle, RIGHT2LEFT);
        if(rule_MovingFaster(ppVehicle)  && !changed && bestFlow){
            if (rule_Safe(ppVehicle, RIGHT2LEFT)){
                if (getRandom() <= type->left_p){

                    (*ppVehicle)->vy = -1;
                    changed = true;
                }//end-if (getRandom() < type->left_p){
            }//end-if (r05_Change(idx, RIGHT2LEFT)){
        }//end-if(r01_Motivated(idx) && !changed ){
    }else{//if (iDir == RIGHT2LEFT){
      //This code executes LEFT2RIGHT
        bestFlow = rule_BestFlow(ppVehicle, LEFT2RIGHT);
        if(rule_MovingFaster(ppVehicle) && !changed && bestFlow){
            if (rule_Safe(ppVehicle, LEFT2RIGHT)){
                if (getRandom() <= type->right_p){
                    (*ppVehicle)->vy = +1;
                    //changed = true;

                }//end-if (getRandom() < type->left_p){
            }//end-if (r05_Change(idx, RIGHT2LEFT)){
        }//end-if(r01_Motivated(idx) && !changed ){
    }//if (iDir == RIGHT2LEFT){


   
}

/*
 * Safe rules, verify if the car can chance, it he fits on the destination lane
 */
bool TModel_ML_SYM::rule_Safe(Vehicle **ppVehicle, int iDir){
    Entity    *pEntity = NULL;
    Vehicle   *vAheadVehicle = NULL,
              *vBackVehicle = NULL;

    tpVehiclesType *type = (*ppVehicle)->type;


    int iMyY = static_cast<int> ((*ppVehicle)->y),
        iMyX = (*ppVehicle)->x,
        iBackDistance = 0,
        iAheadDistance = 0,
        //iPtr = 0,
        iLen =  type->size ,
        iVel =  ((*ppVehicle)->vx);

    if ((iDir == RIGHT2LEFT) && (iMyY == 0))
        return false;

    if ((iDir == LEFT2RIGHT) && (iMyY == (mConfigure->mCellY-1)))
        return false;

    switch (iDir){
        case RIGHT2LEFT: iMyY--; break;
        case LEFT2RIGHT: iMyY++; break;
    }




    //Immediately beside
    for (int i = 0; i < iLen; i++){
    	iMyX = (*ppVehicle)->x - i;

    	if (iMyX < 0)
    		iMyX = mConfigure->mCellX + iMyX;

    	pEntity = mGrid->getGrid(iMyX, iMyY);
    	if (pEntity != NULL)
    		return false;
    }



    //Find ahead  vehicle
    iMyX = (*ppVehicle)->x;
    iAheadDistance = 0;
    do{
    	iMyX++;
        iAheadDistance++;
        if (iMyX == mConfigure->mCellX)
        	iMyX = 0;

        pEntity =  mGrid->getGrid(iMyX, iMyY);


    }while ((pEntity == NULL) && (iAheadDistance <= mConfigure->mCellX));



    if (iAheadDistance > mConfigure->mCellX)
    	return true;

    if (iAheadDistance > 0)
    	iAheadDistance--;

    vAheadVehicle = (Vehicle *) pEntity;



    //Find back  vehicle
    iMyX = (*ppVehicle)->x - iLen;
    iBackDistance = 0;
    //Finding the previous vehicle
    do{
    	iMyX--;
    	iBackDistance++;
        if (iMyX < 0)
        	iMyX = mConfigure->mCellX - 1;

        pEntity = mGrid->getGrid(iMyX, iMyY);
        //assert(iAheadDistance < 101);
    }while ((pEntity == NULL) && (iBackDistance <= mConfigure->mCellX));


    if (iBackDistance > mConfigure->mCellX)
     	return true;


    vBackVehicle =  (Vehicle*) pEntity;
    if (iBackDistance > 0)
    	iBackDistance--;

    //#1
    if (vBackVehicle->whoAmI().compare("Vehicle") == 0)
    {
        type = vBackVehicle->type;
         
        double beta      = 1.0f - betaFunction(type->param);
        double dheadway  =  static_cast<double>(type->aheadInt);

        int diffV = vBackVehicle->vx - iVel;
        if (diffV > 0){
            double dTH = static_cast<double>(iBackDistance) / static_cast<double>(diffV);

            if (dTH < (dheadway * beta))
                return false;



        }//end-if (diffV != 0){
        /*
        assert(ds >= 0.0f);

        if (iBackDistance < iVel);
            return false;
        */
        //The rule is based on temporal distance which considers as distance as velocity of both vehicles
        //Both vehicles move at the same velocity: diffV is zero and in this case it is safe to change
        //My vehicle is faster than previous, diffV is minor than zero. It is safe to change
        //It is important to not hit one to another when analyzed vehicle is slower than the previous one
    }




    //#2
    if (vAheadVehicle->whoAmI().compare("Vehicle") == 0)
    {
        type = (*ppVehicle)->type;
        //double alpha     = betaFunction(type->param);

        double beta      = 1.0f - betaFunction(type->param);
        double dheadway  =  static_cast<double>(type->aheadInt);

        int diffV = iVel - vAheadVehicle->vx;
        if (diffV != 0){
            double dTH = static_cast<double>(iAheadDistance) / static_cast<double>(diffV);
            if (dTH < (dheadway * beta))
                return false;


        }//end-if (diffV != 0){

    }else{
      cerr << "Erro in considering ahead vehicle at line 995 at TModel.cpp file" << endl;
      exit(-1);
    }


//    if (iVel > iAheadDistance)
  //  	return false;

    return true;
}

/*
 * Being motivated whenever the driver cannot move in desired fVelocity
 * In case of symmetric hule, this motivation can be applied for both sides
 * Left to Right - Right to Left
 *
 */
bool TModel_ML_SYM::rule_MovingFaster(Vehicle **ppVehicle){

   double   dVel = 0.0f,
            dTH   = 0.0f,
            deltaV    = 0.0f,
            dheadway  = 0.0f,
            dsafedist = 0.0f,
//            deltaVMax = 50.0f,
            ds = 0.0f,
            alpha = 0.0f,
            beta  = 0.0f;

   int iVel = 0,
       iDist = 0,
       iAheadVel = 0,
       iMax = 0;

   tpVehiclesType *type = (*ppVehicle)->type;

   alpha = betaFunction(type->param);
   beta  = 1.0f - betaFunction(type->param);
   deltaV    =  static_cast<double>((*ppVehicle)->type->inc);
   dheadway  =  static_cast<double>((*ppVehicle)->type->aheadInt);
   dsafedist =   static_cast<double>((*ppVehicle)->type->safeDist);

   iMax = min(mConfigure->mVMax, (*ppVehicle)->type->vMax);
   getDistanceAndVelocity(*ppVehicle  , &iDist, &iAheadVel);


   dVel = static_cast<double> ((*ppVehicle)->vx) + ( (deltaV * alpha) - 0.5f);
   //plot '2_4.txt' u (floor((4*$1)-0.5)):($2) w p
   dVel = floor(dVel);

   iVel = static_cast<int> (dVel);

   iVel = max(iVel, 0);

   iVel = min (iVel, iMax);
   dVel = static_cast<double> (iVel);
   if (fabs(dVel - static_cast<double> (iAheadVel)) < ERROR)
       return false;

   dTH = static_cast<double>(iDist) / (dVel - static_cast<double> (iAheadVel)); //+ (deltaV * alpha));

   if (( dTH >  0.0f) && (dTH <= (dheadway * beta))){
    ds = (dsafedist * beta) ;//* dTH	;
   }


   assert(ds >= 0.0f);
   iDist -= truncCurve3(ds);

   if (iDist < 0)
      iDist = 0;

   if (iVel > iDist)
       return true;

   return false;



}

/*
 * Verify if the destination lane presents best flow
 */
bool TModel_ML_SYM::rule_BestFlow(Vehicle **ppVehicle, int iDir){


    Vehicle /* *vAheadVehicle  = NULL,*/
            *vBesideVehicle = NULL;

    Entity *pEntity = NULL;
    tpVehiclesType *type = (*ppVehicle)->type;

    int iMyY = static_cast<int> ((*ppVehicle)->y),
        iMyX = (*ppVehicle)->x,
        iBesideDistance = 0,
        iLen =  type->size ,
        iVel = static_cast<int> ((*ppVehicle)->vx),
        iVelBeside = 0;

    if ((iDir == RIGHT2LEFT) && (iMyY == 0))
        return false;

    if ((iDir == LEFT2RIGHT) && (iMyY == (mConfigure->mCellY-1)))
        return false;

    switch (iDir){
        case RIGHT2LEFT: iMyY--; break;
        case LEFT2RIGHT: iMyY++; break;
    }
    //Immediately beside
    for (int i = 0; i < iLen; i++){
        iMyX = (*ppVehicle)->x - i;

        if (iMyX < 0)
            iMyX = mConfigure->mCellX + iMyX;

        pEntity = mGrid->getGrid(iMyX, iMyY);
        if (pEntity != NULL)
            return false;
    }

    iMyX = (*ppVehicle)->x;
    //iAheadDistance = 0;
    do{
        iMyX++;
        iBesideDistance++;
        if (iMyX == mConfigure->mCellX)
            iMyX = 0;

        pEntity = mGrid->getGrid(iMyX, iMyY);


    }while ((pEntity == NULL) && (iBesideDistance <= mConfigure->mCellX));

    if (iBesideDistance > mConfigure->mVMax)
        return true;

    if (iBesideDistance > 0)
        iBesideDistance--;

    vBesideVehicle =  (Vehicle *) pEntity;
    iVelBeside = static_cast<int> (vBesideVehicle->vx);



    //iBesideDistance += iVelBeside;




    if (iVelBeside <= 0)
        return false;




    //iBesideDistance += iVelBeside;

    if (iVel <=  iVelBeside)
        return true;


    return false;


}

void TModel_ML_SYM::finalizer(void){

    TModel::finalizer();

}
 


/**
 * Apply symmetric rules or simple ahead movement based on T_UFRRJ
 */
TModel_ML_ASYM::TModel_ML_ASYM():
TModel_ML_SYM(){
    //mIndexDensity(0){
};

TModel_ML_ASYM::~TModel_ML_ASYM(){
};

void TModel_ML_ASYM::changeLaneRules(int step, Vehicle **ppVehicle, int iDir){
    tpVehiclesType *type = (*ppVehicle)->type;

    bool changed = false;
    bool bestFlow = false;
    if (iDir == RIGHT2LEFT){
        bestFlow = rule_BestFlow(ppVehicle, RIGHT2LEFT);
        if(rule_MovingFaster(ppVehicle)  && !changed && bestFlow){
            if (rule_Safe(ppVehicle, RIGHT2LEFT)){
                if (getRandom() <= type->left_p){

                    (*ppVehicle)->vy = -1;
                    changed = true;


                }//end-if (getRandom() < type->left_p){
            }//end-if (r05_Change(idx, RIGHT2LEFT)){
        }//end-if(r01_Motivated(idx) && !changed ){
    }else{
        //bestFlow = rule_BestFlow(idx, LEFT2RIGHT);
        if(rule_FasterBehind(ppVehicle) && !changed ){
            if (rule_Safe(ppVehicle, LEFT2RIGHT)){
                if (getRandom() <= type->right_p){
                    (*ppVehicle)->vy = +1;
                    changed = true;


                }//end-if (getRandom() < type->left_p){
            }//end-if (r05_Change(idx, RIGHT2LEFT)){
        }//end-if(r01_Motivated(idx) && !changed ){


        bestFlow = rule_BestFlow(ppVehicle, LEFT2RIGHT);
        if(rule_FasterAhead(ppVehicle) && !changed && bestFlow){
            if (rule_Safe(ppVehicle, LEFT2RIGHT)){
                if (getRandom() <= type->right_p){
                    (*ppVehicle)->vy = +1;
                    changed = true;


                }//end-if (getRandom() < type->left_p){
            }//end-if (r05_Change(idx, RIGHT2LEFT)){
        }//end-if(r01_Motivated(idx) && !changed ){
    }// if (iDir == RIGHT2LEFT){

    
     
    
}


/*
 * r03_Motivated rule makes drivers change to slower lane.
 * r03_Motivated rule takes into account ahead vehicle.
 * In case of Brazil, it is right lane.
 */
bool TModel_ML_ASYM::rule_FasterAhead(Vehicle **ppVehicle){

   double   dVel = 0.0f,
            dTH   = 0.0f,
            deltaV    = 0.0f,
            dheadway  = 0.0f,
            dsafedist = 0.0f,
//            deltaVMax = 50.0f,
            ds = 0.0f,
            alpha = 0.0f,
            beta  = 0.0f;

   int iVel = 0,
       iDist = 0,
       iAheadVel = 0,
       iMax = 0;


   tpVehiclesType *type = (*ppVehicle)->type;

   alpha = betaFunction(type->param);
   beta  = 1.0f - betaFunction(type->param);

   deltaV    =  (*ppVehicle)->type->inc;
   dheadway  =  (*ppVehicle)->type->aheadInt;
   dsafedist =   (*ppVehicle)->type->safeDist;

   iMax = min(mConfigure->mVMax, (*ppVehicle)->type->vMax);
   getDistanceAndVelocity(*ppVehicle, &iDist, &iAheadVel);
   if (iDist >= iMax)
       return true;


   dVel = static_cast<double> ((*ppVehicle)->vx) + ( (deltaV * alpha) - 0.5f);
   //plot '2_4.txt' u (floor((4*$1)-0.5)):($2) w p
   dVel = floor(dVel);

   iVel = static_cast<int> (dVel);

   iVel = max(iVel, 0);

   iVel = min (iVel, iMax);

   dVel = static_cast<double> (iVel);


    if (fabs((dVel - static_cast<double>(iAheadVel))) <= ERROR)
        return false;

   dTH = static_cast<double>(iDist) / (dVel - static_cast<double> (iAheadVel)); //+ (deltaV * alpha));

   if (( dTH >  0.0f) && (dTH <= (dheadway * beta))){
    ds = (dsafedist * beta) ;//* dTH	;
   }


   assert(ds >= 0.0f);
   iDist -= truncCurve3(ds);

   if (iDist < 0)
      iDist = 0;

   if (iVel <= iDist)
       return true;

   return false;



}

/*
 * Brief: back vehicle is getting closer, the analyzed one becomes
 * motived to change the lane
 */

bool TModel_ML_ASYM::rule_FasterBehind(Vehicle **ppVehicle){
    double   dVel = 0.0f,
        dTH   = 0.0f,
        deltaV    = 0.0f,
        dheadway  = 0.0f,
        dsafedist = 0.0f,
//            deltaVMax = 50.0f,
        ds = 0.0f,
        alpha = 0.0f,
        beta  = 0.0f;

    //Veículo que vem detrás
    Vehicle *vBackVehicle;

    //Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = NULL;

    int iVel       = 0,
        //iBackVel   = 0,
        iBackDist  = 0,
        iMax = 0;

    Entity *entity = getBack(&iBackDist, (*ppVehicle)->x,  (*ppVehicle)->y, (*ppVehicle)->lg);

    if (entity->whoAmI().compare("Vehicle") == 0){
      vBackVehicle = (Vehicle*) entity;

      type = vBackVehicle->type;

       iMax = min(mConfigure->mVMax, type->vMax);

      alpha = betaFunction(type->param);
      beta  = 1.0f - betaFunction(type->param);

      deltaV    =  type->inc;
      dheadway  =  type->aheadInt;
      dsafedist =   type->safeDist;

      dVel = static_cast<double> (vBackVehicle->vx) + ( (deltaV * alpha) -   0.5f);

      dVel = floor(dVel);

      iVel = static_cast<int> (dVel);

      iVel = max(iVel, 0);

      iVel = min (iVel, iMax);

      dVel = static_cast<double> (iVel);
      int auxv = (*ppVehicle)->vx;

      if (fabs((dVel - static_cast<double>(auxv))) <= ERROR)
          return false;

      dTH = static_cast<double>(iBackDist) / (dVel - static_cast<double>(auxv)); //+ (deltaV * alpha));

      if (( dTH >  0.0f) && (dTH <= (dheadway * beta))){
       ds = (dsafedist * beta) ;//* dTH	;
      }


     assert(ds >= 0.0f);
     iBackDist -= truncCurve3(ds);

     if (iBackDist < 0)
        iBackDist = 0;

     if (iVel > iBackDist)
         return true;

     return false;
    }
    return false;


}


