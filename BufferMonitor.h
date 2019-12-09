//
// Created by mateusz on 12/9/19.
//

#ifndef MONITOR_BUFFERMONITOR_H
#define MONITOR_BUFFERMONITOR_H

#include <memory>
#include "monitor.h"
#include "Message.h"

using PMessage = std::shared_ptr<Message>;

class BufferMonitor : public Monitor {
public:
    BufferMonitor(int bufferSize) : Monitor(), BUFFER_SIZE_(bufferSize), count_(0) {
        priorityHead_ = buffer_.end();
    }

    void enterItem(const PMessage &m) {
        enter();

        if (count_ == BUFFER_SIZE_) {
            wait(full_);
        }

        switch (m->getPriority()) {
            case HIGH:
                insertPriorityMessage(m);
                break;
            case LOW:
                insertNormalMessage(m);
                break;
            default:
                break;
        }

        ++count_;

        if (count_ == 1) {
            signal(empty_);
        }

        leave();
    }

    PMessage removeItem() {
        enter();

        if (count_ == 0) {
            wait(empty_);
        }
        PMessage mes = popMessage();

        --count_;
        if (count_ == BUFFER_SIZE_ - 1) {
            signal(full_);
        }

        leave();
        return mes;
    }

private:
    Condition full_, empty_;
    int count_;
    const int BUFFER_SIZE_;

    std::list<PMessage> buffer_;
    std::list<PMessage>::const_iterator priorityHead_;

    void insertPriorityMessage(const PMessage &message) {
        if (priorityHead_ !=  buffer_.end()){
            buffer_.insert(priorityHead_, message);
            priorityHead_ = std::next(priorityHead_, 1);
        }
        else {
            // first message with priority
            buffer_.push_front(message);
            priorityHead_ = buffer_.begin();
        }
    }

    void insertNormalMessage(const PMessage &message) {
        buffer_.push_back(message);
    }

    PMessage popMessage() {
        PMessage message = buffer_.front();
        if (message == *priorityHead_) {
            priorityHead_ = buffer_.end();
        }
        buffer_.pop_front();
        return message;
    }


};

#endif //MONITOR_BUFFERMONITOR_H
