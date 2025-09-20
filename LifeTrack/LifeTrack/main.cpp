#include "LifeTrack.h"
#include <QApplication>
#include <QTranslator>
#include <QDir>

#include "common/global.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"

#include "management/dataCenter.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

void initSpdlog()
{
    try
    {

        QString path = qApp->applicationDirPath() + "/log";
        QDir dir(path);
        if (!dir.exists())
            bool ok = dir.mkpath(path); //只创建一级子目录，即必须保证上级目录存在

        QString path1 = path;
        path += "/log.txt";

        spdlog::rotating_logger_mt("logger", path.toStdString(), 1024 * 1024 * 50, 10);

        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][thread %t][%@,%!][%l] : %v");
        spdlog::get("logger")->flush_on(spdlog::level::debug);
        spdlog::get("logger")->set_level(spdlog::level::debug);

        SPDLOG_LOGGER_DEBUG(spdlog::get("logger"), "initSpdlog DEBUG!");

    }
    catch (const spdlog::spdlog_ex& ex)
    {

    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //
    gobalInit();
    initSpdlog();
    CDataCenter::GetInstance()->initInfo();

    LifeTrack w;
    w.show();
    return app.exec();
}
