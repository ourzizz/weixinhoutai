#ifndef EMPLOYEEFORM_H
#define EMPLOYEEFORM_H
#include <QtWidgets>
#include <QDialog>

class QComboBox;
class QDataWidgetMapper;
class QDateEdit;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSqlRelationalTableModel;

enum {
    Msg_Id = 0,
    Msg_Msgcontent = 1,
    Msg_msgpubtime = 2,
    Msg_Fileid = 3,
    Msg_Ksid = 4,
    Msg_Deadtime = 5
};

class EmployeeForm : public QDialog
{
    Q_OBJECT

    public:
        EmployeeForm(int id,int fileid, QWidget *parent = 0);

        void done(int result);

        private slots:
            void addEmployee();
        void deleteEmployee();

    private:
        QSqlRelationalTableModel *tableModel;
        QDataWidgetMapper *mapper;
        QLabel *nameLabel;
        QLabel *departmentLabel;
        QLabel *extensionLabel;
        QLabel *emailLabel;
        QLabel *startDateLabel;
        QLineEdit *nameEdit;
        QComboBox *departmentComboBox;
        QDateEdit *extensionLineEdit;
        QLineEdit *emailEdit;
        QDateEdit *startDateEdit;
        QPushButton *firstButton;
        QPushButton *previousButton;
        QPushButton *nextButton;
        QPushButton *lastButton;
        QPushButton *addButton;
        QPushButton *deleteButton;
        QPushButton *closeButton;
        QDialogButtonBox *buttonBox;
};

#endif
