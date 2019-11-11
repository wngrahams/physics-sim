/*
 * sim.cpp
 *
 * main driver function for the project
 */

#include <vector>
#include "sim.hpp"

#define DT 0.0001
#define NUM_OF_ITERATIONS 100000
#define NUM_OF_SPRINGS_IN_CUBE 28
#define NUM_OF_MASSES_IN_CUBE 8
#define K_GROUND 100000

int main() {

    double t = 0.0;
    double initial_rest_length = 1.0;
    double K_spring = 10000;
    double x_origin = 0;
    double y_origin = 0;
    double z_origin = 10;

    Cube *c0 = new Cube(x_origin, y_origin, z_origin, initial_rest_length, K_spring);

    for (int i = 0; i < NUM_OF_MASSES_IN_CUBE; i++) {
        std::cout << c0->get_masses()[i];
    }

    // simulation loop
    for (int iteration = 0; iteration < NUM_OF_ITERATIONS; iteration++) {
        // sum forces
        std::vector<double> f_s0 {};


    }

    return 0;
}