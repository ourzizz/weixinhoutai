#include <QtSql>
#include <QtWidgets>
#include "FileForm.h"
#include <QMessageBox>

FileForm::FileForm(int fileid,QWidget *parent) : QDialog(parent)
{/*{{{*/ //fileid为文件列表中传入的文件id
    nameEdit = new QLineEdit; //姓名表单输入框
    nameLabel = new QLabel(tr("文件名称:"));
    nameLabel->setBuddy(nameEdit);

    kaoshiComboBox = new QComboBox;/*{{{*/ //下拉列表不用管
    kaoshiLabel = new QLabel(tr("所属考试"));
    kaoshiLabel->setBuddy(kaoshiComboBox);

    pubTimeEdit = new QDateEdit;
    pubTimeLabel = new QLabel(tr("发布时间"));
    pubTimeLabel->setBuddy(pubTimeEdit);

    webAddEdit= new QLineEdit;
    webAddLabel = new QLabel(tr("源地址"));
    webAddLabel->setBuddy(webAddEdit);

    addButton = new QPushButton(tr("添加文件"));
    deleteButton = new QPushButton(tr("删除文件"));
    closeButton = new QPushButton(tr("关闭"));
    submitButton = new QPushButton(tr("提交"));

    buttonBox = new QDialogButtonBox;/*{{{*/
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(submitButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::AcceptRole);/*}}}*/

    tableModel = new QSqlRelationalTableModel(this);/*{{{*/
    tableModel->setTable("ksfile");
    tableModel->setRelation(Ksfile_Ksid,
                            QSqlRelation("kstype", "ksid", "ksname"));
    tableModel->setFilter(QString("ksfile.ksfileid = %1 ").arg(fileid));
    tableModel->select(); //这个和对话框绑定


    QSqlTableModel *relationModel =
            tableModel->relationModel(Ksfile_Ksid);
    //if(-1 != id)
        //relationModel->setFilter(QString("ksfileid=%1").arg(fileid));
    kaoshiComboBox->setModel(relationModel);
    kaoshiComboBox->setModelColumn(
            //relationModel->fieldIndex("kstype.ksname"));
            relationModel->fieldIndex("ksname"));

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(nameEdit, Ksfile_Wjmc);
    mapper->addMapping(kaoshiComboBox, Ksfile_Ksid);
    mapper->addMapping(pubTimeEdit, Ksfile_Fileputime);
    mapper->addMapping(webAddEdit, Ksfile_Webaddress);

    if (fileid != -1) { //从msg窗口传过来的msg_id
        for (int row = 0; row < tableModel->rowCount(); ++row) {
            QSqlRecord record = tableModel->record(row);
            if (record.value(Ksfile_Ksfileid).toInt() == fileid) {
                mapper->setCurrentIndex(row);
                break;
            }
        }
    } else {
        mapper->toFirst();
        tableModel->insertRow(0);
        mapper->setCurrentIndex(0);
    }

    connect(addButton, SIGNAL(clicked()), this, SLOT(addFile()));
    connect(deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteFile()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(QDialog::done(0)));/*}}}*/
    connect(submitButton, SIGNAL(clicked()), this, SLOT(accept()));/*}}}*/
    //connect(submitButton, SIGNAL(clicked()), this, SLOT(manualSubmit()));[>}}}<]

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLabel, 1, 0);
    mainLayout->addWidget(nameEdit, 1, 1, 1, 2);
    mainLayout->addWidget(pubTimeLabel, 2, 0);
    mainLayout->addWidget(pubTimeEdit, 2, 1, 1, 2);
    mainLayout->addWidget(kaoshiLabel, 3, 0);
    mainLayout->addWidget(kaoshiComboBox, 3, 1, 1, 2);
    mainLayout->addWidget(webAddLabel, 4, 0);
    mainLayout->addWidget(webAddEdit, 4, 1,1,2);

    mainLayout->addWidget(buttonBox, 7, 0, 1, 3);
    mainLayout->setRowMinimumHeight(6, 10);
    mainLayout->setRowStretch(6, 1);
    mainLayout->setColumnStretch(2, 1);
    setLayout(mainLayout);

    nameEdit->setFocus();
    setWindowTitle(tr("文件管理"));
}/*}}}*/

void FileForm::done(int result)
{/*{{{*/ //所有关闭操作都会调用这个done函数，所以这里的submit要注释掉
    if(!(nameEdit->text().isEmpty()))
        mapper->submit();
    QDialog::done(result);
}/*}}}*/

void FileForm::manualSubmit()
{
    if(!(nameEdit->text().isEmpty()))
        mapper->submit();
}

void FileForm::addFile()
{/*{{{*/
    int row = mapper->currentIndex();
    if(!(nameEdit->text().isEmpty()))
        mapper->submit();
    tableModel->insertRow(row);
    mapper->setCurrentIndex(row);

    nameEdit->clear();
    pubTimeEdit->clear();
    webAddEdit->clear();
    nameEdit->setFocus();
}/*}}}*/

void FileForm::deleteFile()
{/*{{{*/
    int row = mapper->currentIndex();
    tableModel->removeRow(row);
    if(!(nameEdit->text().isEmpty()))
        mapper->submit();
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}/*}}}*/
