#include "widget.h"

#include <QApplication>
#include <QIcon>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    QVBoxLayout *layout = new QVBoxLayout(&w);
    QListWidget *listWidget = new QListWidget(&w);

    QList<QString> list = {"firefox-developer-edition", "QtProject-qtcreator", "foot",
                           "freetube", "krita", "gimp", "nnn", "sxiv", "org.pwmt.zathura"};

    for (const QString &currString : list) {
        qDebug() << currString;

        //QIcon appIcon(iconPath);
        QIcon appIcon = QIcon::fromTheme(currString);

        //QLabel iconLabel;
        //iconLabel.setPixmap(appIcon.pixmap(64, 64));

        //QLabel appName(currString);

        QListWidgetItem *item = new QListWidgetItem(appIcon, currString);

        listWidget->addItem(item);
    }

    layout->addWidget(listWidget);

    w.show();
    return a.exec();
}
