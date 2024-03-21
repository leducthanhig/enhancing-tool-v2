#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    setTheme();

    MainWindow w;
    w.show();

    QSharedMemory sharedMemory;
    sharedMemory.setKey("EnhancingToolV2");

    if (sharedMemory.attach()) {
        QMessageBox msg(QMessageBox::Critical, "Error!!!", "Another task of this application is already running!", QMessageBox::Close);
        msg.setStyleSheet("QPushButton{height: 25px}");
        msg.exec();
        return 1;
    }
    sharedMemory.create(1);

    return app.exec();
}