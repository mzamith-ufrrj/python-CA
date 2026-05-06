#ifndef _MOVEMENTSENSOR_HPP_
#define _MOVEMENTSENSOR_HPP_
#include <vector>
#include "Grid.hpp"
#include "ModelTypes.hpp"
#include <string>
#include <cstring>
#include <memory>
using namespace std;
class MovementSensor
{

public:

   MovementSensor();
   ~MovementSensor();


   void setFileName(string f, string d ){mFileName = f; mDensityAsString = d;};

   void saveStatistic(int);
   void update(void);
   void reset(void);

   void setCellInMovement(int, int, int, Vehicle *);
   void setCellInStopped(int);
   void clear(void);

   void setSaveFiles(bool, bool);

   float getAverageSpeedPhoto(void) { return mAvVeloPhoto; };
   float getAverageSpeedFixed(void) { return mAvVeloFixed; };
   void  setAverageSpeed(int);
   
   shared_ptr<Grid> mGrid;
   
protected:
   void getPhoto(int);
   void getFixed(int);

  //Grid *mGrid;
   vector<float>   mDPhoto,   /* <!Brief Road density based on a picture of the road  > */
                   mVPhoto,   /* <!Brief Road average velocity based on a picture of the road  > */
                   mVFixed,   /* <!Brief Arverage velocity considering a fixed point > */
                   mFFixed,   /* <!Brief Arverage flow considering a fixed point > */
                   mSFixed;   /* <!Brief Stop vehicles on detector cell and one time step > */
   float mSamplesPhoto,
         mSamplesFixed;

   string mFileName,
          mDensityAsString;
   //string mPath;

   bool mFixed,
        mPhoto;

   float mAvVeloPhoto,
         mAvVeloFixed;




};

#endif
