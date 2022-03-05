#ifndef MADALAN_H
#define MADALAN_H
#include "Guest.h"
class Madalan: public Guset {
public:
    Madalan(int index): Guset("马大岚", index) {

    }
    virtual void Order() {
        if (tick > 10) {
            cout << "我要吃夸父炸串" << endl;
            ordered = true;
        }
    }
    virtual int Pay() {
        return 0;
    }
    virtual Dish *BuildDish() {
        return new KuafuZhachuan();
    }
};
#endif