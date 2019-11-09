/*
 * mass.hpp
 *
 * Header file for mass object
 *
 * A mass represents an object with posistion and mass, which can have a force
 * applied to give it acceleration and velocity.
 */

#ifndef __MASS_H__
#define __MASS_H__

#include <vector>

class Mass {

public:
    
    Mass();  // default constructor
    // constructor with parameters:
    Mass(float, std::vector<float>, std::vector<float>, std::vector<float>);
    ~Mass();  // destructor
    Mass(const Mass&);  // copy constructor
    Mass& operator=(const Mass&);  // overloaded assignment operator

    // overloaded comparison operators:
    friend bool operator==(const Mass&, const Mass&);  // equality
    friend bool operator!=(const Mass&, const Mass&);

    // getter/setter functions:
    float get_mass() const { return mass; }
    void set_mass(const float m) { mass = m; }

    std::vector<float> get_pos() const { return pos; }  // returns copy of vec
    void set_pos(const float, const float, const float);
    void set_pos(const std::vector<float> p) { pos = p; } 

    std::vector<float> get_vel() const { return vel; }  // returns copy of vec
    void set_vel(const float, const float, const float);
    void set_vel(const std::vector<float> v) { vel = v; }

    std::vector<float> get_acc() const { return acc; }  // returns copy of vec
    void set_acc(const float, const float, const float);
    void set_acc(const std::vector<float> a) { acc = a; } 

    // additional getters for position components for shorthand
    float x() const { return pos[0]; }
    float y() const { return pos[1]; }
    float z() const { return pos[2]; }

    // function to free unused memory in member vectors
    void shrink_vectors() const;

private:

    float mass;  // mass in kg

    // position, velocity, acceleration vectors:
    std::vector<float> pos;  // position in meters
    std::vector<float> vel;  // velocity in meters/second
    std::vector<float> acc;  // acceleration in meters/(second^2)

};

#endif
