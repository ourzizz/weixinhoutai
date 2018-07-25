#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>

class QDialogButtonBox;
class QLabel;
class QModelIndex;
class QPushButton;
class QSplitter;
class QSqlRelationalTableModel;
class QTableView;

enum {
    Department_Id = 0,
    Department_Name = 1,
    Department_LocationId = 2,
    filepubtime = 3,
    webaddress = 4,
};

class MainForm : public QWidget
{
    Q_OBJECT

public:
    MainForm();

private slots:
    void updateMsgView();
    void updateFileView();
    void addDepartment();
    void deleteDepartment();
    void editEmployees();

private:
    void createDepartmentPanel();
    void createEmployeePanel();
    void createKaoshiPanel();

    QSqlRelationalTableModel *departmentModel;
    QSqlRelationalTableModel *employeeModel;
    QSqlRelationalTableModel *kaoshiModel;

    QWidget *departmentPanel;
    QWidget *employeePanel;
    QWidget *kaoshiPanel;
    QLabel *departmentLabel;
    QLabel *employeeLabel;
    QLabel *kaoshiLabel;

    QTableView *departmentView;
    QTableView *employeeView;
    QTableView *kaoshiView;

    QSplitter *splitter;
    QSplitter *kaoshisplitter;

    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *editButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
};

#endif
