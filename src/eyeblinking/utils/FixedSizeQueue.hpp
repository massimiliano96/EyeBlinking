#include <math.h>
#include <deque>
#include <queue>

template <typename T, int capacity, typename Container = std::deque<T>>
class FixedSizeQueue : public std::queue<T, Container>
{
public:
    void push(const T& value)
    {
        if (this->size() == capacity)
        {
            this->c.pop_front();
        }
        std::queue<T, Container>::push(value);
    }

    T mean() const
    {
        T mean = 0;
        FixedSizeQueue<T, capacity> copy = *this;
        while (!copy.empty())
        {
            T front = copy.front();
            mean += front;
            copy.pop();
        }
        return mean / capacity;
    }

    T variability() const
    {
        T mean = this->mean();
        T sumSquaredDeviation = 0;
        FixedSizeQueue<T, capacity> copy = *this;
        while (!copy.empty())
        {
            T front = copy.front();
            sumSquaredDeviation += std::pow((front - mean), 2);
            copy.pop();
        }
        return sumSquaredDeviation / (capacity - 1);
    }

    void clear()
    {
        FixedSizeQueue<T, capacity> empty;
        std::swap(*this, empty);
    }
};