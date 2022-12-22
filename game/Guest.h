#ifndef GUEST_H
#define GUEST_H

#include <iostream>
using namespace std;
class Dish {
public:
    Dish(const std::string &name): name(name), percent(100) {

    }
    std::string What() {
        return name;
    }
    void Eat() {
        if (percent >= 10) {
            percent -= 10;
        }
    }
    int Left() {
        return percent;
    }
private:
    std::string name;
    int percent;
};

class GouLiang: public Dish {

public:
    GouLiang(): Dish("狗粮") {

    }
};

class Guset {
public:
    Guset(const std::string &name): name(name), dish(NULL), ordered(false), tick(0), hungry(0) {
    
    }
    std::string Who() {
        return name;
    }
    void Whosaid() {
        cout << name << " 说 ";
    }
    void Eating() {
        if (!ordered && !dish) {
            cout << name << " 还没有点菜" << endl;
            return ;
        }
        if (dish) {
            if (dish->Left() == 0) {
                cout << name << "酒足饭饱" << endl;
            } else {
                dish->Eat();
                cout << name << "说:" << dish->What() << "真好吃，还有" << dish->Left() << "% 就吃完了" << endl;
            }
        } else {
            cout << name << "还没有吃上饭" << endl;
        }
    }

    void IncreaseTick() {
        tick ++;
        if (!dish && ordered) {
            hungry ++;
        } 
    }

    int GetHungry() {
        return hungry;
    }

    void GetDish(Dish *dish) {
        this->dish = dish;
        ordered = 0;
        hungry = 0;
    }
    virtual void Order() = 0;
    virtual int Pay() = 0;

protected:
    std::string name;
    Dish *dish;
    bool ordered;
    int tick;
    int hungry;
};

#endif