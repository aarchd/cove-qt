#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include "launcher.h"
#include "statusbar.h"

#include <LayerShellQt/Shell>
#include <LayerShellQt/Window>

int main(int argc, char *argv[])
{
    LayerShellQt::Shell::useLayerShell();

    QGuiApplication app(argc, argv);

    // Launcher
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
    launcherLayer->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
    launcherLayer->setExclusiveZone(-1);
    launcherLayer->setScope("cove-launcher");

    Launcher launcher;
    launcherView.rootContext()->setContextProperty("launcher", &launcher);

    launcherView.setSource(QUrl("qrc:/qml/launcher/Main.qml"));
    launcherView.setColor(QColor(Qt::transparent));

    launcherView.showFullScreen();

    // StatusBar
    QQuickView statusBarView;
    LayerShellQt::Window *statusBarLayer = LayerShellQt::Window::get(&statusBarView);

    if (!statusBarLayer) {
        qWarning() << "Failed to get LayerShellQt window!";
        return -1;
    }

    statusBarLayer->setLayer(LayerShellQt::Window::LayerOverlay);
    statusBarLayer->setAnchors({
        LayerShellQt::Window::AnchorTop,
        LayerShellQt::Window::AnchorLeft,
        LayerShellQt::Window::AnchorRight
    });
    statusBarLayer->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
    statusBarLayer->setScope("cove-statusbar");

    StatusBar statusBar;
    statusBarLayer->setExclusiveZone(statusBarView.screen()->size().height() * statusBar.heightPercent() / 100);
    statusBarView.rootContext()->setContextProperty("statusBar", &statusBar);

    statusBarView.setSource(QUrl("qrc:/qml/statusbar/Main.qml"));
    statusBarView.setColor(QColor(Qt::transparent));

    statusBarView.show();

    return app.exec();
}
