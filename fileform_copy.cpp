#include <QtSql>
#include <QtWidgets>
#include "FileForm.h"
#include <QMessageBox>

FileForm::FileForm(int id,int fileid,QWidget *parent)
    : QDialog(parent)
{/*{{{*/
    nameEdit = new QLineEdit; //姓名表单输入框
    nameLabel = new QLabel(tr("消息内容:"));
    nameLabel->setBuddy(nameEdit);

    departmentComboBox = new QComboBox;/*{{{*/ //下拉列表不用管
    pubtimeLabel = new QLabel(tr("所属文件"));
    pubtimeLabel->setBuddy(departmentComboBox);

    nameLineEdit = new QLineEdit;
    nameLineEdit->setValidator(new QIntValidator(0, 99999, this));

    extensionLabel = new QLabel(tr("发布时间"));
    extensionLabel->setBuddy(nameLineEdit);

    emailEdit = new QLineEdit;
    emailLabel = new QLabel(tr("所属考试"));
    emailLabel->setBuddy(emailEdit);

    startDateEdit = new QDateEdit;
    startDateEdit->setCalendarPopup(true);
    QDate today = QDate::currentDate();
    startDateEdit->setDateRange(today.addDays(-90), today.addDays(90));
    startDateLabel = new QLabel(tr("&失效时间:"));
    startDateLabel->setBuddy(startDateEdit);

    //firstButton = new QPushButton(tr("<< &First"));
    //previousButton = new QPushButton(tr("< &Previous"));
    //nextButton = new QPushButton(tr("&Next >"));
    //lastButton = new QPushButton(tr("&Last >>"));

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
    departmentComboBox->setModel(relationModel);
    departmentComboBox->setModelColumn(
            relationModel->fieldIndex("wjmc"));

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

    //connect(firstButton, SIGNAL(clicked()), mapper, SLOT(toFirst()));
    //connect(previousButton, SIGNAL(clicked()),
            //mapper, SLOT(toPrevious()));
    //connect(nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    //connect(lastButton, SIGNAL(clicked()), mapper, SLOT(toLast()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addFile()));
    connect(deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteFile()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));

    //QHBoxLayout *topButtonLayout = new QHBoxLayout;
    //topButtonLayout->setContentsMargins(20, 0, 20, 5);
    //topButtonLayout->addStretch();
    //topButtonLayout->addWidget(firstButton);
    //topButtonLayout->addWidget(previousButton);
    //topButtonLayout->addWidget(nextButton);
    //topButtonLayout->addWidget(lastButton);
    //topButtonLayout->addStretch();

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

    /*if (id == -1) {
        nextButton->setFocus();
    } else {
        nameEdit->setFocus();
    }*/
    nameEdit->setFocus();


    setWindowTitle(tr("消息管理"));
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
