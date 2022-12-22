#ifndef DANTA_H
#define DANTA_H
#include "Guest.h"
class Danta: public Guset {
public:
    Danta(): Guset("蛋挞") {

    }
    virtual void Order() {
        if (tick > 5 && !ordered) {
            cout << "我要吃狗粮" << endl;
            ordered = true;
        }
    }
    virtual int Pay() {
        return 0;
    }
};
#endif