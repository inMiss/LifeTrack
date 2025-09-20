#pragma once

#include <QWidget>
#include <QObject>
#include <QMap>
#include <QDate>

#include "../utilities/Singleton.h"
#include "../utilities/dbmana.h"
#include "../utilities/blockingconcurrentqueue.h"
#include "../common/common.h"
#include "structCommon.h"

class CDataCenter :public QObject, public utils::Singleton<CDataCenter>
{
    friend class utils::Singleton<CDataCenter>;
    CDataCenter();
    ~CDataCenter();
public:
    void initInfo();

    QMap<QString, SImportLevelDictionary> getImportLevelDictionar();
    QMap<QString, SBelongTypeDictionary> getBelongTypeDictionary();
    QMap<QString, SRepeatTypeDictionary> getRepeatTypeDictionar();
    QMap<QString, SRewardsTypeDictionary> getRewardsTypeDictionary();
    QMap<QString, SStatusTypeDictionary> getStatusLevelDictionar();

    QMap<QString, STaskListPtr> getAllTasks();
    QMap<QString, STaskListPtr> getTasksByDate(const QDate& chooseDate);
    QString getImportLevelShowContent(const QString& textId);
    QString getBelongTypeShowContent(const QString& textId);
    QString getRepeatTypeShowContent(const QString& textId);
    QString getStatusTypeShowContent(const QString& textId);

    QMap<QString, SRewards> getAllRewards();
    //数据库操作
    void addNewTask(const STaskListPtr& list);
    void addNewTasks(const QMap<QString, STaskListPtr>& list);
private:
    void initDbPath();
    bool selectDbData(const QString& sql, QSqlQuery& result, const QString& tableName, const QString& connName = "Select", const QString& dbPath = "");
    //无返回值的增、删、改
    bool operationDb(const QString& sql, const QString& connName);
    //表格操作  
    //daily_summary 每日总结
    void selectDailySummaryTable(const QString& strarTime = "", const QString& endTime = "", bool bSelectAll = true);
    void insertDailySummaryTable(const SDailySummary& data);
    //incentive_content 激励话语
    void selectIncentiveContentTable(bool bSelectAll = true);
    void insertIncentiveContentTable(const SIncentiveContent& data);
    //rewards 奖励
    void selectRewardsTable(bool bSelectAll = true);
    void insertRewardsTable(const SRewards& data);
    void updateRewardsTable(const SRewards& data);
    //task_list--目前先读取所有的，后期看是否需要分出1个月或者1定时间之前的为历史数据表格
    void selectTaskListTable(bool bSelectAll = true);
    void insertTaskListTable(const STaskListPtr& data);
    void updateTaskListTable(const STaskListPtr& data);
    //belong_type_dictionary 任务所属类别
    void selectSBelongTypeDictionary(bool bSelectAll = true);
    void insertSBelongTypeDictionary(const SBelongTypeDictionary& data);
    //import_levle_dictionary 任务级别
    void selectSImportLevelDictionary(bool bSelectAll = true);
    void insertSImportLevelDictionary(const SImportLevelDictionary& data);

    //会涉及到具体业务逻辑编写的字典，不提供外部设定功能--但是界面的相关combox需要显示全部，因此每次还是读取出来
    //repeat_type_dictionary 任务级别
    void selectSRepeatTypeDictionary(bool bSelectAll = true);
    void insertSRepeatTypeDictionary(const SRepeatTypeDictionary& data);
    //rewards_type_dictionary 任务级别
    void selectSRewardsTypeDictionary(bool bSelectAll = true);
    void insertSRewardsTypeDictionary(const SRewardsTypeDictionary& data);
    //task_status_dictionary 任务级别
    void selectSStatusTypeDictionary(bool bSelectAll = true);
    void insertSStatusTypeDictionary(const SStatusTypeDictionary& data);

    //数据库写入线程
    //void _writeDb();
private:
    QMap<QDate, QMap<QString, STaskListPtr>> m_mapDateToTasks;//date,<id,task>
    QMap<QString, STaskListPtr>  m_mapAllTasks;//<id,task>
    QMap<QString, SIncentiveContent> m_mapAllIncentiveContent;
    QMap<QString, SDailySummary>  m_mapAllDailySummary;
    QMap<QString, SRewards> m_mapAllRewards;
    //
    QMap<QString, SBelongTypeDictionary> m_mapAllBelongType;
    QMap<QString, SImportLevelDictionary> m_mapAllImportLevelType;
    QMap<QString, SRepeatTypeDictionary> m_mapAllIRepeatType;
    QMap<QString, SRewardsTypeDictionary> m_mapAllRewardsType;
    QMap<QString, SStatusTypeDictionary> m_mapAllStatusType;
    //数据库操作
    DBMana  m_dbManager;
    QString m_dbPath;
    QString m_qsDataBaseFilePath;


    QMutex  m_dataMutex;
};
