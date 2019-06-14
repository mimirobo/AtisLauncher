#include "editablelabel.h"
#include "ui_editablelabel.h"

#include <QKeyEvent>

EditableLabel::EditableLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditableLabel),
    editable(true)
{
    ui->setupUi(this);
    ui->lineEdit->hide();
    ui->label->installEventFilter(this);
    ui->lineEdit->installEventFilter(this);
}

EditableLabel::~EditableLabel()
{
    delete ui;
}

bool EditableLabel::eventFilter(QObject *watched, QEvent *event)
{
    if(!editable)
        return false;
    if(event->type() == QEvent::MouseButtonDblClick)
    {
        QLabel *lbl = dynamic_cast<QLabel*>(watched);
        if(!lbl)
            return false;
        ui->label->setVisible(false);
        ui->lineEdit->setText(ui->label->text());
        ui->lineEdit->setVisible(true);
        ui->lineEdit->setFocus();
        ui->lineEdit->selectAll();
        return QWidget::eventFilter(watched, event);
    }
    if(event->type() == QEvent::FocusOut || event->type() == QEvent::KeyPress)
    {
        QLineEdit *lineedit = dynamic_cast<QLineEdit*>(watched);
        if(!lineedit)
            return false;

        bool key_ok= true;
        bool accept_change = false;
        if(event->type() == QEvent::KeyPress)
        {
            key_ok =false;
            QKeyEvent *ke = static_cast<QKeyEvent *>(event);
            if (ke->key() == Qt::Key_Escape)
                key_ok =true;
            if(ke->key()== Qt::Key_Return || ke->key()==Qt::Key_Enter)
            {
                key_ok = true;
                accept_change = true;
            }

        }
        if(key_ok)
        {
            if(accept_change)
            {
                ui->label->setText(ui->lineEdit->text());
                emit CaptionChanged(ui->label->text());
            }
            ui->label->setVisible(true);
            ui->lineEdit->setVisible(false);
        }
    }
    return false;


}

void EditableLabel::setCaption(const QString &caption)
{
    ui->label->setText(caption);
}

