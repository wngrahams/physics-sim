#include "cube.hpp"

#define NUM_OF_MASSES 8
#define NUM_OF_SPRINGS 28

// default constructor
Cube::Cube() {
    for (int i = 0; i < NUM_OF_MASSES; i++) {
        masses.emplace_back(Mass());
    }
    for (int i = 0; i < NUM_OF_SPRINGS; i++) {
        for (int j = i + 1; j < NUM_OF_SPRINGS; j++) {
            springs.emplace_back(Spring(1.0, 1.0, &masses[i], &masses[j]));
        }
    }
}

Cube::Cube(double x_origin, double y_origin, double z_origin, double rest_length, double k) {
    //  index: xyz
    //  0: 000
    //  1: 010
    //  2: 110
    //  3: 100
    //  4: 001
    //  5: 011
    //  6: 111
    //  7: 101
    Mass *m = new Mass();
    m->set_pos(x_origin, y_origin, z_origin);
    masses.emplace_back(*m);
    m->set_pos(x_origin, y_origin + rest_length, z_origin);
    masses.emplace_back(*m);
    m->set_pos(x_origin + rest_length, y_origin + rest_length, z_origin);
    masses.emplace_back(*m);
    m->set_pos(x_origin + rest_length, y_origin, z_origin);
    masses.emplace_back(*m);
    m->set_pos(x_origin, y_origin, z_origin + rest_length);
    masses.emplace_back(*m);
    m->set_pos(x_origin, y_origin + rest_length, z_origin + rest_length);
    masses.emplace_back(*m);
    m->set_pos(x_origin + rest_length, y_origin + rest_length, z_origin + rest_length);
    masses.emplace_back(*m);
    m->set_pos(x_origin + rest_length, y_origin, z_origin + rest_length);
    masses.emplace_back(*m);

    for (int i = 0; i < NUM_OF_SPRINGS; i++) {
        for (int j = i + 1; j < NUM_OF_SPRINGS; j++) {
            springs.emplace_back(Spring(k, rest_length, &masses[i], &masses[j]));
        }
    }

    delete m;
}

// copy constructor
Cube::Cube(const Cube& c) {
}

