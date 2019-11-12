#include <iostream>
#include "main.hpp"

using namespace std;

int main() {

    vector<Mass> mass;
    vector<Spring> spring;
    initialize_cube(mass, spring);



    // simulation loop
    for (int iteration = 0; iteration < NUM_OF_ITERATIONS; iteration++) {



    }


}

void initialize_cube(vector<Mass> mass, vector<Spring> spring) {
    // create masses
    Mass temp = {WEIGHT_PER_MASS,
                 {0, 0, 0},
                 {0, 0, 0},
                 {0, 0, 0}};
    temp.p = {0, 0, INITIAL_HEIGHT};
    mass.emplace_back(temp);
    temp.p = {L0, 0, INITIAL_HEIGHT};
    mass.emplace_back(temp);
    temp.p = {L0, L0, INITIAL_HEIGHT};
    mass.emplace_back(temp);
    temp.p = {0, L0, INITIAL_HEIGHT};
    mass.emplace_back(temp);
    temp.p = {0, 0, INITIAL_HEIGHT + L0};
    mass.emplace_back(temp);
    temp.p = {L0, 0, INITIAL_HEIGHT + L0};
    mass.emplace_back(temp);
    temp.p = {L0, L0, INITIAL_HEIGHT + L0};
    mass.emplace_back(temp);
    temp.p = {0, L0, INITIAL_HEIGHT + L0};
    mass.emplace_back(temp);

//    for (int i = 0; i < NUM_OF_MASSES; i++) {
//        cout << mass[i].p[0] << "\t";
//        cout << mass[i].p[1] << "\t";
//        cout << mass[i].p[2] << "\t\n";
//    }

    Spring t = {K, L0, 0,};

    spring

}