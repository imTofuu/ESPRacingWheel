#pragma once

template <typename return_t, typename... params_t>
class CallbackList {
public:

    using callback_t = return_t(*)(params_t...);
    
    template<size_t initalsize_c = 16>
    CallbackList();
    ~CallbackList() { delete[] m_callbacks; }

    void addCallback(callback_t callback);

    void callAll(params_t... params);

    size_t length() { return m_len; }
    size_t size() { return m_capacity; }

private:

    callback_t* m_callbacks;
    size_t m_capacity;
    size_t m_len = 0;
};

template<typename return_t, typename... params_t>
template<size_t initialsize_c>
CallbackList<return_t, params_t...>::CallbackList() : m_callbacks(new callback_t[initialsize_c]()), m_capacity(initialsize_c) {
    static_assert(initialsize_c > 0, "Initial size must be greater than 0");
}


template<typename return_t, typename... params_t>
void CallbackList<return_t, params_t...>::addCallback(callback_t callback) {
    if (m_len == m_capacity) {
        m_capacity *= 2;
        callback_t* newCallbacks = new callback_t[m_capacity]();
        for (int i = 0; i < m_len; i++) {
            newCallbacks[i] = m_callbacks[i];
        }
        delete[] m_callbacks;
       m_callbacks = newCallbacks;
    }
    m_callbacks[m_len] = callback;
    m_len++;
}

template<typename return_t, typename... params_t>
void CallbackList<return_t, params_t...>::callAll(params_t... params) {
    for (int i = 0; i < m_len; i++) {
        m_callbacks[i](params...);
    }
}