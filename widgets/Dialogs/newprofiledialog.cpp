#include "newprofiledialog.h"
#include "ui_newprofiledialog.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

NewProfileDialog::NewProfileDialog(QWidget *parent, bool display_already_exists) :
    QDialog(parent),
    ui(new Ui::NewProfileDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
    if(!display_already_exists)
    {
        ui->error_lbl->hide();
    }
}

NewProfileDialog::~NewProfileDialog()
{
    delete ui;
}

void NewProfileDialog::accept()
{
    if(ui->lineEdit->text().isEmpty())
        return;
    QDialog::accept();
    emit profileAdded(ui->lineEdit->text());
}

int NewProfileDialog::exec()
{
    /*this->setWindowOpacity(0.0);
    QPropertyAnimation* anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(1000);
    anim->setEasingCurve(QEasingCurve::OutCirc); // just demonstration, there are a lot of curves to choose
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);*/
    //QSize last_size = this->size();

/*    QParallelAnimationGroup *par = new QParallelAnimationGroup(this);

    QPropertyAnimation* anim = new QPropertyAnimation(this, "size");
    anim->setDuration(1000);
    anim->setEasingCurve(QEasingCurve::OutCirc); // just demonstration, there are a lot of curves to choose
    anim->setStartValue(QSize(0,0));
    anim->setEndValue(last_size);
    //anim->start(QAbstractAnimation::DeleteWhenStopped);

    QPropertyAnimation* anim = new QPropertyAnimation(this, "x");
    anim->setDuration(1000);
    anim->setEasingCurve(QEasingCurve::OutCirc); // just demonstration, there are a lot of curves to choose
    anim->setStartValue(QSize(0,0));
    anim->setEndValue(last_size);

    par->addAnimation(anim);*/

    /*QPropertyAnimation *anim = new QPropertyAnimation(this, "geometry");
    anim->setDuration(1000);
    anim->setStartValue(QRect(0,0,0,0));
    anim->setEndValue(QRect(mapToGlobal(QPoint((parentWidget()->width()/2)-(last_size.width()/2),
                                   (parentWidget()->height()/2)-(last_size.height()/2))),
                            last_size));
    anim->start(QAbstractAnimation::DeleteWhenStopped);*/
    //QRect parentRect = parentWidget()->geometry();
    //this->setGeometry(QRect(parentRect.center()-this->geometry().center(),last_size));

    return QDialog::exec();
}
