#ifndef MONITOR_BUFFERMONITOR_H
#define MONITOR_BUFFERMONITOR_H

#include <memory>
#include <chrono>
#include "monitor.h"
#include "Message.h"
#include "Utils.h"
#include "Timer.h"

using PMessage = std::shared_ptr<Message>;

class BufferMonitor : public Monitor {
public:
    BufferMonitor(int bufferSize) : Monitor(), BUFFER_SIZE_(bufferSize), count_(0) {
        priorityHead_ = buffer_.end();
        Timer::getInstance(); // initialization of Timer
    }

    void enterItem(const PMessage &m) {
        enter();

        if (count_ == BUFFER_SIZE_) {
            wait(full_);
        }

        m->setTimestamp(Timer::getInstance().getTimeFromBegin());

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
        ++send_messages_;
        Utils::printMessageWithTag(m, "SEND");
        printMessagesInQueue();

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
        ++read_messages_;
        if (count_ == BUFFER_SIZE_ - 1) {
            signal(full_);
        }

        leave();
        return mes;
    }

    void printStats() const {
        std::cout << "SEND: " << send_messages_ << '\t'
                  << "IN_BUFFER: " << count_ << '\t'
                  << "READ: " << read_messages_ << std::endl;
    }

    void printMessagesInQueue() {
        std::cout << "\t\t\t\t\t\tMessages in buffer: " << buffer_.size() << std::endl;
        for (auto &msg : buffer_) {
            Utils::printMessageWithTag(msg, "BUFFER");
        }
        std::cout << std::endl;
    }

private:
    Condition full_, empty_;
    int count_;
    int send_messages_;
    int read_messages_;
    const int BUFFER_SIZE_;

    std::list<PMessage> buffer_;
    std::list<PMessage>::const_iterator priorityHead_;

    void insertPriorityMessage(const PMessage &message) {
        if (priorityHead_ != buffer_.end()) {
            auto next = std::next(priorityHead_, 1);
            buffer_.insert(next, message);
            priorityHead_ = std::next(priorityHead_, 1);
        } else {
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
