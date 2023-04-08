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
};