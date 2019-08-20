#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog {
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();

private slots:
    void on_buttonFind_Next_clicked();
    void on_checkBoxMatch_Case_toggled(bool checked);
    void on_checkBoxWrap_Around_toggled(bool checked);
    void on_radioDown_toggled(bool checked);

private:
    Ui::FindDialog *ui;
};

#endif  // FINDDIALOG_H
