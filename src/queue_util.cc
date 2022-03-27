#include "queue_util.h"

// template<class T>
void FlushQueue::Enqueue(dictEntry* entry) {
    _queue.push(entry);
}

// template<class T>
dictEntry* FlushQueue::Dequeue(void) {
    dictEntry* result;

    if (_queue.empty() == true) {
        return nullptr;
    }
    result = _queue.front();
    _queue.pop();

    return result;
}
