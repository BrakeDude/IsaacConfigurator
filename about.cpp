#include "about.h"
#include "ui_about.h"

about::about(QWidget *parent) :
    QDialog(parent),
    ui_about(new Ui::about)
{
    ui_about->setupUi(this);
}

about::~about()
{
    delete ui_about;
}
