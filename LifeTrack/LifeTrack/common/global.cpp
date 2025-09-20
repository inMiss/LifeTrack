
#include "global.h"
#include <QDir>
#include <QDateTime>
#include <QApplication>

QString g_applicationDirPath = "";
QString g_dataBaseDirPath = "";
QString g_IniConfigDirPath = "";

//各语言相关文件
QString g_chineseLanguageFilePath = "";
QString g_englishLanguageFilePath = "";

void gobalInit()
{
    g_applicationDirPath = qApp->applicationDirPath() + "/";
    g_dataBaseDirPath = g_applicationDirPath + "db/";
    g_IniConfigDirPath = g_applicationDirPath + "config/";

    QDir dir(g_dataBaseDirPath);
    if (!dir.exists())
        QDir().mkdir(g_dataBaseDirPath);

    if (dir.exists(g_IniConfigDirPath) == false)
        dir.mkdir(g_IniConfigDirPath);

    //各语言相关文件
    g_chineseLanguageFilePath = g_applicationDirPath + "language/chinese.ini";
    g_englishLanguageFilePath = g_applicationDirPath + "language/english.ini";
}
