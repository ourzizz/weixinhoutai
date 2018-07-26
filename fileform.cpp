#include <QtSql>
#include <QtWidgets>
#include "FileForm.h"
#include <QMessageBox>

FileForm::FileForm(QString ksid,QWidget *parent)
    : QDialog(parent)
{/*{{{*/
    nameEdit = new QLineEdit; //姓名表单输入框
    nameLabel = new QLabel(tr("文件名称:"));
    nameLabel->setBuddy(nameEdit);


    pubTimeEdit = new QLineEdit;
    pubtimeLabel = new QLabel(tr("发布时间"));
    pubtimeLabel->setBuddy(pubTimeEdit);

    webAddEdit= new QLineEdit;
    webAddLabel = new QLabel(tr("所属考试"));
    webAddLabel->setBuddy(webAddEdit);

    addButton = new QPushButton(tr("添加文件"));
    deleteButton = new QPushButton(tr("删除文件"));
    closeButton = new QPushButton(tr("关闭"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::AcceptRole);/*}}}*/

    tableModel = new QSqlRelationalTableModel(this);
    tableModel->setTable("ksfile");
    //tableModel->setRelation(Ksfile_Ksfileid,
                            //QSqlRelation("kstype", "ksid", "ksid"));
    tableModel->setFilter(QString("ksfile.ksid = \"%1\" ").arg(fileid));
    //tableModel->setSort(Msg_Deadtime, Qt::AscendingOrder);
    tableModel->select(); //这个和对话框绑定

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(nameEdit, Msg_Msgcontent);
    mapper->addMapping(departmentComboBox, Msg_Fileid);
    mapper->addMapping(nameLineEdit, Msg_msgpubtime);
    mapper->addMapping(emailEdit, Msg_Ksid);
    mapper->addMapping(startDateEdit, Msg_Deadtime);

    if (id != -1) { //从msg窗口传过来的msg_id
        for (int row = 0; row < tableModel->rowCount(); ++row) {
            QSqlRecord record = tableModel->record(row);
            if (record.value(Msg_Id).toInt() == id) {
                mapper->setCurrentIndex(row);
                break;
            }
        }
    } else {
        mapper->toFirst();
    }

    connect(addButton, SIGNAL(clicked()), this, SLOT(addFile()));
    connect(deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteFile()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));


    QGridLayout *mainLayout = new QGridLayout;
    //mainLayout->addLayout(topButtonLayout, 0, 0, 1, 3);
    mainLayout->addWidget(nameLabel, 1, 0);
    mainLayout->addWidget(nameEdit, 1, 1, 1, 2);
    mainLayout->addWidget(pubtimeLabel, 2, 0);
    mainLayout->addWidget(departmentComboBox, 2, 1, 1, 2);
    mainLayout->addWidget(extensionLabel, 3, 0);
    mainLayout->addWidget(nameLineEdit, 3, 1);
    mainLayout->addWidget(emailLabel, 4, 0);
    mainLayout->addWidget(emailEdit, 4, 1, 1, 2);
    mainLayout->addWidget(startDateLabel, 5, 0);
    mainLayout->addWidget(startDateEdit, 5, 1);
    mainLayout->addWidget(buttonBox, 7, 0, 1, 3);
    mainLayout->setRowMinimumHeight(6, 10);
    mainLayout->setRowStretch(6, 1);
    mainLayout->setColumnStretch(2, 1);
    setLayout(mainLayout);

    nameEdit->setFocus();
    setWindowTitle(tr("文件管理"));
}/*}}}*/

void FileForm::done(int result)
{/*{{{*/
    mapper->submit();
    QDialog::done(result);
}/*}}}*/

void FileForm::addFile()
{/*{{{*/
    int row = mapper->currentIndex();
    mapper->submit();
    tableModel->insertRow(row);
    mapper->setCurrentIndex(row);

    nameEdit->clear();
    nameLineEdit->clear();
    startDateEdit->setDate(QDate::currentDate());
    nameEdit->setFocus();
}/*}}}*/

void FileForm::deleteFile()
{/*{{{*/
    int row = mapper->currentIndex();
    tableModel->removeRow(row);
    mapper->submit();
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}/*}}}*/
