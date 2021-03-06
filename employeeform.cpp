#include <QtSql>
#include <QtWidgets>
#include "employeeform.h"
#include <QMessageBox>

EmployeeForm::EmployeeForm(int id,int fileid,QWidget *parent) 
    : QDialog(parent) //消息类
{/*{{{*///传入消息列表中的被选中行的消息id和消息所属文件的id号
    nameEdit = new QLineEdit; //姓名表单输入框
    nameLabel = new QLabel(tr("消息内容:"));
    nameLabel->setBuddy(nameEdit);

    departmentComboBox = new QComboBox;/*{{{*/ //下拉列表不用管
    departmentLabel = new QLabel(tr("所属文件"));
    departmentLabel->setBuddy(departmentComboBox);

    extensionLineEdit = new QDateEdit;
    extensionLineEdit->setCalendarPopup(true);
    QDate today = QDate::currentDate();
    extensionLineEdit->setDateRange(today.addDays(-290), today.addDays(290));
    extensionLabel = new QLabel(tr("发布时间"));
    extensionLabel->setBuddy(extensionLineEdit);

    emailEdit = new QLineEdit;
    emailLabel = new QLabel(tr("所属考试"));
    emailLabel->setBuddy(emailEdit);

    startDateEdit = new QDateEdit;
    startDateEdit->setCalendarPopup(true);
    //QDate today = QDate::currentDate();
    startDateEdit->setDateRange(today.addDays(-90), today.addDays(90));
    startDateLabel = new QLabel(tr("&失效时间:"));
    startDateLabel->setBuddy(startDateEdit);
    addButton = new QPushButton(tr("&Add"));
    deleteButton = new QPushButton(tr("&Delete"));
    closeButton = new QPushButton(tr("&Close"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::AcceptRole);/*}}}*/

    tableModel = new QSqlRelationalTableModel(this);
    tableModel->setTable("ksmsg");
    tableModel->setRelation(Msg_Fileid,
                            QSqlRelation("ksfile", "ksfileid", "wjmc"));
    tableModel->setFilter(QString("ksmsg.ksfileid = %1").arg(fileid));
    tableModel->setSort(Msg_Deadtime, Qt::AscendingOrder);
    tableModel->select(); //这个和对话框绑定

    QSqlTableModel *relationModel =
            tableModel->relationModel(Msg_Fileid);
    if(-1 != id)
        relationModel->setFilter(QString("ksfileid=%1").arg(fileid));
    departmentComboBox->setModel(relationModel);
    departmentComboBox->setModelColumn(
            relationModel->fieldIndex("wjmc"));

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(nameEdit, Msg_Msgcontent);
    mapper->addMapping(departmentComboBox, Msg_Fileid);
    mapper->addMapping(extensionLineEdit, Msg_msgpubtime);
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
    } else {//id=-1表示没有在窗口选取任何行
        tableModel->insertRow(0);
        mapper->setCurrentIndex(0);
        //mapper->toFirst();
    }

    connect(addButton, SIGNAL(clicked()), this, SLOT(addEmployee()));
    connect(deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteEmployee()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLabel, 1, 0);
    mainLayout->addWidget(nameEdit, 1, 1, 1, 2);
    mainLayout->addWidget(departmentLabel, 2, 0);
    mainLayout->addWidget(departmentComboBox, 2, 1, 1, 2);
    mainLayout->addWidget(extensionLabel, 3, 0);
    mainLayout->addWidget(extensionLineEdit, 3, 1);
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
    setWindowTitle(tr("消息管理"));
}/*}}}*/

void EmployeeForm::done(int result)
{/*{{{*/
    if(!(nameEdit->text().isEmpty()))
        mapper->submit();
    QDialog::done(result);
}/*}}}*/

void EmployeeForm::addEmployee()
{/*{{{*/
    int row = mapper->currentIndex();
    if(!(nameEdit->text().isEmpty()))
        mapper->submit();
    tableModel->insertRow(row);
    mapper->setCurrentIndex(row);

    nameEdit->clear();
    extensionLineEdit->clear();
    startDateEdit->setDate(QDate::currentDate());
    nameEdit->setFocus();
}/*}}}*/

void EmployeeForm::deleteEmployee()
{/*{{{*/
    int row = mapper->currentIndex();
    if(!(nameEdit->text().isEmpty()))
        tableModel->removeRow(row);
    mapper->submit();
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}/*}}}*/
