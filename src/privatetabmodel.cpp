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

#include "privatetabmodel.h"
#include "declarativewebutils.h"

PrivateTabModel::PrivateTabModel(QObject *parent)
    : DeclarativeTabModel(1, parent),
      m_nextLinkId(1)
{
    // Startup should be synced to this.
    if (!m_loaded) {
        m_loaded = true;
        QMetaObject::invokeMethod(this, "loaded", Qt::QueuedConnection);
    }
}

PrivateTabModel::~PrivateTabModel()
{
}

int PrivateTabModel::createTab() {
    return nextTabId();
}

int PrivateTabModel::createLink(int tabId, QString url, QString title) {
    return m_nextLinkId++;
}

void PrivateTabModel::updateTitle(int tabId, int linkId, QString url, QString title)
{
}

void PrivateTabModel::removeTab(int tabId)
{
}

int PrivateTabModel::nextLinkId() {
}

void PrivateTabModel::updateTab(int tabId, QString url, QString title, QString path) {
}

void PrivateTabModel::navigateTo(int tabId, QString url, QString title, QString path) {
}

void PrivateTabModel::updateThumbPath(int tabId, QString path)
{
}
