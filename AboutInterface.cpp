#include "AboutInterface.h"
#include "ui_AboutInterface.h"

AboutInterface::AboutInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutInterface)
{
    ui->setupUi(this);
}

AboutInterface::~AboutInterface()
{
    delete ui;
}
