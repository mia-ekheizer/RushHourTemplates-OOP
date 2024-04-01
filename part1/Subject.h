#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"
#include "OOP5EventException.h"
#include <vector>

using namespace std;

template<typename T>
class Subject {
private:
    vector<Observer<T>*> observers;

    bool isObserverKnown(Observer<T>& anObserver) {
        for (int i = 0; i < observers.size(); i++) {
            if (observers[i] == &anObserver) {
                return true;
            }
        }
        return false;
    }

public:
    Subject() = default;

    virtual ~Subject() = default;

    void notify(const T& message) {
        for (int i= 0; i < observers.size(); i++) {
            observers[i]->handleEvent(message);
        }
    }

    void addObserver(Observer<T>& anObserver) {
        if (this->isObserverKnown(anObserver))
        {
            throw ObserverAlreadyKnownToSubject();
        }
        else {
            observers.push_back(&anObserver);
        }
    }

    void removeObserver(Observer<T>& anObserver) {
        for (int i= 0; i < observers.size(); i++) {
            if (observers[i] == &anObserver) {
                typename vector<Observer<T>*>::iterator toRemove = observers.begin() + i;
                observers.erase(toRemove);
                return;
            }
        }
        throw ObserverUnknownToSubject();
    }

    Subject<T>& operator+=(Observer<T>& anObserver) {
        this->addObserver(anObserver);
        return *this;
    }

    Subject<T>& operator-=(Observer<T>& anObserver) {
        this->removeObserver(anObserver);
        return *this;
    }

    Subject<T>& operator()(const T& message) {
        this->notify(message);
        return *this;
    }
};

#endif // SUBJECT_H