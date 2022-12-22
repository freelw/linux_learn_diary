#include <unistd.h>
#include <iostream>

#include "Guest.h"
#include "Danta.h"

#define GAME_TICK 1000000
#define MAX_GUEST 50
using namespace std;

int loop_cnt = 0;
bool should_desc = false;

void validate_desc();
class Guset;
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
                guests[i]->GetDish(new GouLiang());
            }
        }
    }
}

Guset* rand_come() {
    return new Danta();
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
    if (loop_cnt % 50) {
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
    
    cout << endl;
}

void guest_action() {
    for (int i = 0; i < MAX_GUEST; ++ i) {
        if (guests[i]) {
            guests[i]->IncreaseTick();
        }
    }
    for (int i = 0; i < MAX_GUEST; ++ i) {
        if (guests[i]) {
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