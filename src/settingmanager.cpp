/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Dmitry Rozhkov <dmitry.rozhkov@jollamobile.com>
**
****************************************************************************/

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "settingmanager.h"
#include "dbmanager.h"

#include <MGConfItem>
#include <qmozcontext.h>
#include <QVariant>
#include "bookmarkmanager.h"

static SettingManager *gSingleton = 0;

SettingManager::SettingManager(QObject *parent)
    : QObject(parent)
    , m_initialized(false)
{
    m_clearPrivateDataConfItem = new MGConfItem("/apps/sailfish-browser/actions/clear_private_data", this);
    m_clearHistoryConfItem = new MGConfItem("/apps/sailfish-browser/actions/clear_history", this);
    m_clearCookiesConfItem = new MGConfItem("/apps/sailfish-browser/actions/clear_cookies", this);
    m_clearPasswordsConfItem = new MGConfItem("/apps/sailfish-browser/actions/clear_passwords", this);
    m_clearCacheConfItem = new MGConfItem("/apps/sailfish-browser/actions/clear_cache", this);
    m_clearBookmarksConfItem = new MGConfItem("/apps/sailfish-browser/actions/clear_bookmarks", this);
    m_searchEngineConfItem = new MGConfItem("/apps/sailfish-browser/settings/search_engine", this);
    m_doNotTrackConfItem = new MGConfItem("/apps/sailfish-browser/settings/do_not_track", this);
    m_autostartPrivateBrowsing = new MGConfItem("/apps/sailfish-browser/settings/autostart_private_browsing", this);

    // Look and feel related settings
    m_toolbarSmall = new MGConfItem("/apps/sailfish-browser/settings/toolbar_small", this);
    m_toolbarLarge = new MGConfItem("/apps/sailfish-browser/settings/toolbar_large", this);
    connect(m_toolbarSmall, SIGNAL(valueChanged()), this, SIGNAL(toolbarSmallChanged()));
    connect(m_toolbarLarge, SIGNAL(valueChanged()), this, SIGNAL(toolbarLargeChanged()));
}

bool SettingManager::clearHistoryRequested() const
{
    return m_clearPrivateDataConfItem->value(QVariant(false)).toBool() ||
            m_clearHistoryConfItem->value(QVariant(false)).toBool();
}

bool SettingManager::initialize()
{
    if (m_initialized) {
        return false;
    }

    bool clearedData = clearPrivateData();
    if (!clearedData) {
        clearedData |= clearCookies();
        clearedData |= clearPasswords();
        clearedData |= clearCache();
        clearedData |= clearBookmarks();
        clearedData |= clearHistory();
    }
    setSearchEngine();
    doNotTrack();

    connect(m_clearPrivateDataConfItem, SIGNAL(valueChanged()),
            this, SLOT(clearPrivateData()));
    connect(m_clearHistoryConfItem, SIGNAL(valueChanged()),
            this, SLOT(clearHistory()));
    connect(m_clearCookiesConfItem, SIGNAL(valueChanged()),
            this, SLOT(clearCookies()));
    connect(m_clearPasswordsConfItem, SIGNAL(valueChanged()),
            this, SLOT(clearPasswords()));
    connect(m_clearCacheConfItem, SIGNAL(valueChanged()),
            this, SLOT(clearCache()));
    connect(m_clearBookmarksConfItem, SIGNAL(valueChanged()),
            this, SLOT(clearBookmarks()));
    connect(m_searchEngineConfItem, SIGNAL(valueChanged()),
            this, SLOT(setSearchEngine()));
    connect(m_doNotTrackConfItem, SIGNAL(valueChanged()),
            this, SLOT(doNotTrack()));

    m_initialized = true;
    return clearedData;
}

int SettingManager::toolbarSmall()
{
    return m_toolbarSmall->value(72).value<int>();
}

int SettingManager::toolbarLarge()
{
    return m_toolbarLarge->value(108).value<int>();
}

SettingManager *SettingManager::instance()
{
    if (!gSingleton) {
        gSingleton = new SettingManager();
    }
    return gSingleton;
}

bool SettingManager::clearPrivateData()
{
    bool actionNeeded = m_clearPrivateDataConfItem->value(QVariant(false)).toBool();
    if (actionNeeded) {
        QMozContext::GetInstance()->sendObserve(QString("clear-private-data"), QString("passwords"));
        QMozContext::GetInstance()->sendObserve(QString("clear-private-data"), QString("cookies"));
        QMozContext::GetInstance()->sendObserve(QString("clear-private-data"), QString("cache"));
        DBManager::instance()->clearHistory();
        m_clearPrivateDataConfItem->set(QVariant(false));
    }
    return actionNeeded;
}

bool SettingManager::clearHistory()
{
    bool actionNeeded = m_clearHistoryConfItem->value(false).toBool();
    if (actionNeeded) {
        DBManager::instance()->clearHistory();
        m_clearHistoryConfItem->set(false);
    }
    return actionNeeded;
}

bool SettingManager::clearCookies()
{
    bool actionNeeded = m_clearCookiesConfItem->value(false).toBool();
    if (actionNeeded) {
        QMozContext::GetInstance()->sendObserve(QString("clear-private-data"), QString("cookies"));
        m_clearCookiesConfItem->set(false);
    }
    return actionNeeded;
}

bool SettingManager::clearPasswords()
{
    bool actionNeeded = m_clearPasswordsConfItem->value(false).toBool();
    if (actionNeeded) {
        QMozContext::GetInstance()->sendObserve(QString("clear-private-data"), QString("passwords"));
        m_clearPasswordsConfItem->set(false);
    }
    return actionNeeded;
}

bool SettingManager::clearCache()
{
    bool actionNeeded = m_clearCacheConfItem->value(false).toBool();
    if (actionNeeded) {
        QMozContext::GetInstance()->sendObserve(QString("clear-private-data"), QString("cache"));
        m_clearCacheConfItem->set(false);
    }
    return actionNeeded;
}

bool SettingManager::clearBookmarks()
{
    bool actionNeeded = m_clearBookmarksConfItem->value(false).toBool();
    if (actionNeeded) {
        BookmarkManager* bookmarks = BookmarkManager::instance();
        bookmarks->clear();
        m_clearBookmarksConfItem->set(false);
    }
    return actionNeeded;
}

void SettingManager::setSearchEngine()
{
    QVariant searchEngine = m_searchEngineConfItem->value(QVariant(QString("Google")));
    QMozContext::GetInstance()->setPref(QString("browser.search.defaultenginename"), searchEngine);
}

void SettingManager::doNotTrack()
{
    QMozContext::GetInstance()->setPref(QString("privacy.donottrackheader.enabled"),
                                        m_doNotTrackConfItem->value(false));
}

void SettingManager::setAutostartPrivateBrowsing(bool privateMode)
{
    m_autostartPrivateBrowsing->set(privateMode);
}

bool SettingManager::autostartPrivateBrowsing() const
{
    return m_autostartPrivateBrowsing->value(false).toBool();
}
