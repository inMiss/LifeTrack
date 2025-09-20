#include "commonFunc.h"
#include "../language/language.h"
#include "../management/structCommon.h"
//////////////////////////////////////////////////////////////////////////
// 
// ///////////////////////////////////////////////////////////////////////
QString getLanguegeShowText(const QString& text)
{
    return language::CLanguageLoad::GetInstance()->getShowText(text);
}

QString dateTimeToShowText(const QDateTime& dateTime)
{
    if (dateTime == DATETIME_EMPTY || dateTime == DATETIME_MAX)
    {
        return "---";
    }
    return dateTime.toString("yyyy-MM-dd hh:mm:ss");
}

QDateTime textToDateTime(const QString& dateTimeText)
{
    QDateTime showTime;
    if (dateTimeText.isEmpty() || dateTimeText == DATETIME_EMPTY_SHOW)
    {
        showTime = QDateTime::currentDateTime();
    }
    else
    {
        showTime = QDateTime::fromString(dateTimeText, "yyyy-MM-dd hh:mm:ss");
    }
    return showTime;
}

QDate textToDate(const QString& dateTimeText)
{
    QDateTime time = textToDateTime(dateTimeText);
    return time.date();
}

double get2DateTimeTextDiff(const QString& beginTime, const QString& endTime)
{
    if (beginTime.isEmpty() || endTime.isEmpty())
    {
        return 0;
    }
    QDateTime startDateTime = textToDateTime(beginTime);
    QDateTime endDateTime = textToDateTime(endTime);
    return startDateTime.secsTo(endDateTime);
}

QString getNewTaskId(const QString& parentId , const int index)
{
   QString currentId =  QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());
   if (parentId.isEmpty())
   {
       return currentId + "-" + QString::number(index);
   }

   return parentId + "-" + currentId + "-" + QString::number(index);
}
