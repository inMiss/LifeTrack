#ifndef LIFETRACK_COMMON_GLOBAL_H
#define LIFETRACK_COMMON_GLOBAL_H

#include <QString>

extern QString g_applicationDirPath;
extern QString g_dataBaseDirPath;
extern QString g_IniConfigDirPath;

//各语言相关文件
extern QString g_chineseLanguageFilePath;
extern QString g_englishLanguageFilePath;

//UI资源路径
extern QString g_uiResourceEngilshPath;
//全局函数
extern void gobalInit();

#endif