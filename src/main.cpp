#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include "dock.h"

#include <LayerShellQt/Shell>
#include <LayerShellQt/Window>

int main(int argc, char *argv[])
{
    LayerShellQt::Shell::useLayerShell();

    QGuiApplication app(argc, argv);

    QQuickView view;

    LayerShellQt::Window *layerShell = LayerShellQt::Window::get(&view);
    if (!layerShell) {
        qWarning() << "Failed to get LayerShellQt window!";
        return -1;
    }

    layerShell->setLayer(LayerShellQt::Window::LayerBackground);
    layerShell->setAnchors({
        LayerShellQt::Window::AnchorTop,
        LayerShellQt::Window::AnchorBottom,
        LayerShellQt::Window::AnchorLeft,
        LayerShellQt::Window::AnchorRight
    });
    layerShell->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityOnDemand);
    layerShell->setExclusiveZone(-1);
    layerShell->setScope("cove");

    Dock dock;
    view.rootContext()->setContextProperty("dock", &dock);

    view.setSource(QUrl("qrc:/qml/Main.qml"));
    view.setColor(QColor(Qt::transparent));

    view.showFullScreen();

    return app.exec();
}
