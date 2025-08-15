#include "onimai.h"

#include <QNetworkRequest>
#include "Application.h"
#include "InstanceImportTask.h"
#include "InstanceList.h"
#include "ui/MainWindow.h"
#include "ui/dialogs/CustomMessageBox.h"
#include "ui/dialogs/ProgressDialog.h"
#include "ui/pages/modplatform/ImportPage.h"

#include "FastFileIconProvider.h"
#include "minecraft/auth/AccountList.h"
#include "ui/dialogs/OfflineLoginDialog.h"

bool isFirst = true;

void installModPack(MainWindow* mainWindow, QString url, QString packName)
{
    QMap<QString, QString> map;
    InstanceImportTask* main = new InstanceImportTask(QUrl::fromUserInput(url), mainWindow, std::move(map));
    main->setName(packName);
    main->setShouldOverride(true);
    mainWindow->instanceFromInstanceTask(main);

}

void loadVersionJson(MainWindow* mainWindow, QJsonObject json)
{
    bool update = true;

    QFile file("versionOnimai.json");
    QFileInfo fileInfo(file.fileName());
    qDebug() << "Load" << file.fileName() << "Full path:" << fileInfo.absoluteFilePath();
    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
        update = jsonDoc["version"].toInt() != json["version"].toInt();
        file.close();
    }

    if (update) {
        QString name = "ONIMAI";
        if (json.contains("name_pack"))
            name = json["name_pack"].toString();
        installModPack(mainWindow, REPOSITORY_VERSION + json["file"].toString(), name);

        file.open(QIODevice::WriteOnly);
        file.write(QJsonDocument(json).toJson());
        file.close();
    }
}

void onimaiLoadMainWidget(MainWindow* mainWindow)
{
#ifdef REPOSITORY_VERSION
    if (isFirst) {
        if (APPLICATION->accounts().get()->m_accounts.size() == 0) {
            MinecraftAccountPtr account = OfflineLoginDialog::newAccount(mainWindow,  QObject::tr("Please enter your desired username to add your offline account."));

            if (account) {
                APPLICATION->accounts().get()->addAccount(account);
                if (APPLICATION->accounts().get()->m_accounts.count() == 1) {
                    APPLICATION->accounts().get()->setDefaultAccount(account);
                }
            }
        }

        auto *manager = new QNetworkAccessManager(mainWindow);

        QObject::connect(manager, &QNetworkAccessManager::finished, mainWindow, [mainWindow, manager](QNetworkReply* reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
                loadVersionJson(mainWindow, document.object());
            }
            reply->deleteLater();
        });
        manager->get(QNetworkRequest(QUrl(REPOSITORY_VERSION +"version.json")));

        isFirst = false;
    }
#endif
}

void onimaiLoadLauncher(Application* application) {}