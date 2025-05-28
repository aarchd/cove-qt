#pragma once

#include <QStringList>
#include <QSize>

class DesktopIconLoader
{
public:
    static QStringList loadDesktopIcons(const QStringList &desktopFileNames, const QSize &iconSize = QSize(64, 64));
};
