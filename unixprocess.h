#ifndef UNIXPROCESS_H
#define UNIXPROCESS_H

#include "process.h"

#include <fstream>

#define FIFO_PATH "/tmp/notpad-fifo"

class UnixMessageQueue : public Process::MessageQueue {
public:
    UnixMessageQueue(bool isServer, std::fstream *namedPipe);
    ~UnixMessageQueue();
    void listen(Process::Message (*handler)(const Process::Message &message));
    Process::Message send(const Process::Message &message);

private:
    std::fstream *fifo;
};

#endif  // UNIXPROCESS_H
