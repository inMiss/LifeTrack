#ifndef LIFETRACK_STRUCTCOMMON_COMMON_H
#define LIFETRACK_STRUCTCOMMON_COMMON_H

#include <QString>
#include <QDateTime>
#include <QSharedDataPointer>
#include "../common/commonFunc.h"

#define DATETIME_EMPTY QDateTime(QDate(1000, 1, 1), QTime(1, 1, 1))
#define DATETIME_MAX QDateTime(QDate(9999, 1, 1), QTime(1, 1, 1))
#define DATETIME_EMPTY_SHOW "---"
//���͵Ȳ���Ӱ������ҵ���߼���ֻ��Ӱ��һЩ��ʾ���������趨����˿��Խ����ⲿ���
//�����������ͣ���Ӣ��ѧϰ�������
//#define BELONG_ID_FITNESS "belong_id_fintness" //����
//#define BELONG_ID_WRITING "belong_id_write" //д��
//belong_type_dictionary [������������]
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

    QString     belong_id;        //��������id
    QString     show_content;     //����������ʾ˵��
    QString     parent_belong_id; //����Ƕ������ͣ�����ط���������һ�����͵�belong_id
};

//��Ҫ�ּ�����Ӱ������ҵ���߼���ֻ��Ӱ��һЩ��ʾ���������趨�ͷ�ֵ���㣬��˿��Խ����ⲿ���
//���Ƿ�ֵ�����Ӱ�쵽���е��������ݼ��㣬�������ݵ�ǰ��һ�£���˲������������
//#define IMPORT_LEVEL_FIRST "import_level_first"//1
//#define IMPORT_LEVEL_SECOND "import_level_second"//2
//#define IMPORT_LEVEL_THIRD "import_level_third"//3
//#define IMPORT_LEVEL_FOUTH "import_level_fouth"//4
//import_levle_dictionary [��Ҫ�̶ȱ��]
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

    QString     level_id;        //����id
    QString     shouw_text;     //��ʾ����
    int     default_value;     //�������Ĭ�Ϸ�ֵ
};


//���漰������ҵ���߼���д���ֵ䣬���ṩ�ⲿ�趨����,���ǽ����combox��ʾ��������Ҫ��db��ȡ
//��������
//#define REWARDS_TYPE_DEFINE "rewards_type_define"  //Ĭ��/�ɾ�
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

    QString     type;        //��������id
    QString     show_content;     //����������ʾ˵��
};

//�ظ�����
#define REPEAT_TYPE_ONCE "repeat_type_once"//һ��
#define REPEAT_TYPE_EVERYDAY "repeat_type_everyday"//ÿ���ظ�
#define REPEAT_TYPE_FORGETTING_CURVE "repeat_type_forgetting_curve"//��������
#define REPEAT_TYPE_ALWAYS "repeat_type_always"//һֱ����
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

    QString     repeat_type;        //��������id
    QString     show_content;     //����������ʾ˵��
};

//״̬
 #define TASK_STATUS_BEGIN       "task_status_begin"//��ʼ
 #define TASK_STATUS_INPROGRESS  "task_status_inProgress"//������
 #define TASK_STATUS_STOP        "task_status_stop"//�ж�
 #define TASK_STATUS_RESTART     "task_status_restart"//���¿�ʼ
 #define TASK_STATUS_FILED       "task_status_filed"//ʧ��
 #define TASK_STATUS_FINISH      "task_status_finish"          //���
 #define TASK_STATUS_UNDETERMINED "task_status_undetermined" //δȷ��

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

    QString     task_status;        //��������id
    QString     show_content;     //����������ʾ˵��
};
///////////////////////////////////////////
//daily_summary [ÿ���ܽ���]
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

    QString     id;       //�������
    QString   date;           //��ǰ����
    QString     content;        //��дÿ���ܽ�����
    int         task_total;     //������������
    int         finish_task_sum;//����ɵ���������
    double      point_total;    //���������ܻ���
    double      finish_point_sum;//���ջ�ȡ���ܽ��
};

//rewards [�趨�Ľ���/�ɾ�]
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
    QString         id;             //���id
    bool        is_time_limited;//�Ƿ�����ʱ��ɵ�Ը��
    QString   start_time;     //��ʼʱ��
    QString   end_time;       //����ʱ��
    QString     goal_type;      //�������ͣ���ֵ�ۼƣ�ָ������
    QString     content;        //��������
    double      value;          //��ֵ���ͣ��Ĵ���ֵ
    QString     status;         //��ǰ����״̬--��ɡ�ʧ�ܵ�
    QString     remark;         //��ע
};

//task_list [��������]
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
    QString     id;      //����id
    QString     name;        //�������ݣ�����
    QString   start_time;         //��ʼʱ��
    QString   end_time;
    double      during_time;         //����ʱ��(����ʱ��,��λ:s)
    QString   create_time;
    QString     import_level;         //��Ҫ�Բ㼶
    int         value;             //����������趨�ķ�ֵ
    QString     repeat_type;         //�ظ����͡�ÿ�ա��ظ���
    int         finish_times;             //��ɴ�����Ĭ��Ϊ0����ÿ�գ�������ۼ�
    double      sum_points;             //��������ۼƷ����������������Ҳ��Ҫ�ۼ�
    QString     belong_type;         //�������ͣ�Ӣ�������
    bool        is_deduction;//�Ƿ���з�ֵ�ļ�ȥ������ʧ�ܿ۳��ۼƷ�ֵ
    bool        is_countdown;//�Ƿ���е���ʱ���ѣ�
    bool        is_can_pause;//�Ƿ��ܹ�������ͣ�����false����ϵ���ֱ���ж�Ϊʧ��
    QString     task_status;         //����״̬����ʼ����������ͣ
    QString   finish_time;          //�������ʱ��
    QString   pause_time;             //��ͣ��ʼʱ��
    QString     parent_task;         //������id
    QString     remark;         //��ע
};

using STaskListPtr = QSharedPointer<STaskList>;

//incentive_content [�������]
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
    QString     number;         //��ǰ����״̬--��ɡ�ʧ�ܵ�
    QString     content;         //��ע
};
#endif