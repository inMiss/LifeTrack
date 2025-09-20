#include "dataCenter.h"
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QFile>
#include <QVariant>

#include "../common/global.h"
#include "../spdlog/spdlog.h"
#include "../language/language.h"


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

QMap<QString, STaskListPtr> CDataCenter::getAllTasks()
{
    return m_mapAllTasks;
}

QMap<QString, STaskListPtr> CDataCenter::getTasksByDate(const QDate& chooseDate)
{
    return m_mapDateToTasks[chooseDate];
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

void CDataCenter::addNewTask(const STaskListPtr& list)
{
    insertTaskListTable(list);
}

void CDataCenter::addNewTasks(const QMap<QString, STaskListPtr>& list)
{
    QMap<QString, STaskListPtr>::const_iterator iter = list.begin();
    for (iter; iter != list.end(); ++iter)
    {
        addNewTask(iter.value());
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
    selectTaskListTable();

    selectSBelongTypeDictionary();
    selectSImportLevelDictionary();
    selectSRepeatTypeDictionary();
    selectSRewardsTypeDictionary();
    selectSStatusTypeDictionary();
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
void CDataCenter::selectTaskListTable(bool bSelectAll /* = true */)
{
    QString sql;
    if (!bSelectAll)
    {
        return;
    }

    sql = "select * from task_list";

    QString log = "selectTaskListTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());

    QSqlQuery result;
    selectDbData(sql, result, "Select");
    //解析
    while (result.next())
    {
        STaskListPtr task = STaskListPtr(new STaskList());
        task->id = result.value("id").toString();
        task->name = result.value("name").toString();
        task->start_time = result.value("start_time").toString();//QDateTime::fromString(result.value("start_time").toString(), "yyyy-MM-dd hh:mm:ss");
        task->end_time = result.value("end_time").toString();//QDateTime::fromString(result.value("end_time").toString(), "yyyy-MM-dd hh:mm:ss");
        task->during_time = result.value("during_time").toDouble();
        task->create_time = result.value("create_time").toString();//QDateTime::fromString(result.value("create_time").toString(), "yyyy-MM-dd hh:mm:ss");
        task->import_level = result.value("import_level").toString();
        task->value = result.value("value").toDouble();
        task->repeat_type = result.value("repeat_type").toString();
        task->finish_times = result.value("finish_times").toDouble();
        task->sum_points = result.value("sum_points").toDouble();
        task->belong_type = result.value("belong_type").toString();
        task->is_deduction = (result.value("is_deduction").toInt() == 0)?false : true;
        task->is_countdown = (result.value("is_countdown").toInt() == 0) ? false : true;
        task->is_can_pause = (result.value("is_can_pause").toInt() == 0) ? false : true;
        task->task_status = result.value("task_status").toString();
        task->finish_time = result.value("finish_time").toString();//QDateTime::fromString(result.value("finish_time").toString(), "yyyy-MM-dd hh:mm:ss");
        task->pause_time = result.value("pause_time").toString();//QDateTime::fromString(result.value("pause_time").toString(), "yyyy-MM-dd hh:mm:ss");
        task->parent_task = result.value("parent_task").toString();
        task->remark = result.value("remark").toString();

        m_mapAllTasks[task->id] = task;
        m_mapDateToTasks[textToDate(task->start_time)][task->id] = task;
    }

}

//因为可能会产生多个子任务，并且界面需要显示，因此id和parentId在界面阶段就需要添加成功，但是其id的确定可以通过逻辑函数接口进行产生
void CDataCenter::insertTaskListTable(const STaskListPtr& data)
{
    //QString dataId = QString::number(m_mapAllTasks.size() + 1) + "-" + QString::number(QDateTime::currentSecsSinceEpoch());

    QString sql = "insert into task_list(id,name,start_time,end_time,during_time,create_time,import_level,value,repeat_type,finish_times,sum_points,belong_type,is_deduction,is_countdown,is_can_pause,task_status,finish_time,pause_time,parent_task,remark) values(\""
        + data->id + "\","
        + "\"" + data->name + "\","
        + "\"" + data->start_time/*data->start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->end_time/*data->end_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","      
        + QString::number(data->during_time) + ","
        + "\"" + data->create_time/*data->create_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->import_level + "\","
        + QString::number(data->value) + ","
        + "\"" + data->repeat_type + "\","
        + QString::number(data->finish_times) + ","
        + QString::number(data->sum_points) + ","
        + "\"" + data->belong_type + "\","
        + QString::number(data->is_deduction) + ","
        + QString::number(data->is_countdown) + ","
        + QString::number(data->is_can_pause) + ","
        + "\"" + data->task_status + "\","
        + "\"" + data->finish_time/*data->finish_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->pause_time/*data->pause_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\","
        + "\"" + data->parent_task + "\","
        + "\"" + data->remark + "\")";

    QString log = "insertTaskListTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Insert"))
    {
        m_mapAllTasks[data->id] = data;
        m_mapDateToTasks[textToDate(data->start_time)][data->id] = data;
    }
    return;
}

void CDataCenter::updateTaskListTable(const STaskListPtr& data)
{
    if (0 == m_mapAllTasks.count(data->id))
    {
        return;
    }

    //存储到数据库
    QString sql = "UPDATE task_list SET name=\"" + data->name + "\""
        + ",start_time=\"" + data->start_time/*data->start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\""
        + ",end_time=\"" + data->start_time/*data->start_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\""
        + ",during_time=" + data->during_time
        + ",create_time=" + data->create_time
        + ",import_level=\"" + data->import_level + "\""
        + ",value=" + data->value
        + ",repeat_type=\"" + data->repeat_type + "\""
        + ",finish_times=" + data->finish_times
        + ",sum_points=" + data->sum_points
        + ",belong_type=\"" + data->belong_type + "\""
        + ",is_deduction=" + data->is_deduction
        + ",is_countdown=" + data->is_countdown
        + ",is_can_pause=" + data->is_can_pause
        + ",task_status=\"" + data->task_status + "\""
        + ",finish_time=\"" + data->finish_time/*data->finish_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\""
        + ",pause_time=\"" + data->pause_time/*data->pause_time.toString("yyyy-MM-dd hh:mm:ss")*/ + "\""
        + ",parent_task=\"" + data->parent_task + "\""
        + ",remark=\"" + data->remark + "\""
        + " WHERE id= \"" + data->id + "\"";

    QString log = "updateTaskListTable sql:" + sql;
    SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), log.toStdString().c_str());
    if (operationDb(sql, "Update"))
    {
        m_mapAllTasks[data->id] = data;
        m_mapDateToTasks[textToDate(data->start_time)][data->id] = data;
    }

    m_mapAllTasks[data->id] = data;
    m_mapDateToTasks[textToDate(data->start_time)][data->id] = data;
   
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