#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::FindDialog) {
    ui->setupUi(this);
}

FindDialog::~FindDialog() { delete ui; }

void FindDialog::on_radioDown_toggled(bool checked) {
    // TODO: Save Find direction
}

void FindDialog::on_checkBoxMatch_Case_toggled(bool checked) {
    // TODO: Save 'Match Case' state
}

void FindDialog::on_checkBoxWrap_Around_toggled(bool checked) {
    // TODO: Save 'Wrap Around' state
}

void FindDialog::on_buttonFind_Next_clicked() {
    // TODO: Implement 'Find Next'
}
