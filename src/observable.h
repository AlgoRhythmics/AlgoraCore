#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <vector>
#include <functional>
#include <algorithm>
#include <set>

namespace Algora {

template<typename... Ts>
class Observable
{
public:
    typedef typename std::function<void(Ts...)> Notification;
    Observable() : notificationInProgress(false) { }

    Observable(const Observable &other) = delete;
    Observable& operator=(const Observable &other) = delete;
    Observable(Observable &&other) = default;
    Observable& operator=(Observable &&other) = default;

    void addObserver(void *id, const Notification &fun, bool delay = true) {
        if (notificationInProgress && delay) {
            delayedAdditions.push_back({id, fun});
        } else {
            observers.push_back({id, fun});
        }
    }

    void removeObserver(void *id, bool delay = true) {
        if (notificationInProgress && delay) {
            delayedRemovals.insert(id);
        } else {
            auto i = observers.begin();
            while (i != observers.end()) {
                if (id == i->first) {
                    i = observers.erase(i);
                    break;
                } else {
                    i++;
                }
            }
        }
    }

    void notifyObservers(Ts... ts) {
        // no concurrency support!
        notificationInProgress = true;
        for (const auto &[_, fun] : observers) { fun(ts...); (void) _; }
        notificationInProgress = false;

        if (!delayedAdditions.empty()) {
            std::move(delayedAdditions.begin(), delayedAdditions.end(), std::back_inserter(observers));
            delayedAdditions.clear();
        }

        if (!delayedRemovals.empty()) {
            auto n = delayedRemovals.size();
            for (auto i = observers.begin(), last = observers.end(); i != last && n > 0; ) {
                if (delayedRemovals.find(i->first) != delayedRemovals.end()) {
                    i = observers.erase(i);
                    n--;
                } else {
                    i++;
                }
            }
            delayedRemovals.clear();
        }
    }

    bool hasObservers() const {
        return !observers.empty();
    }

    void clear() {
        observers.clear();
    }

private:
    bool notificationInProgress;
    std::vector<std::pair<void*, Notification>> observers;
    std::vector<std::pair<void*, Notification>> delayedAdditions;
    std::set<void*> delayedRemovals;
};

}

#endif // OBSERVABLE_H
