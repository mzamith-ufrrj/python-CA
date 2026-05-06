#ifndef _STATISTIC_HPP_
#define _STATISTIC_HPP_
#include "Configure.hpp"
#include "Grid.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
using namespace std;
class Statistic
{
    public:
        Statistic();
        ~Statistic();
        void setDensity(string ss){ mDensityAsString  = ss; };
        void setVAdusted(void){ mVAdjusted++; }
        void setVelocity(unsigned int);
        void saveStatistic(void);

        void setGrid(shared_ptr<Grid>g){ mGrid = g; }
        void setConfigure(shared_ptr<Configure>);
    private:
        string mDensityAsString;
        unsigned int mVAdjusted,
                     *mVelocities;
        
        shared_ptr<Grid>      mGrid;
        shared_ptr<Configure> mConfigure;
        //Configure *mConfigure;
};
#endif