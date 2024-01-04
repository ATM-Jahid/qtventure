#include <QGuiApplication>
#include <QQuickView>
#include <QDebug>
#include <LayerShellQt/Shell>
#include <LayerShellQt/Window>

int main(int argc, char *argv[])
{
    LayerShellQt::Shell::useLayerShell();
    QGuiApplication app(argc, argv);

    QQuickWindow::setDefaultAlphaBuffer(true);

    QQuickView window;
    const QUrl url(u"qrc:/shell2/Main.qml"_qs);

    window.setSource(url);
    window.setColor(QColor(0,0,0,0));
    qDebug() << &window;

    if (auto layerShellWindow = LayerShellQt::Window::get(&window)) {
        layerShellWindow->setScope("bar");
        layerShellWindow->setLayer(LayerShellQt::Window::LayerTop);
        layerShellWindow->setAnchors(LayerShellQt::Window::AnchorLeft);
        layerShellWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
        layerShellWindow->setExclusiveZone(30);
    }

    window.show();

    return app.exec();
}
