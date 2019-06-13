#include "searchform.h"
#include "ui_searchform.h"
//Qt Material
#include <qtmaterialiconbutton.h>
#include <lib/qtmaterialtheme.h>
//Qt Animation
#include <QPropertyAnimation>

SearchForm::SearchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchForm)
{
    ui->setupUi(this);
    initUI();
}

SearchForm::~SearchForm()
{
    delete closeBtn;
    delete ui;
}

void SearchForm::initUI()
{
    //Close Button
    closeBtn = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "cancel"), this);
    closeBtn->setColor(QColor(Qt::white));
    closeBtn->setIconSize(QSize(28,28));
    connect(closeBtn, &QtMaterialIconButton::clicked, [this]()
    {
        CloseSearchTool();
    });
    ui->horizontalLayout->addWidget(closeBtn);

    //Search Text Field
    ui->lineEdit->setLabel("Search");
    connect(ui->lineEdit, &QtMaterialTextField::textChanged,
            [this](const QString &text)
    {
        emit SearchTextChanged(text);
    });
}

void SearchForm::CloseSearchTool()
{
    int final_x = this->x();
    int final_y = this->y();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(350);
    animation->setEasingCurve(QEasingCurve::InQuint);
    animation->setStartValue(QRect(final_x, 0, this->width(), this->height()));
    animation->setEndValue(QRect(final_x, -this->height(), this->width(), this->height()));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(animation, &QPropertyAnimation::finished,[this]()
    {
        this->hide();
    });
}

void SearchForm::ClearSearch()
{
    ui->lineEdit->clear();
}

void SearchForm::showEvent(QShowEvent *event)
{
    int final_x = this->x();
    QWidget::showEvent(event);
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::InOutQuart);
    animation->setStartValue(QRect(final_x, -this->height(), this->width(), this->height()));
    animation->setEndValue(QRect(final_x, 0, this->width(), this->height()));
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    connect(animation, &QPropertyAnimation::finished,[this]()
    {
        ui->lineEdit->setFocus();
        ui->lineEdit->selectAll();
    });
}
