
#include "language.h"
#include "../common/global.h"

namespace language
{
    CLanguageLoad::CLanguageLoad(ELanguageType type /* = common::e_language_chinese */)
        : m_currentLanguage(type)
        , m_pLanguageSetting(nullptr)
    {
        reSetLanguageType(type);
    } 

    CLanguageLoad::~CLanguageLoad()
    {
        delete m_pLanguageSetting;
        m_pLanguageSetting = nullptr;
    }

    QString CLanguageLoad::getShowText(const QString& qsLable)
    {   
        if (NULL == m_pLanguageSetting)
        {
            return "";
        }
        return m_pLanguageSetting->value(qsLable).toString();
    }

    ELanguageType CLanguageLoad::getCurrentLanguageType()
    {
        return m_currentLanguage;
    }

    void CLanguageLoad::reSetLanguageType(ELanguageType type /* = common::e_language_chinese */)
    {
        //读取对应的翻译文件
        loadLangeuageContent(getLanguageFilePath(type));
        //
        m_currentLanguage = type;
    }

    QString CLanguageLoad::getLanguageFilePath(ELanguageType tpye)
    {
        switch (tpye)
        {
        case e_language_english:
            return g_englishLanguageFilePath;
        default:
            return g_chineseLanguageFilePath;
        }
    }

    void CLanguageLoad::loadLangeuageContent(const QString& languagePath)
    {
        delete m_pLanguageSetting;
        m_pLanguageSetting = nullptr;
        m_pLanguageSetting = new QSettings(languagePath, QSettings::IniFormat);
    }

}