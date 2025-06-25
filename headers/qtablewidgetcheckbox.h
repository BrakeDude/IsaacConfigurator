#ifndef QTABLEWIDGETCHECKBOX_H
#define QTABLEWIDGETCHECKBOX_H

#include <QTableWidgetItem>
#include <QCheckBox>
class QTableWidgetCheckBox : public QTableWidgetItem
{
public:
    bool operator<(const QTableWidgetItem &other) const override
    {
        if (other.column() == 0) {
            // compare checkboxes by their state
            QCheckBox *checkbox1 = dynamic_cast<QCheckBox *>(tableWidget()->cellWidget(row(), column()));
            QCheckBox *checkbox2 = dynamic_cast<QCheckBox *>(tableWidget()->cellWidget(other.row(), other.column()));
            return checkbox1->isChecked() < checkbox2->isChecked();
        } else {
            // compare other items normally
            return QTableWidgetItem::operator<(other);
        }
    }
};

#endif // QTABLEWIDGETCHECKBOX_H
