#pragma once

template <typename return_t, typename... params_t>
class CallbackList {
public:

    using callback_t = return_t(*)(params_t...);
    
    template<size_t initalsize_c = 16>
    CallbackList();

    void addCallback(callback_t callback);

    void callAll(params_t... params);

    size_t length() { return len; }
    size_t size() { return capacity; }

private:

    callback_t* callbacks;
    size_t capacity;
    size_t len = 0;
};

template<typename return_t, typename... params_t>
template<size_t initialsize_c>
CallbackList<return_t, params_t...>::CallbackList() : callbacks(new callback_t[initialsize_c]()), capacity(initialsize_c) {
    static_assert(initialsize_c > 0, "Initial size must be greater than 0");
}


template<typename return_t, typename... params_t>
void CallbackList<return_t, params_t...>::addCallback(callback_t callback) {
    if (len == capacity) {
        capacity *= 2;
        callback_t* newCallbacks = new callback_t[capacity]();
        for (int i = 0; i < len; i++) {
            newCallbacks[i] = callbacks[i];
        }
        delete[] callbacks;
        callbacks = newCallbacks;
    }
    callbacks[len] = callback;
    len++;
}

template<typename return_t, typename... params_t>
void CallbackList<return_t, params_t...>::callAll(params_t... params) {
    for (int i = 0; i < len; i++) {
        callbacks[i](params...);
    }
}