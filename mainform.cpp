#include <QtGui>
#include <QtSql>
#include <String>
#include "employeeform.h"
#include "mainform.h"

MainForm::MainForm()
{/*{{{*/
    createDepartmentPanel();//创建dpt ep 两个窗口
    createEmployeePanel();
    createKaoshiPanel();

    splitter = new QSplitter(Qt::Vertical);
    splitter->setFrameStyle(QFrame::StyledPanel);
    splitter->addWidget(departmentPanel);
    splitter->addWidget(employeePanel);

    //kaoshisplitter = new QSplitter(Qt::Vertical);
    //kaoshisplitter->addWidget(kaoshiPanel);

    addButton = new QPushButton(tr("&Add Dept."));
    deleteButton = new QPushButton(tr("&Delete Dept."));
    editButton = new QPushButton(tr("&Edit Employees..."));
    quitButton = new QPushButton(tr("&Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(editButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::AcceptRole);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addDepartment()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteDepartment()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editEmployees()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(kaoshiPanel);
    mainLayout->addWidget(splitter);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    setWindowTitle(tr("Staff Manager"));
    //kaoshiView->setCurrentIndex(kaoshiModel->index(0, 0));
    departmentView->setCurrentIndex(departmentModel->index(0, 0));
}/*}}}*/

void MainForm::updateMsgView()
{/*{{{*/
    QModelIndex index = departmentView->currentIndex(); //获得Depview的当前行号
    if (index.isValid()) {
        QSqlRecord record = departmentModel->record(index.row()); //获得Depview的当前记录
        int id = record.value("ksfileid").toInt();
        employeeModel->setFilter(QString("ksmsg.ksfileid = %1").arg(id));
        employeeLabel->setText(tr("配置 %1 考试的消息msg")
                               .arg(record.value("ksid").toString())); //设置标题
                               //.arg(id)); 设置标题
    } else {
        employeeModel->setFilter("ksfileid = -1");
        employeeLabel->setText(tr("E&mployees"));
    }
    employeeModel->select();
    employeeView->horizontalHeader()->setVisible(
            employeeModel->rowCount() > 0);
            

}/*}}}*/

void MainForm::addDepartment()
{/*{{{*/
    int row = departmentModel->rowCount();
    departmentModel->insertRow(row);
    QModelIndex index = departmentModel->index(row, Department_Name);
    departmentView->setCurrentIndex(index);
    departmentView->edit(index);
}/*}}}*/

void MainForm::deleteDepartment()
{/*{{{*/
    QModelIndex index = departmentView->currentIndex();
    if (!index.isValid())
        return;

    QSqlDatabase::database().transaction();
    QSqlRecord record = departmentModel->record(index.row());
    int id = record.value(Department_Id).toInt();
    int numEmployees = 0;

    QSqlQuery query(QString("SELECT COUNT(*) FROM employee "
                            "WHERE departmentid = %1").arg(id));
    if (query.next())
        numEmployees = query.value(0).toInt();
    if (numEmployees > 0) {
        int r = QMessageBox::warning(this, tr("Delete Department"),
                    tr("Delete %1 and all its employees?")
                    .arg(record.value(Department_Name).toString()),
                    QMessageBox::Yes | QMessageBox::No);
        if (r == QMessageBox::No) {
            QSqlDatabase::database().rollback();
            return;
        }

        query.exec(QString("DELETE FROM employee "
                           "WHERE departmentid = %1").arg(id));
    }

    departmentModel->removeRow(index.row());
    departmentModel->submitAll();
    QSqlDatabase::database().commit();

    updateMsgView();
    departmentView->setFocus();
}/*}}}*/

void MainForm::editEmployees()
{//弹出编辑框,这个函数在Employeespanel没出来之前可以pass/*{{{*/
    int employeeId = -1;
    QModelIndex index = employeeView->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = employeeModel->record(index.row());
        employeeId = record.value(Employee_Id).toInt();
    }

    EmployeeForm form(employeeId, this);
    form.exec();
    updateMsgView();
}/*}}}*/

void MainForm::createDepartmentPanel()
{/*{{{*/
    departmentPanel = new QWidget;

    departmentModel = new QSqlRelationalTableModel(this);
    departmentModel->setTable("ksfile");
    //departmentModel->setRelation(1,
            //QSqlRelation("kstype", "ksid", "ksname"));
    departmentModel->setSort(Department_Name, Qt::AscendingOrder);
    //departmentModel->setHeaderData(Department_Name, Qt::Horizontal,
                                   //tr("ksid."));
    //departmentModel->setHeaderData(Department_LocationId,
                                   //Qt::Horizontal, tr("pubtime"));
    departmentModel->select();

    departmentView = new QTableView;
    departmentView->setModel(departmentModel);
    departmentView->setItemDelegate(new QSqlRelationalDelegate(this));
    departmentView->setSelectionMode(
            QAbstractItemView::SingleSelection);
    departmentView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //departmentView->setColumnHidden(Department_Id, true);
    departmentView->resizeColumnsToContents();
    departmentView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    departmentView->horizontalHeader()->setStretchLastSection(true);

    departmentLabel = new QLabel(tr("Depar&tments"));
    departmentLabel->setBuddy(departmentView);

    connect(departmentView->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex &,
                                     const QModelIndex &)),
            this, SLOT(updateMsgView()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(departmentLabel);
    layout->addWidget(departmentView);
    departmentPanel->setLayout(layout);
}/*}}}*/

void MainForm::createEmployeePanel()
{/*{{{*/
    employeePanel = new QWidget;
    employeeModel = new QSqlRelationalTableModel(this);
    employeeModel->setTable("ksmsg");
    employeeModel->setRelation(3,
            QSqlRelation("ksfile","ksfileid","ksid"));
    //employeeModel->setSort(Employee_Name, Qt::AscendingOrder);
    //employeeModel->setHeaderData(2, Qt::Horizontal,
                                 //tr("Name"));
    //employeeModel->setHeaderData(3, Qt::Horizontal,
                                 //tr("Ext."));
    //employeeModel->setHeaderData(6, Qt::Horizontal,
                                 //tr("Email"));

    employeeView = new QTableView;
    employeeView->setModel(employeeModel);
    employeeView->setSelectionMode(QAbstractItemView::SingleSelection);
    employeeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    employeeView->horizontalHeader()->setStretchLastSection(true);
    employeeView->setColumnHidden(0, true);
    employeeView->setColumnHidden(3, true);
    employeeView->setColumnHidden(4, true);

    employeeLabel = new QLabel(tr("E&mployees"));
    employeeLabel->setBuddy(employeeView);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(employeeLabel);
    layout->addWidget(employeeView);
    employeePanel->setLayout(layout);
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

    kaoshiLabel = new QLabel(tr("E&mployees"));
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
        departmentModel->setFilter(QString("ksfile.ksid = \"%1\" ").arg(id));
        //departmentModel->setFilter(QString("ksfile.ksid = \"gwyks\" "));
        departmentLabel->setText(tr(" %1 文件管理")
                               .arg(record.value("ksname").toString())); //设置标题
                               //.arg(id)); 设置标题
    } else {
        departmentModel->setFilter("ksid = -1");
        departmentLabel->setText(tr("考试文件"));
    }
    departmentModel->select();
    departmentView->horizontalHeader()->setVisible(
            departmentModel->rowCount() > 0);
            

}/*}}}*/
