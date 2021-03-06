//  illarionserver - server for the game Illarion
//  Copyright 2011 Illarion e.V.
//
//  This file is part of illarionserver.
//
//  illarionserver is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Affero General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  illarionserver is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Affero General Public License for more details.
//
//  You should have received a copy of the GNU Affero General Public License
//  along with illarionserver.  If not, see <http://www.gnu.org/licenses/>.

#include "Logger.hpp"

#include "constants.hpp"

#include <iostream>
#include <map>
#include <string>

LogType<LogPriority::EMERGENCY>::type Logger::emergency;
LogType<LogPriority::ALERT>::type Logger::alert;
LogType<LogPriority::CRITICAL>::type Logger::critical;
LogType<LogPriority::ERROR>::type Logger::error;
LogType<LogPriority::WARNING>::type Logger::warn;
LogType<LogPriority::NOTICE>::type Logger::notice;
LogType<LogPriority::INFO>::type Logger::info;
LogType<LogPriority::DEBUG>::type Logger::debug;

void log_message(LogPriority priority, LogFacility facility, const std::string &message) {
    if constexpr (useSysLog) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        syslog(static_cast<int>(priority) | static_cast<int>(facility), "%s", message.c_str());
    } else {
        static const std::map<LogPriority, std::string> priorityText{
                {LogPriority::EMERGENCY, "emerg"}, {LogPriority::ALERT, "alert"},     {LogPriority::CRITICAL, "crit"},
                {LogPriority::ERROR, "err"},       {LogPriority::WARNING, "warning"}, {LogPriority::NOTICE, "notice"},
                {LogPriority::INFO, "info"},       {LogPriority::DEBUG, "debug"}};

        static const std::map<LogFacility, std::string> facilityText{
                {LogFacility::Database, "Database"}, {LogFacility::World, "World"}, {LogFacility::Script, "Script"},
                {LogFacility::Player, "Player"},     {LogFacility::Chat, "Chat"},   {LogFacility::Admin, "Admin"},
                {LogFacility::Other, "Other"}};

        std::cout << facilityText.at(facility) << " (" << priorityText.at(priority) << "): " << message << std::endl;
    }
}
