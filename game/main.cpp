#include <cstdlib>
#include <unistd.h>
#include <iostream>

#include "Guest.h"
#include "Danta.h"
#include "Lidawang.h"
#include "Xixi.h"
#include "Madalan.h"

#define GAME_TICK 1000000
#define MAX_GUEST 10
using namespace std;

int loop_cnt = 0;
bool should_desc = false;

void validate_desc();
Guset* guests[MAX_GUEST];

void print_now() {
    if (loop_cnt % 10 == 0) {
        time_t now = time(0);
        char* dt = ctime(&now);
        cout << "本地日期和时间：" << dt << endl;
    }
}

void serv() {
    for (int i = 0; i < MAX_GUEST; ++ i) {
        if (guests[i]) {
            if (guests[i]->GetHungry() > 5) {
                guests[i]->GetDish(guests[i]->BuildDish());
            }
        }
    }
}

Guset* rand_come() {
    int index = rand() % 4;
    switch (index) {
    case 0:
        return new Danta(loop_cnt);
    case 1:
        return new Madalan(loop_cnt);
    case 2:
        return new Xixi(loop_cnt);
    default:
        return new Lidawang(loop_cnt);
    }
}

void validate_desc() {
    should_desc = true;
}

void update_desc() {
    if (loop_cnt % 10) {
        return ;
    }
    validate_desc();
}

void guest_come() {
    if (loop_cnt % 10) {
        return ;
    }
    for (int i = 0; i < MAX_GUEST; ++ i) {
        if (NULL == guests[i]){
            guests[i] = rand_come();
            cout << guests[i]->Who() << "来了" << endl;
            validate_desc();
            break;
        }
    }
}

void desc() {
    if (!should_desc) {
        return ;
    }
    cout << "小店里面的客人有 ： ";
    for (int i = 0; i < MAX_GUEST; ++ i) {
        if (guests[i]) {
            cout << guests[i]->Who() << " ";
        }
    }
    cout << endl;
}

void guest_action() {
    for (int i = 0; i < MAX_GUEST; ++ i) {
        if (guests[i]) {
            guests[i]->IncreaseTick();
        }
    }
    for (int i = 0; i < MAX_GUEST; ++ i) {
        if (guests[i] && !guests[i]->hasOrdered() && !guests[i]->hasDish()) {
            guests[i]->Order();
        }
    }
    for (int i = 0; i < MAX_GUEST; ++ i) {
        if (guests[i]) {
            if (guests[i]->GetHungry() > 2) {
                validate_desc();
            }
        }
    }
    for (int i = 0; i < MAX_GUEST; ++ i) {
        if (guests[i]) {
            guests[i]->Eating();
        }
    }
}

void main_loop() {    
    while (1) {
        update_desc();
        print_now();
        guest_come();
        guest_action();
        serv();
        usleep(GAME_TICK);
        loop_cnt ++;
        desc();
        should_desc = false;
    }
}

void init() {
    srand((unsigned)time(NULL));
    for (int i = 0; i < MAX_GUEST; ++ i) {
        guests[i] = NULL;
    }
}

int main() {
    cout << "欢迎" << endl;
    init();
    main_loop();
    return 0;
}