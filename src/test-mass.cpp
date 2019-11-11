/*
 * test-mass.cpp
 *
 * test program for Mass Class
 */

#include "sim.hpp"

int main() {
    Mass *m0 = new Mass();
    std::cout << *m0 << std::endl;
    std::vector<double> p1 = {1.1, -1.1, 7.5};
    std::vector<double> v1 = {0.1, 0.0, 0.0};
    std::vector<double> a1 = {0.0, -A_GRAV, 0.0};
    Mass *m1 = new Mass( 10.0, p1, v1, a1);
    std::cout << *m1 << std::endl;
    Mass *m2 = new Mass(*m1);
    std::cout << *m2 << std::endl;
    *m2 = *m0;
    std::cout << *m2 << std::endl;
    assert(*m2 != *m1);

    std::vector<double> m1_acc = m1->get_acc();
    std::cout << "(" << m1_acc[X] << ", " << m1_acc[Y] << ", ";
    std::cout << m1_acc[Z] << ")\n";

    delete m0;
    delete m1;
    delete m2;
    
    return 0;
}
