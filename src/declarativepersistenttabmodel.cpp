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

#include "declarativepersistenttabmodel.h"
#include "dbmanager.h"
#include "declarativewebutils.h"

DeclarativePersistentTabModel::DeclarativePersistentTabModel(QObject *parent)
    : DeclarativeTabModel(DBManager::instance()->getMaxTabId() + 1, parent)
{
    connect(DBManager::instance(), SIGNAL(tabsAvailable(QList<Tab>)),
            this, SLOT(tabsAvailable(QList<Tab>)));
    connect(DBManager::instance(), SIGNAL(tabChanged(Tab)),
            this, SLOT(tabChanged(Tab)));
    connect(DeclarativeWebUtils::instance(), SIGNAL(beforeShutdown()),
            this, SLOT(saveActiveTab()));
}

DeclarativePersistentTabModel::~DeclarativePersistentTabModel()
{
}

void DeclarativePersistentTabModel::classBegin()
{
    DBManager::instance()->getAllTabs();
}

void DeclarativePersistentTabModel::tabsAvailable(QList<Tab> tabs)
{
    beginResetModel();
    int oldCount = count();
    m_tabs.clear();
    m_tabs = tabs;

    if (m_tabs.count() > 0) {
        QString activeTabId = DBManager::instance()->getSetting("activeTabId");
        bool ok = false;
        int tabId = activeTabId.toInt(&ok);
        int index = findTabIndex(tabId);
        if (index >= 0) {
            m_activeTab = m_tabs[index];
        } else {
            // Fallback for browser update as this "activeTabId" is a new setting.
            m_activeTab = m_tabs[0];
        }
        emit activeTabIndexChanged();
    } else {
        emit tabsCleared();
    }

    endResetModel();

    if (count() != oldCount) {
        emit countChanged();
    }

    int maxTabId = DBManager::instance()->getMaxTabId();
    if (m_nextTabId != maxTabId + 1) {
        m_nextTabId = maxTabId + 1;
        emit nextTabIdChanged();
    }

    // Startup should be synced to this.
    if (!m_loaded) {
        m_loaded = true;
        emit loadedChanged();
    }
}

void DeclarativePersistentTabModel::tabChanged(const Tab &tab)
{
#if DEBUG_LOGS
    qDebug() << "new tab data:" << &tab;
#endif
    if (m_tabs.isEmpty()) {
        qWarning() << "No tabs!";
        return;
    }

    int i = findTabIndex(tab.tabId());
    if (i > -1) {
        QVector<int> roles;
        Tab oldTab = m_tabs[i];
        if (oldTab.url() != tab.url()) {
            roles << UrlRole;
        }
        if (oldTab.title() != tab.title()) {
            roles << TitleRole;
        }
        if (oldTab.thumbnailPath() != tab.thumbnailPath()) {
            roles << ThumbPathRole;
        }
        m_tabs[i] = tab;
        QModelIndex start = index(i, 0);
        QModelIndex end = index(i, 0);
        emit dataChanged(start, end, roles);
    }

    if (tab.tabId() == m_activeTab.tabId()) {
        m_activeTab = tab;
        emit activeTabChanged(tab.tabId(), tab.tabId(), true);
    }
}

int DeclarativePersistentTabModel::createTab() {
    return DBManager::instance()->createTab();
}

int DeclarativePersistentTabModel::createLink(int tabId, QString url, QString title) {
    return DBManager::instance()->createLink(tabId, url, title);
}

void DeclarativePersistentTabModel::updateTitle(int tabId, int linkId, QString url, QString title)
{
    DBManager::instance()->updateTitle(tabId, linkId, url, title);
}

void DeclarativePersistentTabModel::removeTab(int tabId)
{
    DBManager::instance()->removeTab(tabId);
}

int DeclarativePersistentTabModel::nextLinkId() {
    return DBManager::instance()->nextLinkId();
}

void DeclarativePersistentTabModel::updateTab(int tabId, QString url, QString title, QString path) {
    DBManager::instance()->updateTab(tabId, url, "", "");
}

void DeclarativePersistentTabModel::navigateTo(int tabId, QString url, QString title, QString path) {
    DBManager::instance()->navigateTo(tabId, url, "", "");
}

void DeclarativePersistentTabModel::updateThumbPath(int tabId, QString path)
{
    DBManager::instance()->updateThumbPath(tabId, path);
}

void DeclarativePersistentTabModel::saveActiveTab() const
{
    DBManager::instance()->saveSetting("activeTabId", QString("%1").arg(m_activeTab.tabId()));
}
