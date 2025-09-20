#ifndef LIFETRACK_STRUCTCOMMON_COMMON_H
#define LIFETRACK_STRUCTCOMMON_COMMON_H

#include <QString>
#include <QDateTime>
#include <QSharedDataPointer>
#include "../common/commonFunc.h"

#define DATETIME_EMPTY QDateTime(QDate(1000, 1, 1), QTime(1, 1, 1))
#define DATETIME_MAX QDateTime(QDate(9999, 1, 1), QTime(1, 1, 1))
#define DATETIME_EMPTY_SHOW "---"
//类型等不会影响具体的业务逻辑，只会影响一些显示或者任务设定，因此可以进行外部添加
//任务所属类型，如英语学习、健身等
//#define BELONG_ID_FITNESS "belong_id_fintness" //健身
//#define BELONG_ID_WRITING "belong_id_write" //写作
//belong_type_dictionary [任务所属类型]
typedef struct SBelongTypeDictionary {
    SBelongTypeDictionary() {};
    SBelongTypeDictionary(QString belong_id, QString show_content, QString parent_belong_id)
    {
        this->belong_id = belong_id;
        this->show_content = show_content;
        this->parent_belong_id = parent_belong_id;
    };
    ~SBelongTypeDictionary()
    {
        this->belong_id.clear();
        this->show_content.clear();
        this->parent_belong_id.clear();
    }

    QString     belong_id;        //所属种类id
    QString     show_content;     //所属种类显示说明
    QString     parent_belong_id; //如果是二级类型，这个地方会是上面一级类型的belong_id
};

//重要分级不会影响具体的业务逻辑，只会影响一些显示或者任务设定和分值计算，因此可以进行外部添加
//但是分值计算会影响到已有的所有数据计算，导致数据的前后不一致，因此不建议随意更改
//#define IMPORT_LEVEL_FIRST "import_level_first"//1
//#define IMPORT_LEVEL_SECOND "import_level_second"//2
//#define IMPORT_LEVEL_THIRD "import_level_third"//3
//#define IMPORT_LEVEL_FOUTH "import_level_fouth"//4
//import_levle_dictionary [重要程度表格]
typedef struct SImportLevelDictionary {
    SImportLevelDictionary() {};
    SImportLevelDictionary(QString level_id, QString shouw_text,int default_value)
    {
        this->level_id = level_id;
        this->shouw_text = shouw_text;
        this->default_value = default_value;
    };
    ~SImportLevelDictionary()
    {
        this->level_id.clear();
        this->shouw_text.clear();
    }

    QString     level_id;        //级别id
    QString     shouw_text;     //显示文字
    int     default_value;     //所代表的默认分值
};


//会涉及到具体业务逻辑编写的字典，不提供外部设定功能,但是界面的combox显示所有则需要从db读取
//奖励类型
//#define REWARDS_TYPE_DEFINE "rewards_type_define"  //默认/成就
typedef struct SRewardsTypeDictionary {
    SRewardsTypeDictionary() {};
    SRewardsTypeDictionary(QString type, QString show_content)
    {
        this->type = type;
        this->show_content = show_content;
    };
    ~SRewardsTypeDictionary()
    {
        this->type.clear();
        this->show_content.clear();
    }

    QString     type;        //所属种类id
    QString     show_content;     //所属种类显示说明
};

//重复类型
#define REPEAT_TYPE_ONCE "repeat_type_once"//一次
#define REPEAT_TYPE_EVERYDAY "repeat_type_everyday"//每天重复
#define REPEAT_TYPE_FORGETTING_CURVE "repeat_type_forgetting_curve"//遗忘曲线
#define REPEAT_TYPE_ALWAYS "repeat_type_always"//一直持续
typedef struct SRepeatTypeDictionary {
    SRepeatTypeDictionary() {};
    SRepeatTypeDictionary(QString repeat_type, QString show_content)
    {
        this->repeat_type = repeat_type;
        this->show_content = show_content;
    };
    ~SRepeatTypeDictionary()
    {
        this->repeat_type.clear();
        this->show_content.clear();
    }

    QString     repeat_type;        //所属种类id
    QString     show_content;     //所属种类显示说明
};

//状态
 #define TASK_STATUS_BEGIN       "task_status_begin"//开始
 #define TASK_STATUS_INPROGRESS  "task_status_inProgress"//进行中
 #define TASK_STATUS_STOP        "task_status_stop"//中断
 #define TASK_STATUS_RESTART     "task_status_restart"//重新开始
 #define TASK_STATUS_FILED       "task_status_filed"//失败
 #define TASK_STATUS_FINISH      "task_status_finish"          //完成
 #define TASK_STATUS_UNDETERMINED "task_status_undetermined" //未确认

typedef struct SStatusTypeDictionary {
    SStatusTypeDictionary() {};
    SStatusTypeDictionary(QString task_status, QString show_content)
    {
        this->task_status = task_status;
        this->show_content = show_content;
    };
    ~SStatusTypeDictionary()
    {
        this->task_status.clear();
        this->show_content.clear();
    }

    QString     task_status;        //所属种类id
    QString     show_content;     //所属种类显示说明
};
///////////////////////////////////////////
//daily_summary [每日总结表格]
typedef struct SDailySummary {
    SDailySummary() {};
    SDailySummary(QString countId,QString date, QString content, int task_total, int finish_task_sum, double point_total, double finish_point_sum)
    {
        this->id = countId;
        this->date = date;
        this->content = content;
        this->task_total = task_total;
        this->finish_task_sum = finish_task_sum;
        this->point_total = point_total;
        this->finish_point_sum = finish_point_sum;
    };
    ~SDailySummary()
    {
        this->content.clear();
    }

    QString     id;       //自增序号
    QString   date;           //当前日期
    QString     content;        //所写每日总结内容
    int         task_total;     //当日任务总数
    int         finish_task_sum;//所完成的任务总数
    double      point_total;    //当日任务总积分
    double      finish_point_sum;//当日获取的总结分
};

//rewards [设定的奖励/成就]
typedef struct SRewards {
    SRewards() 
    {
        value = -1;
        is_time_limited = false;
    };
    SRewards(QString id, bool is_time_limited, QString start_time, QString end_time, QString goal_type, QString content, double value,QString status, QString remark)
    {
        this->id = id;
        this->is_time_limited = is_time_limited;
        this->start_time = start_time;
        this->end_time = end_time;
        this->goal_type = goal_type;
        this->content = content;
        this->value = value;
        this->status = status;
        this->remark = remark;
    };
    ~SRewards()
    {
        this->goal_type.clear();
        this->content.clear();
        this->status.clear();
        this->remark.clear();
    }
    QString         id;             //序号id
    bool        is_time_limited;//是否是限时完成的愿望
    QString   start_time;     //开始时间
    QString   end_time;       //结束时间
    QString     goal_type;      //奖励类型：分值累计，指定任务
    QString     content;        //奖励内容
    double      value;          //分值类型，的达标分值
    QString     status;         //当前奖励状态--完成、失败等
    QString     remark;         //备注
};

//task_list [任务属性]
typedef struct STaskList {
    STaskList() 
    {
        this->id = "";
        this->name = "";
        this->start_time = DATETIME_EMPTY_SHOW;
        this->end_time = DATETIME_EMPTY_SHOW;
        this->during_time = 0;
        this->create_time = this->start_time;
        this->import_level = "import_level_first";
        this->value = 0;
        this->repeat_type = "repeat_type_once";
        this->finish_times = 0;
        this->sum_points = 0;
        this->belong_type = "belong_id_fintness";
        this->is_deduction = false;
        this->is_countdown = false;
        this->is_can_pause = false;
        this->task_status = "task_status_undetermined";
        this->finish_time = DATETIME_EMPTY_SHOW;
        this->pause_time = DATETIME_EMPTY_SHOW;
        this->parent_task = "";
        this->remark = "";
    };
    STaskList(QString id, QString name, QString start_time, QString end_time, double during_time, QString create_time, QString import_level, int value, QString repeat_type,
        int finish_times, double sum_points, QString belong_type, bool is_deduction, bool is_countdown, bool is_can_pause, QString task_status, QString finish_time
        , QString pause_strat_time, QString parent_task, QString remark)
    {
        this->id = id;
        this->name = name;
        this->start_time = start_time;
        this->end_time = end_time;
        this->during_time = during_time;
        this->create_time = create_time;
        this->import_level = import_level;
        this->value = value;
        this->repeat_type = repeat_type;
        this->finish_times = finish_times;
        this->sum_points = sum_points;
        this->belong_type = belong_type;
        this->is_deduction = is_deduction;
        this->is_countdown = is_countdown;
        this->is_can_pause = is_can_pause;
        this->task_status = task_status;
        this->finish_time = finish_time;
        this->pause_time = pause_strat_time;
        this->parent_task = parent_task;
        this->remark = remark;
    };
    ~STaskList()
    {
        this->id.clear();
        this->name.clear();
        this->import_level.clear();
        this->repeat_type.clear();
        this->belong_type.clear();
        this->task_status.clear();
        this->parent_task.clear();
        this->remark.clear();
    }
    QString     id;      //任务id
    QString     name;        //任务内容，名称
    QString   start_time;         //开始时间
    QString   end_time;
    double      during_time;         //持续时间(结束时间,单位:s)
    QString   create_time;
    QString     import_level;         //重要性层级
    int         value;             //这个任务所设定的分值
    QString     repeat_type;         //重复类型、每日、重复等
    int         finish_times;             //完成次数，默认为0，如每日，则持续累加
    double      sum_points;             //这个任务累计分数，如果有子任务，也需要累加
    QString     belong_type;         //所属类型，英语、锻炼等
    bool        is_deduction;//是否进行分值的减去，任务失败扣除累计分值
    bool        is_countdown;//是否进行倒计时提醒，
    bool        is_can_pause;//是否能够进行暂停，如果false，则断掉后，直接判断为失败
    QString     task_status;         //任务状态，开始、结束、暂停
    QString   finish_time;          //任务完成时间
    QString   pause_time;             //暂停开始时间
    QString     parent_task;         //父任务id
    QString     remark;         //备注
};

using STaskListPtr = QSharedPointer<STaskList>;

//incentive_content [激励语句]
typedef struct SIncentiveContent {
    SIncentiveContent() {};
    SIncentiveContent(QString number,QString content)
    {
        this->number = number;
        this->content = content;

    };
    ~SIncentiveContent()
    {
        this->content.clear();
    }
    QString     number;         //当前奖励状态--完成、失败等
    QString     content;         //备注
};
#endif