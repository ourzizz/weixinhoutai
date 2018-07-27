#ifndef FILEFORM_H
#define FILEFORM_H
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
        FileForm(int msgid, QWidget *parent = 0);

        void done(int result);
        void manualSubmit();

        private slots:
            void addFile();
        void deleteFile();

    private:
        QSqlRelationalTableModel *tableModel;
        QDataWidgetMapper *mapper;
        QLabel *nameLabel;
        QLabel *pubTimeLabel;
        QLabel *webAddLabel;
        QLabel *kaoshiLabel;

        QComboBox *kaoshiComboBox;
        QLineEdit *nameEdit;
        QDateEdit *pubTimeEdit;
        QLineEdit *webAddEdit;

        QPushButton *addButton;
        QPushButton *deleteButton;
        QPushButton *closeButton;
        QPushButton *submitButton;
        QDialogButtonBox *buttonBox;
};

#endif
