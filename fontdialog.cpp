#include "fontdialog.h"
#include "ui_fontdialog.h"

FontDialog::FontDialog(const QFont &currentFont, QWidget *parent) :
QDialog(parent),
ui(new Ui::FontDialog)
{
    ui->setupUi(this);

    ui->fontComboBox->setCurrentFont(currentFont);
    chosenFont = currentFont;
}

FontDialog::~FontDialog()
{
    delete ui;
}

QFont FontDialog::getChosenFont() const
{
    return chosenFont;
}

void FontDialog::on_fontComboBox_currentFontChanged(const QFont &f)
{
    chosenFont = f;
}
