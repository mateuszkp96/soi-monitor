//
// Created by mateusz on 12/9/19.
//

#ifndef MONITOR_MONITOR_H
#define MONITOR_MONITOR_H

#include <semaphore.h>
#include <list>
#include <vector>
#include <memory>
#include <string>

class Semaphore {
public:

    Semaphore(int value) {
        if (sem_init(&sem, 0, value) != 0)
            throw "sem_init: failed";
    }

    ~Semaphore() {
        sem_destroy(&sem);
    }

    void p() {
        if (sem_wait(&sem) != 0)
            throw "sem_wait: failed";
    }

    void v() {
        if (sem_post(&sem) != 0)
            throw "sem_post: failed";
    }

private:
    sem_t sem;
};

class Condition {
    friend class Monitor;

public:
    Condition() : w(0) {
        waitingCount = 0;
    }

    void wait() {
        w.p();
    }

    bool signal() {
        if (waitingCount) {
            --waitingCount;
            w.v();
            return true;
        }//if
        else
            return false;
    }

private:
    Semaphore w;
    int waitingCount; //liczba oczekujacych watkow
};


class Monitor {
public:
    Monitor() : s(1) {}

    void enter() {
        s.p();
    }

    void leave() {
        s.v();
    }

    void wait(Condition &cond) {
        ++cond.waitingCount;
        leave();
        cond.wait();
    }

    void signal(Condition &cond) {
        if (cond.signal())
            enter();
    }


private:
    Semaphore s;
};

#endif //MONITOR_MONITOR_H
