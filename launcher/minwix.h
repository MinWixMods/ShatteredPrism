#pragma once

#include <QFileInfo>
#include <QJsonValue>
#include <QString>
#include <fstream>

inline QString getRepository(){
    QFile file("repository.json");
    QFileInfo fileInfo(file.fileName());
    qDebug() << "Load" << file.fileName() << "Full path:" << fileInfo.absoluteFilePath();

    QString result;

    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
        result = jsonDoc["hausPedik"].toString();
        qDebug() << "https repository:" << result;
        file.close();
    }

    return result;
}

#include <functional>

class Application;
class MainWindow;
class Task;

void minWixLoadMainWidget(MainWindow* mainWindow);
void minWixLoadLauncher(Application* application);