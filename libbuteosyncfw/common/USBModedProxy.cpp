/*
 * This file is part of buteo-syncfw package
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: Sateesh Kavuri <sateesh.kavuri@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */
/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -v -p USBModedProxy -c USBModedProxy usb_moded.xml
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file, with several edits.
 * If this file is to be regenerated, the changes must be backed up and merged
 */

#include <QDBusConnection>
#include "USBModedProxy.h"
#include "LogMacros.h"

using namespace Buteo;
static const QString USB_MODE_SERVICE("com.meego.usb_moded");
static const QString USB_MODE_OBJECT("/com/meego/usb_moded");
static const QString SYNC_MODE_NAME("pc_suite");
static const QString MTP_MODE_NAME("mtp_mode");

/*
 * Implementation of interface class USBModedProxy
 */

USBModedProxy::USBModedProxy(QObject *parent)
    : QDBusAbstractInterface(USB_MODE_SERVICE, USB_MODE_OBJECT, staticInterfaceName(), QDBusConnection::systemBus(),
                             parent),
      m_isConnected(false)
{
    FUNCTION_CALL_TRACE(lcButeoTrace);
    initUsbModeTracking();
}

USBModedProxy::~USBModedProxy()
{
}

void USBModedProxy::initUsbModeTracking()
{
    if (!QObject::connect(this, &USBModedProxy::sig_usb_state_ind,
                          this, &USBModedProxy::slotModeChanged)) {
        qCCritical(lcButeoCore) << "Failed to connect to USB moded signal! USB notifications will not be available.";
    }

    QDBusPendingReply<QString> reply = this->mode_request();
    QDBusPendingCallWatcher *watch = new QDBusPendingCallWatcher(reply, this);
    connect(watch, &QDBusPendingCallWatcher::finished,
            this,  &USBModedProxy::handleUsbModeReply);
}

void USBModedProxy::handleUsbModeReply(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<QString> reply = *call;

    if (reply.isError()) {
        qCWarning(lcButeoCore) << "Call to" << USB_MODE_SERVICE << "failed:" << reply.error();
    } else {
        slotModeChanged(reply.value());
    }

    call->deleteLater();
}

void USBModedProxy::slotModeChanged(const QString &mode)
{
    FUNCTION_CALL_TRACE(lcButeoTrace);

    bool isConnected = (mode == MTP_MODE_NAME || mode == SYNC_MODE_NAME);

    if (m_isConnected != isConnected) {
        m_isConnected = isConnected;
        emit usbConnection(m_isConnected);
    }
}

bool USBModedProxy::isUSBConnected()
{
    FUNCTION_CALL_TRACE(lcButeoTrace);
    return m_isConnected;
}
