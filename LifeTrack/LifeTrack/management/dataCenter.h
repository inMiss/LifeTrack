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
    //���ݿ����
    void addNewTask(const STaskListPtr& list);
    void addNewTasks(const QMap<QString, STaskListPtr>& list);
private:
    void initDbPath();
    bool selectDbData(const QString& sql, QSqlQuery& result, const QString& tableName, const QString& connName = "Select", const QString& dbPath = "");
    //�޷���ֵ������ɾ����
    bool operationDb(const QString& sql, const QString& connName);
    //������  
    //daily_summary ÿ���ܽ�
    void selectDailySummaryTable(const QString& strarTime = "", const QString& endTime = "", bool bSelectAll = true);
    void insertDailySummaryTable(const SDailySummary& data);
    //incentive_content ��������
    void selectIncentiveContentTable(bool bSelectAll = true);
    void insertIncentiveContentTable(const SIncentiveContent& data);
    //rewards ����
    void selectRewardsTable(bool bSelectAll = true);
    void insertRewardsTable(const SRewards& data);
    void updateRewardsTable(const SRewards& data);
    //task_list--Ŀǰ�ȶ�ȡ���еģ����ڿ��Ƿ���Ҫ�ֳ�1���»���1��ʱ��֮ǰ��Ϊ��ʷ���ݱ��
    void selectTaskListTable(bool bSelectAll = true);
    void insertTaskListTable(const STaskListPtr& data);
    void updateTaskListTable(const STaskListPtr& data);
    //belong_type_dictionary �����������
    void selectSBelongTypeDictionary(bool bSelectAll = true);
    void insertSBelongTypeDictionary(const SBelongTypeDictionary& data);
    //import_levle_dictionary ���񼶱�
    void selectSImportLevelDictionary(bool bSelectAll = true);
    void insertSImportLevelDictionary(const SImportLevelDictionary& data);

    //���漰������ҵ���߼���д���ֵ䣬���ṩ�ⲿ�趨����--���ǽ�������combox��Ҫ��ʾȫ�������ÿ�λ��Ƕ�ȡ����
    //repeat_type_dictionary ���񼶱�
    void selectSRepeatTypeDictionary(bool bSelectAll = true);
    void insertSRepeatTypeDictionary(const SRepeatTypeDictionary& data);
    //rewards_type_dictionary ���񼶱�
    void selectSRewardsTypeDictionary(bool bSelectAll = true);
    void insertSRewardsTypeDictionary(const SRewardsTypeDictionary& data);
    //task_status_dictionary ���񼶱�
    void selectSStatusTypeDictionary(bool bSelectAll = true);
    void insertSStatusTypeDictionary(const SStatusTypeDictionary& data);

    //���ݿ�д���߳�
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
    //���ݿ����
    DBMana  m_dbManager;
    QString m_dbPath;
    QString m_qsDataBaseFilePath;


    QMutex  m_dataMutex;
};
