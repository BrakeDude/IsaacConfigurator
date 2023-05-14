#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QCheckBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString directory;
    QString configDir;
    bool getGamePath();
    void loadMods(QString directory, QTableWidget *table);
    void LoadConfigFile(QString confDir);
    void ReloadModList();

private slots:
    void on_actionOpen_Mods();
    void on_actionOpen_Config();
};

#endif // MAINWINDOW_H
