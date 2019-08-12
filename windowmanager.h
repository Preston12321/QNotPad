#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "editorwindow.h"

#include <QLinkedList>

class WindowManager
{
public:
    WindowManager();
    ~WindowManager();
    void addWindow();
    void addWindow(EditorWindow *window);
    void removeWindow(EditorWindow *window);
    int count();

private:
    QLinkedList<EditorWindow *> *windows;
};

#endif // WINDOWMANAGER_H
