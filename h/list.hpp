//
// Created by os on 8/25/26.
//

#ifndef PROJECT_BASE_V1_1_LIST_HPP
#define PROJECT_BASE_V1_1_LIST_HPP
#include "print.h"

template <typename T>
class List {
private:
    struct Elem {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head, *tail;
public:
    List() : head(0), tail(0) {}
    List(const List<T>&) = delete;
    List<T> &operator = (const List<T> &) = delete;

    void addFirst(T *data) {
        Elem *newElem = new Elem(data, head);
        head = newElem;
        if (!tail) { tail = head; }
    }

    void addLast(T *data) {
        Elem *newElem = new Elem(data, 0);

        if (tail) {
            tail->next = newElem;
            tail = newElem;
        } else {
            head = tail = newElem;
        }
    }

    T* removeFirst() {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T* ret =  elem->data;
        delete elem;
        return ret;
    }

    T* peekFirst() {
        if (!head) { return 0; }
        return head->data;
    }

    T* removeLast() {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next) {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T* peekLast() {
        if (!tail) { return 0; }
        return tail->data;
    }

    int size() const {
        int count = 0;
        for (Elem* curr = head; curr != nullptr; curr = curr->next) {
            count++;
        }
        return count;
    }
};

#endif //PROJECT_BASE_V1_1_LIST_HPP
