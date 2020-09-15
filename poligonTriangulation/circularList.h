#pragma once

template <class T>
class CircularList
{
    int _size;
public:
    struct Node
    {
        T data;
        Node* next;
        Node* prev;
        int id;
    };

    Node* head;
    CircularList() : head(nullptr), _size(0) {};
    inline int size() { return _size; };

    void push(T newElement)
    {
        Node* node = new Node({newElement, nullptr, nullptr});
        if (!head)
        {
            node->next = node;
            node->prev = node;
            head = node;
        }
        else
        {
            node->next = head;
            node->prev = head->prev;
            head->prev->next = node;
            head->prev = node;
        }
        _size++;
    };

    void push_after(T* refNode, T newElement)
    {
        if (!refNode || !_size) return;

        Node* node = new Node({newElement, nullptr, nullptr});
        newElement->next = refNode->next;
        newElement->prev = refNode;
        refNode->next->prev = newElement;
        refNode->next = newElement;
        _size++;
    };

    void remove(Node* element)
    {
        if (_size == 1)
        {
            free(element);
            head = nullptr;
        }
        else
        {
            if(head == element) head = element->next;
            element->next->prev = element->prev;
            element->prev->next = element->next;
            free(element);
        }
        _size--;
    };
};
