#ifndef KAOSHIFORM_H
#define KAOSHIFORM_H
#include <QtWidgets>
#include <QDialog>
#include <QString>

class QComboBox;
class QDataWidgetMapper;
class QDateEdit;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSqlRelationalTableModel;

enum {
    Kstype_Pinyin = 0,
    Kstype_Name = 1,
    Kstype_Hot = 2,
};

class KaoshiForm : public QDialog
{
    Q_OBJECT

    public:
        KaoshiForm(QString ksid, QWidget *parent = 0);

        void done(int result);
        void manualSubmit();

        private slots:
            void addFile();
        void deleteFile();

    private:
        QSqlRelationalTableModel *tableModel;
        QDataWidgetMapper *mapper;
        QLabel *pinyinLabel;
        QLabel *nameLabel;
        QLabel *hotLabel;

        //QLabel *kaoshiLabel;
        //QComboBox *kaoshiComboBox;
        QLineEdit *pinyinEdit;
        QLineEdit *nameEdit;
        QLineEdit *hotEdit;

        QPushButton *addButton;
        QPushButton *closeButton;
        QPushButton *submitButton;
        QDialogButtonBox *buttonBox;
};

#endif
