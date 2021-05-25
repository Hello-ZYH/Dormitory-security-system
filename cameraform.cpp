#include "cameraform.h"
#include "ui_cameraform.h"



CameraForm::CameraForm(QWidget *parent) : QWidget(parent), ui(new Ui::CameraForm)
{
    ui->setupUi(this);
}



CameraForm::~CameraForm()
{
    delete ui;
}
