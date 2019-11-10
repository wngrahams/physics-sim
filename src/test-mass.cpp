/*
 * test-mass.cpp
 *
 * test program for Mass Class
 */

#include "sim.hpp"

int main() {
    Mass *m0 = new Mass();
    std::cout << *m0 << std::endl;
    std::vector<float> p1 = {1.1f, -1.1f, 7.5f};
    std::vector<float> v1 = {0.1f, 0.0f, 0.0f};
    std::vector<float> a1 = {0.0f, -A_GRAV, 0.0f};
    Mass *m1 = new Mass( 10.0f, p1, v1, a1);
    std::cout << *m1 << std::endl;
    Mass *m2 = new Mass(*m1);
    std::cout << *m2 << std::endl;
    *m2 = *m0;
    std::cout << *m2 << std::endl;
    assert(*m2 != *m1);

    std::vector<float> m1_acc = m1->get_acc();
    std::cout << "(" << m1_acc[X] << ", " << m1_acc[Y] << ", ";
    std::cout << m1_acc[Z] << ")\n";

    delete m0;
    delete m1;
    delete m2;
    
    return 0;
}
