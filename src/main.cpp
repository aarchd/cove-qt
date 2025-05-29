#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include "launcher.h"

#include <LayerShellQt/Shell>
#include <LayerShellQt/Window>

int main(int argc, char *argv[])
{
    LayerShellQt::Shell::useLayerShell();

    QGuiApplication app(argc, argv);

    QQuickView launcherView;
    LayerShellQt::Window *launcherLayer = LayerShellQt::Window::get(&launcherView);

    if (!launcherLayer) {
        qWarning() << "Failed to get LayerShellQt window!";
        return -1;
    }

    launcherLayer->setLayer(LayerShellQt::Window::LayerBackground);
    launcherLayer->setAnchors({
        LayerShellQt::Window::AnchorTop,
        LayerShellQt::Window::AnchorBottom,
        LayerShellQt::Window::AnchorLeft,
        LayerShellQt::Window::AnchorRight
    });
    launcherLayer->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityOnDemand);
    launcherLayer->setExclusiveZone(-1);
    launcherLayer->setScope("cove");

    Launcher launcher;
    launcherView.rootContext()->setContextProperty("launcher", &launcher);

    launcherView.setSource(QUrl("qrc:/qml/Main.qml"));
    launcherView.setColor(QColor(Qt::transparent));

    launcherView.showFullScreen();

    return app.exec();
}
