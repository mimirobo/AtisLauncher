#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QWidget>

class QtMaterialIconButton;

namespace Ui {
class SearchForm;
}

class SearchForm : public QWidget
{
    Q_OBJECT

public:
    explicit SearchForm(QWidget *parent = 0);
    ~SearchForm();
    void initUI();
    void CloseSearchTool();
    void ClearSearch();
signals:
    void SearchTextChanged(const QString&);
protected:
    void showEvent(QShowEvent *event);
private:
    Ui::SearchForm *ui;
    QtMaterialIconButton *closeBtn;
};

#endif // SEARCHFORM_H
