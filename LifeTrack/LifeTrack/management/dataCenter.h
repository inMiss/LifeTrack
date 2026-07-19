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

    QMap<QString, STaskListPtr> getAllCompleteTasks();
    QMap<QString, STaskListPtr> getAllActiveTasks();
    //这两个函数是获取总表格中的不同表格数据，避免前端界面需要根据一些字段去进行判断，导致更多的执行步骤。
    QMap<QString, STaskListPtr> getActiveTableViewTasks();
    bool isActiveTableViewShowData(const QString& repeatType, const QString& taskStatus);
    QMap<QString, STaskListPtr> getNoActiveTableViewTasks();
    QString getImportLevelShowContent(const QString& textId);
    QString getBelongTypeShowContent(const QString& textId);
    QString getRepeatTypeShowContent(const QString& textId);
    QString getStatusTypeShowContent(const QString& textId);

    QMap<QString, SRewards> getAllRewards();
    //ActiveTask数据库操作
    void addActiveTableTask(const STaskListPtr& list);
    void addActiveTableTasks(const QMap<QString, STaskListPtr>& list);
    void updateActiveTableTask(const STaskListPtr& list);
    void updateActiveTableTask(const QString& taskId);//这个函数使用前保证需要修改的数据已经修改完毕了
    //
    void addCompleteTableTask(const STaskListPtr& list);
    void addCompleteTableTasks(const QMap<QString, STaskListPtr>& list);
    //
    void deleteUnStartTask(const QString& taskId);
    void deleteCompleteTask(const QString& taskId);


    //查询一个任务的历史数据查询
    QMap<QString, SDailyHistory> getTaskDailtHistoryData(const QString& taskId);
    //查询一个任务的状态变更
    QMap<QString, SStatusChange> getTaskStatusChangeData(const QString& taskId);

    //修改task的状态
    void setTaskStatus(const QString& taskId, const QString& laterTaskStatus);
    //修改任务的开始时间
    void setTaskStartTime(const QString& taskId);
    //设置task的完成次数和失败次数
    void setTaskFinishAndFailTimes(const QString& taskId, const int nFinishTimes, const int nFailTimes);
    void setTaskStatusChangeReason(const QString& taskId, const QString& reason);

    //添加每日任务的历史记录
    void addDailyHistory(const SDailyHistory& history);
    void addStatusChange(const SStatusChange& change);
    //任务状态变更记录

    //任务状态变更和历史记录
    void addOrUpdateDailyHistoryBytaskId(const QString& taskId, const QString& activeType, const QString& remark);
    void addOrUpdateStatusChangeBytaskId(const QString& taskId, const QString& activeType, const QString& reason);

    //根据任务id获取tasklist数据
    bool getTaskListById(const QString& taskId, STaskListPtr& result);
    QString getTaskListRepeatTypeById(const QString& taskId);
private:
    void initDbPath();
    bool selectDbData(const QString& sql, QSqlQuery& result, const QString& tableName, const QString& connName = "Select", const QString& dbPath = "");
    //无返回值的增、删、改
    bool operationDb(const QString& sql, const QString& connName);
    //查询是否存在
    bool isDataExiteInTable(const QString& sql, const QString& connName);
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
    //active_task--进行中的任务
    void selectActiveTaskTable(bool bSelectAll = true);
    void insertActiveTaskTable(const STaskListPtr& data);
    void updateActiveTaskTable(const STaskListPtr& data);
    void deleteActiveTaskTable(const QString& task_id);
    //complete_task--已经完成的任务
    void selectCompleteTaskTable(bool bSelectAll = true);
    void insertCompleteTaskTable(const STaskListPtr& data);
    void updateCompleteTaskTable(const STaskListPtr& data);
    void deleteCompleteTaskTable(const QString& task_id);
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

    //历史记录表格
    void selectDailtHistoryTable(const QString& taskId = "", bool bSelectAll = true);
    void insertDailtHistoryTable(const SDailyHistory& data);
    void updateDailtHistoryTable(const SDailyHistory& data);
    //任务状态变化记录表格
    void selectStatusChangeTable(const QString& taskId = "", bool bSelectAll = true);
    void insertStatusChangeTable(const SStatusChange& data);
    void updateStatusChangeTable(const SStatusChange& data);
    //数据库写入线程
    //void _writeDb();
    
    //根据从active中获取到活跃区的任务，即进行中或者重新进行中的任务，创建今天的最新任务
    //只是修改map中每个任务的一些属性，不会自动创建新的tasklist，新的taskList产生只有界面操作
    void initTodayRunningTask();
private:
    QMap<QString, STaskListPtr>  m_mapAllActiveTasks;//<id,task>Active 数据库表格包含所有的active和noactive任务
    QMap<QString, STaskListPtr>  m_mapAllCompleteTasks;//包含所有已经完成的任务
    QMap<QString, SIncentiveContent> m_mapAllIncentiveContent;
    QMap<QString, SDailySummary>  m_mapAllDailySummary;
    QMap<QString, SRewards> m_mapAllRewards;

    QMap<QString, QMap<QString,SDailyHistory>> m_mapAllDailyHistory; //<task_id,<task_date,data>>
    QMap<QString, QMap<QString, SStatusChange>> m_mapAllStatusChange;//<task_id,<change_time,data>>
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
