#pragma once

#include <QStringList>
#include <QSize>

class DesktopIconLoader
{
public:
    // Load icons for given desktop files; cache PNGs in ~/.cache/cove/icons/
    // iconSize defaults to 64x64 but can be customized
    static QStringList loadDesktopIcons(const QStringList &desktopFileNames, const QSize &iconSize = QSize(64, 64));
};
