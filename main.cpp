#include <thread>
#include <string>
#include <memory>
#include <functional>
#include <iostream>
#include "Message.h"
#include "BufferMonitor.h"
#include "Utils.h"

using PMessage = std::shared_ptr<Message>;

BufferMonitor bufferMonitor(20);

//================================================USER THREAD===========================================================
class UserThread {
public:
    UserThread(int userId, const std::function<std::string()> &getMessageFunc, int numOfMessages)
            : userId_(userId), getMessageFunc_(getMessageFunc), NUM_OF_MESSAGES(numOfMessages), messageCount_(0) {}

    void operator()() {
        for (int i = 0; i < NUM_OF_MESSAGES; ++i) {
            Priority p = Priority(rand() % 2);
            std::string content = getMessageFunc_();

            if (content.size() >= 8 && content.size() <= 64) {
                PMessage message = Utils::createMessage(p, content, userId_);
                bufferMonitor.enterItem(message);
            }
        }
    }

private:
    int userId_;
    int messageCount_;
    const int NUM_OF_MESSAGES;
    std::function<std::string()> getMessageFunc_;
};

//============================================CONSUMER THREAD===========================================================
class ConsumerThread {
public:
    ConsumerThread(int allMessages) : NUM_OF_MESSAGES(allMessages), run(true), processedMessages(0) {}

    void operator()() {
        while (run) {
            PMessage msg = bufferMonitor.removeItem();
            ++processedMessages;
            Utils::printMessageWithTag(msg, "READ");
            run = processedMessages < NUM_OF_MESSAGES;
        }

    }

private:
    bool run;
    int processedMessages;
    const int NUM_OF_MESSAGES;
};

void testEmptyBufferRead() {
    ConsumerThread consumer(0);
    std::thread c1(consumer);

    Utils::sleepForMilliseconds(5000);
    bufferMonitor.printStats();
    c1.join();
}

void testFullBufferWrite() {
    int userCount = 0;
    std::vector<std::thread> threads;
    threads.reserve(3);
    threads.emplace_back(UserThread(++userCount, [] { return "User 1 message"; }, 10));
    threads.emplace_back(UserThread(++userCount, [] { return "User 2 message"; }, 10));
    threads.emplace_back(UserThread(++userCount, [] { return "User 3 message"; }, 10));

    Utils::sleepForMilliseconds(5000);
    bufferMonitor.printStats();

    for (auto &thread : threads) {
        thread.join();
    }
}

void testPriority() {
    int userCount = 0;
    std::vector<std::thread> threads;
    threads.reserve(6);

    threads.emplace_back(ConsumerThread(50));
    threads.emplace_back(UserThread(++userCount, [] { return "User 1 message"; }, 10));
    threads.emplace_back(UserThread(++userCount, [] { return "User 2 message"; }, 10));
    threads.emplace_back(UserThread(++userCount, [] { return "User 3 message"; }, 10));
    threads.emplace_back(UserThread(++userCount, [] { return "User 4 message"; }, 10));
    threads.emplace_back(UserThread(++userCount, [] { return "User 5 message"; }, 10));
    Utils::sleepForMilliseconds(5000);
    bufferMonitor.printStats();

    for (auto &thread : threads) {
        thread.join();
    }
}

void testBadMessageLength() {
    int userCount = 0;

    std::vector<std::thread> threads;
    threads.reserve(3);
    threads.emplace_back(ConsumerThread(10));
    threads.emplace_back(UserThread(++userCount, Utils::getBadLengthMessage(), 10));
    threads.emplace_back(UserThread(++userCount, [] { return "Normal message"; }, 10));

    Utils::sleepForMilliseconds(5000);

    bufferMonitor.printStats();
    for (auto &thread : threads) {
        thread.join();
    }

}

void testSameTimeAddMessage() {
    int userCount = 0;
    std::vector<std::thread> threads;
    threads.reserve(4);

    threads.emplace_back(UserThread(++userCount, [] { return "User 1 message"; }, 20));
    Utils::sleepForMilliseconds(2000);
    threads.emplace_back(UserThread(++userCount, [] { return "User 2 message"; }, 5));
    threads.emplace_back(UserThread(++userCount, [] { return "User 3 message"; }, 5));
    Utils::sleepForMilliseconds(2000);

    std::cout << "\n\n\nRIVALIZATION START" << std::endl;
    threads.emplace_back(ConsumerThread(30));

    bufferMonitor.printStats();
    for (auto &thread : threads) {
        thread.join();
    }
}

int main() {
//    testBadMessageLength();
//    testPriority();
//    testEmptyBufferRead();
//    testFullBufferWrite();
    testSameTimeAddMessage();
    return 0;
}
