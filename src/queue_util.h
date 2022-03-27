#pragma once

#include <queue>

struct dictEntry;

// template <class T>
class FlushQueue {
  public:
    FlushQueue(void) {}
    virtual ~FlushQueue(void) {}
    void Enqueue(dictEntry *entry);
    dictEntry *Dequeue(void);
    int IsSize(void) { return _queue.size(); }

  private:
    std::queue<dictEntry *> _queue;
};
