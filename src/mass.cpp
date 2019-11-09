/*
 * mass.cpp
 *
 * Implementation of Mass member functions
 */

#include "mass.hpp"

// default constructor:
Mass::Mass() {

    mass = 0.0f;
    pos = {0.0f, 0.0f, 0.0f};
    vel = {0.0f, 0.0f, 0.0f};
    acc = {0.0f, 0.0f, 0.0f};

    shrink_vectors();
}

// constructor with parameters:
Mass::Mass(float m, std::vector<float> p, 
                    std::vector<float> v, 
                    std::vector<float> a) {
    
    mass = m;
    pos = p;
    vel = v;
    acc = a;

    shrink_vectors();
}

// copy constructor:
Mass::Mass(const Mass& M) {
    mass = M.get_mass();
    pos = M.get_pos();
    vel = M.get_vel();
    acc = M.get_acc();

    shrink_vectors();
}

// overloaded assignment operator:
Mass& Mass::operator=(const Mass& rhs) {
    if (this == &rhs) return *this;

    mass = rhs.get_mass();
    pos = rhs.get_pos();
    vel = rhs.get_vel();
    acc = rhs.get_acc();

    shrink_vectors();

    return *this;
}

// overloaded ostream operator
std::ostream& operator<<(std::ostream& os, const Mass& m) {
    os << "\tmass: " << m.get_mass() << " kg\n";
    os << "\tpos: " << m.x() << " " << m.y() << " " << m.z() << " (m)\n";
    os << "\tvel: " << m.get_vel()[0] << " ";
    os << m.get_vel()[1] << " " << m.get_vel()[2] << " (m/s)\n";
    os << "\tacc: " << m.get_acc()[0] << " ";
    os << m.get_acc()[1] << " " << m.get_acc()[2] << " (m/(s^2))\n";
    return os;
}

/* overloaded equality operator
 * 
 * just compares mass for equality right now
 * TODO: decide if more needs to be compared or if this is sufficient
 */
bool operator==(const Mass& lhs, const Mass& rhs) {
    return (lhs.get_mass() == rhs.get_mass());
}

// overloaded inequality operator
bool operator!=(const Mass& lhs, const Mass& rhs) {
    return !(lhs == rhs);
}

// set position with 3 floats:
void Mass::set_pos(float x, float y, float z) {
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

// set velocity with 3 floats:
void Mass::set_vel(float x, float y, float z) {
    vel[0] = x;
    vel[1] = y;
    vel[2] = z;
}

// set acceleration with 3 floats:
void Mass::set_acc(float x, float y, float z) {
    acc[0] = x;
    acc[1] = y;
    acc[2] = z;
}

// free unused memory in member vectors:
void Mass::shrink_vectors() {
    pos.shrink_to_fit();
    vel.shrink_to_fit();
    acc.shrink_to_fit();
}

