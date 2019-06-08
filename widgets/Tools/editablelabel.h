#ifndef EDITABLELABEL_H
#define EDITABLELABEL_H

#include <QWidget>

namespace Ui {
class EditableLabel;
}

class EditableLabel : public QWidget
{
    Q_OBJECT

public:
    explicit EditableLabel(QWidget *parent = 0);
    ~EditableLabel();
    bool eventFilter(QObject *watched, QEvent *event);

    void setCaption(const QString &caption);
    inline void setEditable(bool enable){ editable = enable;}
signals:
    void CaptionChanged(QString new_caption);
private:
    Ui::EditableLabel *ui;
    bool editable;
};

#endif // EDITABLELABEL_H
