#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QSettings>

QVariant loadPref(QSettings *pref, const QString &key);
QVariant loadPref(QSettings *pref, const QString &group, const QString &key);
QVariant loadPref(QSettings *pref, const QString &key,
                  const QVariant &defValue);
QVariant loadPref(QSettings *pref, const QString &group, const QString &key,
                  const QVariant &defValue);

void savePref(QSettings *pref, const QString &key, const QVariant &value);
void savePref(QSettings *pref, const QString &group, const QString &key,
              const QVariant &value);

#endif  // PREFERENCES_H
