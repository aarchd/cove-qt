#pragma once

#include <QObject>
#include <libupower-glib/upower.h>
#include <glib-2.0/glib.h>

class Battery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int percentage READ percentage NOTIFY percentageChanged)
    Q_PROPERTY(int state READ state NOTIFY stateChanged)

public:
    explicit Battery(QObject *parent = nullptr);
    ~Battery();

    int percentage() const;
    int state() const;

Q_SIGNALS:
    void percentageChanged();
    void stateChanged();

private:
    static void onDeviceChanged(UpDevice *device, GParamSpec *pspec, gpointer user_data);

    UpClient *m_client = nullptr;
    UpDevice *m_device = nullptr;

    int m_percentage = 0;
    int m_state = 0;
};
