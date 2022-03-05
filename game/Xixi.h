#ifndef XIXI_H
#define XIXI_H
#include "Guest.h"
class Xixi: public Guset {
public:
    Xixi(int index): Guset("溪溪", index) {

    }
    virtual void Order() {
        if (tick > 1) {
            cout << "我要吃奶奶" << endl;
            ordered = true;
        }
    }
    virtual int Pay() {
        return 0;
    }
    virtual Dish *BuildDish() {
        return new NaiNai();
    }
};
#endif