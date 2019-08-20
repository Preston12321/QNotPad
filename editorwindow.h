#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QLabel>
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
    void on_actionDelete_triggered();
    void on_actionFind_triggered();
    void on_actionFind_Next_triggered();
    void on_actionFont_Style_triggered();
    void on_actionGo_To_triggered();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionPage_Setup_triggered();
    void on_actionPrint_triggered();
    void on_actionReplace_triggered();
    bool on_actionSave_triggered();
    bool on_actionSave_As_triggered();
    void on_actionTime_Date_triggered();
    void on_actionWord_Wrap_toggled(bool shouldWrap);
    void on_plainTextEdit_cursorPositionChanged();
    void on_plainTextEdit_modificationChanged();
    void on_plainTextEdit_selectionChanged();
    void updateStatusBarCursorLabel();

    void on_plainTextEdit_textChanged();

private:
    Ui::EditorWindow *ui;
    QSettings *pref;
    QString applicationFilePath;
    QString filePath;
    QLabel statusBarCursorLabel;
    void closeEvent(QCloseEvent *event);
    bool doSaveAs();
    bool saveFile();
    bool doFileModifiedCheck();
};

#endif  // EDITORWINDOW_H
