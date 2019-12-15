#ifndef MONITOR_MESSAGE_H
#define MONITOR_MESSAGE_H

#include <string>

enum Priority {
    HIGH, LOW
};

class Message {
public:
    Message(Priority p, const std::string &content, int userId)
            : priority_(p), content_(content), userId_(userId), timestamp_(0) {}

    Priority getPriority() const {
        return priority_;
    }

    std::string getContent() const {
        return content_;
    }

    int getUserId() const {
        return userId_;
    }

    int getTimestamp() const {
        return timestamp_;
    }

    int setTimestamp(int timestamp) {
        timestamp_ = timestamp;
    }

private:
    Priority priority_;
    std::string content_;
    int userId_;
    int timestamp_;
};

#endif //MONITOR_MESSAGE_H
