#include "editorwindow.h"
#include "fontdialog.h"
#include "preferences.h"
#include "ui_editorwindow.h"

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QFontDialog>
#include <QProcess>
#include <QSettings>
#include <QTextStream>

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
    toggleWordWrap(loadPref(pref, "Editor", "WordWrap", false).toBool());

    // Disable some of the edit buttons in newly-opened documents
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);

    // Manually set text for menu buttons using translation
    ui->menuFile->setTitle(tr("File"));
    ui->actionNew->setText(tr("New"));
    ui->actionOpen->setText(tr("Open"));
    ui->actionSave->setText(tr("Save"));
    ui->actionSave_As->setText(tr("Save As"));

    ui->menuEdit->setTitle(tr("Edit"));
    ui->actionUndo->setText(tr("Undo"));
    ui->actionRedo->setText(tr("Redo"));
    ui->actionCut->setText(tr("Cut"));
    ui->actionCopy->setText(tr("Copy"));
    ui->actionPaste->setText(tr("Paste"));
    ui->actionFind->setText(tr("Find"));
    ui->actionReplace->setText(tr("Replace"));

    ui->menuView->setTitle(tr("View"));
    ui->actionWord_Wrap->setText(tr("Word Wrap"));
    ui->actionFont_Style->setText(tr("Font Style"));

    // Get file path for this executable, for spawning new processes
    applicationFilePath = QApplication::instance()->applicationFilePath();
}

EditorWindow::~EditorWindow() { delete ui; }

void EditorWindow::on_actionWord_Wrap_toggled(bool shouldWrap) {
    toggleWordWrap(shouldWrap);
}

void EditorWindow::toggleWordWrap(bool shouldWrap) {
    ui->plainTextEdit->setWordWrapMode(
        shouldWrap ? QTextOption::WrapAtWordBoundaryOrAnywhere
                   : QTextOption::NoWrap);

    if (ui->actionWord_Wrap->isChecked() != shouldWrap)
        ui->actionWord_Wrap->setChecked(shouldWrap);

    // Save to preferences
    savePref(pref, "Editor", "WordWrap", shouldWrap);
}

void EditorWindow::doSaveAs() {
    auto path = QFileDialog::getSaveFileName(this, tr("Save As"));

    if (path.isNull() || path.isEmpty()) return;

    filePath = path;
    saveFile();
}

void EditorWindow::saveFile() {
    auto contents = ui->plainTextEdit->toPlainText();

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        // TODO: Show error message
        return;
    }
    QTextStream out(&file);
    out << contents;
    file.close();

    setWindowTitle(filePath.mid(filePath.lastIndexOf("/") + 1));
}

void EditorWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    // TODO: Do this in separate thread?
    // Save to preferences
    savePref(pref, "Window", "Size", event->size());
}

void EditorWindow::on_actionFont_Style_triggered() {
    auto document = ui->plainTextEdit->document();
    bool accepted;
    auto font = QFontDialog::getFont(&accepted, document->defaultFont(), this);

    if (accepted) {
        document->setDefaultFont(font);
    }
}

void EditorWindow::on_actionSave_As_triggered() { doSaveAs(); }

void EditorWindow::on_actionSave_triggered() {
    if (filePath.isNull() || filePath.isEmpty()) {
        doSaveAs();
    } else {
        saveFile();
    }
}

void EditorWindow::on_actionNew_triggered() {
    QProcess::startDetached(applicationFilePath);
}
