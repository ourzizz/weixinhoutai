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
    Ksfile_Ksfileid = 0,
    Ksfile_Ksid = 1,
    Ksfile_Wjmc = 2,
    Ksfile_Fileputime = 3,
    Ksfile_Webaddress = 4,
};

class FileForm : public QDialog
{
    Q_OBJECT

    public:
        FileForm(int id,int fileid, QWidget *parent = 0);

        void done(int result);

        private slots:
            void addFile();
        void deleteFile();

    private:
        QSqlRelationalTableModel *tableModel;
        QDataWidgetMapper *mapper;
        QLabel *nameLabel;
        QLabel *pubtimeLabel;
        QLabel *webAddLabel;

        QLineEdit *nameEdit;
        //QComboBox *departmentComboBox;
        QLineEdit *nameEdit;
        QLineEdit *pubTimeEdit;
        QDateEdit *webAddEdit;

        QPushButton *addButton;
        QPushButton *deleteButton;
        QPushButton *closeButton;
        QDialogButtonBox *buttonBox;
};

#endif
