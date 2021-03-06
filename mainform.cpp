#include <QtGui>
#include <QtSql>
#include <String>
#include <QTcpSocket>
#include "employeeform.h"
#include "fileform.h"
#include "kaoshiform.h"
#include "mainform.h"

MainForm::MainForm()
{/*{{{*/
    createFilePanel();//创建dpt ep 两个窗口
    createMsgPanel();
    createKaoshiPanel();

    socket = new QTcpSocket();

    editKaoshiButton = new QPushButton(tr("&编辑考试."));
    //addButton = new QPushButton(tr("&添加文件."));
    editButton = new QPushButton(tr("编辑消息"));
    editFileButton = new QPushButton(tr("编辑文件"));
    quitButton = new QPushButton(tr("&Quit"));
    updataHomePageButton = new QPushButton(tr("更新首页"));

    editfilebuttonBox = new QDialogButtonBox;
    editfilebuttonBox->addButton(editFileButton,QDialogButtonBox::ActionRole);

    editkaoshibuttonBox = new QDialogButtonBox;
    editkaoshibuttonBox->addButton(editKaoshiButton,QDialogButtonBox::ActionRole);

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(updataHomePageButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(editButton, QDialogButtonBox::ActionRole);

    splitter = new QSplitter(Qt::Vertical);
    splitter->setFrameStyle(QFrame::StyledPanel);
    splitter->addWidget(filePanel);
    splitter->addWidget(editfilebuttonBox);
    splitter->addWidget(msgPanel);

    //connect(addButton, SIGNAL(clicked()), this, SLOT(addfile()));
    connect(editKaoshiButton, SIGNAL(clicked()), this, SLOT(editkaoshi()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editmsgs()));
    connect(editFileButton, SIGNAL(clicked()), this, SLOT(editfile()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(updataHomePageButton, SIGNAL(clicked()), this, SLOT(callserver()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(kaoshiPanel);
    mainLayout->addWidget(editkaoshibuttonBox);
    buttonBox->addButton(quitButton, QDialogButtonBox::AcceptRole);
    mainLayout->addWidget(splitter);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    setWindowTitle(tr("后台管理"));
    fileView->setCurrentIndex(fileModel->index(0, 0));
}/*}}}*/

void MainForm::updateMsgView()
{/*{{{*/
    QModelIndex index = fileView->currentIndex(); //获得Depview的当前行号
    if (index.isValid()) {
        QSqlRecord record = fileModel->record(index.row()); //获得Depview的当前记录
        int id = record.value("ksfileid").toInt();
        msgModel->setFilter(QString("ksmsg.ksfileid = %1").arg(id));
        msgLabel->setText(tr("配置 %1 考试的消息msg")
                               .arg(record.value("ksid").toString())); //设置标题
                               //.arg(id)); 设置标题
    } else {
        msgModel->setFilter("ksfileid = -1");
        msgLabel->setText(tr("E&mployees"));
    }
    msgModel->select();
    msgView->horizontalHeader()->setVisible(
            msgModel->rowCount() > 0);
            

}/*}}}*/

void MainForm::addfile()
{/*{{{*/
    int row = fileModel->rowCount();
    fileModel->insertRow(row);
    QModelIndex index = fileModel->index(row, Ksfile_ksid);
    fileView->setCurrentIndex(index);
    fileView->edit(index);
}/*}}}*/

void MainForm::deletefile()
{/*{{{*/
    QModelIndex index = fileView->currentIndex();
    if (!index.isValid())
        return;

    QSqlDatabase::database().transaction();
    QSqlRecord record = fileModel->record(index.row());
    int id = record.value(Ksfile_ksfileid).toInt();
    int nummsgs = 0;

    QSqlQuery query(QString("SELECT COUNT(*) FROM msg "
                            "WHERE fileid = %1").arg(id));
    if (query.next())
        nummsgs = query.value(0).toInt();
    if (nummsgs > 0) {
        int r = QMessageBox::warning(this, tr("Delete file"),
                    tr("Delete %1 and all its msgs?")
                    .arg(record.value(Ksfile_ksid).toString()),
                    QMessageBox::Yes | QMessageBox::No);
        if (r == QMessageBox::No) {
            QSqlDatabase::database().rollback();
            return;
        }

        query.exec(QString("DELETE FROM msg "
                           "WHERE fileid = %1").arg(id));
    }

    fileModel->removeRow(index.row());
    fileModel->submitAll();
    QSqlDatabase::database().commit();

    updateMsgView();
    fileView->setFocus();
}/*}}}*/

void MainForm::editmsgs()
{//弹出编辑框,这个函数在msgspanel没出来之前可以pass/*{{{*/
    int msgId = -1;
    int ksfileid = -1;
    QModelIndex index = msgView->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = msgModel->record(index.row());
        msgId = record.value(Msg_Id).toInt();
        ksfileid = record.value(Msg_Fileid).toInt();
    }

    EmployeeForm form(msgId,ksfileid,this);
    form.exec();
    updateMsgView();
}/*}}}*/

void MainForm::editfile()
{//弹出编辑框,这个函数在msgspanel没出来之前可以pass/*{{{*/
    int ksfileid = -1;
    QModelIndex index = fileView->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = fileModel->record(index.row());
        ksfileid = record.value(Ksfile_ksfileid).toInt();
    }
    FileForm form(ksfileid,this);
    form.exec();
    updateFileView();
}/*}}}*/

void MainForm::editkaoshi()
{//弹出编辑框,这个函数在msgspanel没出来之前可以pass/*{{{*/
    QString ksid = "";
    QModelIndex index = kaoshiView->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = kaoshiModel->record(index.row());
        ksid = record.value(Kstype_Pinyin).toString();
    }
    KaoshiForm form(ksid,this);
    form.exec();
    updateKaoshiView();
}/*}}}*/

void MainForm::createFilePanel()
{/*{{{*/
    filePanel = new QWidget;
    fileModel = new QSqlRelationalTableModel(this);
    fileModel->setTable("ksfile");
    fileModel->setSort(Ksfile_ksid, Qt::AscendingOrder);
    fileModel->select();

    fileView = new QTableView;
    fileView->setModel(fileModel);
    fileView->setItemDelegate(new QSqlRelationalDelegate(this));
    fileView->setSelectionMode(
            QAbstractItemView::SingleSelection);
    fileView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //fileView->setColumnHidden(Ksfile_ksfileid, true);
    fileView->resizeColumnsToContents();
    fileView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fileView->horizontalHeader()->setStretchLastSection(true);
    fileView->setColumnHidden(0, true);
    fileView->setColumnHidden(1, true);

    fileLabel = new QLabel(tr("Depar&tments"));
    fileLabel->setBuddy(fileView);

    connect(fileView->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex &,
                    const QModelIndex &)),
            this, SLOT(updateMsgView()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(fileLabel);
    layout->addWidget(fileView);
    filePanel->setLayout(layout);
}/*}}}*/

void MainForm::createMsgPanel()
{/*{{{*/
    msgPanel = new QWidget;
    msgModel = new QSqlRelationalTableModel(this);
    msgModel->setTable("ksmsg");
    msgModel->setRelation(3,
            QSqlRelation("ksfile","ksfileid","ksfileid")); //列名不能写错,如果第三个参数给的是string类型那么toint就失败了
    msgModel->setSort(Msg_Id, Qt::AscendingOrder);
    msgModel->setHeaderData(1, Qt::Horizontal,
    tr("消息内容"));
    msgModel->setHeaderData(2, Qt::Horizontal,
    tr("发布时间"));
    msgModel->setHeaderData(5, Qt::Horizontal,
    tr("失效时间"));

    msgView = new QTableView;
    msgView->setModel(msgModel);
    msgView->setSelectionMode(QAbstractItemView::SingleSelection);
    msgView->setSelectionBehavior(QAbstractItemView::SelectRows);
    msgView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    msgView->horizontalHeader()->setStretchLastSection(true);
    msgView->setColumnHidden(0, true);
    msgView->setColumnHidden(3, true);
    msgView->setColumnHidden(4, true);

    msgLabel = new QLabel(tr("E&mployees"));
    msgLabel->setBuddy(msgView);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(msgLabel);
    layout->addWidget(msgView);
    msgPanel->setLayout(layout);
}/*}}}*/

void MainForm::createKaoshiPanel() 
{/*{{{*/
    kaoshiPanel = new QWidget;
    kaoshiModel = new QSqlRelationalTableModel(this);
    kaoshiModel->setTable("kstype");
    kaoshiModel->select();
    kaoshiView = new QTableView;
    kaoshiView->setModel(kaoshiModel);
    kaoshiView->setSelectionMode(QAbstractItemView::SingleSelection);
    kaoshiView->setSelectionBehavior(QAbstractItemView::SelectRows);
    kaoshiView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    kaoshiView->horizontalHeader()->setStretchLastSection(true);

    kaoshiLabel = new QLabel(tr("考试版面"));
    kaoshiLabel->setBuddy(kaoshiView);

    connect(kaoshiView->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex &,
                    const QModelIndex &)),
            this, SLOT(updateFileView()));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(kaoshiLabel);
    layout->addWidget(kaoshiView);
    kaoshiPanel->setLayout(layout);
}/*}}}*/

void MainForm::updateFileView()
{/*{{{*/
    QModelIndex index = kaoshiView->currentIndex(); //获得当前行号
    if (index.isValid()) {
        QSqlRecord record = kaoshiModel->record(index.row()); //获得Depview的当前记录
        QString id = record.value("ksid").toString();
        fileModel->setFilter(QString("ksfile.ksid = \"%1\" ").arg(id));
        //fileModel->setFilter(QString("ksfile.ksid = \"gwyks\" "));
        fileLabel->setText(tr(" %1 文件管理")
                .arg(record.value("ksname").toString())); //设置标题
        //.arg(id)); 设置标题
    } else {
        fileModel->setFilter("ksid = -1");
        fileLabel->setText(tr("考试文件"));
    }
    fileModel->select();
    fileView->horizontalHeader()->setVisible(
            fileModel->rowCount() > 0);
}/*}}}*/

void MainForm::updateKaoshiView()
{/*{{{*/
    //QModelIndex index = kaoshiView->currentIndex(); //获得当前行号
    //if (index.isValid()) {
        //QSqlRecord record = kaoshiModel->record(index.row()); //获得Depview的当前记录
        //QString id = record.value("ksid").toString();
        ////kaoshiModel->setFilter(QString("ksfile.ksid = \"%1\" ").arg(id));
        ////kaoshiLabel->setText(tr(" %1 考试管理")
                ////.arg(record.value("ksname").toString())); //设置标题
        ////.arg(id)); 设置标题
    //} else {
        //fileModel->setFilter("ksid = -1");
        //fileLabel->setText(tr("考试文件"));
    //}
    kaoshiModel->select();
    kaoshiView->horizontalHeader()->setVisible(
            fileModel->rowCount() > 0);
}/*}}}*/

void MainForm::callserver()
{
    socket->connectToHost("58.87.119.233",9999);
    socket->write("UpdateAllPage");
    socket->disconnectFromHost();
}
