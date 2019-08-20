#include "editorwindow.h"
#include "preferences.h"
#include "ui_editorwindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QFontDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QTextStream>
#include <QtDebug>

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::EditorWindow) {
    ui->setupUi(this);

    setWindowTitle("Untitled Document");

    // Obtain the application preferences, if any
    pref = new QSettings;

    // Window-related preferences...

    // Set window size from preferences
    resize(loadPref(pref, "Window", "Size", QSize(600, 400)).toSize());

    // Editor-related preferences...

    // Set Word Wrap from preferences
    on_actionWord_Wrap_toggled(
        loadPref(pref, "Editor", "WordWrap", false).toBool());

    // Disable some of the edit buttons in newly-opened documents
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionDelete->setEnabled(false);

    // Manually set text for menu buttons using translation
    ui->menuFile->setTitle(tr("File"));
    ui->actionNew->setText(tr("New"));
    ui->actionOpen->setText(tr("Open..."));
    ui->actionSave->setText(tr("Save"));
    ui->actionSave_As->setText(tr("Save As..."));
    ui->actionPage_Setup->setText(tr("Page Setup..."));
    ui->actionPrint->setText(tr("Print..."));
    ui->actionExit->setText(tr("Exit"));

    ui->menuEdit->setTitle(tr("Edit"));
    ui->actionUndo->setText(tr("Undo"));
    ui->actionRedo->setText(tr("Redo"));
    ui->actionCut->setText(tr("Cut"));
    ui->actionCopy->setText(tr("Copy"));
    ui->actionPaste->setText(tr("Paste"));
    ui->actionDelete->setText(tr("Delete"));
    ui->actionFind->setText(tr("Find..."));
    ui->actionFind_Next->setText(tr("Find Next"));
    ui->actionReplace->setText(tr("Replace..."));
    ui->actionGo_To->setText(tr("Go To..."));
    ui->actionSelect_All->setText(tr("Select All"));
    ui->actionTime_Date->setText(tr("Time/Date"));

    ui->menuView->setTitle(tr("View"));
    ui->actionWord_Wrap->setText(tr("Word Wrap"));
    ui->actionFont_Style->setText(tr("Font Style..."));

    // Populate status bar
    statusBarCursorLabel.setText("Line 0, Col 0");
    ui->statusBar->addPermanentWidget(&statusBarCursorLabel);

    // Get file path for this executable, for spawning new processes
    applicationFilePath = QApplication::instance()->applicationFilePath();
}

EditorWindow::~EditorWindow() { delete ui; }

// Event handler overrides

void EditorWindow::closeEvent(QCloseEvent *event) {
    if (doFileModifiedCheck()) {
        event->accept();
        return;
    }

    event->ignore();
}

void EditorWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    // MAYBE: Save to preferences in separate thread?
    // Save to preferences
    savePref(pref, "Window", "Size", event->size());
}

// Instance methods

bool EditorWindow::doSaveAs() {
    auto path = QFileDialog::getSaveFileName(this, tr("Save As"));

    if (path.isNull() || path.isEmpty()) return false;

    filePath = path;
    return saveFile();
}

bool EditorWindow::saveFile() {
    auto contents = ui->plainTextEdit->toPlainText();

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(this, "File Error",
                             "Failed to save file. Please try again.");
        return false;
    }
    QTextStream out(&file);
    out << contents;
    file.close();

    setWindowTitle(filePath.mid(filePath.lastIndexOf("/") + 1));
    ui->plainTextEdit->document()->setModified(false);

    return true;
}

bool EditorWindow::doFileModifiedCheck() {
    if (!ui->plainTextEdit->document()->isModified()) return true;

    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                              QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    bool proceed;

    switch (msgBox.exec()) {
        case QMessageBox::Save:
            // Let on_actionSave_triggered do (Save/Save As)
            // and proceed if successful
            proceed = on_actionSave_triggered();
            break;
        case QMessageBox::Discard:
            // Not worried about overriding document, so proceed
            proceed = true;
            break;
        case QMessageBox::Cancel:
            // Cancel was clicked
            proceed = false;
            break;
        default:
            // Should never be reached
            qWarning() << "Invalid input returned from QMessageBox";
            proceed = false;
            break;
    }

    return proceed;
}

void EditorWindow::updateStatusBarCursorLabel() {
    auto cursor = ui->plainTextEdit->textCursor();
    QString message;

    if (cursor.hasSelection()) {
        ui->actionDelete->setEnabled(true);

        int size = cursor.selectedText().size();
        message = QString(QString::number(size)).append(" selected");
    } else {
        ui->actionDelete->setEnabled(false);

        message = QString("Line ")
                      .append(QString::number(cursor.blockNumber()))
                      .append(", Col ")
                      .append(QString::number(cursor.positionInBlock()));
    }

    statusBarCursorLabel.setText(message);
}

// QPlainTextEdit related slots

void EditorWindow::on_plainTextEdit_cursorPositionChanged() {
    updateStatusBarCursorLabel();
}

void EditorWindow::on_plainTextEdit_selectionChanged() {
    updateStatusBarCursorLabel();
}

void EditorWindow::on_plainTextEdit_textChanged() {
    // Ensure an update when user does Ctrl+Backspace
    updateStatusBarCursorLabel();
}

void EditorWindow::on_plainTextEdit_modificationChanged() {
    if (ui->plainTextEdit->document()->isModified()) {
        setWindowTitle("*" + windowTitle());
        return;
    }

    if (!filePath.isNull() && !filePath.isEmpty()) {
        setWindowTitle(filePath.mid(filePath.lastIndexOf("/" + 1)));
        return;
    }

    setWindowTitle("Untitled Document");
}

// QAction related slots

void EditorWindow::on_actionWord_Wrap_toggled(bool shouldWrap) {
    ui->plainTextEdit->setWordWrapMode(
        shouldWrap ? QTextOption::WrapAtWordBoundaryOrAnywhere
                   : QTextOption::NoWrap);

    if (ui->actionWord_Wrap->isChecked() != shouldWrap)
        ui->actionWord_Wrap->setChecked(shouldWrap);

    // Save to preferences
    savePref(pref, "Editor", "WordWrap", shouldWrap);
}

void EditorWindow::on_actionFont_Style_triggered() {
    auto document = ui->plainTextEdit->document();
    bool accepted;
    auto font = QFontDialog::getFont(&accepted, document->defaultFont(), this);

    if (accepted) {
        document->setDefaultFont(font);
    }
}

bool EditorWindow::on_actionSave_As_triggered() { return doSaveAs(); }

bool EditorWindow::on_actionSave_triggered() {
    return (filePath.isNull() || filePath.isEmpty()) ? doSaveAs() : saveFile();
}

void EditorWindow::on_actionNew_triggered() {
    QProcess::startDetached(applicationFilePath);
}

void EditorWindow::on_actionOpen_triggered() {
    // If user doesn't discard or save document (if modified), don't proceed
    if (!doFileModifiedCheck()) return;

    auto path = QFileDialog::getOpenFileName(this, tr("Open File"));

    if (path.isNull() || path.isEmpty()) return;

    filePath = path;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error",
                             "Failed to open file. Please try again.");
        return;
    }
    QTextStream read(&file);
    QString contents = read.readAll();
    file.close();

    setWindowTitle(filePath.mid(filePath.lastIndexOf("/") + 1));

    auto doc = ui->plainTextEdit->document();
    doc->setPlainText(contents);
    doc->setModified(false);
}

void EditorWindow::on_actionDelete_triggered() {
    ui->plainTextEdit->textCursor().removeSelectedText();
}

void EditorWindow::on_actionPage_Setup_triggered() {
    // TODO: Add 'Page Setup' dialog
}

void EditorWindow::on_actionPrint_triggered() {
    // TODO: Add 'Print' dialog
}

void EditorWindow::on_actionFind_triggered() {
    // TODO: Show 'Find' dialog
}

void EditorWindow::on_actionFind_Next_triggered() {
    // TODO: Implement 'Find Next' action
}

void EditorWindow::on_actionReplace_triggered() {
    // TODO: Add 'Replace' dialog
}

void EditorWindow::on_actionGo_To_triggered() {
    // TODO: Add 'Go To' dialog
}

void EditorWindow::on_actionTime_Date_triggered() {
    auto dt = QDateTime::currentDateTime().toString("h:mm AP M/d/yyyy");
    ui->plainTextEdit->textCursor().insertText(dt);
}
