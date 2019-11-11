/*
 * sim.cpp
 *
 * main driver function for the project
 */

#include <vector>
#include "sim.hpp"

#define DT 0.0001
#define NUM_OF_ITERATIONS 100000
#define NUM_OF_SPRINGS_IN_CUBE 28
#define NUM_OF_MASSES_IN_CUBE 8
#define KC 100000

int main() {

    double t = 0.0;
    double initial_rest_length = 1.0;
    double spring_constant = 10000;

    // initialize cube
    // first masses
    std::vector<double> p0 = {0,                   0,                   10};
    std::vector<double> p1 = {initial_rest_length, 0,                   10};
    std::vector<double> p2 = {initial_rest_length, initial_rest_length, 10};
    std::vector<double> p3 = {0,                   initial_rest_length, 10};
    std::vector<double> p4 = {0,                   0,                   10 - initial_rest_length};
    std::vector<double> p5 = {initial_rest_length, 0,                   10 - initial_rest_length};
    std::vector<double> p6 = {initial_rest_length, initial_rest_length, 10 - initial_rest_length};
    std::vector<double> p7 = {0,                   initial_rest_length, 10 - initial_rest_length};
    std::vector<double> v0 = {0, 0, 0};
    std::vector<double> a0 = {0, 0, 0}; // cube initially at rest
    auto *m0 = new Mass(0.1, p0, v0, a0);
    Mass *m1 = new Mass(0.1, p1, v0, a0);
    Mass *m2 = new Mass(0.1, p2, v0, a0);
    Mass *m3 = new Mass(0.1, p3, v0, a0);
    Mass *m4 = new Mass(0.1, p4, v0, a0);
    Mass *m5 = new Mass(0.1, p5, v0, a0);
    Mass *m6 = new Mass(0.1, p6, v0, a0);
    Mass *m7 = new Mass(0.1, p7, v0, a0);
    // then springs
    auto *s0 = new Spring(spring_constant, initial_rest_length, m0, m1);
    Spring *s1 = new Spring(spring_constant, initial_rest_length, m0, m2);
    Spring *s2 = new Spring(spring_constant, initial_rest_length, m0, m3);
    Spring *s3 = new Spring(spring_constant, initial_rest_length, m0, m4);
    Spring *s4 = new Spring(spring_constant, initial_rest_length, m0, m5);
    Spring *s5 = new Spring(spring_constant, initial_rest_length, m0, m6);
    Spring *s6 = new Spring(spring_constant, initial_rest_length, m0, m7);
    Spring *s7 = new Spring(spring_constant, initial_rest_length, m1, m2);
    auto *s8 = new Spring(spring_constant, initial_rest_length, m1, m3);
    Spring *s9 = new Spring(spring_constant, initial_rest_length, m1, m4);
    Spring *s10 = new Spring(spring_constant, initial_rest_length, m1, m5);
    Spring *s11 = new Spring(spring_constant, initial_rest_length, m1, m6);
    Spring *s12 = new Spring(spring_constant, initial_rest_length, m1, m7);
    Spring *s13 = new Spring(spring_constant, initial_rest_length, m2, m3);
    Spring *s14 = new Spring(spring_constant, initial_rest_length, m2, m4);
    Spring *s15 = new Spring(spring_constant, initial_rest_length, m2, m5);
    Spring *s16 = new Spring(spring_constant, initial_rest_length, m2, m6);
    Spring *s17 = new Spring(spring_constant, initial_rest_length, m2, m7);
    Spring *s18 = new Spring(spring_constant, initial_rest_length, m3, m4);
    auto *s19 = new Spring(spring_constant, initial_rest_length, m3, m5);
    auto *s20 = new Spring(spring_constant, initial_rest_length, m3, m6);
    Spring *s21 = new Spring(spring_constant, initial_rest_length, m3, m7);
    Spring *s22 = new Spring(spring_constant, initial_rest_length, m4, m5);
    Spring *s23 = new Spring(spring_constant, initial_rest_length, m4, m6);
    Spring *s24 = new Spring(spring_constant, initial_rest_length, m4, m7);
    Spring *s25 = new Spring(spring_constant, initial_rest_length, m5, m6);
    Spring *s26 = new Spring(spring_constant, initial_rest_length, m5, m7);
    Spring *s27 = new Spring(spring_constant, initial_rest_length, m6, m7);

    std::vector<Mass> masses;
    masses.push_back(*m0);
    masses.push_back(*m1);
    masses.push_back(*m2);
    masses.push_back(*m3);
    masses.push_back(*m4);
    masses.push_back(*m5);
    masses.push_back(*m6);
    masses.push_back(*m7);
    std::vector<Spring> springs;
    springs.push_back(*s0);
    springs.push_back(*s1);
    springs.push_back(*s2);
    springs.push_back(*s3);
    springs.push_back(*s4);
    springs.push_back(*s5);
    springs.push_back(*s6);
    springs.push_back(*s7);
    springs.push_back(*s8);
    springs.push_back(*s9);
    springs.push_back(*s10);
    springs.push_back(*s11);
    springs.push_back(*s12);
    springs.push_back(*s13);
    springs.push_back(*s14);
    springs.push_back(*s15);
    springs.push_back(*s16);
    springs.push_back(*s17);
    springs.push_back(*s18);
    springs.push_back(*s19);
    springs.push_back(*s20);
    springs.push_back(*s21);
    springs.push_back(*s22);
    springs.push_back(*s23);
    springs.push_back(*s24);
    springs.push_back(*s25);
    springs.push_back(*s26);
    springs.push_back(*s27);

    // create force vectors for masses
    std::vector<double> f_m0;
    std::vector<double> f_m1;
    std::vector<double> f_m2;
    std::vector<double> f_m3;
    std::vector<double> f_m4;
    std::vector<double> f_m5;
    std::vector<double> f_m6;
    std::vector<double> f_m7;
    std::vector<std::vector<double>> f_on_m = {f_m0, f_m1, f_m2, f_m3, f_m4, f_m5, f_m6, f_m7};



    // simulation loop
    for (int iteration = 0; iteration < NUM_OF_ITERATIONS; iteration++) {
        // sum forces
        std::vector<double> f_s0 {};


    }




    delete m0;
    delete m1;
    delete m2;
    delete m3;
    delete m4;
    delete m5;
    delete m6;
    delete m7;
    delete s0;
    delete s1;
    delete s2;
    delete s3;
    delete s4;
    delete s5;
    delete s6;
    delete s7;
    delete s8;
    delete s9;
    delete s10;
    delete s11;
    delete s12;
    delete s13;
    delete s14;
    delete s15;
    delete s16;
    delete s17;
    delete s18;
    delete s19;
    delete s20;
    delete s21;
    delete s22;
    delete s23;
    delete s24;
    delete s25;
    delete s26;
    delete s27;


    return 0;
}