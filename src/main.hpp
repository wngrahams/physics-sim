#ifndef PHYSICS_SIM_MAIN_HPP
#define PHYSICS_SIM_MAIN_HPP

using namespace std;
#include <vector>
#include <cmath>

// cube variables
#define WEIGHT_PER_MASS 0.1
#define L0_SIDE 0.1
#define K_SPRING 10000.0
#define INITIAL_HEIGHT 5.0
#define NUM_OF_MASSES 8
#define NUM_OF_SPRINGS 28
#define DIMENSIONS 3

// world variables
#define G -9.80665
#define K_GROUND 100000.0
#define DT 0.001
#define V_DAMP_CONST 0.999

#define NUM_OF_ITERATIONS 2000

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

// initialize cube with masses and springs
void initialize_cube(vector<Mass> &, vector<Spring> &);
// calculate distance between two points
double dist(vector<double>, vector<double>);
// calculate force from springs, gravity
void calculate_force(vector<Mass> &, vector<Spring> &, vector<vector<double>> &);
// add external forces
void add_external_force(vector<Mass> &, vector<Spring> &, vector<vector<double>> &);
// add force due to ground
void add_ground_force(vector<Mass> &, vector<Spring> &, vector<vector<double>> &);
// update position of masses due to force
void update_position(vector<Mass> &, vector<Spring> &, vector<vector<double>> &);



#endif //PHYSICS_SIM_MAIN_HPP
