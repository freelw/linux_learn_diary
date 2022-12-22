#ifndef LIDAWANG_H
#define LIDAWANG_H
#include "Guest.h"
class Lidawang: public Guset {
public:
    Lidawang(int index): Guset("李大望", index) {

    }
    virtual void Order() {
        if (tick > 10) {
            cout << "我要吃羊肉泡馍" << endl;
            ordered = true;
        }
    }
    virtual int Pay() {
        return 0;
    }
    virtual Dish *BuildDish() {
        return new YangRouPaoMo();
    }
};
#endif