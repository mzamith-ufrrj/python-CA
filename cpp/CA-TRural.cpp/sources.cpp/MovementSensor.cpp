/*
 *
 *   plot 'statistic.SYNC-FASTER-4-2.txt' u 4:3 w p, 'statistic.SYNC-FASTER-4-2.txt' u 7:6 w p
 *   plot 'statistic.ML-ASYM-DEFAULT.txt' u ((($4+$7)/2)*133):($3/($3+$6)) w p, 'statistic.ML-ASYM-DEFAULT.txt' u ((($4+$7)/2)*133):($6/($3+$6)) w p
 *
 */
//plot 'statistic.T-UFRRJ-ML-SYM.txt'  u ($4*1000/7.5):($3*(1.5/7.5)*3600) w p

#include "MovementSensor.hpp"
#include "ModelTypes.hpp"

#include <cassert>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <cstdlib>
#include <cstdio>

using namespace std;
MovementSensor::MovementSensor():
mFixed(true),
mPhoto(true),
mAvVeloPhoto(0.0f),
mAvVeloFixed(0.0f)
{



}

MovementSensor::~MovementSensor(){


}
//This update method works quite well
void MovementSensor::clear(void){

   for (int i = 0; i < mGrid->getCellYi(); i++){
      mVPhoto[i] = mDPhoto[i] =   0.0f;
      mVFixed[i] = mSFixed[i] =  mFFixed[i] = 0.0f;
   }

   mSamplesPhoto = 0.0f;
   mSamplesFixed = 0.0f;

}

void MovementSensor::setSaveFiles(bool p, bool f){
   this->mPhoto = p;
   this->mFixed = f;
}

void MovementSensor::getPhoto(int timeInstant){
   if (mSamplesPhoto < 2.0f) return;

   std::fstream output;
   string fileName = "";

   fileName += "statistic.photo." + mFileName + "-" + mDensityAsString + ".csv";

   float avVelocity = 0.0f;
   output.open(fileName, std::fstream::out | std::fstream::app);
   assert(output.is_open());
   output << setw(10) << timeInstant << ";" << setw(4) << mSamplesPhoto << ";";
   output << fixed << setw(10) << setprecision(5);
   for (int i = 0; i <  mGrid->getCellYi(); i++){

      float v = mVPhoto[i] / mSamplesPhoto;
      float d = mDPhoto[i] / mSamplesPhoto;

      if (isnan(v))
         v = 0.0f;
      if (isnan(d))
         d = 0.0f;

      output << fixed << setw(10) << setprecision(5) <<    (v * d) <<  ";" ;
      output << fixed << setw(10) << setprecision(5)  <<         d  <<  ";" ;
      output << fixed << setw(10) << setprecision(5)  <<         v  <<  ";" ;

      avVelocity += v;



   }

   output << endl;
   // output << gaps << endl;
   output.close();

   mAvVeloPhoto =  avVelocity /  mGrid->getCellYf();
}

void MovementSensor::getFixed(int timeInstant){
   if (mSamplesFixed < 2.0f) return;

   std::fstream output;
   string fileName = "";

   fileName += "statistic.fixed." + mFileName + "-" + mDensityAsString + ".csv";

   output.open(fileName, std::fstream::out | std::fstream::app);
   assert(output.is_open());

   output << setw(10) << timeInstant << ";" << setw(4) << mSamplesFixed << ";";
   output << fixed << setw(10) << setprecision(5);

   float avVelocity = 0.0f;
   for (int i = 0; i <  mGrid->getCellYi(); i++){
      float flow = static_cast<float>(mFFixed[i]);
      float veloc = static_cast<float>(mVFixed[i]);
      float density = static_cast<float>(mSFixed[i]);
      int iVel = static_cast<int>(veloc) ;





      if (flow > 0.0f)
         veloc /= flow;

      flow  /= mSamplesFixed;

      density /= mSamplesFixed;

/*
      if (flow > 5.0){
         cerr << "ERROR [" << timeInstant << "] flow: " << flow << endl;
      }

   */
      if (iVel > 0){
         density += (flow / veloc);
      }

      if (isnan(flow) || isinf(flow))
         flow = 0.0f;
      if (isnan(veloc) || isinf(veloc))
         veloc = 0.0f;
      if (isnan(density) || isinf(density))
         density = 0.0f;




      output << fixed << setw(10) << setprecision(5) <<  flow     <<  ";" ;
      output << fixed << setw(10) << setprecision(5)  << density  <<  ";" ;
      output << fixed << setw(10) << setprecision(5)  << veloc    <<  ";" ;

      avVelocity += veloc;



   }

   output << endl;
   // output << gaps << endl;
   output.close();

   mAvVeloFixed =  avVelocity /  mGrid->getCellYf();



}

void MovementSensor::saveStatistic(int timeInstant){
   if (mPhoto)
    this->getPhoto(timeInstant);

   if (mFixed)
    this->getFixed(timeInstant);

   this->clear();
}


void MovementSensor::reset(void){


  // mTime=0.0f;
   int y =   mGrid->getCellYi();
   assert(y > 0);

   mDPhoto.resize(y);
   mVPhoto.resize(y);

   mVFixed.resize(y);
   mFFixed.resize(y);
   mSFixed.resize(y);

   mSamplesPhoto = 0.0f;

   mSamplesFixed = 0.0f;

}

void MovementSensor::update(void){
   if (mFixed)
      mSamplesFixed++;

   if (mPhoto){

      mSamplesPhoto++;
      for (int y = 0; y <  mGrid->getCellYi(); y++){

         float v = 0.0f;
         float s = 0.0f;
         float c = 0.0f;
         for (int x = 0; x <  mGrid->getCellXi(); x++){
               Entity *e =  mGrid->getGrid(x, y);

               if (e != EMPTY){

                  if (e->whoAmI().compare("Vehicle") == 0){
                     Vehicle *vehicle = static_cast<Vehicle*>(e);
                     v += static_cast<float> (vehicle->vx);
                     if (vehicle->yChange)
                        c++;
                     s++;

                  }//end-if (e->whoAmI().compare("Vehicle") == 0){
               }//end-  if (idx != EMPTY){

         }//end-for (int x = 0; x <  mGrid->getCellXi(); x++){
         float av = v / s;
         float ad = (s /  mGrid->getCellXf());
         mVPhoto[y] += av;
         mDPhoto[y] += ad;


      }//end-for (int y = 0; y <  mGrid->getCellYi(); y++){


      Entity *ptr =  mGrid->getEntities();
         while (ptr != NULL){
         if (ptr->whoAmI().compare("Vehicle") == 0){
               Vehicle *vehicle = static_cast<Vehicle *> (ptr);
               Vehicle **ppVehicle  = &vehicle;
               (*ppVehicle)->yChange = false;
         }
         ptr = ptr->next;
      }




   }

}


void MovementSensor::setCellInMovement(int cells, int velocity, int y, Vehicle *v){
   /*
   float flow = static_cast<float>(cells) + mFFixed[y];

   cerr << "\t\tV(" << v->ID << "," << cells << "," << velocity << ")" << endl;

   if ((flow/12.0f) > 5.0f)
      cerr << "\t\t\tERROR: " << v->ID << "\t" << (flow/12.0f) << endl;
   */

   mVFixed[y] += static_cast<float>(cells * velocity);
   mFFixed[y] += static_cast<float>(cells);
}


void MovementSensor::setCellInStopped(int y){
   mSFixed[y]++;
}

void  MovementSensor::setAverageSpeed(int v){
    mAvVeloPhoto = static_cast<float>(v);
    mAvVeloFixed = mAvVeloPhoto;
}
