#ifndef DANTA_H
#define DANTA_H
#include "Guest.h"
class Danta: public Guset {
public:
    Danta(int index): Guset("蛋挞", index) {

    }
    virtual void Order() {
        if (tick > 5) {
            cout << "我要吃狗粮" << endl;
            ordered = true;
        }
    }
    virtual int Pay() {
        return 0;
    }
    virtual Dish *BuildDish() {
        return new GouLiang();
    }
};
#endif