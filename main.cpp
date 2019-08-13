#include "editorwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Set values for QSettings
    a.setOrganizationName("LockeTech");
    a.setOrganizationDomain("prestonlocke.net");
    a.setApplicationName("QNotPad");

    EditorWindow w;
    w.show();

    return a.exec();
}
