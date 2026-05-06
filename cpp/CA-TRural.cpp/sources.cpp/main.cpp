/*
 * File Main
 * Traffic Cellular Automata simulation  TCA-S
 */


#include <cstdlib>
#include <string>
#include <cassert>
#include <string>
#include <iostream>
#include <iomanip>
#include "Configure.hpp"
#include "CellularAutomata.hpp"
using namespace std;
void help(){
    cout << "Help" << endl;
    exit(EXIT_FAILURE);
}

int main(int ac, char **av)
{
   string filename;
   float density =0.1;
   bool verbose_flag = false;
   if ((ac == 1) || (ac >= 5))
        help();
    else if(ac == 4){
        filename = av[1];
        density = stof(av[2]);
        verbose_flag = true;
    }else if (ac == 3){
        filename = av[1];
        density = stof(av[2]);
    }else help();
   

   Configure        *configure        = new Configure();
   CellularAutomata *cellularAutomata = new CellularAutomata();

   configure->loadConfigFile(filename);
   configure->mDensity = density / 100.0f;
   assert((configure->mDensity > 0.0f) && (configure->mDensity <= 1.0f));
   
   if (verbose_flag){
      cout << "=====================================================" << endl;
      cout << "   Config file: " << filename << endl;
      cout << "Global density: " << fixed << setprecision(4) << density << endl << endl;
      cout << "=====================================================" << endl;
      configure->print();
      cellularAutomata->setVerbose(); 
   }
   
   
   cellularAutomata->init(configure);
   cellularAutomata->exec();

   delete cellularAutomata;
   delete configure;
   
   return EXIT_SUCCESS;
}

