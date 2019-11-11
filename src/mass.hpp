/*
 * mass.hpp
 *
 * Header file for mass class
 *
 * A mass represents an object with posistion and mass, which can have a force
 * applied to give it acceleration and velocity.
 */

#ifndef __MASS_H__
#define __MASS_H__

#include <cassert>
#include <iostream>
#include <vector>

class Mass {

public:
    
    Mass();  // default constructor
    // constructor with parameters
    Mass(double, std::vector<double>, std::vector<double>, std::vector<double>);
    ~Mass() {}  // destructor
    Mass(const Mass&);  // copy constructor
    Mass& operator=(const Mass&);  // overloaded assignment operator

    // overloaded comparison operators:
    friend bool operator==(const Mass&, const Mass&);  // equality
    friend bool operator!=(const Mass&, const Mass&);

    // overloaded ostream operator
    friend std::ostream& operator<<(std::ostream&, const Mass&);

    // getter/setter functions:
    double get_mass() const { return mass; }
    void set_mass(const double m) { assert(m >= 0); mass = m; }

    std::vector<double> get_pos() const { return pos; }  // returns copy of vec
    void set_pos(const double, const double, const double);
    void set_pos(const std::vector<double> p) { pos = p; }

    std::vector<double> get_vel() const { return vel; }  // returns copy of vec
    void set_vel(const double, const double, const double);
    void set_vel(const std::vector<double> v) { vel = v; }

    std::vector<double> get_acc() const { return acc; }  // returns copy of vec
    void set_acc(const double, const double, const double);
    void set_acc(const std::vector<double> a) { acc = a; }

    // additional getters for position components for shorthand
    double x() const { return pos[0]; }
    double y() const { return pos[1]; }
    double z() const { return pos[2]; }

    // function to free unused memory in member vectors
    void shrink_vectors();

private:

    double mass;  // mass in kg

    // position, velocity, acceleration vectors:
    std::vector<double> pos;  // position in meters
    std::vector<double> vel;  // velocity in meters/second
    std::vector<double> acc;  // acceleration in meters/(second^2)

};

#endif
