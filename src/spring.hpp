/*
 * spring.hpp
 *
 * Header file for spring class
 *
 * A spring represents a connection between two mass objects with rest length
 * l0 (m) and spring constant k (N/m)
 */

#ifndef __SPRING_H__
#define __SPRING_H__

#include <cassert>
#include <iostream>
#include <vector>

#include "mass.hpp"

class Spring {

public:
    
    Spring();  // default constructor
    // constructor with parameters:
    Spring(const double, const double, const Mass*, const Mass*);
    ~Spring() { m1 = NULL; m2 = NULL; }  // destructor
    Spring(const Spring&);  // copy constructor
    Spring& operator=(const Spring&);  // overloaded assignment operator

    // overloaded comparison operators:
    friend bool operator==(const Spring&, const Spring&);  // equality
    friend bool operator!=(const Spring&, const Spring&);  // inequality

    // overloaded ostream operator
    friend std::ostream& operator<<(std::ostream&, const Spring&);

    // getter/setter functions:
    double get_k() const { return k; }
    void set_k(const double _k) { k = _k; }

    double get_l0() const { return l0; }
    void set_l0(const double _l0) { l0 = _l0; }

    const Mass* get_m1() const { return m1; }
    void set_m1(const Mass* _m1) { assert(_m1 != m2); m1 = _m1; }

    const Mass* get_m2() const { return m2; }
    void set_m2(const Mass* _m2) { assert(_m2 != m1); m2 = _m2; }

private:

    double k;  // spring constant in (N/m)
    double l0;  // rest length in (m)
    const Mass *m1;  // pointer to Mass object that spring is connected to
    const Mass *m2;  // pointer to Mass object that spring is connected to

};

#endif
