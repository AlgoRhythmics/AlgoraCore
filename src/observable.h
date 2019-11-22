#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <vector>
#include <functional>
#include <algorithm>
#include <set>
#include <cassert>

namespace Algora {

template<typename... Ts>
class Observable
{
public:
    typedef typename std::function<void(Ts...)> Notification;
    Observable() : notificationsInProgress(0U), delayedRemovals(false) { }

    Observable(const Observable &other) = delete;
    Observable& operator=(const Observable &other) = delete;
    Observable(Observable &&other) = default;
    Observable& operator=(Observable &&other) = default;

    void addObserver(void *id, const Notification &fun, bool delay = true) {
        if (notificationsInProgress > 0 && delay) {
            delayedAdditions.push_back({id, fun});
        } else {
            observers.push_back({id, fun});
        }
    }

    void removeObserver(void *id, bool delay = true) {
        auto i = observers.begin();
        while (i != observers.end()) {
            if (id == i->first) {
                //i = observers.erase(i);
                i->first = this;
                break;
            } else {
                i++;
            }
        }
        assert(i != observers.end()); // observer not found
        if (notificationsInProgress == 0 || !delay) {
            if (i + 1 != observers.end()) {
                *i = std::move(observers.back());
            }
            observers.pop_back();
        } else {
            delayedRemovals = true;
        }
    }

    void notifyObservers(Ts... ts) {
        // no concurrency support!
        notificationsInProgress++;
        for (const auto &[id, fun] : observers) {
            if (id != this) {
                fun(ts...);
            }
        }
        notificationsInProgress--;

        if (notificationsInProgress == 0) {
            if (!delayedAdditions.empty()) {
                std::move(delayedAdditions.begin(), delayedAdditions.end(), std::back_inserter(observers));
                delayedAdditions.clear();
            }

            if (delayedRemovals) {
                for (auto i = 0U; i < observers.size(); i++) {
                    if (observers[i].first == this) {
                        observers[i] = std::move(observers.back());
                        observers.pop_back();
                        i--;
                    }
                }
                delayedRemovals = false;
            }
        }
    }

    bool hasObservers() const {
        return !observers.empty();
    }

    void clear() {
        observers.clear();
    }

private:
    unsigned int notificationsInProgress;
    std::vector<std::pair<void*, Notification>> observers;
    std::vector<std::pair<void*, Notification>> delayedAdditions;
    bool delayedRemovals;
};

}

#endif // OBSERVABLE_H
