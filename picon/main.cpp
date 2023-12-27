#include "widget.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QSettings>
#include <QVBoxLayout>

QString findDesktopFile(const QString &appName) {
    QString desktopPath = "/usr/share/applications/";
    QString desktopFileName = appName + ".desktop";

    if (QFile::exists(desktopPath + desktopFileName)) {
        return desktopPath + desktopFileName;
    }

    QStringList matchingFiles = QDir(desktopPath).entryList(QStringList("*" + desktopFileName));

    if (matchingFiles.length() == 1) {
        return desktopPath + matchingFiles.first();
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

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Widget w;
    QVBoxLayout *mainLayout = new QVBoxLayout(&w);

    QHash<QString, QIcon> iconCache;

    QList<QList<QString>> bigList = {{"firefoxdeveloperedition", "org.qt-project.qtcreator", "foot"},
                            {"FreeTube", "krita", "GIMP", "foot"}, {"nnn", "sxiv", "org.pwmt.zathura"}};

    for (const QList<QString> &list : bigList) {
        QVBoxLayout *wsLayout = new QVBoxLayout;

        for (const QString &currString : list) {
            qDebug() << currString;

            QLabel *iconLabel = new QLabel;

            if (!iconCache.contains(currString)) {
                QIcon appIcon = QIcon::fromTheme(iconNameFromAppName(currString));
                iconCache.insert(currString, appIcon);
            }

            QIcon cachedIcon = iconCache.value(currString);
            QPixmap iconPix = cachedIcon.pixmap(24, 24);

            iconLabel->setPixmap(iconPix);
            iconLabel->setToolTip(currString);

            wsLayout->addWidget(iconLabel);
            wsLayout->setAlignment(Qt::AlignTop);
        }
        mainLayout->addLayout(wsLayout);
    }

    w.setLayout(mainLayout);
    w.show();
    return a.exec();
}
