/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -a org.bluez.Adapter1 -c OrgBluezAdapter1 -N xml/org.bluez.Adapter1.xml
 *
 * qdbusxml2cpp is Copyright (C) 2015 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef ORG_BLUEZ_ADAPTER1_H
#define ORG_BLUEZ_ADAPTER1_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "dbus_types.h"

QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface org.bluez.Adapter1
 */
class OrgBluezAdapter1: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.Adapter1")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.bluez.Adapter1\">\n"
"    <method name=\"StartDiscovery\"/>\n"
"    <method name=\"SetDiscoveryFilter\">\n"
"      <annotation value=\"PropertyMap\" name=\"org.qtproject.QtDBus.QtTypeName.In0\"/>\n"
"      <arg direction=\"in\" type=\"a{sv}\" name=\"properties\"/>\n"
"    </method>\n"
"    <method name=\"StopDiscovery\"/>\n"
"    <method name=\"RemoveDevice\">\n"
"      <arg direction=\"in\" type=\"o\" name=\"device\"/>\n"
"    </method>\n"
"    <property access=\"read\" type=\"s\" name=\"Address\"/>\n"
"    <property access=\"read\" type=\"s\" name=\"Name\"/>\n"
"    <property access=\"readwrite\" type=\"s\" name=\"Alias\"/>\n"
"    <property access=\"read\" type=\"u\" name=\"Class\"/>\n"
"    <property access=\"readwrite\" type=\"b\" name=\"Powered\"/>\n"
"    <property access=\"readwrite\" type=\"b\" name=\"Discoverable\"/>\n"
"    <property access=\"readwrite\" type=\"u\" name=\"DiscoverableTimeout\"/>\n"
"    <property access=\"readwrite\" type=\"b\" name=\"Pairable\"/>\n"
"    <property access=\"readwrite\" type=\"u\" name=\"PairableTimeout\"/>\n"
"    <property access=\"read\" type=\"b\" name=\"Discovering\"/>\n"
"    <property access=\"read\" type=\"as\" name=\"UUIDs\"/>\n"
"    <property access=\"read\" type=\"s\" name=\"Modalias\"/>\n"
"  </interface>\n"
        "")
public:
    OrgBluezAdapter1(QObject *parent);
    virtual ~OrgBluezAdapter1();

	// PROPERTIES
    Q_PROPERTY(QString Address READ address)
    QString address() const;

    Q_PROPERTY(QString Alias READ alias WRITE setAlias)
    QString alias() const;
    void setAlias(const QString &value);

    Q_PROPERTY(uint Class READ clazz)
    uint clazz() const;

    Q_PROPERTY(bool Discoverable READ discoverable WRITE setDiscoverable)
    bool discoverable() const;
    void setDiscoverable(bool value);

    Q_PROPERTY(uint DiscoverableTimeout READ discoverableTimeout WRITE setDiscoverableTimeout)
    uint discoverableTimeout() const;
    void setDiscoverableTimeout(uint value);

    Q_PROPERTY(bool Discovering READ discovering)
    bool discovering() const;

    Q_PROPERTY(QString Modalias READ modalias)
    QString modalias() const;

    Q_PROPERTY(QString Name READ name)
    QString name() const;

    Q_PROPERTY(bool Pairable READ pairable WRITE setPairable)
    bool pairable() const;
    void setPairable(bool value);

    Q_PROPERTY(uint PairableTimeout READ pairableTimeout WRITE setPairableTimeout)
    uint pairableTimeout() const;
    void setPairableTimeout(uint value);

    Q_PROPERTY(bool Powered READ powered WRITE setPowered)
    bool powered() const;
    void setPowered(bool value);

    Q_PROPERTY(QStringList UUIDs READ uUIDs)
    QStringList uUIDs() const;

public Q_SLOTS: // METHODS
    void RemoveDevice(const QDBusObjectPath &device);
    void SetDiscoveryFilter(PropertyMap properties);
    void StartDiscovery();
    void StopDiscovery();
Q_SIGNALS: // SIGNALS
};

#endif