#include <QtGui>
#include <QtSql>
#include <cstdlib>
#include <QtWidgets>

#include "mainform.h"

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    //db.setHostName("58.87.119.233");
    db.setHostName("192.168.1.107");
    db.setDatabaseName("bjks2");
    db.setUserName("root");  
    db.setPassword("123123");  
    if (!db.open()) {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                             db.lastError().text());
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    createConnection();
    MainForm form;
    form.resize(500, 600);
    form.show();
    return app.exec();
}
