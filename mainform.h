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
    Ksfile_ksfileid = 0,
    Ksfile_ksid = 1,
    Ksfile_wjmc = 2,
    ksfile_filepubtime = 3,
    Ksfile_webaddress = 4,
};

class MainForm : public QWidget
{
    Q_OBJECT

public:
    MainForm();

private slots:
    void updateMsgView();
    void updateFileView();
    void addfile();
    void deletefile();
    void editmsgs();

private:
    void createFilePanel();
    void createMsgPanel();
    void createKaoshiPanel();

    QSqlRelationalTableModel *fileModel;
    QSqlRelationalTableModel *msgModel;
    QSqlRelationalTableModel *kaoshiModel;

    QWidget *filePanel;
    QWidget *msgPanel;
    QWidget *kaoshiPanel;
    QLabel *fileLabel;
    QLabel *msgLabel;
    QLabel *kaoshiLabel;

    QTableView *fileView;
    QTableView *msgView;
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
