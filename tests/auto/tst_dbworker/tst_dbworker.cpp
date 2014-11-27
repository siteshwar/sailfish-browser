/****************************************************************************
**
** Copyright (C) 2014
** Contact: Siteshwar Vashisht <siteshwar@gmail.com>
**
****************************************************************************/

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "dbmanager.h"
#include "dbworker.h"
#include "testobject.h"
#include <QtTest>

class tst_dbworker : public TestObject {
    Q_OBJECT

public:
    tst_dbworker();

private slots:
    void initTestCase();
    void addTabs_data();
    void addTabs();
    /*void createLink_data();
    void createLink();*/
    void getAllTabs();
    void getTab();
    void navigateTo();
    void updateTab();

    void updateTitle_data();
    void updateTitle();
    void updateThumbPath_data();
    void updateThumbPath();

    //void goForward(int tabId);
    //void goBack(int tabId);

    void saveSetting_data();
    void saveSetting();
    void getSettings_data();
    void getSettings();
    void deleteSetting();

    //void clearHistory();
    //void clearTabHistory(int tabId);

    //void removeTab();
    //void removeAllTabs();

private:
    int createTab(QString url, QString title);
    int maxTabId;
    int maxLinkId;
    DBWorker* worker;
};

tst_dbworker::tst_dbworker():maxTabId(0),maxLinkId(0) {
    worker = DBManager::instance()->getWorker();
}

void tst_dbworker::initTestCase() {
    worker->clearHistory();
}

void tst_dbworker::addTabs_data()
{
    QTest::addColumn<QString>("url");
    QTest::addColumn<QString>("title");
    QTest::newRow("http") << "http://foobar" << "FooBar 1";
    QTest::newRow("https") << "https://foobar" << "FooBar 2";
    QTest::newRow("file") << "file://foo/bar" << "FooBar 3";
}

void tst_dbworker::addTabs() {
    QFETCH(QString, url);
    QFETCH(QString, title);

    maxTabId = createTab(url, title);
    QVERIFY(maxTabId == worker->getMaxTabId());
}
/*
void tst_dbworker::createLink_data() {
    QTest::addColumn<QString>("url");
    QTest::addColumn<QString>("title");
    QTest::addColumn<QString>("thumbPath");

    QTest::newRow("http") << "http://foobar" << "FooBar 4" << "file:///tmp/a";
}

void tst_dbworker::createLink() {
    QFETCH(QString, url);
    QFETCH(QString, title);
    QFETCH(QString, thumbPath);

    maxLinkId = worker->createLink(url, title, thumbPath);
    QVERIFY(maxLinkId == worker->getMaxLinkId());
}
*/
void tst_dbworker::getAllTabs() {
    QSignalSpy tabsAvailableSpy(DBManager::instance(), SIGNAL(tabsAvailable(QList<Tab>)));
    QBENCHMARK {
        DBManager::instance()->getAllTabs();
    }
    waitSignals(tabsAvailableSpy, 1);
}

void tst_dbworker::getTab() {
    QSignalSpy tabAvailableSpy(DBManager::instance(), SIGNAL(tabAvailable(Tab)));
    QBENCHMARK {
        worker->getTab(maxTabId);
    }
    waitSignals(tabAvailableSpy, 1);
}

void tst_dbworker::navigateTo() {
    QString url = "http://google.com/", title = "google";

    QSignalSpy historyAvailableSpy(worker, SIGNAL(historyAvailable(QList<Link>)));
    QSignalSpy tabHistoryAvailableSpy(worker, SIGNAL(tabHistoryAvailable(int, QList<Link>)));

    QBENCHMARK(
        worker->navigateTo(maxTabId, url, title, "")
    );
    //Q_VERIFY(worker->getLink(url).url() == url);
    //worker->getHistory(url);
    //waitSignals(historyAvailableSpy, 1);
    /*QList<QVariant> arguments = historyAvailableSpy.takeFirst();
    qDebug() << "Type is" << arguments.at(0).typeName();
    qDebug() << "Size is " << arguments.at(0).toList();
    QVERIFY(arguments.at(0).toList().size() == 1);

    worker->getTabHistory(maxTabId);
    waitSignals(tabHistoryAvailableSpy, 1);
    arguments = tabHistoryAvailableSpy.takeFirst();
    QVariant variant = arguments.at(1);
    variant.convert(variant.userType());
    QVERIFY(variant.value<Link>().url() == url);*/
}

void tst_dbworker::updateTitle_data() {
    QTest::addColumn<int>("tabId");
    QTest::addColumn<int>("linkId");
    QTest::addColumn<QString>("url");
    QTest::addColumn<QString>("title");
    QTest::newRow("title1") << maxTabId << maxLinkId << "http://foobar" << "FooBar 4";
}

void tst_dbworker::updateTitle() {
    QFETCH(int, tabId);
    QFETCH(int, linkId);
    QFETCH(QString, url);
    QFETCH(QString, title);

    QSignalSpy titleChangedSignal(worker, SIGNAL(titleChanged(int, int, QString, QString)));
    worker->updateTitle(tabId, linkId, url, title);
    waitSignals(titleChangedSignal, 1);
}

void tst_dbworker::updateThumbPath_data() {
    QTest::addColumn<int>("tabId");
    QTest::addColumn<QString>("path");
    QTest::newRow("thumb1") << maxTabId << "file:///xyz";
}

void tst_dbworker::updateThumbPath() {
    QFETCH(int, tabId);
    QFETCH(QString, path);

    QSignalSpy thumbPathChangedSignal(worker, SIGNAL(thumbPathChanged(int, QString)));
    worker->updateThumbPath(tabId, path);
    waitSignals(thumbPathChangedSignal, 1);
}

int tst_dbworker::createTab(QString url, QString title)
{
    int tabId = DBManager::instance()->createTab();
    DBManager::instance()->createLink(tabId, url, title);
    return tabId;
}

void tst_dbworker::updateTab() {
    QString url = "http://yahoo.com", title = "yahoo";
    worker->updateTab(maxTabId, url, title, "");
}

void tst_dbworker::saveSetting_data() {
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("value");
    QTest::newRow("foo") << "foo" << "1";
    QTest::newRow("bar") << "bar" << "2";
    QTest::newRow("baz") << "baz" << "3";
}

void tst_dbworker::saveSetting() {
    QFETCH(QString, name);
    QFETCH(QString, value);
    worker->saveSetting(name, value);
    SettingsMap map = worker->getSettings();
    QVERIFY(map.keys().indexOf(name) != -1);
}

void tst_dbworker::getSettings_data() {
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("value");
    QTest::newRow("foo") << "foo" << "1";
    QTest::newRow("bar") << "bar" << "2";
    QTest::newRow("baz") << "baz" << "3";
}

void tst_dbworker::getSettings() {
    QFETCH(QString, name);
    QFETCH(QString, value);

    SettingsMap settings = worker->getSettings();
    QVERIFY(settings[name] == value);
}

void tst_dbworker::deleteSetting() {
    SettingsMap map = worker->getSettings();
    map.remove("bar");
    QVERIFY(map.size() == 2);
}

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    tst_dbworker testcase;
    return QTest::qExec(&testcase, argc, argv); \
}

#include "tst_dbworker.moc"
