#pragma once

#include "ui_Register.h"
#include <QDialog>
class Register : public QDialog
{
    Q_OBJECT

  public:
    Register(QWidget *parent = nullptr);
    ~Register();
    Ui::RegisterClass ui;
    bool registerSuccess = false;

  private:

  private slots:
    void on_pushButton_register_clicked();
};
