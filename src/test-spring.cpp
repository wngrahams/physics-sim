/*
 * test-spring.cpp
 *
 * program to test String class implementation
 */

#include "sim.hpp"

int main() {
    Mass *m0 = new Mass();
    std::cout << *m0 << std::endl;
    std::vector<double> p1 = {1.1f, -1.1f, 7.5f};
    std::vector<double> v1 = {0.1f, 0.0f, 0.0f};
    std::vector<double> a1 = {0.0f, -A_GRAV, 0.0f};
    Mass *m1 = new Mass(10.0f, p1, v1, a1);
    std::cout << *m1 << std::endl;
    Mass *m2 = new Mass(*m1);
    std::cout << *m2 << std::endl;
    *m2 = *m0;
    std::cout << *m2 << std::endl;
    assert(*m2 != *m1);

    std::vector<double> m1_acc = m1->get_acc();
    std::cout << "(" << m1_acc[X] << ", " << m1_acc[Y] << ", ";
    std::cout << m1_acc[Z] << ")\n";

    Spring *s0 = new Spring();
    s0->set_k(2.4f);
    s0->set_l0(1.1f);
    s0->set_m1(m0);
    s0->set_m2(m1);
    std::cout << *s0 << std::endl;

    Spring *s1 = new Spring();
    s1->set_k(2.4f);
    s1->set_l0(1.1f);
    s1->set_m1(m1);
    s1->set_m2(m0);
    std::cout << *s1 << std::endl;

    assert(*s0 == *s1);

    s1->set_m1(m2);
    std::cout << *s1 << std::endl;


    delete s0;
    delete s1;
    delete m0;
    delete m1;
    delete m2;
    
    return 0;
}
