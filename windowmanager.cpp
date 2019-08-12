#include "windowmanager.h"
#include "editorwindow.h"

#include <QLinkedList>

WindowManager::WindowManager()
{
    windows = new QLinkedList<EditorWindow *>();
}

WindowManager::~WindowManager()
{
    for (auto window: (*windows)) delete window;
    delete windows;
}

void WindowManager::addWindow(EditorWindow *window)
{
    if (window != nullptr && !windows->contains(window))
        windows->append(window);
}

void WindowManager::removeWindow(EditorWindow *window)
{
    if (window != nullptr) windows->removeOne(window);
}

int WindowManager::count()
{
    return windows->size();
}
