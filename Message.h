//
// Created by mateusz on 12/9/19.
//

#ifndef MONITOR_MESSAGE_H
#define MONITOR_MESSAGE_H

#include <string>
#include <utility>

enum Priority {
    HIGH, LOW
};

class Message {
public:
    Message(Priority p, const std::string &content, int uId, int mId)
            : priority_(p), content_(content), userId_(uId), messageId_(mId) {}

    Priority getPriority() const {
        return priority_;
    }

    std::string getContent() const {
        return content_;
    }

    int getUserId() const {
        return userId_;
    }

    int getMessageId() const {
        return messageId_;
    }

private:
    Priority priority_;
    std::string content_;
    int userId_;
    int messageId_;
};

#endif //MONITOR_MESSAGE_H
