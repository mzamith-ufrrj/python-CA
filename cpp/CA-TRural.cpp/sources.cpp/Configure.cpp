#include "Configure.hpp"
#include <cassert>
#include <fstream>
#include <cstddef>
#include <jsoncpp/json/json.h>
using namespace Json;
using namespace std;

Configure::Configure():
mTypeModel(Configure::SINGLE_LANE),
mVMax(0),
mCellX(0), mCellY(0),
mDeltaH(0.0), mDefaultSize(0.0),
mSTime(0), mDTime(0), mStTime(0),
mDensity(0.0),
mFixed(false), mPhoto(false), mLogCluster(false), //mLog(false),
mDeceleration(0), mTimePerception(0.0),
mModelName(""),
//mRoadBlock(0), mLogVehicles(-1),
mProfile(NULL){
    
};


Configure::Configure(Configure *config){
    assert(config != NULL);

    mModelName = config->mModelName;
    mTypeModel = config->mTypeModel;
    mVMax = config->mVMax;
    mCellX = config->mCellX;
    mCellY = config->mCellY;
    mDeltaH = config->mDeltaH;
    mDefaultSize = config->mDefaultSize;
    mSTime  = config->mSTime;
    mDTime  = config->mDTime;
    mStTime = config->mStTime;
    mDensity = config->mDensity;
    mFixed = config->mFixed;
    mPhoto  = config->mPhoto;
    mLogCluster = config->mLogCluster;
    //mLog  = config->mLog;
    mDeceleration = config->mDeceleration;
    mTimePerception  = config->mTimePerception;
   
    mTypeIndex = config->mTypeIndex;
    assert(mTypeIndex > 0);
    assert(posix_memalign(reinterpret_cast<void**> (&mProfile), ALIGN, mTypeIndex *  sizeof(tpVehiclesType)) == 0);
    memcpy(mProfile, config->mProfile, mTypeIndex * sizeof(tpVehiclesType));



};
Configure::~Configure(){
    clear();
};



void Configure::loadConfigFile(const string& configFile){
    Value root; 
    fstream input;
    

    input.open(configFile);
    assert(input.is_open());
    input >> root;
    input.close();
    mModelName   = root["global-params"].get("name", "UTF-8" ).asString();
    mVMax        = root["global-params"].get("max-speed", "UTF-8" ).asInt();
    mCellX       = root["global-params"].get("cellX", "UTF-8" ).asInt();
    mCellY       = root["global-params"].get("cellY", "UTF-8" ).asInt();
    mDeltaH      = root["global-params"].get("size", "UTF-8" ).asFloat();
    mDefaultSize = root["global-params"].get("default-size", "UTF-8" ).asFloat();
    mSTime       = root["global-params"].get("time-steps", "UTF-8" ).asInt();
    mDTime       = root["global-params"].get("transiente-steps", "UTF-8" ).asInt();
    mStTime      = root["global-params"].get("statistic-steps", "UTF-8" ).asInt();
    mFixed       = root["global-params"].get("fixed", "UTF-8" ).asBool();
    mPhoto       = root["global-params"].get("photo", "UTF-8" ).asBool();
    mLogCluster  = root["global-params"].get("logCluster", "UTF-8" ).asBool();
    //mLog         = root["global-params"].get("time-perception", "UTF-8" ).asBool();
    mDeceleration   = root["global-params"].get("deceleration", "UTF-8" ).asInt();
    mTimePerception = root["global-params"].get("time-perception", "UTF-8" ).asFloat();
    mTypeModel = root["global-params"].get("type-model", "UTF-8" ).asInt();
    mTypeIndex = root["global-params"].get("n-profiles", "UTF-8" ).asInt();
    assert(mTypeIndex > 0);
    assert(posix_memalign(reinterpret_cast<void**> (&mProfile), ALIGN, mTypeIndex *  sizeof(tpVehiclesType)) == 0);

    Value profiles = root["profile"];
    for ( int i = 0; i < profiles.size(); i++ ){
        bzero(mProfile[i].description, STRINGSIZE);
        string name  = profiles[i].get("name", "UTF-8" ).asString();
        strcpy(mProfile[i].description, name.c_str());
        mProfile[i].percent = profiles[i].get("occ", "UTF-8" ).asDouble();
        mProfile[i].size        = profiles[i].get("size", "UTF-8" ).asInt();
        mProfile[i].vMax        = profiles[i].get("max-speed", "UTF-8" ).asInt();
        mProfile[i].param[0]    = profiles[i].get("betaA-acc", "UTF-8" ).asDouble();
        mProfile[i].param[1]    = profiles[i].get("betaB-acc", "UTF-8" ).asDouble();
        mProfile[i].inc         = profiles[i].get("inc", "UTF-8" ).asDouble();
        mProfile[i].desc        = profiles[i].get("desc", "UTF-8" ).asInt();
        mProfile[i].left_p      = profiles[i].get("left-p", "UTF-8" ).asDouble();
        mProfile[i].right_p     = profiles[i].get("right-p", "UTF-8" ).asDouble();
        mProfile[i].aheadInt    = profiles[i].get("aheadway", "UTF-8" ).asInt();
        mProfile[i].safeDist    = profiles[i].get("safedist", "UTF-8" ).asInt();
        
    }
    
    
}   


void Configure::setProfiles(int i){
    mTypeIndex = i;
    assert(posix_memalign(reinterpret_cast<void**> (&mProfile), ALIGN, mTypeIndex *  sizeof(tpVehiclesType)) == 0);
};

void Configure::setProfile(int index,
                           char *name,
                           float percent,
                           int size,
                           int vMax,
                           double alpha,
                           double beta,
                           double inc,
                           int desc,
                           int aheadway,
                           int safedist,
                           float left_p,
                           float right_p){

    assert(index < mTypeIndex);
    strcpy(mProfile[index].description, name);
    mProfile[index].percent = percent;
    mProfile[index].size = size;
    mProfile[index].vMax = vMax;

    mProfile[index].param[0] = alpha;
    mProfile[index].param[1] = beta;
    
    mProfile[index].inc = inc;
    mProfile[index].desc = desc;
    mProfile[index].aheadInt = aheadway;
    mProfile[index].safeDist = safedist;
    mProfile[index].left_p = left_p;
    mProfile[index].right_p = right_p;



}
/**
 *  inicialize
 *  @param fileName: config file name
 */
void Configure::print(void){
    cout << " Parameters: \n";
    cout << " Model = " << mModelName << "\t type: " << mTypeModel << endl;
    cout << " cellX  = " << mCellX << endl;
    cout << " cellY  = " << mCellY << endl;
    cout << " DeltaH = " << mDeltaH << endl;
    cout << " sTime        = " << mSTime << endl;
    cout << " dTime        = " << mDTime << endl;
    cout << " stTime       = " << mStTime << endl;
    //cout << " deltaTime    = " << mParam.deltaTime << endl;
    cout << " Density     = " << fixed << setprecision(5) << mDensity << endl;
    cout << " vMax         = " << mVMax << endl;
    cout << " *deceleration = " << mDeceleration << endl;

    cout << " spc. perc.   = " << fixed << setprecision(1) << mTimePerception << endl;
    //cout << " road block   = " << boolalpha  <<  mRoadBlock << endl;

    cout << " Log files: " << endl;
    cout << " \tfixed           = " << boolalpha  << mFixed << endl;
    cout << " \tphoto           = " << boolalpha  <<  mPhoto << endl;
    cout << " \tlogCluster      = " << boolalpha  << mLogCluster << endl;
    


    cout << "=====================================================" << endl << endl;
    cout << fixed << setprecision(0);
    cout << "Groups of vehicles: " <<  mTypeIndex  << endl;
    for (int i = 0; i < mTypeIndex ; i++){
        tpVehiclesType *type =  &mProfile[i];
        cout << "("  << i << ") \t";
        cout << "Profile: " << type->description << " " \
        << "Percent: " << fixed << setprecision(2) << type->percent \
        << " Size: " << type->size \
        << " vMax: " << type->vMax \
        << " Alpha parameter ACC(" << setprecision(0) << type->param[0] << ", " << type->param[1] << ") "
        << " Inc: " << setprecision(0) << type->inc \
        << " Desc: " << type->desc \
        << " Aheadway:" << type->aheadInt \
        << " safeDist:" << type->safeDist \
        << " Left prob.:" <<   setprecision(2) << type->left_p << " Right prob." << type->right_p \
        << endl;
    }
    cout.flush();



};
void Configure::clear(void){
    mTypeIndex = 0;
    if (mProfile != NULL)
        free(mProfile);

    mProfile = NULL;
}
