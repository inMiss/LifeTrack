#include "dataCenter.h"
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <QDebug>
#include <QVector>

#include "../common/global.h"
#include "../spdlog/spdlog.h"
#include "../language/language.h"
#include "../common/commonFunc.h"


//////////////////////////////////////////////////////////////////////////
// 
// ///////////////////////////////////////////////////////////////////////
CDataCenter::CDataCenter()
{
    initDbPath();
}

CDataCenter::~CDataCenter()
{

}
//////////////////////////////////////////////////////////////////////////
// 外部接口
// ///////////////////////////////////////////////////////////////////////

QMap<QString, SImportLevelDictionary> CDataCenter::getImportLevelDictionar() 
{
    return m_mapAllImportLevelType;
}
QMap<QString, SBelongTypeDictionary> CDataCenter::getBelongTypeDictionary()
{
    return m_mapAllBelongType;
}

QMap<QString, SRepeatTypeDictionary> CDataCenter::getRepeatTypeDictionar()
{
    return m_mapAllIRepeatType;
}
QMap<QString, SRewardsTypeDictionary> CDataCenter::getRewardsTypeDictionary()
{
    return m_mapAllRewardsType;
}
QMap<QString, SStatusTypeDictionary> CDataCenter::getStatusLevelDictionar()
{
    return m_mapAllStatusType;
}

QMap<QString, STaskListPtr> CDataCenter::getAllActiveTasks()
{
    return m_mapAllActiveTasks;
}

QMap<QString, STaskListPtr> CDataCenter::getAllCompleteTasks()
{
    return m_mapAllCompleteTasks;
}

bool CDataCenter::isActiveTableViewShowData(const QString& repeatType, const QString& taskStatus)
{
    if(TASK_STATUS_UNSTART == taskStatus || TASK_STATUS_PAUSE == taskStatus || TASK_STATUS_AUTOPAUSE)
    {
        return false;
    }
    return true;
}

QMap<QString, STaskListPtr> CDataCenter::getActiveTableViewTasks()
{
    QMap<QString, STaskListPtr> result;
    QMap<QString, STaskListPtr>::const_iterator iter = m_mapAllActiveTasks.begin();
    for (iter; iter != m_mapAllActiveTasks.end(); ++iter)
    {
        if(isActiveTableViewShowData(iter.value()->repeat_type,iter.value()->task_status))
        {
            result[iter.value()->id] = iter.value();
        }
    }
    return result;
}
QMap<QString, STaskListPtr> CDataCenter::getNoActiveTableViewTasks()
{
    QMap<QString, STaskListPtr> result;
    QMap<QString, STaskListPtr>::const_iterator iter = m_mapAllActiveTasks.begin();
    for (iter; iter != m_mapAllActiveTasks.end(); ++iter)
    {
        if (TASK_STATUS_UNSTART == iter.value()->task_status || TASK_STATUS_PAUSE == iter.value()->task_status || TASK_STATUS_AUTOPAUSE == iter.value()->task_status)
        {
            result[iter.value()->id] = iter.value();
        }
    }
    return result;
}

QMap<QString, SRewards> CDataCenter::getAllRewards()
{
    return m_mapAllRewards;
}

QString CDataCenter::getImportLevelShowContent(const QString& textId)
{
    return m_mapAllImportLevelType[textId].shouw_text;
}

QString CDataCenter::getBelongTypeShowContent(const QString& textId)
{
    return m_mapAllBelongType[textId].show_content;
}
QString CDataCenter::getRepeatTypeShowContent(const QString& textId)
{
    return m_mapAllIRepeatType[textId].show_content;
}
QString CDataCenter::getStatusTypeShowContent(const QString& textId)
{
    return m_mapAllStatusType[textId].show_content;
}

//查询一个任务的历史数据查询
QMap<QString, SDailyHistory> CDataCenter::getTaskDailtHistoryData(const QString& taskId)
{
    selectDailtHistoryTable(taskId, false);
    return m_mapAllDailyHistory[taskId];
}
//查询一个任务的状态变更
QMap<QString, SStatusChange> CDataCenter::getTaskStatusChangeData(const QString& taskId)
{
    selectStatusChangeTable(taskId, false);
    return m_mapAllStatusChange[taskId];
}

void CDataCenter::addActiveTableTask(const STaskListPtr& list)
{
    insertActiveTaskTable(list);
}

void CDataCenter::addActiveTableTasks(const QMap<QString, STaskListPtr>& list)
{
    QMap<QString, STaskListPtr>::const_iterator iter = list.begin();
    for (iter; iter != list.end(); ++iter)
    {
        addActiveTableTask(iter.value());
    }
}

void CDataCenter::updateActiveTableTask(const STaskListPtr& list)
{
    updateActiveTaskTable(list);
}

void CDataCenter::updateActiveTableTask(const QString& taskId)
{
    STaskListPtr tempTask = nullptr;
    if (!getTaskListById(taskId, tempTask))
    {
        return;
    }
    updateActiveTaskTable(tempTask);
}

////////////////////////////////
void CDataCenter::setTaskStatus(const QString& taskId, const QString& laterTaskStatus)
{
    if (m_mapAllActiveTasks.contains(taskId))
    {
        m_mapAllActiveTasks[taskId]->task_status = laterTaskStatus;
    }
    else if (m_mapAllCompleteTasks.contains(taskId))
    {
        m_mapAllCompleteTasks[taskId]->task_status = laterTaskStatus;
    }
}

void CDataCenter::setTaskStartTime(const QString& taskId)
{
    if (m_mapAllActiveTasks.contains(taskId))
    {
        m_mapAllActiveTasks[taskId]->start_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }
    else if (m_mapAllCompleteTasks.contains(taskId))
    {
        m_mapAllCompleteTasks[taskId]->start_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }
}

void CDataCenter::setTaskFinishAndFailTimes(const QString& taskId, const int nFinishTimes, const int nFailTimes)
{
    if (m_mapAllActiveTasks.contains(taskId))
    {
        m_mapAllActiveTasks[taskId]->finish_times = nFinishTimes;
        m_mapAllActiveTasks[taskId]->fail_times = nFailTimes;
    }
    else if (m_mapAllCompleteTasks.contains(taskId))
    {
        m_mapAllCompleteTasks[taskId]->finish_times = nFinishTimes;
        m_mapAllCompleteTasks[taskId]->fail_times = nFailTimes;
    }
}

void CDataCenter::setTaskStatusChangeReason(const QString& taskId, const QString& reason)
{
    if (m_mapAllActiveTasks.contains(taskId))
    {
        m_mapAllActiveTasks[taskId]->statue_resaon = reason;
    }
    else if (m_mapAllCompleteTasks.contains(taskId))
    {
        m_mapAllCompleteTasks[taskId]->statue_resaon = reason;
    }
}

////////////////////////////////
void CDataCenter::addDailyHistory(const SDailyHistory& history)
{
    insertDailtHistoryTable(history);
}

void CDataCenter::addStatusChange(const SStatusChange& change)
{
    insertStatusChangeTable(change);
}

void CDataCenter::addCompleteTableTask(const STaskListPtr& list)
{
    insertActiveTaskTable(list);
}

void CDataCenter::addCompleteTableTasks(const QMap<QString, STaskListPtr>& list)
{
    QMap<QString, STaskListPtr>::const_iterator iter = list.begin();
    for (iter; iter != list.end(); ++iter)
    {
        addCompleteTableTask(iter.value());
    }
}

void CDataCenter::deleteUnStartTask(const QString& taskId)
{
    deleteActiveTaskTable(taskId);
}

void CDataCenter::deleteCompleteTask(const QString& taskId)
{
    deleteCompleteTaskTable(taskId);
}

bool CDataCenter::getTaskListById(const QString& taskId, STaskListPtr& result)
{
    if (m_mapAllActiveTasks.contains(taskId))
    {
        result = m_mapAllActiveTasks[taskId];
    }
    else if (m_mapAllCompleteTasks.contains(taskId))
    {
        result = m_mapAllCompleteTasks[taskId];
    }
    else
    {
        return false;
    }
    return true;
}

QString CDataCenter::getTaskListRepeatTypeById(const QString& taskId)
{
    if (m_mapAllActiveTasks.contains(taskId))
    {
        return m_mapAllActiveTasks[taskId]->repeat_type;
    }
    
    if (m_mapAllCompleteTasks.contains(taskId))
    {
        return m_mapAllCompleteTasks[taskId]->repeat_type;
    }
    return "";
}

void CDataCenter::addOrUpdateDailyHistoryBytaskId(const QString& taskId, const QString& activeType, const QString& remark)
{   
    STaskListPtr tempTask = nullptr;
    if (!getTaskListById(taskId,tempTask))
    {
        return;
    }
    SDailyHistory history;
    history.task_id = tempTask->id;
    history.task_activeType = activeType;
    history.task_date = QDate::currentDate().toString("yyyy-MM-dd");
    history.task_status = tempTask->task_status;
    history.finish_time = tempTask->finish_time;
    history.remark = remark;

    QString sql = QString("SELECT * FROM %1 WHERE %2 = \"%3\" AND %4 = \"%5\" ").arg("dailt_history").arg("task_id").arg(history.task_id).arg("task_date").arg(history.task_date);
    if ((m_mapAllDailyHistory.contains(taskId) && m_mapAllDailyHistory[taskId].contains(history.task_date)) || isDataExiteInTable(sql, "Select"))
    {
        updateDailtHistoryTable(history);
    }
    else
    {
        insertDailtHistoryTable(history);
    }
}

void CDataCenter::addOrUpdateStatusChangeBytaskId(const QString& taskId, const QString& activeType, const QString& reason)
{
    STaskListPtr tempTask = nullptr;
    if (!getTaskListById(taskId, tempTask))
    {
        return;
    }

    SStatusChange change;
    change.task_id = tempTask->id;
    change.task_activeType = activeType;
    change.task_status = tempTask->task_status;
    change.change_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    change.reason = reason;
   
    QString sql = QString("SELECT * FROM %1 WHERE %2 = \"%3\" AND %4 = \"%5\" ").arg("status_change").arg("task_id").arg(change.task_id).arg("change_time").arg(change.change_time);
    if ((m_mapAllStatusChange.contains(taskId) && m_mapAllStatusChange[taskId].contains(change.change_time)) || isDataExiteInTable(sql, "Select"))
    {
        updateStatusChangeTable(change);
    }
    else
    {
        insertStatusChangeTable(change);
    }
}

//////////////////////////////////////////////////////////////////////////
// 预加载的数据
// ///////////////////////////////////////////////////////////////////////
void CDataCenter::initInfo()
{
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), "initDataCenter Start!");
    initDbPath();
    selectDailySummaryTable();
    selectIncentiveContentTable();
    selectRewardsTable();
    
    selectSBelongTypeDictionary();
    selectSImportLevelDictionary();
    selectSRepeatTypeDictionary();
    selectSRewardsTypeDictionary();
    selectSStatusTypeDictionary();

    selectCompleteTaskTable();
    //这个数据读取可能导致表格的更新，因此最后读取，简便操作，也减少表格更改次数
    selectActiveTaskTable();
}

void CDataCenter::initDbPath()
{
    m_qsDataBaseFilePath = g_dataBaseDirPath + "lifeTrack.db";
    QString log = "dataBase Path: " + m_qsDataBaseFilePath;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
}

bool CDataCenter::selectDbData(const QString& sql, QSqlQuery& result, const QString& tableName, const QString& connName /* = "Select" */, const QString& dbPath /* = "" */)
{
    QString operDbPath = (dbPath.isEmpty()) ? m_qsDataBaseFilePath : dbPath;
    QSqlQuery* data = m_dbManager.select(sql, connName, operDbPath);
    result = *data;
    return true;
}

bool CDataCenter::operationDb(const QString& sql, const QString& connName)
{
    return m_dbManager.execute(sql, connName, m_qsDataBaseFilePath);
}

bool CDataCenter::isDataExiteInTable(const QString& sql, const QString& connName)
{
    QSqlQuery* data = m_dbManager.select(sql, connName, m_qsDataBaseFilePath);
    if (0 < data->size())
    {
        return true;
    }
    return false;
}

//daily_summary 每日总结
void CDataCenter::selectDailySummaryTable(const QString& strarTime /* = "" */, const QString& endTime /* = "" */, bool bSelectAll /* = true */)
{
    QString sql;
    if (!bSelectAll)
    {
        return;
    }

    sql = "select * from daily_summary";

    QString log = "selectDailySummaryTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    QSqlQuery result;
    selectDbData(sql, result, "Select");
    //解析
    while (result.next())
    {
        SDailySummary daily;
        daily.id = result.value("id").toString();
        daily.date = result.value("tool_name").toString();//QDateTime::fromString(result.value("tool_name").toString(), "yyyy-MM-dd hh:mm:ss");
        daily.content = result.value("content").toString();
        daily.task_total = result.value("task_total").toInt();
        daily.finish_task_sum = result.value("finish_task_sum").toInt();
        daily.point_total = result.value("point_total").toDouble();
        daily.finish_point_sum = result.value("finish_point_sum").toDouble();

        m_mapAllDailySummary[daily.id] = daily;
    }
}

void CDataCenter::insertDailySummaryTable(const SDailySummary& data)
{
    QString dataId = QString::number(m_mapAllDailySummary.size() + 1) + "-" + data.date/*QString::number(data.date.toTime_t())*/;

    QString sql = "insert into ams_config(id,date,content,task_total,finish_task_sum,point_total,finish_point_sum) values(\""
        + dataId + "\","
        + "\"" + data.date/*data.date.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data.content + "\","
        + QString::number(data.task_total) + ","
        + QString::number(data.finish_task_sum) + ","
        + QString::number(data.point_total) + ","
        + QString::number(data.finish_point_sum) + ")";

    QString log = "insertDailySummaryTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Insert"))
    {
        m_mapAllDailySummary[data.id] = data;
    }
    return ;
}

//incentive_content 激励话语
void CDataCenter::selectIncentiveContentTable(bool bSelectAll /* = true */)
{
    QString sql;
    if (!bSelectAll)
    {
        return;
    }

    sql = "select * from daily_summary";

    QString log = "selectDailySummaryTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    QSqlQuery result;
    selectDbData(sql, result, "Select");
    //解析
    while (result.next())
    {
        SIncentiveContent incentive;
        incentive.number = result.value("number").toString();
        incentive.content = result.value("content").toString();

        m_mapAllIncentiveContent[incentive.number] = incentive;
    }
}

void CDataCenter::insertIncentiveContentTable(const SIncentiveContent& data)
{
    QString dataId = QString::number(m_mapAllIncentiveContent.size() + 1) + "-" + QString::number(QDateTime::currentSecsSinceEpoch());
    QString sql = "insert into incentive_content(number,content) values(\""
        + dataId + "\","
        + "\"" + data.content + "\")";

    QString log = "insertIncentiveContentTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Insert"))
    {
        m_mapAllIncentiveContent[data.number] = data;
    }
    return;
}

//rewards 奖励
void CDataCenter::selectRewardsTable(bool bSelectAll /* = true */)
{
    QString sql;
    if (!bSelectAll)
    {
        return;
    }

    sql = "select * from rewards";

    QString log = "selectRewardsTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    QSqlQuery result;
    selectDbData(sql, result, "Select");
    //解析
    while (result.next())
    {
        SRewards reward;
        reward.id = result.value("id").toString();
        reward.is_time_limited = (result.value("is_time_limited").toInt() == 0) ? false : true;
        reward.start_time = result.value("start_time").toString();//QDateTime::fromString(result.value("start_time").toString(), "yyyy-MM-dd hh:mm:ss");
        reward.end_time = result.value("end_time").toString();//QDateTime::fromString(result.value("end_time").toString(), "yyyy-MM-dd hh:mm:ss");
        reward.goal_type = result.value("goal_type").toString();
        reward.content = result.value("content").toString();
        reward.value = result.value("value").toDouble();
        reward.status = result.value("status").toString();
        reward.remark = result.value("remark").toString();

        m_mapAllRewards[reward.id] = reward;
    }
}

void CDataCenter::insertRewardsTable(const SRewards& data)
{
    QString dataId = QString::number(m_mapAllDailySummary.size() + 1) + "-" + QString::number(QDateTime::currentSecsSinceEpoch());

    QString sql = "insert into rewards(id,is_time_limited,start_time,end_time,goal_type,content,value,status,remark) values(\""
        + dataId + "\","
        + "\"" + QString::number(data.is_time_limited) + "\","
        + "\"" + data.start_time/*data.start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data.end_time/*data.end_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data.goal_type + "\","
        + "\"" + data.content + "\","
        +   QString::number(data.is_time_limited) + ","
        + "\"" + data.status + "\","
        + "\"" + data.remark + "\")";

    QString log = "insertRewardsTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Insert"))
    {
        m_mapAllRewards[dataId] = data;
    }
    return;
}

void CDataCenter::updateRewardsTable(const SRewards& data)
{
    if (0 == m_mapAllRewards.count(data.id))
    {
        return;
    }

    //存储到数据库
    QString sql = "UPDATE rewards SET remark=\"" + data.remark + "\""
        + ",is_time_limited=" + data.is_time_limited
        + ",start_time=\"" + data.start_time/*data.start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\""
        + ",end_time=\"" + data.end_time/*data.end_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\""
        + ",goal_type=\"" + data.goal_type + "\""
        + ",content=\"" + data.content + "\""
        + ",value=" + QString::number(data.value)
        + ",status=\"" + data.status + "\""
        + " WHERE id= \"" + data.id + "\"";

    QString log = "updateRewardsTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Update"))
    {
        m_mapAllRewards[data.id] = data;
    }
}
//task_list--目前先读取所有的，后期看是否需要分出1个月或者1定时间之前的为历史数据表格
void CDataCenter::selectActiveTaskTable(bool bSelectAll /* = true */)
{
    QString sql;
    if (!bSelectAll)
    {
        return;
    }

    sql = "select * from active_task";

    QString log = "selectActiveTaskTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    QSqlQuery result;
    selectDbData(sql, result, "Select");
    //解析
    while (result.next())
    {
        STaskListPtr task = STaskListPtr(new STaskList());
        task->id = result.value("id").toString();
        task->name = result.value("name").toString();
        
        task->start_date = result.value("start_date").toString();//QDateTime::fromString(result.value("start_time").toString(), "yyyy-MM-dd hh:mm:ss");
        task->deadline = result.value("deadline").toString();//QDateTime::fromString(result.value("end_time").toString(), "yyyy-MM-dd hh:mm:ss");
        
        task->create_time = result.value("create_time").toString();
        task->start_time = result.value("start_time").toString();//QDateTime::fromString(result.value("create_time").toString(), "yyyy-MM-dd hh:mm:ss");      
        task->finish_time = result.value("finish_time").toString();
        task->during_time = result.value("during_time").toDouble();

        task->import_level = result.value("import_level").toString();
        task->value = result.value("value").toDouble();
        task->sum_points = result.value("sum_points").toDouble();

        task->repeat_type = result.value("repeat_type").toString();
        task->finish_times = result.value("finish_times").toDouble();
        task->fail_times = result.value("fail_times").toDouble();

        task->belong_type = result.value("belong_type").toString();


        task->is_deduction = (result.value("is_deduction").toInt() == 0)?false : true;
        task->is_countdown = (result.value("is_countdown").toInt() == 0) ? false : true;
        task->is_can_pause = (result.value("is_can_pause").toInt() == 0) ? false : true;
        task->fail_threshold = result.value("fail_threshold").toDouble();

        task->task_status = result.value("task_status").toString();
        task->statue_resaon = result.value("statue_resaon").toString();//QDateTime::fromString(result.value("finish_time").toString(), "yyyy-MM-dd hh:mm:ss");
        task->parent_task = result.value("parent_task").toString();
        task->remark = result.value("remark").toString();

        m_mapAllActiveTasks[task->id] = task;

    }

    //进行中的任务更新
    initTodayRunningTask();
}

//因为可能会产生多个任务，并且界面需要显示，因此id和parentId在界面阶段就需要添加成功，但是其id的确定可以通过逻辑函数接口进行产生
void CDataCenter::insertActiveTaskTable(const STaskListPtr& data)
{
    //QString dataId = QString::number(m_mapAllTasks.size() + 1) + "-" + QString::number(QDateTime::currentSecsSinceEpoch());

    QString sql = "insert into active_task(id,name,start_date,deadline,create_time,start_time,finish_time,during_time,import_level,value,sum_points,repeat_type,finish_times,fail_times,belong_type,is_deduction,is_countdown,is_can_pause,fail_threshold,task_status,statue_resaon,parent_task,remark) values(\""
        + data->id + "\","
        + "\"" + data->name + "\","
        + "\"" + data->start_date/*data->start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->deadline/*data->end_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->create_time/*data->create_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->start_time/*data->create_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->finish_time/*data->create_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + QString::number(data->during_time) + ","       
        + "\"" + data->import_level + "\","
        + QString::number(data->value) + ","
        + QString::number(data->sum_points) + ","
        + "\"" + data->repeat_type + "\","
        + QString::number(data->finish_times) + ","
        + QString::number(data->fail_times) + ","
        + "\"" + data->belong_type + "\","
        + QString::number(data->is_deduction) + ","
        + QString::number(data->is_countdown) + ","
        + QString::number(data->is_can_pause) + ","
        + QString::number(data->fail_threshold) + ","
        + "\"" + data->task_status + "\","
        + "\"" + data->statue_resaon/*data->finish_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->parent_task + "\","
        + "\"" + data->remark + "\")";

    QString log = "insertActiveTaskTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Insert"))
    {
        m_mapAllActiveTasks[data->id] = data;
    }
    return;
}

void CDataCenter::updateActiveTaskTable(const STaskListPtr& data)
{
    if (0 == m_mapAllActiveTasks.count(data->id))
    {
        return;
    }

    //存储到数据库
    QString sql = "UPDATE active_task SET name=\"" + data->name + "\""
        + ",start_date=\"" + data->start_date/*data->start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\""
        + ",deadline=\"" + data->deadline/*data->start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\""
        + ",create_time=\"" + data->create_time + "\""
        + ",start_time=\"" + data->start_time + "\""
        + ",finish_time=\"" + data->finish_time + "\""
        + ",during_time=" + QString::number(data->during_time)
        + ",import_level=\"" + data->import_level + "\""
        + ",value=" + QString::number(data->value)
        + ",sum_points=" + QString::number(data->sum_points)
        + ",repeat_type=\"" + data->repeat_type + "\""
        + ",finish_times=" + QString::number(data->finish_times)
        + ",fail_times=" + QString::number(data->fail_times)
        + ",belong_type=\"" + data->belong_type + "\""
        + ",is_deduction=" + QString::number(data->is_deduction)
        + ",is_countdown=" + QString::number(data->is_countdown)
        + ",is_can_pause=" + QString::number(data->is_can_pause)
        + ",fail_threshold=" + QString::number(data->fail_threshold)
        + ",task_status=\"" + data->task_status + "\""
        + ",statue_resaon=\"" + data->statue_resaon + "\""
        + ",parent_task=\"" + data->parent_task + "\""
        + ",remark=\"" + data->remark + "\""
        + " WHERE id= \"" + data->id + "\"";

    QString log = "updateActiveTaskTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    // 控制台直接输出--直接输出完整字符串
    qDebug().noquote() << log;  // noquote() 避免自动添加引号


    if (operationDb(sql, "Update"))
    {
        m_mapAllActiveTasks[data->id] = data;
    }
}

void CDataCenter::deleteActiveTaskTable(const QString& task_id)
{
    //存储到数据库
    QString sql = "delete from active_task where id=\"" + task_id + "\"";
    QString log = "deleteActiveTaskTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Delete"))
    {
        m_mapAllActiveTasks.remove(task_id);
    }
}


//CompleteTask--目前先读取所有的，后期看是否需要分出1个月或者1定时间之前的为历史数据表格
void CDataCenter::selectCompleteTaskTable(bool bSelectAll /* = true */)
{
    QString sql;
    if (!bSelectAll)
    {
        return;
    }

    sql = "select * from completed_task";

    QString log = "selectCompleteTaskTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    QSqlQuery result;
    selectDbData(sql, result, "Select");
    //解析
    while (result.next())
    {
        STaskListPtr task = STaskListPtr(new STaskList());
        task->id = result.value("id").toString();
        task->name = result.value("name").toString();

        task->start_date = result.value("start_date").toString();//QDateTime::fromString(result.value("start_time").toString(), "yyyy-MM-dd hh:mm:ss");
        task->deadline = result.value("deadline").toString();//QDateTime::fromString(result.value("end_time").toString(), "yyyy-MM-dd hh:mm:ss");

        task->create_time = result.value("create_time").toString();
        task->create_time = result.value("start_time").toString();//QDateTime::fromString(result.value("create_time").toString(), "yyyy-MM-dd hh:mm:ss");      
        task->finish_time = result.value("finish_time").toString();
        task->during_time = result.value("during_time").toDouble();

        task->import_level = result.value("import_level").toString();
        task->value = result.value("value").toDouble();
        task->sum_points = result.value("sum_points").toDouble();

        task->repeat_type = result.value("repeat_type").toString();
        task->finish_times = result.value("finish_times").toDouble();
        task->fail_times = result.value("fail_times").toDouble();

        task->belong_type = result.value("belong_type").toString();


        task->is_deduction = (result.value("is_deduction").toInt() == 0) ? false : true;
        task->is_countdown = (result.value("is_countdown").toInt() == 0) ? false : true;
        task->is_can_pause = (result.value("is_can_pause").toInt() == 0) ? false : true;
        task->fail_threshold = result.value("fail_threshold").toDouble();

        task->task_status = result.value("task_status").toString();
        task->statue_resaon = result.value("statue_resaon").toString();//QDateTime::fromString(result.value("finish_time").toString(), "yyyy-MM-dd hh:mm:ss");
        task->parent_task = result.value("parent_task").toString();
        task->remark = result.value("remark").toString();

        m_mapAllCompleteTasks[task->id] = task;
    }

}

//因为可能会产生多个任务，并且界面需要显示，因此id和parentId在界面阶段就需要添加成功，但是其id的确定可以通过逻辑函数接口进行产生
void CDataCenter::insertCompleteTaskTable(const STaskListPtr& data)
{
    //QString dataId = QString::number(m_mapAllTasks.size() + 1) + "-" + QString::number(QDateTime::currentSecsSinceEpoch());

    QString sql = "insert into completed_task(id,name,start_date,deadline,create_time,start_time,finish_time,during_time,import_level,value,sum_points,repeat_type,finish_times,fail_times,belong_type,is_deduction,is_countdown,is_can_pause,fail_threshold,task_status,statue_resaon,parent_task,remark) values(\""
        + data->id + "\","
        + "\"" + data->name + "\","
        + "\"" + data->start_date/*data->start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->deadline/*data->end_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->create_time/*data->create_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->start_time/*data->create_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->finish_time/*data->create_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + QString::number(data->during_time) + ","
        + "\"" + data->import_level + "\","
        + QString::number(data->value) + ","
        + QString::number(data->sum_points) + ","
        + "\"" + data->repeat_type + "\","
        + QString::number(data->finish_times) + ","
        + QString::number(data->fail_times) + ","
        + "\"" + data->belong_type + "\","
        + QString::number(data->is_deduction) + ","
        + QString::number(data->is_countdown) + ","
        + QString::number(data->is_can_pause) + ","
        + QString::number(data->fail_threshold) + ","
        + "\"" + data->task_status + "\","
        + "\"" + data->statue_resaon/*data->finish_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->parent_task + "\","
        + "\"" + data->remark + "\")";

    QString log = "insertCompletedTaskTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Insert"))
    {
        m_mapAllCompleteTasks[data->id] = data;
    }
    return;
}

void CDataCenter::updateCompleteTaskTable(const STaskListPtr& data)
{
    if (0 == m_mapAllCompleteTasks.count(data->id))
    {
        return;
    }
    //存储到数据库
    QString sql = "UPDATE completed_task SET name=\"" + data->name + "\""
        + ",start_date=\"" + data->start_date/*data->start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\""
        + ",deadline=\"" + data->deadline/*data->start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\""
        + ",create_time=\"" + data->create_time + "\""
        + ",start_time=\"" + data->start_time + "\""
        + ",finish_time=\"" + data->finish_time + "\""
        + ",during_time=" + QString::number(data->during_time)
        + ",import_level=\"" + data->import_level + "\""
        + ",value=" + QString::number(data->value)
        + ",sum_points=" + QString::number(data->sum_points)
        + ",repeat_type=\"" + data->repeat_type + "\""
        + ",finish_times=" + QString::number(data->finish_times)
        + ",fail_times=" + QString::number(data->fail_times)
        + ",belong_type=\"" + data->belong_type + "\""
        + ",is_deduction=" + QString::number(data->is_deduction)
        + ",is_countdown=" + QString::number(data->is_countdown)
        + ",is_can_pause=" + QString::number(data->is_can_pause)
        + ",fail_threshold=" + QString::number(data->fail_threshold)
        + ",task_status=\"" + data->task_status + "\""
        + ",statue_resaon=\"" + data->statue_resaon + "\""
        + ",parent_task=\"" + data->parent_task + "\""
        + ",remark=\"" + data->remark + "\""
        + " WHERE id= \"" + data->id + "\"";
        

    QString log = "updateCompletedTaskTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Update"))
    {
        m_mapAllCompleteTasks[data->id] = data;
    }
}

void CDataCenter::deleteCompleteTaskTable(const QString& task_id)
{
    //存储到数据库
    QString sql = "delete from complete_task where id=\"" + task_id + "\"";
    QString log = "deleteCompleteTaskTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Delete"))
    {
        m_mapAllCompleteTasks.remove(task_id);
    }
}


//belong_type_dictionary 激励话语
void CDataCenter::selectSBelongTypeDictionary(bool bSelectAll /* = true */)
{
    QString sql;
    if (!bSelectAll)
    {
        return;
    }

    sql = "select * from belong_type_dictionary";

    QString log = "selectSBelongTypeDictionary sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    QSqlQuery result;
    selectDbData(sql, result, "Select");
    //解析
    QString tempLanguageText = "";//用户自定义添加的类型，不会存在翻译
    while (result.next())
    {
        SBelongTypeDictionary belong;
        belong.belong_id = result.value("belong_id").toString();
        tempLanguageText = language::CLanguageLoad::GetInstance()->getShowText(belong.belong_id);
        belong.show_content = tempLanguageText.isEmpty() ? result.value("show_content").toString() : tempLanguageText;

        m_mapAllBelongType[belong.belong_id] = belong;
    }
}

void CDataCenter::insertSBelongTypeDictionary(const SBelongTypeDictionary& data)
{
    QString sql = "insert into belong_type_dictionary(belong_id,content) values(\""
        + data.belong_id + "\","
        + "\"" + data.show_content + "\")";

    QString log = "insertSBelongTypeDictionary sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Insert"))
    {
        m_mapAllBelongType[data.belong_id] = data;
    }
    return;
}

//import_levle_dictionary 
void CDataCenter::selectSImportLevelDictionary(bool bSelectAll /* = true */)
{
    QString sql;
    if (!bSelectAll)
    {
        return;
    }

    sql = "select * from import_levle_dictionary";

    QString log = "selectSImportLevelDictionary sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    QSqlQuery result;
    selectDbData(sql, result, "Select");
    //解析
    QString tempLanguageText = "";//用户自定义添加的类型，不会存在翻译
    while (result.next())
    {
        SImportLevelDictionary level;
        level.level_id = result.value("level_id").toString();
        tempLanguageText = language::CLanguageLoad::GetInstance()->getShowText(level.level_id);
        level.shouw_text = tempLanguageText.isEmpty() ? result.value("shouw_text").toString() : tempLanguageText;
        level.default_value = result.value("default_value").toInt();
        m_mapAllImportLevelType[level.level_id] = level;
    }
}

void CDataCenter::insertSImportLevelDictionary(const SImportLevelDictionary& data)
{
    QString sql = "insert into import_levle_dictionary(level_id,shouw_text,default_value) values(\""
        + data.level_id + "\","
        + "\"" + data.shouw_text + "\","
        + QString::number(data.default_value) + ")";

    QString log = "insertSImportLevelDictionary sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Insert"))
    {
        m_mapAllImportLevelType[data.level_id] = data;
    }
    return;
}

//repeat_type_dictionary 
void CDataCenter::selectSRepeatTypeDictionary(bool bSelectAll /* = true */)
{
    QString sql;
    if (!bSelectAll)
    {
        return;
    }

    sql = "select * from repeat_type_dictionary";

    QString log = "selectSRepeatTypeDictionary sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    QSqlQuery result;
    selectDbData(sql, result, "Select");
    //解析
    QString tempLanguageText = "";//用户自定义添加的类型，不会存在翻译
    while (result.next())
    {
        SRepeatTypeDictionary repeat;
        repeat.repeat_type = result.value("repeat_type").toString();
        repeat.show_content = result.value("show_content").toString();
        m_mapAllIRepeatType[repeat.repeat_type] = repeat;
    }
}

void CDataCenter::insertSRepeatTypeDictionary(const SRepeatTypeDictionary& data)
{
    QString sql = "insert into repeat_type_dictionary(repeat_type,show_content) values(\""
        + data.repeat_type + "\","
        + data.show_content + ")";

    QString log = "insertSRepeatTypeDictionary sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Insert"))
    {
        m_mapAllIRepeatType[data.repeat_type] = data;
    }
    return;
}

//rewards_type_dictionary 
void CDataCenter::selectSRewardsTypeDictionary(bool bSelectAll /* = true */)
{
    QString sql;
    if (!bSelectAll)
    {
        return;
    }

    sql = "select * from rewards_type_dictionary";

    QString log = "selectSRewardsTypeDictionary sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    QSqlQuery result;
    selectDbData(sql, result, "Select");
    //解析
    QString tempLanguageText = "";//用户自定义添加的类型，不会存在翻译
    while (result.next())
    {
        SRewardsTypeDictionary rewards;
        rewards.type = result.value("type").toString();
        rewards.show_content = result.value("show_content").toString();
        m_mapAllRewardsType[rewards.type] = rewards;
    }
}

void CDataCenter::insertSRewardsTypeDictionary(const SRewardsTypeDictionary& data)
{
    QString sql = "insert into rewards_type_dictionary(type,show_content) values(\""
        + data.type + "\","
        + "\"" + data.show_content + "\")";

    QString log = "insertSImportLevelDictionary sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Insert"))
    {
        m_mapAllRewardsType[data.type] = data;
    }
    return;
}

//task_status_dictionary 
void CDataCenter::selectSStatusTypeDictionary(bool bSelectAll /* = true */)
{
    QString sql;
    if (!bSelectAll)
    {
        return;
    }

    sql = "select * from task_status_dictionary";

    QString log = "selectSStatusTypeDictionary sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    QSqlQuery result;
    selectDbData(sql, result, "Select");
    //解析
    QString tempLanguageText = "";//用户自定义添加的类型，不会存在翻译
    while (result.next())
    {
        SStatusTypeDictionary status;
        status.task_status = result.value("task_status").toString();
        status.show_content = result.value("show_content").toString();
        m_mapAllStatusType[status.task_status] = status;
    }
}

void CDataCenter::insertSStatusTypeDictionary(const SStatusTypeDictionary& data)
{
    QString sql = "insert into task_status_dictionary(task_status,show_content) values(\""
        + data.task_status + "\","
        + "\"" + data.show_content + "\")";

    QString log = "insertSStatusTypeDictionary sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Insert"))
    {
        m_mapAllStatusType[data.task_status] = data;
    }
    return;
}

//daily_history 历史记录表格
void CDataCenter::selectDailtHistoryTable(const QString& taskId /* = "" */, bool bSelectAll /* = true */)
{
    QString sql;
    if (!bSelectAll && !taskId.isEmpty())
    {
        sql = "select * from dailt_history where task_id = \"" + taskId + "\"";
    }
    else
    {
        sql = "select * from dailt_history";
    }


    QString log = "selectDailtHistoryTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    QSqlQuery result;
    selectDbData(sql, result, "Select");
    //解析
    while (result.next())
    {
        SDailyHistory history;
        history.task_id = result.value("task_id").toString();
        history.task_activeType = result.value("task_activeType").toString();
        history.task_date = result.value("task_date").toString();//QDateTime::fromString(result.value("start_time").toString(), "yyyy-MM-dd hh:mm:ss");
        history.task_status = result.value("task_status").toString();//QDateTime::fromString(result.value("end_time").toString(), "yyyy-MM-dd hh:mm:ss");
        history.finish_time = result.value("finish_time").toString();
        history.remark = result.value("remark").toString();

        m_mapAllDailyHistory[history.task_id][history.task_date] = history;
    }
}

void CDataCenter::insertDailtHistoryTable(const SDailyHistory& data)
{
    QString sql = "insert into dailt_history(task_id,task_activeType,task_date,task_status,finish_time,remark) values(\""
        + data.task_id + "\","
        + "\"" + data.task_activeType/*data.start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data.task_date/*data.end_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data.task_status + "\","
        + "\"" + data.finish_time + "\","
        + "\"" + data.remark + "\")";

    QString log = "insertDailtHistoryTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Insert"))
    {
        m_mapAllDailyHistory[data.task_id][data.task_date] = data;
    }
    return;
}

void CDataCenter::updateDailtHistoryTable(const SDailyHistory& data)
{
    if (0 == m_mapAllDailyHistory.count(data.task_id) || 0 == m_mapAllDailyHistory[data.task_id].count(data.task_date))
    {
        return;
    }

    //存储到数据库
    QString sql = "UPDATE dailt_history SET task_activeType=\"" + data.task_activeType + "\""
        + ",task_status=" + data.task_status
        + ",finish_time=\"" + data.finish_time/*data.start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\""
        + ",remark=\"" + data.remark/*data.end_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\""
        + " WHERE task_id= \"" + data.task_id + "\" AND task_date = \""  + data.task_date + " \"";

    QString log = "updateDailtHistory sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Update"))
    {
        m_mapAllDailyHistory[data.task_id][data.task_date] = data;
    }
}

//status_change 任务状态变更记录
void CDataCenter::selectStatusChangeTable(const QString& taskId /* = "" */, bool bSelectAll /* = true */)
{
    QString sql;
    if (!bSelectAll && !taskId.isEmpty())
    {
        sql = "select * from status_change where task_id = \"" + taskId + "\"";
    }
    else
    {
        sql = "select * from status_change";
    }

    

    QString log = "selectStatusChangeTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    QSqlQuery result;
    selectDbData(sql, result, "Select");
    //解析
    while (result.next())
    {
        SStatusChange change;
        change.task_id = result.value("task_id").toString();
        change.task_activeType = result.value("task_activeType").toString();
        change.task_status = result.value("task_status").toString();//QDateTime::fromString(result.value("start_time").toString(), "yyyy-MM-dd hh:mm:ss");
        change.change_time = result.value("change_time").toString();//QDateTime::fromString(result.value("end_time").toString(), "yyyy-MM-dd hh:mm:ss");
        change.reason = result.value("reason").toString();

        m_mapAllStatusChange[change.task_id][change.change_time] = change;
    }
}

void CDataCenter::insertStatusChangeTable(const SStatusChange& data)
{
    QString sql = "insert into status_change(task_id,task_activeType,task_status,change_time,reason) values(\""
        + data.task_id + "\","
        + "\"" + data.task_activeType/*data.start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data.task_status/*data.end_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data.change_time + "\","
        + "\"" + data.reason + "\")";

    QString log = "insertStatusChangeTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Insert"))
    {
        m_mapAllStatusChange[data.task_id][data.change_time] = data;
    }
    return;
}

void CDataCenter::updateStatusChangeTable(const SStatusChange& data)
{
    if (0 == m_mapAllStatusChange.count(data.task_id) || 0 == m_mapAllStatusChange[data.task_id].count(data.change_time))
    {
        return;
    }

    //存储到数据库
    QString sql = "UPDATE status_change SET task_activeType=\"" + data.task_activeType + "\""
        + ",task_status=" + data.task_status
        + ",reason=\"" + data.reason + "\""
        + " WHERE task_id= \"" + data.task_id + "\" AND change_time = \"" + data.change_time + " \"";

    QString log = "updateStatusChange sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Update"))
    {
        m_mapAllStatusChange[data.task_id][data.change_time] = data;
    }
}

//根据从active中获取到活跃区的任务，即进行中或者重新进行中的任务，创建今天的最新任务
    //只是修改map中每个任务的一些属性，不会自动创建新的tasklist，新的taskList产生只有界面操作
void CDataCenter::initTodayRunningTask()
{
    //因为数据会改变
    QMap<QString, STaskListPtr>::iterator iter = m_mapAllActiveTasks.begin();
    QString taskStatus = "";
    QString repeatType = "";
    int nFailThreshold = 0;
    int nFinishTimes = 0;
    int nFailTimes = 0;
    QDateTime startTime;
    QDate endTime;
    QDateTime finishTime;

    //在循环中存储的sql语句，之后再运行
    //因为已有的借口，不然增删查改肯定涉及到已有数据m_mapAllActiveTasks的修改，
    //因此不可能这变循环，那边又在不断的删除，因此这个里面其实可以不做删除操作
    QMap<QString, STaskListPtr> updateData;//需要更新的数据
    QVector<STaskListPtr> deleteAndAddData;//需要在activeTabl中删除，在complete增加的数据
    QMap<QString, QString> taskIdToRemarkOrReason;//当你改变属性，比如状态task_status的时候，可能需要存储history和status改变，需要记录原因

    QVector<QString> newDailyDataTaskID;//这个地方主要是创建新的任务，需要更新日常表格，因此需要记录相关id；这个地方的remark或理由全是 “任务开始”
    QVector<QString> statusChangeTasksId;//具有立碑模式的记录，这里主要记录id
    QMap<QString, QString> statusChangeTaskIdReason;//任务完成状态在自动暂停之前，逻辑上不会产生冲突

    bool isTheEndDay = false;
    QDate newestTaskStartTime;//计算这个任务，最新任务应该开始的世界，比如每日任务，需要计算当天是否在期限内，是就需要创建新任务
                                //如果是记忆曲线类似的，那么下一次可能是1天后、可能是2/4/7/15天后，所以需要计算最新任务时间
    int ForgetingCurve[6] = { 0,1,2,4,7,15 };//记忆曲线：1、2、4、7、15天
    int forgetCurveDiff[5] = {0,1,2,3,8 };//在今日完成的基础上加上多少
    for (iter; iter != m_mapAllActiveTasks.end(); ++iter)
    {
        taskStatus = iter.value()->task_status;
        //如果已经手动完成或者失败的数据，相关属性已经修改完毕了，需要只是删除和移动就行，但是需要分清楚任务重复类型，需要每日更新的，就直接更新
        // 如果类似于一次性的任务，直接移动就行
        //除非在特别的点崩溃了，但是如果嫌些完成表格，再写活跃列表就可以了，因为map的更新是在数据插入之后##########################注意的地方
        //崩溃情况：
        //1.在状态或者map改变前，那么这个数据不会进入表格中，
        //2.表格写入崩溃，这个数据库不会存储改变后的结果
        //3.如果写入后，进行后续操作的时候，崩溃,只会造成active和complete存在两份数据，但是插入的时候是插入不进去的
        //4.因此只需要删除当前的和添加新的就行，如果一开始存在，插入失败不会修改数据
        startTime = textToDateTime(iter.value()->start_time);       

        repeatType = iter.value()->repeat_type;
        nFailThreshold = iter.value()->fail_threshold;
        nFinishTimes = iter.value()->finish_times;
        nFailTimes = iter.value()->fail_times;
        //如果任务的日期和今天相同，就不做处理，如果不相同才会处理
        //因为进行时的任务只有每天第一次打开的时候创建，因此会将日期更新到最新的今天，
        //如果第二次打开，不会进行任务变更，完成的需要显示在界面，不完成也需要显示，
        //就算当前任务被手动完成，今天也不会归档，因为需要让用户知道今天完成了那些任务，
        //而进行时和暂停时的不需要区分，
        //
        //未开始和暂停的的任务不做操作
        if (TASK_STATUS_UNSTART == taskStatus || TASK_STATUS_PAUSE == taskStatus || TASK_STATUS_AUTOPAUSE == taskStatus)
        {
            continue;
        }

        if (REPEAT_TYPE_ONCE == repeatType)
        {
            //从逻辑上如果没有开始时间，是不会进入这里的
            //但是后续看是否需要添加判空条件--------------------------------------------------------------------等待中
            if (QDate::currentDate() == startTime.date())
                continue;
            //进行中的任务，这个时候会变成失败
            if (TASK_STATUS_INPROGRESS == taskStatus || TASK_STATUS_RESTART == taskStatus)
            {
                newestTaskStartTime = startTime.date().addDays(nFailTimes);
                if (newestTaskStartTime == QDate::currentDate())
                {
                    continue;
                }

                iter.value()->fail_times += 1;
                iter.value()->task_status = TASK_STATUS_FILED;

                taskIdToRemarkOrReason[iter.value()->id] = getLanguegeShowText("reason_task_overTime");
                deleteAndAddData.push_back(iter.value());
                //这个地方的记录主要记录被这里逻辑自动修改的，而不是界面手动操作的
                statusChangeTasksId.push_back(iter.value()->id);
                statusChangeTaskIdReason[iter.value()->id] = getLanguegeShowText("reason_task_overTime");
            }
            //失败或者完成的任务，相关属性已经修改完毕了，需要的只是将这个任务移到该去的数据表格
            else if (TASK_STATUS_FILED == repeatType || TASK_STATUS_FINISH == repeatType)
            {
                deleteAndAddData.push_back(iter.value());
            }
        }
        //在设定的期限范围内，每天创建新的任务-只是更新一部分数据
        else if (REPEAT_TYPE_EVERYDAY == repeatType)
        {
            //还未到开始时间
            if (QDate::currentDate() < startTime.date())
            {
                continue;
            }
            //这中人物是存在截止日期
            endTime = textToDate(iter.value()->deadline);           
            //进行中的任务，这个时候会变成失败
            if (TASK_STATUS_INPROGRESS == taskStatus || TASK_STATUS_RESTART == taskStatus)
            {
                iter.value()->fail_times += 1;
                if (endTime < QDate::currentDate())//
                {
                    iter.value()->task_status = TASK_STATUS_FINISH;
                    deleteAndAddData.push_back(iter.value());
                    taskIdToRemarkOrReason[iter.value()->id] = getLanguegeShowText("reason_task_normalEnd");

                    //这个地方的记录主要记录被这里逻辑自动修改的，而不是界面手动操作的
                    statusChangeTasksId.push_back(iter.value()->id);
                    statusChangeTaskIdReason[iter.value()->id] = getLanguegeShowText("reason_task_normalEnd");
                }
                else
                {
                    iter.value()->fail_times += 1;
                    updateData[iter.value()->id] = (iter.value());
                    //这个地方的记录主要记录被这里逻辑自动修改的，而不是界面手动操作的--新的任务更新了
                    newDailyDataTaskID.push_back(iter.value()->id);
                }
            }
            //这个表明当前任务完成了，所以需要计算最新任务时间
            else if (TASK_STATUS_FILED == taskStatus || TASK_STATUS_FINISH == taskStatus)
            {
                //这个时候一些属性已经被完成了，只需要修改剩下的
                if (endTime < QDate::currentDate())//表明今天已经超过了任务设定的截止日期
                {
                    deleteAndAddData.push_back(iter.value());
                    taskIdToRemarkOrReason[iter.value()->id] = getLanguegeShowText("reason_task_normalEnd");

                    //这个地方的记录主要记录被这里逻辑自动修改的，而不是界面手动操作的
                    statusChangeTasksId.push_back(iter.value()->id);
                    statusChangeTaskIdReason[iter.value()->id] = getLanguegeShowText("reason_task_normalEnd");
                }
                //如果日期相等，则表明是当天的任务，不进行任何操作，
                else if(endTime > QDate::currentDate())//表明可能需要更新任务来表明是今天的任务
                {
                    newestTaskStartTime = startTime.date().addDays(iter.value()->finish_times + iter.value()->fail_times - 1);
                    if (newestTaskStartTime == QDate::currentDate())//如果相等，则表明今天的已经创建了，因此不需要操作
                    {
                        continue;
                    }
                    iter.value()->task_status = TASK_STATUS_INPROGRESS;
                    updateData[iter.value()->id] = (iter.value());

                    //这个地方的记录主要记录被这里逻辑自动修改的，而不是界面手动操作的--新的任务更新了
                    newDailyDataTaskID.push_back(iter.value()->id);
                }
            }
        }
        else if (REPEAT_TYPE_FORGETTING_CURVE == repeatType)
        {
            //记忆曲线：1、2、4、7、15天
            //还未到开始时间
            if (QDate::currentDate() < startTime.date())
            {
                continue;
            }
            //进行中的任务，这个时候会变成失败
            if (TASK_STATUS_INPROGRESS == taskStatus || TASK_STATUS_RESTART == taskStatus)
            {                
                if (0 == iter.value()->finish_times)
                {
                    newestTaskStartTime = QDate::currentDate();
                }
                else
                {
                    newestTaskStartTime = finishTime.date().addDays(ForgetingCurve[iter.value()->finish_times]);
                }

                if (newestTaskStartTime == QDate::currentDate())
                {
                    continue;
                }

                iter.value()->fail_times += 1;//失败次数+1
                updateData[iter.value()->id] = (iter.value());
                //这个任务没有明确的截止日期，所以只需要判断就行
            }//记忆曲线的完成当前任务完成时间会被记录
            else if (TASK_STATUS_FINISH == taskStatus)/*||  TASK_STATUS_FILED== repeatType*///点击失败表明失败次数已经累加了)
            {
                finishTime = textToDateTime(iter.value()->finish_time);
                if(QDate::currentDate() == finishTime.date())
                    continue;

                if (5 == iter.value()->finish_times)
                {
                    deleteAndAddData.push_back(iter.value());
                    taskIdToRemarkOrReason[iter.value()->id] = getLanguegeShowText("reason_task_normalEnd");
                    continue;
                }

                iter.value()->task_status = TASK_STATUS_SLEEP;//休眠状态
                updateData[iter.value()->id] = (iter.value());
            }
            else if(TASK_STATUS_SLEEP == taskStatus)
            {
                newestTaskStartTime = finishTime.date().addDays(ForgetingCurve[iter.value()->finish_times]);
                if (QDate::currentDate() == newestTaskStartTime)
                {
                    iter.value()->task_status = TASK_STATUS_INPROGRESS;//状态改成进行中
                    iter.value()->finish_time = dateTimeToShowText(DATETIME_EMPTY);
                    updateData[iter.value()->id] = (iter.value());

                    //这个地方的记录主要记录被这里逻辑自动修改的，而不是界面手动操作的--新的任务更新了
                    newDailyDataTaskID.push_back(iter.value()->id);
                }
            }
        }
        else if (REPEAT_TYPE_ALWAYS == repeatType)//只有手动完成或者失败、暂停，所以自动不考虑这些情况
        {
            //还未到开始时间
            if (QDate::currentDate() < startTime.date())
            {
                continue;
            }
            //进行中的任务，这个时候会变成失败
            if (TASK_STATUS_INPROGRESS == taskStatus || TASK_STATUS_RESTART == taskStatus)
            {
                newestTaskStartTime = startTime.date().addDays(nFailTimes);
                if (newestTaskStartTime == QDate::currentDate())
                {
                    continue;
                }
                iter.value()->fail_times += 1;
                updateData[iter.value()->id] = (iter.value());
                taskIdToRemarkOrReason[iter.value()->id] = getLanguegeShowText("reason_task_normalEnd");                   
            }
            //这个表明当前任务完成了，所以需要计算最新任务时间
            else if (TASK_STATUS_FILED == taskStatus || TASK_STATUS_FINISH == taskStatus)
            {
                newestTaskStartTime = startTime.date().addDays(iter.value()->finish_times + iter.value()->fail_times - 1);
                if (newestTaskStartTime == QDate::currentDate())//如果相等，则表明今天的已经创建了，因此不需要操作
                {
                    continue;
                }
                iter.value()->task_status = TASK_STATUS_INPROGRESS;
                updateData[iter.value()->id] = (iter.value());

                //这个地方的记录主要记录被这里逻辑自动修改的，而不是界面手动操作的--新的任务更新了
                newDailyDataTaskID.push_back(iter.value()->id);
            }
        }
            
        
    }

    //再次遍历数据，将失败次数过多的任务暂停   
    iter = m_mapAllActiveTasks.begin();
    for (iter; iter != m_mapAllActiveTasks.end(); ++iter)
    {
        if (iter.value()->fail_times >= iter.value()->fail_threshold)
        {
            iter.value()->task_status = TASK_STATUS_AUTOPAUSE;
            taskIdToRemarkOrReason[iter.value()->id] = getLanguegeShowText("reason_task_autoPause");
            updateData[iter.value()->id] = (iter.value());
            if (!statusChangeTaskIdReason.contains(iter.value()->id))
            {
                statusChangeTasksId.push_back(iter.value()->id);
                statusChangeTaskIdReason[iter.value()->id] = getLanguegeShowText("reason_task_autoPause");
            }
        }
    }
    //根据留存的数据更新数据库
    //更新的数据
    QMap<QString, STaskListPtr>::const_iterator iterUpdate = updateData.begin();
    for (iterUpdate; iterUpdate != updateData.end(); ++iterUpdate)
    {
        updateActiveTableTask(iterUpdate.value());
    }

    //删除和移动的数据
    QVector<STaskListPtr>::const_iterator iterDelAndAdd = deleteAndAddData.begin();
    for (iterDelAndAdd; iterDelAndAdd != deleteAndAddData.end(); ++iterDelAndAdd)
    {
        deleteActiveTaskTable((*iterDelAndAdd)->id);
        insertCompleteTaskTable(*iterDelAndAdd);
    }
    //创建任务状态变化的记录
    QVector<QString>::const_iterator statusChangeIter = statusChangeTasksId.begin();
    for (statusChangeIter; statusChangeIter != statusChangeTasksId.end(); ++statusChangeIter)
    {
        addOrUpdateDailyHistoryBytaskId(*statusChangeIter, TASK_ACTIVETYPE_ACTIVE, statusChangeTaskIdReason[*statusChangeIter]);
    }
    //创建每日记录
    QVector<QString>::const_iterator dailyIter = newDailyDataTaskID.begin();
    for (dailyIter; dailyIter != newDailyDataTaskID.end(); ++dailyIter)
    {
        addOrUpdateDailyHistoryBytaskId(*statusChangeIter, TASK_ACTIVETYPE_ACTIVE, getLanguegeShowText("reason_task_start"));
    }
}