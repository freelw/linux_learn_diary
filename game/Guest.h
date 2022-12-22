#ifndef GUEST_H
#define GUEST_H

#include <iostream>
#include <sstream>
using namespace std;
class Dish {
public:
    Dish(const std::string &name): name(name), percent(100) {

    }
    std::string What() {
        return name;
    }
    void Eat() {
        if (percent >= 1) {
            percent -= 1;
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

class YangRouPaoMo: public Dish {
    public:
    YangRouPaoMo(): Dish("羊肉泡馍") {

    }
};

class KuafuZhachuan: public Dish {
    public:
    KuafuZhachuan(): Dish("夸父炸串") {

    }
};

class NaiNai: public Dish {
    public:
    NaiNai(): Dish("奶奶") {

    }
};



class Guset {
public:
    Guset(const std::string &name, int index): dish(NULL), ordered(false), tick(0), hungry(0) {
        ostringstream oss;
        oss << name << "_" << index;
        this->name = oss.str();
    
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

    bool hasOrdered() {
        return ordered;
    }

    bool hasDish() {
        return dish != NULL;
    }

    void GetDish(Dish *dish) {
        this->dish = dish;
        ordered = 0;
        hungry = 0;
    }
    virtual void Order() = 0;
    virtual int Pay() = 0;
    virtual Dish * BuildDish() = 0;

protected:
    std::string name;
    Dish *dish;
    bool ordered;
    int tick;
    int hungry;
};

#endif