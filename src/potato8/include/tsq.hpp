#pragma once

#include <cstring>
#include <queue>
#include <mutex>
#include <semaphore>

template<typename T>
class thread_queue {
    public:
        void push(const T t)
        {
            std::lock_guard<std::mutex> guard(mtx);
            q.push(t);
        }

        void pop()
        {
            std::lock_guard<std::mutex> guard (mtx);    
            q.pop();
        }

        T& peek()
        {
            std::lock_guard<std::mutex> guard (mtx);
            return q.front();
        }

        bool empty()
        {
            bool status;
            {
                std::lock_guard<std::mutex> guard (mtx);
                status = q.empty();
            }
            // if (!status)
            //     printf("TSQ: Queue not empty\n");
            return status;
        }

    private:
        std::queue<T> q;
        std::mutex mtx;
};