//
// Created by Jarrett on 11/10/2019.
//

#ifndef __CUBE_H__
#define __CUBE_H__

#include "mass.hpp"
#include "spring.hpp"

class Cube {

public:

    Cube();  // default constructor
    ~Cube() {}  // destructor
    Cube(const Cube&);  // copy constructor

    const Mass *getM000() const {
        return m000;
    }
    void setM000(const Mass *m000) {
        Cube::m000 = m000;
    }
    const Mass *getM010() const {
        return m010;
    }
    void setM010(const Mass *m010) {
        Cube::m010 = m010;
    }
    const Mass *getM110() const {
        return m110;
    }
    void setM110(const Mass *m110) {
        Cube::m110 = m110;
    }
    const Mass *getM100() const {
        return m100;
    }
    void setM100(const Mass *m100) {
        Cube::m100 = m100;
    }
    const Mass *getM001() const {
        return m001;
    }
    void setM001(const Mass *m001) {
        Cube::m001 = m001;
    }
    const Mass *getM011() const {
        return m011;
    }
    void setM011(const Mass *m011) {
        Cube::m011 = m011;
    }
    const Mass *getM111() const {
        return m111;
    }
    void setM111(const Mass *m111) {
        Cube::m111 = m111;
    }
    const Mass *getM101() const {
        return m101;
    }
    void setM101(const Mass *m101) {
        Cube::m101 = m101;
    }
    const Spring *getS000010() const {
        return s000_010;
    }
    void setS000010(const Spring *s000010) {
        s000_010 = s000010;
    }
    const Spring *getS000110() const {
        return s000_110;
    }
    void setS000110(const Spring *s000110) {
        s000_110 = s000110;
    }
    const Spring *getS000100() const {
        return s000_100;
    }
    void setS000100(const Spring *s000100) {
        s000_100 = s000100;
    }
    const Spring *getS000001() const {
        return s000_001;
    }
    void setS000001(const Spring *s000001) {
        s000_001 = s000001;
    }
    const Spring *getS000011() const {
        return s000_011;
    }
    void setS000011(const Spring *s000011) {
        s000_011 = s000011;
    }
    const Spring *getS000111() const {
        return s000_111;
    }
    void setS000111(const Spring *s000111) {
        s000_111 = s000111;
    }
    const Spring *getS000101() const {
        return s000_101;
    }
    void setS000101(const Spring *s000101) {
        s000_101 = s000101;
    }
    const Spring *getS010110() const {
        return s010_110;
    }
    void setS010110(const Spring *s010110) {
        s010_110 = s010110;
    }
    const Spring *getS010100() const {
        return s010_100;
    }
    void setS010100(const Spring *s010100) {
        s010_100 = s010100;
    }
    const Spring *getS010001() const {
        return s010_001;
    }
    void setS010001(const Spring *s010001) {
        s010_001 = s010001;
    }
    const Spring *getS010011() const {
        return s010_011;
    }
    void setS010011(const Spring *s010011) {
        s010_011 = s010011;
    }
    const Spring *getS010111() const {
        return s010_111;
    }
    void setS010111(const Spring *s010111) {
        s010_111 = s010111;
    }
    const Spring *getS010101() const {
        return s010_101;
    }
    void setS010101(const Spring *s010101) {
        s010_101 = s010101;
    }
    const Spring *getS110100() const {
        return s110_100;
    }
    void setS110100(const Spring *s110100) {
        s110_100 = s110100;
    }
    const Spring *getS110001() const {
        return s110_001;
    }
    void setS110001(const Spring *s110001) {
        s110_001 = s110001;
    }
    const Spring *getS110011() const {
        return s110_011;
    }
    void setS110011(const Spring *s110011) {
        s110_011 = s110011;
    }
    const Spring *getS110111() const {
        return s110_111;
    }
    void setS110111(const Spring *s110111) {
        s110_111 = s110111;
    }
    const Spring *getS110101() const {
        return s110_101;
    }
    void setS110101(const Spring *s110101) {
        s110_101 = s110101;
    }
    const Spring *getS100001() const {
        return s100_001;
    }
    void setS100001(const Spring *s100001) {
        s100_001 = s100001;
    }
    const Spring *getS100011() const {
        return s100_011;
    }
    void setS100011(const Spring *s100011) {
        s100_011 = s100011;
    }
    const Spring *getS100111() const {
        return s100_111;
    }
    void setS100111(const Spring *s100111) {
        s100_111 = s100111;
    }
    const Spring *getS100101() const {
        return s100_101;
    }
    void setS100101(const Spring *s100101) {
        s100_101 = s100101;
    }
    const Spring *getS001011() const {
        return s001_011;
    }
    void setS001011(const Spring *s001011) {
        s001_011 = s001011;
    }
    const Spring *getS001111() const {
        return s001_111;
    }
    void setS001111(const Spring *s001111) {
        s001_111 = s001111;
    }
    const Spring *getS001101() const {
        return s001_101;
    }
    void setS001101(const Spring *s001101) {
        s001_101 = s001101;
    }
    const Spring *getS011111() const {
        return s011_111;
    }
    void setS011111(const Spring *s011111) {
        s011_111 = s011111;
    }
    const Spring *getS011101() const {
        return s011_101;
    }
    void setS011101(const Spring *s011101) {
        s011_101 = s011101;
    }
    const Spring *getS111101() const {
        return s111_101;
    }
    void setS111101(const Spring *s111101) {
        s111_101 = s111101;
    }

private:

    // mass objects
    // named according to (x,y,z) coordinates on unit cube
    // so m010 is the point at (0,1,0) on the unit cube
    // this is for (hopefully) more intuitive understanding
    const Mass *m000;
    const Mass *m010;
    const Mass *m110;
    const Mass *m100;
    const Mass *m001;
    const Mass *m011;
    const Mass *m111;
    const Mass *m101;
    // spring objects
    // 28 springs, connecting each mass object with each other mass object
    // named according to mass objects the spring connects
    // so s000_010 connects m000 and m010
    const Spring *s000_010;
    const Spring *s000_110;
    const Spring *s000_100;
    const Spring *s000_001;
    const Spring *s000_011;
    const Spring *s000_111;
    const Spring *s000_101;
    const Spring *s010_110;
    const Spring *s010_100;
    const Spring *s010_001;
    const Spring *s010_011;
    const Spring *s010_111;
    const Spring *s010_101;
    const Spring *s110_100;
    const Spring *s110_001;
    const Spring *s110_011;
    const Spring *s110_111;
    const Spring *s110_101;
    const Spring *s100_001;
    const Spring *s100_011;
    const Spring *s100_111;
    const Spring *s100_101;
    const Spring *s001_011;
    const Spring *s001_111;
    const Spring *s001_101;
    const Spring *s011_111;
    const Spring *s011_101;
    const Spring *s111_101;

};


#endif //PHYSICS_SIM_CUBE_HPP
