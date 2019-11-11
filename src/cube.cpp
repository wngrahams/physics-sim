//
// Created by Jarrett on 11/10/2019.
//

#include "cube.hpp"

// default constructor
Cube::Cube() {
    m000 = nullptr;
    m010 = nullptr;
    m110 = nullptr;
    m100 = nullptr;
    m001 = nullptr;
    m011 = nullptr;
    m111 = nullptr;
    m101 = nullptr;

    s000_010 = nullptr;
    s000_110 = nullptr;
    s000_100 = nullptr;
    s000_001 = nullptr;
    s000_011 = nullptr;
    s000_111 = nullptr;
    s000_101 = nullptr;
    s010_110 = nullptr;
    s010_100 = nullptr;
    s010_001 = nullptr;
    s010_011 = nullptr;
    s010_111 = nullptr;
    s010_101 = nullptr;
    s110_100 = nullptr;
    s110_001 = nullptr;
    s110_011 = nullptr;
    s110_111 = nullptr;
    s110_101 = nullptr;
    s100_001 = nullptr;
    s100_011 = nullptr;
    s100_111 = nullptr;
    s100_101 = nullptr;
    s001_011 = nullptr;
    s001_111 = nullptr;
    s001_101 = nullptr;
    s011_111 = nullptr;
    s011_101 = nullptr;
    s111_101 = nullptr;
}

// copy constructor
Cube::Cube(const Cube& c) {
    m000 = c.getM000();
    m010 = c.getM010();
    m010 = c.getM010();
    m100 = c.getM100();
    m001 = c.getM001();
    m011 = c.getM011();
    m111 = c.getM111();
    m101 = c.getM101();

    s000_010 = nullptr;
    s000_110 = nullptr;
    s000_100 = nullptr;
    s000_001 = nullptr;
    s000_011 = nullptr;
    s000_111 = nullptr;
    s000_101 = nullptr;
    s010_110 = nullptr;
    s010_100 = nullptr;
    s010_001 = nullptr;
    s010_011 = nullptr;
    s010_111 = nullptr;
    s010_101 = nullptr;
    s110_100 = nullptr;
    s110_001 = nullptr;
    s110_011 = nullptr;
    s110_111 = nullptr;
    s110_101 = nullptr;
    s100_001 = nullptr;
    s100_011 = nullptr;
    s100_111 = nullptr;
    s100_101 = nullptr;
    s001_011 = nullptr;
    s001_111 = nullptr;
    s001_101 = nullptr;
    s011_111 = nullptr;
    s011_101 = nullptr;
    s111_101 = nullptr;
}

