/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Petri M. Gerdt <petri.gerdt@jolla.com>
** Contact: Raine Makelainen <raine.makelainen@jolla.com>
**
****************************************************************************/

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "declarativeprivatetabmodel.h"
#include "declarativewebutils.h"

DeclarativePrivateTabModel::DeclarativePrivateTabModel(QObject *parent)
    : DeclarativeTabModel(1, parent),
      m_nextLinkId(1)
{
    // Startup should be synced to this.
    if (!m_loaded) {
        m_loaded = true;
        QMetaObject::invokeMethod(this, "loaded", Qt::QueuedConnection);
    }
}

DeclarativePrivateTabModel::~DeclarativePrivateTabModel()
{
}

int DeclarativePrivateTabModel::createTab() {
    return nextTabId();
}

int DeclarativePrivateTabModel::createLink(int tabId, QString url, QString title) {
    return m_nextLinkId++;
}

void DeclarativePrivateTabModel::updateTitle(int tabId, int linkId, QString url, QString title)
{
}

void DeclarativePrivateTabModel::removeTab(int tabId)
{
}

int DeclarativePrivateTabModel::nextLinkId() {
}

void DeclarativePrivateTabModel::updateTab(int tabId, QString url, QString title, QString path) {
}

void DeclarativePrivateTabModel::navigateTo(int tabId, QString url, QString title, QString path) {
}

void DeclarativePrivateTabModel::updateThumbPath(int tabId, QString path)
{
}
