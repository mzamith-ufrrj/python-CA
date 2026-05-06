#include "Statistic.hpp"
#include <cassert>

Statistic::Statistic():
mVAdjusted(0),
mVelocities(NULL),
mGrid(NULL),
mConfigure(NULL)
{

};
Statistic::~Statistic(){
    if (mVelocities != NULL){
        free(mVelocities);
    }
};

void Statistic::setConfigure(shared_ptr<Configure> c){ 
    mConfigure = c; 
    if (mVelocities != NULL)
        free(mVelocities);
    //Allocating the average velocity in order to calculate collisiton probability
    assert(posix_memalign(reinterpret_cast<void**> (&mVelocities), ALIGN, mConfigure->mVMax *  sizeof(unsigned int)) == 0);
    bzero(mVelocities, mConfigure->mVMax *  sizeof(unsigned int));
    
};


void Statistic::setVelocity(unsigned int v){
    unsigned int v1 = v;
    if (v1 > mConfigure->mVMax)
        v1 = mConfigure->mVMax;
    mVelocities[v1]++;
};
void Statistic::saveStatistic(void){

   fstream log;
   unsigned int nVehicles = 0;
   Entity *e              = mGrid->getEntities();



   //Applying movement rules
   int iOcc = 0;
   while (e != NULL){
      if (e->whoAmI().compare("Vehicle") == 0){
         nVehicles++;
         iOcc += e->lg;
      }

      e = e->next;
   }

   double percent = static_cast<double> (mVAdjusted) /  static_cast<double>(mConfigure->mSTime - mConfigure->mDTime);
   if (nVehicles > 0)
      percent = percent / static_cast<double> (nVehicles) ;

   string fileName = "cluster.adjusted." + string(mConfigure->mModelName) + "-" + mDensityAsString + ".csv";
   log.open(fileName, fstream::out | fstream::app);
   assert(log.is_open());

   double occupation = static_cast<double>(iOcc) / static_cast<double>(mConfigure->mCellX * mConfigure->mCellY);

   log << fixed << setw(10) << setprecision(5) << occupation << ";" << setw(10) << (mConfigure->mSTime - mConfigure->mDTime) << ";" << setw(10) << mVAdjusted << ";";
   log << fixed << setw(10) << setprecision(5) <<  percent <<  ";";
   for (unsigned int i = 0; i < mConfigure->mVMax; i++){
        log << fixed << setw(10) << mVelocities[i] << ";";
   }
   log << "-1" << endl;
   log.close();



   mVAdjusted = 0;
};
    