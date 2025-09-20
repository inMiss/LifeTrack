#ifndef LIFETRACK_COMMON_COMMONFUNC_H
#define LIFETRACK_COMMON_COMMONFUNC_H

#include "global.h"
#include "common.h"

#include <QString>
#include <QDateTime>

QString getLanguegeShowText(const QString& text);
QString dateTimeToShowText(const QDateTime& dateTime);
QDateTime textToDateTime(const QString& dateTimeText);
QDate textToDate(const QString& dateTimeText);
double get2DateTimeTextDiff(const QString& beginTime, const QString& endTime);
QString getNewTaskId(const QString& parentId = "" , const int index = 0);
#endif