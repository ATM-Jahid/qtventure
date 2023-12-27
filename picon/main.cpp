#include "widget.h"

#include <QApplication>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    QVBoxLayout *mainLayout = new QVBoxLayout(&w);
    //QListWidget *listWidget = new QListWidget(&w);

    QList<QList<QString>> bigList = {{"firefox-developer-edition", "QtProject-qtcreator", "foot"},
                            {"freetube", "krita", "gimp"}, {"nnn", "sxiv", "org.pwmt.zathura"}};

    for (const QList<QString> &list : bigList) {
        QVBoxLayout *wsLayout = new QVBoxLayout;

        for (const QString &currString : list) {
            qDebug() << currString;

            QLabel *iconLabel = new QLabel;

            //QIcon appIcon(iconPath);
            QIcon appIcon = QIcon::fromTheme(currString);
            QPixmap iconPix = appIcon.pixmap(32, 32);

            iconLabel->setPixmap(iconPix);
            iconLabel->setToolTip(currString);

            wsLayout->addWidget(iconLabel);
            wsLayout->setAlignment(Qt::AlignTop);
        }
        mainLayout->addLayout(wsLayout);
    }

    //layout->addWidget(listWidget);

    w.setLayout(mainLayout);
    w.show();
    return a.exec();
}
