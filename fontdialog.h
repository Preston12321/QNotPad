#ifndef FONTDIALOG_H
#define FONTDIALOG_H

#include <QDialog>
#include <QFont>

namespace Ui {
class FontDialog;
}

class FontDialog : public QDialog
{
Q_OBJECT

public:
    FontDialog(const QFont &currentFont, QWidget *parent = nullptr);
    ~FontDialog();
    QFont getChosenFont() const;

private slots:
    void on_fontComboBox_currentFontChanged(const QFont &f);

private:
    Ui::FontDialog *ui;
    QFont chosenFont;
};

#endif // FONTDIALOG_H
