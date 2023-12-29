#include "widget.h"

#include <QApplication>
#include <QByteArray>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPixmap>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QStyle>
#include <QVBoxLayout>

struct Tile {
    int t_pid;
    QString t_address;
    QString t_class;
    QString t_title;
};

QString findDesktopFile(const QString &appName) {
    QString desktopPath = "/usr/share/applications/";
    QString desktopFileName = appName + ".desktop";

    if (QFile::exists(desktopPath + desktopFileName)) {
        return desktopPath + desktopFileName;
    }

    // for classes that have something before (e.g. foot)
    QStringList matchingFiles = QDir(desktopPath).entryList(QStringList("*" + desktopFileName));
    if (matchingFiles.length() == 1) {
        return desktopPath + matchingFiles.first();
    }

    // for classes that have dashes to indicate versions (e.g. Gimp)
    int indexOfDash = appName.indexOf("-");
    if (indexOfDash != -1) {
        QStringList matchingFiles = QDir(desktopPath).entryList(QStringList("*" + appName.first(indexOfDash) + ".desktop"));
        if (matchingFiles.length() == 1) {
            return desktopPath + matchingFiles.first();
        }
    }

    return QString();
}

QString iconNameFromAppName(const QString &appName) {
    QString filePath = findDesktopFile(appName);

    if (filePath.isEmpty()) {
        return appName;
    }

    QSettings desktopFile(filePath, QSettings::IniFormat);
    QString iconName = desktopFile.value("Desktop Entry/Icon").toString();

    return iconName;
}

QList<QMap<QString, Tile>> readJson() {
    // Be careful about 0-indexed workspaces
    int num_wsp = 10;
    QList<QMap<QString, Tile>> tiles(num_wsp);

    QProcess process;
    process.start("sh", QStringList() << "-c" << "hyprctl -j clients");
    process.waitForFinished(-1);
    QByteArray output = process.readAllStandardOutput();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(output, &parseError);

    if (jsonDoc.isNull()) {
        qDebug() << "Error parsing JSON:" << parseError.errorString();
        return tiles;
    }

    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();
        for (const QJsonValue &jsonValue : jsonArray) {
            if (jsonValue.isObject()) {
                QJsonObject jsonObject = jsonValue.toObject();
                QJsonObject wspObject = jsonObject.value("workspace").toObject();

                int ws =  wspObject.value("id").toVariant().toInt();
                int pid = jsonObject.value("pid").toVariant().toInt();
                QString addr =  jsonObject.value("address").toVariant().toString();
                QString cls = jsonObject.value("class").toVariant().toString();
                QString title = jsonObject.value("title").toVariant().toString();

                // Get rid of nonexistent hypr clients
                if (ws != -1 && pid != -1) {
                    tiles[ws-1][addr] = {pid, addr, cls, title};
                }
            } else {
                qDebug() << "Array element is not a JSON object.";
            }
        }
    } else {
        qDebug() << "Top-level structure is not a JSON array.";
    }

    return tiles;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    QVBoxLayout *mainLayout = new QVBoxLayout(&w);

    QList<QMap<QString, Tile>> wspaces = readJson();
    QHash<QString, QIcon> iconCache;

    for (const QMap<QString, Tile> &list : wspaces) {
        QVBoxLayout *wsLayout = new QVBoxLayout;

        QMapIterator<QString, Tile> it(list);
        if (!it.hasNext()) {
            QLabel *emptyLabel = new QLabel;
            wsLayout->addWidget(emptyLabel);
        }

        while (it.hasNext()) {
            it.next();
            Tile tmp = it.value();

            QLabel *iconLabel = new QLabel;

            if (!iconCache.contains(tmp.t_class)) {
                QIcon appIcon = QIcon::fromTheme(iconNameFromAppName(tmp.t_class));

                // fallback icon
                if (appIcon.isNull()) {
                    appIcon = QIcon::fromTheme("tint2");
                }

                iconCache.insert(tmp.t_class, appIcon);
            }

            QIcon cachedIcon = iconCache.value(tmp.t_class);
            QPixmap iconPix = cachedIcon.pixmap(18, 18);

            iconLabel->setPixmap(iconPix);
            iconLabel->setToolTip(tmp.t_title);

            wsLayout->addWidget(iconLabel);
            wsLayout->setAlignment(Qt::AlignTop);
        }

        mainLayout->addLayout(wsLayout);
    }

    w.setStyleSheet("QWidget { background-color: black; }");
    w.resize(24, 720);
    w.setLayout(mainLayout);
    w.show();

    return a.exec();
}
