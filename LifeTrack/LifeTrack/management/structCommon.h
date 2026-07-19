#ifndef LIFETRACK_STRUCTCOMMON_COMMON_H
#define LIFETRACK_STRUCTCOMMON_COMMON_H

#include <QString>
#include <QDateTime>
#include <QSharedDataPointer>
#include <QSharedPointer>

#define DATETIME_EMPTY QDateTime(QDate(1000, 1, 1), QTime(1, 1, 1))
#define DATETIME_MAX QDateTime(QDate(9999, 1, 1), QTime(1, 1, 1))
#define DATE_EMPTY QDate(QDate(1000, 1, 1))
#define DATE_MAX QDate(QDate(9999, 1, 1))
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
    struct SImportLevelDictionary {
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
#define TASK_STATUS_UNSTART "task_status_unstart" //未开始 
#define TASK_STATUS_PAUSE       "task_status_pause"//暂停
#define TASK_STATUS_AUTOPAUSE       "task_status_autoPause"//自动暂停-设定的拖延期限内未开始进行任务，会自动暂停

#define TASK_STATUS_INPROGRESS  "task_status_inProgress"//进行中
#define TASK_STATUS_RESTART     "task_status_restart"//重新进行   

#define TASK_STATUS_FILED       "task_status_filed"//失败
#define TASK_STATUS_FINISH      "task_status_finish"  //完成

#define TASK_STATUS_SLEEP     "task_status_sleep"  //休眠中--目前专属于记忆曲线的，表明在间隔内的等待状态

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
    QString     show_content;       //所属种类显示说明
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
    double      value;          //分值类型：的达标分值
    QString     status;         //当前奖励状态--完成、失败等
    QString     remark;         //备注
};

//task_list [任务属性]
//包含活跃任务表格和完成任务表格，但是参数和结构相同
//active_task 和 completed_task
typedef struct STaskList {
    STaskList() 
    {
        this->id = "";
        this->name = "";

        this->start_date = DATETIME_EMPTY_SHOW;
        this->deadline = DATETIME_EMPTY_SHOW;

        this->create_time = DATETIME_EMPTY_SHOW;
        this->start_time = DATETIME_EMPTY_SHOW;
        this->finish_time = DATETIME_EMPTY_SHOW;
        this->during_time = 0;

        this->import_level = "import_level_first";
        this->value = 0;
        this->sum_points = 0;

        this->belong_type = "belong_id_fintness";

        this->repeat_type = "repeat_type_once";
        this->finish_times = 0;
        this->fail_times = 0;
        
        this->is_deduction = false;
        this->is_countdown = false;
        this->is_can_pause = false;
        this->fail_threshold = 3;

        this->task_status = "task_status_unstart";
        this->statue_resaon = "";

        this->parent_task = "";
        this->remark = "";
 
    };
    STaskList(QString id, QString name, QString start_date, QString deadline, QString create_time, QString start_time, QString finish_time, double during_time,
        QString import_level, int value, double sum_points, QString repeat_type, int finish_times, int fail_times, QString belong_type, bool is_deduction,
        bool is_countdown, bool is_can_pause, int fail_threshold, QString task_status, QString statue_resaon, QString parent_task, QString remark)
    {
        this->id = id;
        this->name = name;

        this->start_date = start_date;
        this->deadline = deadline;

        this->create_time = create_time;
        this->start_time = start_time;

        this->finish_time = finish_time;
        this->during_time = during_time;

        this->import_level = import_level;
        this->value = value;
        this->sum_points = sum_points;

        this->repeat_type = repeat_type;
        this->finish_times = finish_times;
        this->fail_times = fail_times;

        this->belong_type = belong_type;

        this->is_deduction = is_deduction;
        this->is_countdown = is_countdown;
        this->is_can_pause = is_can_pause;
        this->fail_threshold = fail_threshold;

        this->remark = remark;
        this->fail_threshold = fail_threshold;

        this->task_status = task_status;
        this->statue_resaon = statue_resaon;

        this->parent_task = parent_task;
        this->remark = remark;
    };
    ~STaskList()
    {
        this->id.clear();
        this->name.clear();
        this->start_date.clear();
        this->deadline.clear();

        this->create_time.clear();
        this->start_time.clear();
        this->finish_time.clear();
        this->import_level.clear();

        this->repeat_type.clear();
        this->belong_type.clear();
        this->task_status.clear();
        this->statue_resaon.clear();

        this->parent_task.clear();
        this->remark.clear();
    }
    QString     id;             //任务id
    QString     name;           //任务内容，名称

    QString     start_date;        //开始日期--这个任务在创建时候，是否设定了开始日期，有些日期可以设定，比如下周我要做啥，比如一次的任务是有开始时间的
    QString     deadline;         //截止日期
    
    QString     create_time;    //这个任务创建时的时间   
    QString     start_time;       //这个任务的开始时间
    
    QString     finish_time;    //任务完成时间 最近的完成时间-精确到分秒
    double      during_time;         //持续时间(结束时间,单位:s)

    QString     import_level;         //重要性层级
    int         value;                //这个任务所设定的分值
    double      sum_points;          //这个任务累计分数，如果有子任务，也需要累加

    QString     repeat_type;         //重复类型、每日、重复等
    int         finish_times;        //完成次数，默认为0，如每日，则持续累加
    int         fail_times;         //失败次数

    QString     belong_type;         //所属类型，英语、锻炼等

    bool        is_deduction;//是否进行分值的减去，任务失败扣除累计分值
    bool        is_countdown;//是否进行倒计时提醒，
    bool        is_can_pause;//是否能够进行暂停，如果false，则断掉后，直接判断为失败
    int         fail_threshold;//拖延阈值，一个任务如果连续拖延天数，就视为自动暂停，或者失败

    QString     task_status;    //任务状态，开始、结束、暂停
    QString     statue_resaon;//由于自动暂停、失败等原因记录，记录最新的原因

    QString     parent_task;    //父任务id
    QString     remark;         //备注--用户添加的任务详情备注
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

#define TASK_ACTIVETYPE_NO_ACTIVE "task_activeType_no_active" //刚创建状态
#define TASK_ACTIVETYPE_ACTIVE "task_activeType_active" //只要一开始，包括进行、暂停、重启
#define TASK_ACTIVETYPE_FINISHED "task_activeType_finished"//任务完成:这个主要用来在任务详情界面显示是查询任务的基础信息，所以完成状态包括成功和失败，都在完成表格中

//daily_history [历史任务记录表格]
//目前每天会在已有的活跃任务创建每日的任务，不会创建未来的任务，
//但是每个任务当天的状态会被存储下来，作为当天的历史存储
//主要记录活跃任务的每日任务记录
struct SDailyHistory {
    SDailyHistory() { };
    SDailyHistory(QString task_id, QString task_activeType, QString task_date, QString task_status, QString finish_time, QString remark)
    {
        this->task_id = task_id;
        this->task_activeType = task_activeType;
        this->task_status = task_status;
        this->task_date = task_date;
        this->finish_time = finish_time;
        this->remark = remark;
    };
    ~SDailyHistory()
    {
        this->task_id.clear();
        this->task_activeType.clear();
        this->task_status.clear();
        this->task_date.clear();
        this->finish_time.clear();
        this->remark.clear();
    }
    QString     task_id;             //对应的任务id
    QString     task_activeType;    //任务活跃状态
    QString     task_date;        //创建时间:历史记录实际上是创建的任务，每日所生成的新任务，然后一天结束后，会写到历史数据，所以不需要存太多，需要存储这个任务的创建时间
    QString     task_status;    //任务状态，完成、失败
    QString     finish_time;    //这个任务如果是完成的，显示完成时间
    QString     remark;         //备注--成功还是失败的原因，失败可能是超时或者手动输入的
};

//status_change [任务状态变更记录，一个任务状态变化，比如开始、暂停、重新开始、失败、完成、重新开始的节点记录]
struct SStatusChange {
    SStatusChange(){ };
    SStatusChange(QString task_id, QString task_activeType, QString task_status, QString change_time, QString reason)
    {
        this->task_id = task_id;
        this->task_activeType = task_activeType;
        this->task_status = task_status;
        this->change_time = change_time;
        this->reason = reason;
    };
    ~SStatusChange()
    {
        this->task_id.clear();
        this->task_activeType.clear();
        this->task_status.clear();
        this->change_time.clear();
        this->reason.clear();
    }
    QString     task_id;             //对应的任务id
    QString     task_activeType;    //任务活跃状态
    QString     task_status;    //开始/暂停/恢复/自动暂停/成功/放弃 任务状态记录
    QString     change_time;    //这个任务如果是完成的，显示完成时间
    QString     reason;         //变更原因
};

//表格中显示的按钮操作，当然也涉及到任务变更
#define TASK_OPERATE_TYPE_START "start"
#define TASK_OPERATE_TYPE_RESTART "reStart"
#define TASK_OPERATE_TYPE_PAUSE "pause"
#define TASK_OPERATE_TYPE_FAILE "faile"
#define TASK_OPERATE_TYPE_FILISH "finish"
#define TASK_OPERATE_TYPE_EDIT "edit"
#define TASK_OPERATE_TYPE_DELETE "delete"

#endif