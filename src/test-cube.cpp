/*
 * test-cube.cpp
 *
 * test program for Cube Class
 */

#include "sim.hpp"

int main() {
    std::vector<double> p1 = {1.1, -1.1, 7.5};
    std::vector<double> v1 = {0.1, 0.0, 0.0};
    std::vector<double> a1 = {0.0, 0.0, 0.0};
    Mass *m0 = new Mass( 10.0, p1, v1, a1);
    Mass *m1 = new Mass( 10.0, p1, v1, a1);
    Mass *m2 = new Mass( 10.0, p1, v1, a1);
    Mass *m3 = new Mass( 10.0, p1, v1, a1);
    Mass *m4 = new Mass( 10.0, p1, v1, a1);
    Mass *m5 = new Mass( 10.0, p1, v1, a1);
    Mass *m6 = new Mass( 10.0, p1, v1, a1);
    Mass *m7 = new Mass( 10.0, p1, v1, a1);
    Cube *c0 = new Cube();

    std::vector<Mass> mv = c0->get_masses();
    for (int i = 0; i < mv.size(); i++) {
        std::cout << mv[i];
    } // should give empty mass objects

    std::vector<Mass> mvf;
    mvf.emplace_back(*m0);
    mvf.emplace_back(*m1);
    mvf.emplace_back(*m2);
    mvf.emplace_back(*m3);
    mvf.emplace_back(*m4);
    mvf.emplace_back(*m5);
    mvf.emplace_back(*m6);
    mvf.emplace_back(*m7);
    c0->masses = mvf;
    mv = c0->masses;
    for (int i = 0; i < mvf.size(); i++) {
        std::cout << c0->masses[i];
    }

    return 0;
}
