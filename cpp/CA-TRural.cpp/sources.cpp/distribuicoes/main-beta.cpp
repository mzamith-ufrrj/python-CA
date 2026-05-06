#include <cstdlib>
#include <iostream>
#include <random>
#include <boost/math/special_functions/beta.hpp>
#include <fstream>
using namespace std;

int main(int ac, char **av){
    std::mt19937_64  generator (time(nullptr)); //64 bits
    fstream output;
    double a = 4, b = 8;
    cout << endl << "Distruibuição beta:" << endl;
    uniform_real_distribution<double> unif(0,1);
    output.open("lentp.txt", fstream::trunc|fstream::out);

    
    

    for (unsigned int i = 0; i < 10000; i++){
        double p = unif(generator);
        output << p << " " << boost::math::ibeta_inv(a, b, p) << endl;
    }

    output.close();
    return EXIT_SUCCESS;
}