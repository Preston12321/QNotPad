#include "process.h"

#include <queue>
#include <string>

using namespace std;

bool Process::MessageQueue::isServer() const { return server; }

int Process::Message::size() const { return rawBufferLength; }

int Process::Message::remainingSize() const { return int(queuedBuffer.size()); }

void Process::Message::readBytes(char *buffer, int length) {
    int index = 0;
    while (!queuedBuffer.empty() && index < length) {
        buffer[index] = queuedBuffer.front();
        queuedBuffer.pop();
        index++;
    }
}

char Process::Message::readChar() {
    if (queuedBuffer.empty()) return -1;
    char result = queuedBuffer.front();
    queuedBuffer.pop();
    return result;
}

short Process::Message::readShort() {
    int size = sizeof(short);
    char *buffer = new char[size];

    readBytes(buffer, size);

    short result = 0;
    for (int i = 0; i < size; i++) {
        result += (buffer[i] << (8 * i));
    }
    return result;
}

int Process::Message::readInt() {
    int size = sizeof(int);
    char *buffer = new char[size];

    readBytes(buffer, size);

    int result = 0;
    for (int i = 0; i < size; i++) {
        result += (buffer[i] << (8 * i));
    }
    return result;
}

long Process::Message::readLong() {
    int size = sizeof(long);
    char *buffer = new char[size];

    readBytes(buffer, size);

    long result = 0;
    for (int i = 0; i < size; i++) {
        result += (buffer[i] << (8 * i));
    }
    return result;
}

// WARNING: If there are null bytes before index 'length',
// the string returned will be of an unexpected arbitrary size
string Process::Message::readString(int length) {
    char *buffer = new char[length + 1];

    readBytes(buffer, length);
    buffer[length] = '\0';

    return string(buffer);
}

void Process::Message::reset() {
    // Clear current buffer
    while (!queuedBuffer.empty()) {
        queuedBuffer.pop();
    }

    // Reload original buffer
    for (int i = 0; i < rawBufferLength; i++) {
        queuedBuffer.push(rawBuffer[i]);
    }
}
