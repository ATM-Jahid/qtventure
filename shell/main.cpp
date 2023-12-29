#include "widget.h"

#include <LayerShellQt/Shell>
#include <LayerShellQt/Window>
#include <QApplication>
#include <QWidget>
#include <QWindow>
#include <QDebug>

int main(int argc, char *argv[])
{
    uint barWidth = 30;

    LayerShellQt::Shell::useLayerShell();
    QApplication a(argc, argv);

    Widget w;
    w.setStyleSheet("QWidget { background-color: teal; }");
    w.resize(barWidth, 600);
    w.show();
    w.hide();

    QWindow *lwin = w.windowHandle();
    qDebug() << lwin;

    if (LayerShellQt::Window *lsh = LayerShellQt::Window::get(lwin)) {
        qDebug() << lsh;
        lsh->setScope("atombar");
        lsh->setLayer(LayerShellQt::Window::LayerBottom);
        lsh->setExclusiveZone(barWidth);
        lsh->setAnchors(LayerShellQt::Window::AnchorRight);
    }
    w.show();

    return a.exec();
}
