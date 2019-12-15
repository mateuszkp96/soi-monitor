#ifndef MONITOR_UTILS_H
#define MONITOR_UTILS_H

#include <iostream>
#include <memory>
#include <string>
#include <functional>
#include <chrono>
#include "Message.h"

class Utils {
public:
    static std::shared_ptr<Message> createMessage(Priority p, std::string content, int userId);

    static void printMessage(std::shared_ptr<Message> msg);

    static void printBadLengthMessage(std::string content, int userId, int timestamp);

    static void printMessageWithTag(std::shared_ptr<Message> msg, std::string tag);

    static std::function<std::string()> getBadLengthMessage();

    static void sleepForMilliseconds(int value);

private:
    static std::string getPriorityName(Priority priority);
};

std::shared_ptr<Message> Utils::createMessage(Priority p, std::string content, int userId) {
    return std::make_shared<Message>(p, content, userId);
}

void Utils::printMessage(std::shared_ptr<Message> msg) {
    std::cout << "User: " << msg->getUserId() << '\t' << "Timestamp: " << msg->getTimestamp()
              << '\t' << "Content: " << msg->getContent() << '\t' << "Priority: "
              << getPriorityName(msg->getPriority()) << std::endl;
}

void Utils::printBadLengthMessage(std::string content, int userId, int timestamp) {
    std::cout << "Bad message length:" "User: " << userId << '\t' << "Timestamp: "
              << timestamp << '\t' << "Content: " << content << std::endl;
}

std::function<std::string()> Utils::getBadLengthMessage() {
    return [] {
        switch (rand() % 2) {
            case 0:
                return "Short M";
            case 1:
            default:
                return "Very Long Message =================================================================================";
        }
    };
}

std::string Utils::getPriorityName(Priority priority) {
    switch (priority) {
        case HIGH:
            return "HIGH";
        case LOW:
            return "LOW";
        default:
            return "";
    }
}

void Utils::sleepForMilliseconds(int value) {
    std::this_thread::sleep_for(std::chrono::milliseconds(value));
}

void Utils::printMessageWithTag(std::shared_ptr<Message> msg, std::string tag) {
    std::cout << tag << '\t';
    printMessage(msg);
}


#endif //MONITOR_UTILS_H
