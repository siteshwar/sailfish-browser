/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Petri M. Gerdt <petri.gerdt@jollamobile.com>
**
****************************************************************************/

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef DECLARATIVEPERSISTENTTABMODEL_H
#define DECLARATIVEPERSISTENTTABMODEL_H

#include <QAbstractListModel>
#include <QQmlParserStatus>
#include <QPointer>
#include <QScopedPointer>

#include "declarativetabmodel.h"

class DeclarativePersistentTabModel : public DeclarativeTabModel
{
    Q_OBJECT

protected:
    virtual int createTab();
    virtual int createLink(int tabId, QString url, QString title);
    virtual void updateTitle(int tabId, int linkId, QString url, QString title);
    virtual void removeTab(int tabId);
    virtual int nextLinkId();
    virtual void updateTab(int tabId, QString url, QString title, QString path);
    virtual void navigateTo(int tabId, QString url, QString title, QString path);
    virtual void updateThumbPath(int tabId, QString path);

private slots:
    void tabChanged(const Tab &tab);
    void saveActiveTab() const;

public slots:
    // TODO: Move to be private
    void tabsAvailable(QList<Tab> tabs);

public:
    void classBegin();

    DeclarativePersistentTabModel(QObject *parent = 0);
    ~DeclarativePersistentTabModel();
};

#endif // DECLARATIVEPERSISTENTTABMODEL_H
