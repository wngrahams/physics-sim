#include <iostream>
#include "main.hpp"

using namespace std;

int main() {

    vector<Mass> mass;
    vector<Spring> spring;
    initialize_cube(mass, spring);

//    for (int i = 0; i < NUM_OF_MASSES; i++) {
//        cout << mass[i].p[0] << "\t";
//        cout << mass[i].p[1] << "\t";
//        cout << mass[i].p[2] << "\t\n";
//    }

    // simulation loop
    for (int iteration = 0; iteration < NUM_OF_ITERATIONS; iteration++) {
        // calculate force on each spring
        vector<vector<double>> force;
        for (int i = 0; i < NUM_OF_MASSES; i++) {
            force.emplace_back();
            force[i] = {0,0,0};
        }
        calculate_force(mass, spring, force);
        add_external_force(mass, spring, force);
        add_ground_force(mass, spring, force);

        for (int i = 0 ; i < NUM_OF_MASSES; i++) {
            cout << force[i][0] << ", " << force[i][1] << ", " << force[i][2] << "\n";
        }

    }


}

void initialize_cube(vector<Mass> &mass, vector<Spring> &spring) {
    // create masses
    Mass temp_mass = {WEIGHT_PER_MASS,
                 {0, 0, 0},
                 {0, 0, 0},
                 {0, 0, 0}};
    temp_mass.p = {0, 0, INITIAL_HEIGHT}; // 0
    mass.emplace_back(temp_mass);
    temp_mass.p = {L0_SIDE, 0, INITIAL_HEIGHT}; // 1
    mass.emplace_back(temp_mass);
    temp_mass.p = {L0_SIDE, L0_SIDE, INITIAL_HEIGHT}; // 2
    mass.emplace_back(temp_mass);
    temp_mass.p = {0, L0_SIDE, INITIAL_HEIGHT}; // 3
    mass.emplace_back(temp_mass);
    temp_mass.p = {0, 0, INITIAL_HEIGHT + L0_SIDE}; // 4
    mass.emplace_back(temp_mass);
    temp_mass.p = {L0_SIDE, 0, INITIAL_HEIGHT + L0_SIDE}; // 5
    mass.emplace_back(temp_mass);
    temp_mass.p = {L0_SIDE, L0_SIDE, INITIAL_HEIGHT + L0_SIDE}; // 6
    mass.emplace_back(temp_mass);
    temp_mass.p = {0, L0_SIDE, INITIAL_HEIGHT + L0_SIDE}; // 7
    mass.emplace_back(temp_mass);

    spring.emplace_back(Spring {K_SPRING, dist(mass[0].p, mass[1].p), 0, 1});
    spring.emplace_back(Spring {K_SPRING, dist(mass[0].p, mass[2].p), 0, 2});
    spring.emplace_back(Spring {K_SPRING, dist(mass[0].p, mass[3].p), 0, 3});
    spring.emplace_back(Spring {K_SPRING, dist(mass[0].p, mass[4].p), 0, 4});
    spring.emplace_back(Spring {K_SPRING, dist(mass[0].p, mass[5].p), 0, 5});
    spring.emplace_back(Spring {K_SPRING, dist(mass[0].p, mass[6].p), 0, 6});
    spring.emplace_back(Spring {K_SPRING, dist(mass[0].p, mass[7].p), 0, 7});
    spring.emplace_back(Spring {K_SPRING, dist(mass[1].p, mass[2].p), 1, 2});
    spring.emplace_back(Spring {K_SPRING, dist(mass[1].p, mass[3].p), 1, 3});
    spring.emplace_back(Spring {K_SPRING, dist(mass[1].p, mass[4].p), 1, 4});
    spring.emplace_back(Spring {K_SPRING, dist(mass[1].p, mass[5].p), 1, 5});
    spring.emplace_back(Spring {K_SPRING, dist(mass[1].p, mass[6].p), 1, 6});
    spring.emplace_back(Spring {K_SPRING, dist(mass[1].p, mass[7].p), 1, 7});
    spring.emplace_back(Spring {K_SPRING, dist(mass[2].p, mass[3].p), 2, 3});
    spring.emplace_back(Spring {K_SPRING, dist(mass[2].p, mass[4].p), 2, 4});
    spring.emplace_back(Spring {K_SPRING, dist(mass[2].p, mass[5].p), 2, 5});
    spring.emplace_back(Spring {K_SPRING, dist(mass[2].p, mass[6].p), 2, 6});
    spring.emplace_back(Spring {K_SPRING, dist(mass[2].p, mass[7].p), 2, 7});
    spring.emplace_back(Spring {K_SPRING, dist(mass[3].p, mass[4].p), 3, 4});
    spring.emplace_back(Spring {K_SPRING, dist(mass[3].p, mass[5].p), 3, 5});
    spring.emplace_back(Spring {K_SPRING, dist(mass[3].p, mass[6].p), 3, 6});
    spring.emplace_back(Spring {K_SPRING, dist(mass[3].p, mass[7].p), 3, 7});
    spring.emplace_back(Spring {K_SPRING, dist(mass[4].p, mass[5].p), 4, 5});
    spring.emplace_back(Spring {K_SPRING, dist(mass[4].p, mass[6].p), 4, 6});
    spring.emplace_back(Spring {K_SPRING, dist(mass[4].p, mass[7].p), 4, 7});
    spring.emplace_back(Spring {K_SPRING, dist(mass[5].p, mass[6].p), 5, 6});
    spring.emplace_back(Spring {K_SPRING, dist(mass[5].p, mass[7].p), 5, 7});
    spring.emplace_back(Spring {K_SPRING, dist(mass[6].p, mass[7].p), 6, 7});

}

double dist(vector<double> a, vector<double> b) {
    return sqrt(pow(b[0]-a[0],2) + pow(b[1]-a[1],2) + pow(b[2]-a[2],2));
}

void calculate_force(vector<Mass> &mass, vector<Spring> &spring, vector<vector<double>> &force) {

    // force due to spring
    for (int i = 0; i < NUM_OF_SPRINGS; i++) {
        // calculate force vector for spring
        double length = dist(mass[spring[i].m1].p, mass[spring[i].m2].p);
        double forceNormalized = K_SPRING * (length - spring[i].l0);
        vector<double> forceVector = {forceNormalized * (mass[spring[i].m2].p[0] - mass[spring[i].m1].p[0]) / length,
                                      forceNormalized * (mass[spring[i].m2].p[1] - mass[spring[i].m1].p[1]) / length,
                                      forceNormalized * (mass[spring[i].m2].p[2] - mass[spring[i].m1].p[2]) / length};

        // now update force vector for masses that spring touches
        force[spring[i].m1][0] += forceVector[0];
        force[spring[i].m1][1] += forceVector[1];
        force[spring[i].m1][2] += forceVector[2];
        force[spring[i].m2][0] -= forceVector[0];
        force[spring[i].m2][1] -= forceVector[1];
        force[spring[i].m2][2] -= forceVector[2];
    }

    // force due to gravity
    for (int i = 0; i < NUM_OF_MASSES; i++) {
        force[i][2] += G; // note: gravity constant negative
    }
}

void add_external_force(vector<Mass> &mass, vector<Spring> &spring, vector<vector<double>> &force) {
    // this is where i would calculate the external forces ... if there were any
}

void add_ground_force(vector<Mass> &mass, vector<Spring> &spring, vector<vector<double>> &force) {






}
