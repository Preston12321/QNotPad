#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class EditorWindow;
}

class EditorWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = nullptr);
    ~EditorWindow();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_actionWord_Wrap_toggled(bool shouldWrap);
    void on_actionFont_Style_triggered();
    void on_actionSave_As_triggered();
    void on_actionSave_triggered();

private:
    Ui::EditorWindow *ui;
    QSettings *pref;
    QString filePath;
    void toggleWordWrap(bool shouldWrap);
    void doSaveAs();
    void saveFile();
};

#endif  // EDITORWINDOW_H
