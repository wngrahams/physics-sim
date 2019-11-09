/*
 * spring.cpp
 *
 * Implementation of Spring member functions
 */

#include "spring.hpp"

// default constructor:
Spring::Spring() {

    k = 1.0f;
    l0 = 1.0f;
    m1 = NULL;
    m2 = NULL;
}

// constructor with parameters:
Spring::Spring(const float _k, 
               const float _l0, 
               const Mass* _m1, 
               const Mass* _m2) {

    assert(_k > 0);
    k = _k;

    assert(_l0 >= 0);
    l0 = _l0;
    
    m1 = _m1;
    m2 = _m2;
}

// copy constructor:
Spring::Spring(const Spring& s) {
    k = s.get_k();
    l0 = s.get_l0();
    m1 = s.get_m1();
    m2 = s.get_m2();
}

// overloaded assignment operator:
Spring& Spring::operator=(const Spring& rhs) {
    if (this == &rhs) return *this;

    k = rhs.get_k();
    l0 = rhs.get_l0();
    m1 = rhs.get_m1();
    m2 = rhs.get_m2();

    return *this;
}

// overloaded ostream operator
std::ostream& operator<<(std::ostream& os, const Spring& s) {
    os << "\tk: " << s.get_k() << " N/m\n";
    os << "\tl0: " << s.get_l0() << " m\n";
    os << "\tconnects " << s.get_m1() << " to " << s.get_m2() << std::endl;
    os << "\tm1: " << *(s.get_m1()) << std::endl;
    os << "\tm2: " << *(s.get_m2()) << std::endl;
    return os;
}

// overloaded equality operator
bool operator==(const Spring& lhs, const Spring& rhs) {
    return ( lhs.get_k()  == rhs.get_k()  &&
             lhs.get_l0() == rhs.get_l0() &&
             lhs.get_m1() == rhs.get_m1() &&
             lhs.get_m2() == rhs.get_m2()   );
}

// overloaded inequality operator
bool operator!=(const Spring& lhs, const Spring& rhs) {
    return !(lhs == rhs);
}

