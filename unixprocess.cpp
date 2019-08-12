#include "unixprocess.h"
#include "process.h"

#include <fstream>

using namespace std;

Process::MessageQueue *Process::getMessageQueue() { return nullptr; }

UnixMessageQueue::UnixMessageQueue(bool isServer, fstream *namedPipe) {
    fifo = namedPipe;
    server = isServer;
}

UnixMessageQueue::~UnixMessageQueue() {
    if (fifo->is_open()) fifo->close();
    delete fifo;
}

void UnixMessageQueue::listen(
    Process::Message (*handler)(const Process::Message &)) {
    // TODO: Implement
    handler(Process::Message());
}

Process::Message UnixMessageQueue::send(const Process::Message &message) {
    // TODO: Implement
    return message;
}
