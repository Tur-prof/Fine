#include <iostream>
#include <mutex>

struct Node
{
    int _value;
    Node* _next;
    std::mutex* _node_mutex;
    Node(int value) : _value(value), _next(nullptr), _node_mutex(new std::mutex) {}
};

class FineGrainedQueue
{
public:
    Node* _head;
    std::mutex* _queue_mutex;
    FineGrainedQueue() : _head(nullptr), _queue_mutex(new std::mutex) {}

    void push(int value)
    {
        Node* new_node = new Node(value);
        if (_head == nullptr)
        {
            _head = new_node;
            return;
        }
        Node* current = _head;
        while (current->_next != nullptr)
        {
            current = current->_next;
        }
        current->_next = new_node;
    }

    void print()
    {
        Node* current = _head;
        while (current != nullptr)
        {
            std::cout << current->_value << " ";
            current = current->_next;
        }
        std::cout << std::endl;
    }

    void insertIntoMiddle(int value, int pos)
    {
        Node* new_node = new Node(value);

        _queue_mutex->lock();

        Node* current = _head;
        current->_node_mutex->lock();
        _queue_mutex->unlock();

        int count = 0;
        while (count < pos - 2 && current->_next)
        {
            Node* previous = current;
            current->_next->_node_mutex->lock();
            current = current->_next;
            previous->_node_mutex->unlock();
            count++;
        }

        Node* next_node = current->_next;
        current->_next = new_node;
        current->_node_mutex->unlock();
        new_node->_next = next_node;

        this->print();
        std::cout << std::endl;
    }
};

int main()
{
    FineGrainedQueue _queue;

    for (int i = 1; i <= 10; i++)
    {
        _queue.push(i);
    }
    _queue.print();
    _queue.insertIntoMiddle(-100, 3);
    _queue.insertIntoMiddle(-300, 12);
    _queue.insertIntoMiddle(-500, 19);

    std::cout << std::endl;

    return 0;
}