#include <QtSql>
#include <QtWidgets>
#include "KaoshiForm.h"
#include <QMessageBox>

KaoshiForm::KaoshiForm(QString pinyin,QWidget *parent) : QDialog(parent)
{/*{{{*/ //fileid为文件列表中传入的文件id
    nameEdit = new QLineEdit; //姓名表单输入框
    nameLabel = new QLabel(tr("考试名称:"));
    nameLabel->setBuddy(nameEdit);

    pinyinEdit = new QLineEdit;
    pinyinLabel = new QLabel(tr("首拼主键不能重复"));
    pinyinLabel->setBuddy(pinyinEdit);

    hotEdit= new QLineEdit;
    hotLabel = new QLabel(tr("手动热度调整"));
    hotLabel->setBuddy(hotEdit);

    addButton = new QPushButton(tr("添加考试"));
    //deleteButton = new QPushButton(tr("删除文件"));
    closeButton = new QPushButton(tr("关闭"));
    submitButton = new QPushButton(tr("提交"));

    buttonBox = new QDialogButtonBox;/*{{{*/
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(submitButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::AcceptRole);/*}}}*/

    tableModel = new QSqlRelationalTableModel(this);/*{{{*/
    tableModel->setTable("kstype");
    tableModel->select(); //这个和对话框绑定

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(pinyinEdit, Kstype_Pinyin);
    mapper->addMapping(nameEdit, Kstype_Name);
    mapper->addMapping(hotEdit, Kstype_Hot);

    if (pinyin != "" ) { //从msg窗口传过来的msg_id
        for (int row = 0; row < tableModel->rowCount(); ++row) {
            QSqlRecord record = tableModel->record(row);
            if (record.value(Kstype_Pinyin) == pinyin) {
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
    //connect(deleteButton, SIGNAL(clicked()),
           // this, SLOT(deleteFile()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(QDialog::done(0)));/*}}}*/
    connect(submitButton, SIGNAL(clicked()), this, SLOT(accept()));/*}}}*/
    //connect(submitButton, SIGNAL(clicked()), this, SLOT(manualSubmit()));[>}}}<]

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLabel, 1, 0);
    mainLayout->addWidget(nameEdit, 1, 1, 1, 2);
    mainLayout->addWidget(pinyinLabel, 2, 0);
    mainLayout->addWidget(pinyinEdit, 2, 1, 1, 2);
   // mainLayout->addWidget(kaoshiLabel, 3, 0);
   // mainLayout->addWidget(kaoshiComboBox, 3, 1, 1, 2);
    mainLayout->addWidget(hotLabel, 4, 0);
    mainLayout->addWidget(hotEdit, 4, 1,1,2);

    mainLayout->addWidget(buttonBox, 7, 0, 1, 3);
    mainLayout->setRowMinimumHeight(6, 10);
    mainLayout->setRowStretch(6, 1);
    mainLayout->setColumnStretch(2, 1);
    setLayout(mainLayout);

    nameEdit->setFocus();
    setWindowTitle(tr("文件管理"));
}/*}}}*/

void KaoshiForm::done(int result)
{/*{{{*/ //所有关闭操作都会调用这个done函数，所以这里的submit要注释掉
    mapper->submit();
    QDialog::done(result);
}/*}}}*/

void KaoshiForm::manualSubmit()
{
    mapper->submit();
}

void KaoshiForm::addFile()
{/*{{{*/
    int row = mapper->currentIndex();
    mapper->submit();
    tableModel->insertRow(row);
    mapper->setCurrentIndex(row);

    nameEdit->clear();
    pinyinEdit->clear();
    hotEdit->clear();
    nameEdit->setFocus();
}/*}}}*/

void KaoshiForm::deleteFile()
{/*{{{*/
    int row = mapper->currentIndex();
    tableModel->removeRow(row);
    mapper->submit();
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}/*}}}*/
