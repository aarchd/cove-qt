#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQmlContext>
#include "launcher.h"
#include "statusbar.h"

#include <LayerShellQt/Shell>
#include <LayerShellQt/Window>

int main(int argc, char *argv[])
{
    LayerShellQt::Shell::useLayerShell();
    QGuiApplication app(argc, argv);

    QQmlEngine engine;

    //Launcher
    QQuickWindow launcherWindow;
    LayerShellQt::Window *launcherLayer = LayerShellQt::Window::get(&launcherWindow);

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
    QQmlContext *launcherContext = new QQmlContext(engine.rootContext());
    launcherContext->setContextProperty("launcher", &launcher);

    QQmlComponent launcherComponent(&engine, QUrl("qrc:/qml/launcher/Main.qml"));
    QObject *launcherRoot = launcherComponent.create(launcherContext);
    QQuickItem *launcherItem = qobject_cast<QQuickItem *>(launcherRoot);
    launcherItem->setParentItem(launcherWindow.contentItem());

    launcherWindow.setColor(Qt::transparent);
    launcherWindow.showFullScreen();

    //StatusBar
    QQuickWindow statusBarWindow;
    LayerShellQt::Window *statusBarLayer = LayerShellQt::Window::get(&statusBarWindow);

    statusBarLayer->setLayer(LayerShellQt::Window::LayerOverlay);
    statusBarLayer->setAnchors({
        LayerShellQt::Window::AnchorTop,
        LayerShellQt::Window::AnchorLeft,
        LayerShellQt::Window::AnchorRight
    });
    statusBarLayer->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
    statusBarLayer->setScope("cove-statusbar");

    StatusBar statusBar;
    QQmlContext *statusContext = new QQmlContext(engine.rootContext());
    statusContext->setContextProperty("statusBar", &statusBar);
    statusBarWindow.setWidth(statusBarWindow.screen()->size().width());
    statusBarWindow.setHeight(statusBarWindow.screen()->size().height() * statusBar.heightPercent() / 100);

    QQmlComponent statusComponent(&engine, QUrl("qrc:/qml/statusbar/Main.qml"));
    QObject *statusRoot = statusComponent.create(statusContext);
    QQuickItem *statusItem = qobject_cast<QQuickItem *>(statusRoot);
    statusItem->setParentItem(statusBarWindow.contentItem());

    statusBarLayer->setExclusiveZone(statusBarWindow.screen()->size().height() * statusBar.heightPercent() / 100);
    statusBarWindow.setColor(Qt::transparent);
    statusBarWindow.show();

    return app.exec();
}
