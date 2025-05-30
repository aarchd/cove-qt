#include "battery.h"

Battery::Battery(QObject *parent)
    : QObject(parent)
{
    GError *error = nullptr;
    m_client = up_client_new();
    if (!m_client) {
        qWarning() << "Failed to create UpClient";
        return;
    }

    m_device = up_client_get_display_device(m_client);
    if (!m_device) {
        qWarning() << "Failed to get display device";
        g_object_unref(m_client);
        m_client = nullptr;
        return;
    }

    g_signal_connect(m_device, "notify::percentage", G_CALLBACK(onDeviceChanged), this);
    g_signal_connect(m_device, "notify::state", G_CALLBACK(onDeviceChanged), this);

    onDeviceChanged(m_device, nullptr, this);
}

Battery::~Battery()
{
    if (m_device) {
        g_object_unref(m_device);
    }
    if (m_client) {
        g_object_unref(m_client);
    }
}

void Battery::onDeviceChanged(UpDevice *device, GParamSpec *, gpointer user_data)
{
    Battery *self = static_cast<Battery *>(user_data);
    gdouble newPercentage;
    guint newState;

    g_object_get(device,
                 "percentage", &newPercentage,
                 "state", &newState,
                 nullptr);

    if (!qFuzzyCompare(self->m_percentage + 1, newPercentage + 1)) {
        self->m_percentage = newPercentage;
        emit self->percentageChanged();
    }
    if (self->m_state != (int)newState) {
        self->m_state = (int)newState;
        emit self->stateChanged();
    }
}

int Battery::percentage() const { return m_percentage; }
int Battery::state() const { return m_state; }
