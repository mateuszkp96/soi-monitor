#include <iostream>
#include <thread>
#include <string>
#include <memory>
#include "Message.h"
#include "BufferMonitor.h"

using PMessage = std::shared_ptr<Message>;

BufferMonitor bufferMonitor(20);

PMessage createMessage(Priority p, std::string content, int userId, int messageId) {
    return std::make_shared<Message>(p, content, userId, messageId);
}

std::string getPriorityName(Priority priority) {
    switch (priority) {
        case HIGH:
            return "HIGH";
        case LOW:
            return "LOW";
        default:
            return  "";
    }
}

class UserThread {
public:
    UserThread(int userId, const std::string &message, int numOfMessages)
            : userId_(userId), message_(message), NUM_OF_MESSAGES(numOfMessages), messageCount_(0) {}

    void operator()() {
        for (int i = 0; i < NUM_OF_MESSAGES; ++i) {
            Priority p = Priority(rand()%2);
            PMessage message = createMessage(p, message_, userId_, ++messageCount_);
            bufferMonitor.enterItem(message);
        }
    }

private:
    int userId_;
    std::string message_;
    int messageCount_;
    const int NUM_OF_MESSAGES;
};

class ConsumerThread {
public:
    ConsumerThread(int allMessages) : NUM_OF_MESSAGES(allMessages), run(true), processedMessages(0) {}

    void operator()() {
        while (run) {
            PMessage msg = bufferMonitor.removeItem();
            std::cout << "User: " << msg->getUserId() << '\t' << "Message id: " << msg->getMessageId()
                      << '\t' << "Content: " << msg->getContent() << '\t' << "Priority: "<< getPriorityName(msg->getPriority()) << std::endl;
            ++processedMessages;
            run = processedMessages < NUM_OF_MESSAGES;
        }

}

private:
    bool run;
    int processedMessages;
    const int NUM_OF_MESSAGES;
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    int userCount = 0;

    UserThread u1(++userCount, "User 1", 10);
    UserThread u2(++userCount, "User 2", 10);
    UserThread u3(++userCount, "User 3", 10);

    ConsumerThread consumer(30);

    std::thread c1(consumer);

    std::thread t1(u1);
    std::thread t2(u2);
    std::thread t3(u3);

    c1.join();
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
