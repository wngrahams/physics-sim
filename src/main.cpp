#include <iostream>
#include <iomanip>
#include "main.hpp"

using namespace std;


int main() {

    double T = 0.0;
    cout << setprecision(17);
    vector<Mass> mass;
    vector<Spring> spring;

    // create initial cube
    initialize_cube(mass, spring);
    cout << mass[0].p[2] << "\n";


//    for (int i = 0; i < NUM_OF_MASSES; i++) {
//        cout << mass[i].p[0] << "\t";
//        cout << mass[i].p[1] << "\t";
//        cout << mass[i].p[2] << "\t\n";
//    }

//    cout << T << "\n";
//    cout << mass[0].p[0] << ", " << mass[0].p[1] << ", " << mass[0].p[2] << "\n";
//    cout << mass[0].v[0] << ", " << mass[0].v[1] << ", " << mass[0].v[2] << "\n";
//    cout << mass[0].a[0] << ", " << mass[0].a[1] << ", " << mass[0].a[2] << "\n";
//    cout << calculate_energy(mass, spring) << "\n\n";



    // simulation loop
    for (int iteration = 0; iteration < NUM_OF_ITERATIONS; iteration++) {
        // initialize force vector
        vector<vector<double>> force;
        for (int i = 0; i < NUM_OF_MASSES; i++) {
            force.emplace_back();
            force[i] = {0.0, 0.0, 0.0};
        }
        // calculate force on each spring
        calculate_force(mass, spring, force);
        add_external_force(mass, spring, force);
        add_ground_force(mass, spring, force);

        // update position of cube
        update_position(mass, spring, force);

        // calculate energy
        double total_energy = calculate_energy(mass, spring);

        cout << "T: " << T << "\n";
        cout << "p: " << mass[0].p[0] << ", " << mass[0].p[1] << ", " << mass[0].p[2] << "\n";
        cout << "v: " << mass[0].v[0] << ", " << mass[0].v[1] << ", " << mass[0].v[2] << "\n";
        cout << "a: " << mass[0].a[0] << ", " << mass[0].a[1] << ", " << mass[0].a[2] << "\n";
        cout << "f: " << force[0][0] << ", " << force[0][1] << ", " << force[0][2] << "\n";
        cout << "e: " << total_energy << "\n\n";

        // update time
        T += DT;
    }


}

void initialize_cube(vector<Mass> &mass, vector<Spring> &spring) {
    // create masses
    Mass temp_mass = {WEIGHT_PER_MASS,
                 {0.0, 0.0, 0.0},
                 {0.0, 0.0, 0.0},
                 {0.0, 0.0, 0.0}};
    temp_mass.p = {0.0, 0.0, INITIAL_HEIGHT}; // 0
    mass.emplace_back(temp_mass);
    temp_mass.p = {L0_SIDE, 0.0, INITIAL_HEIGHT}; // 1
    mass.emplace_back(temp_mass);
    temp_mass.p = {L0_SIDE, L0_SIDE, INITIAL_HEIGHT}; // 2
    mass.emplace_back(temp_mass);
    temp_mass.p = {0.0, L0_SIDE, INITIAL_HEIGHT}; // 3
    mass.emplace_back(temp_mass);
    temp_mass.p = {0.0, 0.0, INITIAL_HEIGHT + L0_SIDE}; // 4
    mass.emplace_back(temp_mass);
    temp_mass.p = {L0_SIDE, 0.0, INITIAL_HEIGHT + L0_SIDE}; // 5
    mass.emplace_back(temp_mass);
    temp_mass.p = {L0_SIDE, L0_SIDE, INITIAL_HEIGHT + L0_SIDE}; // 6
    mass.emplace_back(temp_mass);
    temp_mass.p = {0.0, L0_SIDE, INITIAL_HEIGHT + L0_SIDE}; // 7
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
    return sqrt(pow(b[0]-a[0],2.0) + pow(b[1]-a[1],2.0) + pow(b[2]-a[2],2.0));
}

void calculate_force(vector<Mass> &mass, vector<Spring> &spring, vector<vector<double>> &force) {
    // force due to spring
    for (int i = 0; i < NUM_OF_SPRINGS; i++) {
        // calculate force vector for spring
        double length = dist(mass[spring[i].m1].p, mass[spring[i].m2].p);
        double forceNormalized = K_SPRING * (length - spring[i].l0);
        // not sure why, but even when there is no horizontal motion,
        // length becomes != spring[i].l0 at some seemingly random point
        if (forceNormalized < 0.00000001) {
            forceNormalized = 0;
        }

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
        force[i][2] += mass[i].m * G; // note: gravity constant is negative
    }
}

void add_external_force(vector<Mass> &mass, vector<Spring> &spring, vector<vector<double>> &force) {
    // this is where i would calculate the external forces ... if there were any
}

void add_ground_force(vector<Mass> &mass, vector<Spring> &spring, vector<vector<double>> &force) {
    for (int i = 0; i < NUM_OF_MASSES; i++) {
        // if "under" ground, then apply restorative force
        if (mass[i].p[2] < 0) {
            force[i][2] += K_GROUND * abs(mass[i].p[2]);
        }
    }
}

void update_position(vector<Mass> &mass, vector<Spring> &spring, vector<vector<double>> &force) {
    for (int i = 0; i < NUM_OF_MASSES; i++) {
        // acceleration, velocity, position calculation
        for (int j = 0; j < DIMENSIONS; j++) {
            mass[i].a[j] = force[i][j] / mass[i].m;
            mass[i].v[j] += mass[i].a[j] * DT;
            mass[i].v[j] = mass[i].v[j] * V_DAMP_CONST; // velocity dampening
            mass[i].p[j] += mass[i].v[j] * DT;
        }
    }
}

double calculate_energy(vector<Mass> &mass, vector<Spring> &spring) {
    double total_energy = 0.0;

    for (int i = 0; i < NUM_OF_MASSES; i++) {
        double m = mass[i].m;
        // kinetic energy
        total_energy += 0.5 * m * pow(dist(mass[i].v, {0, 0, 0}), 2);
        // potential energy due to gravity
        total_energy += m * abs(G) * mass[i].p[2];

        // energy due to ground
        if (mass[i].p[2] < 0) {
            total_energy += 0.5 * K_GROUND * pow(mass[i].p[2], 2); // maybe change to just a reverse kinetic energy
        }

    }

    for (int i = 0; i < NUM_OF_SPRINGS; i++) {
        // potential energy due to springs
        total_energy += 0.5 * K_SPRING * pow(spring[i].l0 - dist(mass[spring[i].m1].p, mass[spring[i].m2].p), 2);
    }


    return total_energy;
}
