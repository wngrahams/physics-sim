//
// Created by Jarrett on 11/10/2019.
//

#ifndef __CUBE_H__
#define __CUBE_H__

#include <cassert>
#include "mass.hpp"
#include "spring.hpp"

class Cube {

public:

    // constructors
    Cube();  // default constructor
    Cube(double, double, double, double, double);  // constructor that creates basic cube with specified parameters
    ~Cube() {}  // destructor
    Cube(const Cube&);  // copy constructor

    // getters and setters
    std::vector<Mass> get_masses() { return masses; };
    void set_masses(std::vector<Mass> _masses) { assert(_masses.size() == 8); masses = _masses; };
    std::vector<Spring> get_springs() { return springs; };
    void set_springs(std::vector<Spring> _springs) { springs = _springs; };

    std::vector<Mass> masses;
    std::vector<Spring> springs;

private:

    //  index: xyz
    //  0: 000
    //  1: 010
    //  2: 110
    //  3: 100
    //  4: 001
    //  5: 011
    //  6: 111
    //  7: 101

    //  index: xyz_xyz
    //  0: 000_010
    //  1: 000_110
    //  2: 000_100
    //  3: 000_001
    //  4: 000_011
    //  5: 000_111
    //  6: 000_101
    //  7: 010_110
    //  8: 010_100
    //  9: 010_001
    //  10: 010_011
    //  11: 010_111
    //  12: 010_101
    //  13: 110_100
    //  14: 110_001
    //  15: 110_011
    //  16: 110_111
    //  17: 110_101
    //  18: 100_001
    //  19: 100_011
    //  20: 100_111
    //  21: 100_101
    //  22: 001_011
    //  23: 001_111
    //  24: 001_101
    //  25: 011_111
    //  26: 011_101
    //  27: 111_101

};


#endif //PHYSICS_SIM_CUBE_HPP
