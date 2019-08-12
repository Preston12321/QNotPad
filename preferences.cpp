#include "preferences.h"

#include <QSettings>

QVariant loadPref(QSettings *pref, const QString &key) {
    return pref->value(key);
}

QVariant loadPref(QSettings *pref, const QString &group, const QString &key) {
    pref->beginGroup(group);
    auto value = pref->value(key);
    pref->endGroup();
    return value;
}

QVariant loadPref(QSettings *pref, const QString &key,
                  const QVariant &defValue) {
    return pref->value(key, defValue);
}

QVariant loadPref(QSettings *pref, const QString &group, const QString &key,
                  const QVariant &defValue) {
    pref->beginGroup(group);
    auto value = pref->value(key, defValue);
    pref->endGroup();
    return value;
}

void savePref(QSettings *pref, const QString &key, const QVariant &value) {
    pref->setValue(key, value);
}

void savePref(QSettings *pref, const QString &group, const QString &key,
              const QVariant &value) {
    pref->beginGroup(group);
    pref->setValue(key, value);
    pref->endGroup();
}
