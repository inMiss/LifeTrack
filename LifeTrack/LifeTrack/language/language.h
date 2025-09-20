#ifndef LIFETRACK_LANGUAGE_LANGUAGELOAD_H
#define LIFETRACK_LANGUAGE_LANGUAGELOAD_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QStringList>

#include "../utilities/Singleton.h"
#include "../common/common.h"


namespace language
{
    class CLanguageLoad:public QObject,public utils::Singleton<CLanguageLoad>
    {
        friend class utils::Singleton<CLanguageLoad>;
        CLanguageLoad(ELanguageType type = e_language_english);
        ~CLanguageLoad();
    public:
        void reSetLanguageType(ELanguageType type = e_language_chinese);
        QString getShowText(const QString& qsLable);
        ELanguageType getCurrentLanguageType();
    private:
        void loadLangeuageContent(const QString& languagePath);
        QString getLanguageFilePath(ELanguageType tpye);
    private:
        QSettings* m_pLanguageSetting;
        ELanguageType m_currentLanguage;
    };
}

#endif