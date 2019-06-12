#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <vector>
#include <functional>

namespace Algora {

template<typename... Ts>
class Observable
{
public:
    typedef typename std::function<void(Ts...)> Notification;
    Observable() { }

    Observable(const Observable &other) = default;
    Observable& operator=(const Observable &other) = default;
    Observable(Observable &&other) = default;
    Observable& operator=(Observable &&other) = default;

    void addObserver(void *id, const Notification &fun) {
        observers.push_back({id, fun});
    }

    void removeObserver(void *id) {
        auto i = observers.begin();
        while (i != observers.end()) {
            if (id == i->first) {
                i = observers.erase(i);
            } else {
                i++;
            }
        }
    }

    void notifyObservers(Ts... ts) {
       for (const auto &[_, fun] : observers) { fun(ts...); (void) _; }
    }

    bool hasObservers() const {
        return !observers.empty();
    }

    void clear() {
        observers.clear();
    }

private:
    std::vector<std::pair<void*, Notification>> observers;
};

}

#endif // OBSERVABLE_H
