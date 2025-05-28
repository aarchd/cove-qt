#include "dock.h"
#include "helper/desktopfile.h"
#include "helper/desktopiconloader.h"

#include <QStandardPaths>
#include <QSettings>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QUrl>
#include <QPixmap>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Dock::Dock(QObject *parent)
    : QObject(parent)
{
    loadConfig();
    loadDesktopIcons();
}

int Dock::widthPercent() const { return m_widthPercent; }
int Dock::heightPercent() const { return m_heightPercent; }
int Dock::iconWidthPercent() const { return m_iconWidthPercent; }
int Dock::iconHeightPercent() const { return m_iconHeightPercent; }
int Dock::bottomMarginPercent() const { return m_bottomMarginPercent; }

QStringList Dock::iconPaths() const
{
    return m_iconPaths;
}

void Dock::loadConfig()
{
    const QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/cove/dock.json";
    QFile file(configPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open config file:" << configPath;
        return;
    }

    const QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    const QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << error.errorString();
        return;
    }
    if (!doc.isObject()) {
        qWarning() << "Config file root is not an object";
        return;
    }

    const QJsonObject root = doc.object();

    const QJsonObject dockObj = root.value("Dock").toObject();
    m_widthPercent = dockObj.value("width_percent").toInt(30);
    m_heightPercent = dockObj.value("height_percent").toInt(10);
    m_iconWidthPercent = dockObj.value("icon_width_percent").toInt(80);
    m_iconHeightPercent = dockObj.value("icon_height_percent").toInt(80);
    m_bottomMarginPercent = dockObj.value("bottom_margin_percent").toInt(2);

    m_apps.clear();
    const QJsonArray appsArray = root.value("Apps").toArray();
    for (const QJsonValue &val : appsArray) {
        if (val.isString()) {
            m_apps.append(val.toString());
        }
    }
}

void Dock::loadDesktopIcons()
{
    QSize iconSize(64, 64); 
    m_iconPaths = DesktopIconLoader::loadDesktopIcons(m_apps, iconSize);
    emit iconPathsChanged();
}
