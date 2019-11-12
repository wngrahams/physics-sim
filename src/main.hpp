#ifndef PHYSICS_SIM_MAIN_HPP
#define PHYSICS_SIM_MAIN_HPP

using namespace std;
#include <vector>

#define WEIGHT_PER_MASS 0.1
#define L0 0.1
#define K 10000
#define INITIAL_HEIGHT 5
#define NUM_OF_MASSES 8
#define NUM_OF_SPRINGS 28
#define NUM_OF_ITERATIONS 100000

struct Mass {
    double m; // mass in kg
    vector<double> p; // position vector in m
    vector<double> v; // velocity vector in m/s
    vector<double> a; // acceleration vector in m/s^2
};

struct Spring {
    double k; // spring constant in N/m
    double l0; // original rest length in m
    int m1; // index of first mass object in spring
    int m2; // index of second mass object in spring
};

void initialize_cube(vector<Mass>, vector<Spring>);


#endif //PHYSICS_SIM_MAIN_HPP
