#ifndef PROCESS_H
#define PROCESS_H

#include <queue>
#include <string>

namespace Process {

class Message {
public:
    int size() const;
    int remainingSize() const;
    void readBytes(char *buffer, int length);
    char readChar();
    short readShort();
    int readInt();
    long readLong();
    std::string readString(int length);
    void reset();

private:
    std::queue<char> queuedBuffer;
    char *rawBuffer;
    int rawBufferLength = 0;
};

class MessageQueue {
public:
    virtual ~MessageQueue() {}
    virtual void listen(Message (*handler)(const Message &message)) = 0;
    virtual Message send(const Message &message) = 0;
    virtual bool isServer() const;

protected:
    bool server;
};

// Returns nullptr if can't obtain MessageQueue
MessageQueue *getMessageQueue();

}  // namespace Process

#endif  // PROCESS_H
