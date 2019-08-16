#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QString>

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
    bool on_actionSave_As_triggered();
    bool on_actionSave_triggered();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_plainTextEdit_modificationChanged();

private:
    Ui::EditorWindow *ui;
    QSettings *pref;
    QString applicationFilePath;
    QString filePath;
    void closeEvent(QCloseEvent *event);
    void toggleWordWrap(bool shouldWrap);
    bool doSaveAs();
    bool saveFile();
    bool doFileModifiedCheck();
};

#endif  // EDITORWINDOW_H
