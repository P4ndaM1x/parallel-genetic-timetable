/**
 * @file Class.cpp
 * @brief Implementation file of the Class class.
 */
#include "Class.hpp"

#include <stdexcept>
#include <string>

Class::Class(const ID id, const Duration durationTime)
    : id{id}
    , durationTime{durationTime}
{
    if (durationTime <= 0) {
        throw std::invalid_argument("Duration time must be greater than 0!");
    }
    if (durationTime > Timetable::slotsPerDay) {
        throw std::invalid_argument(
            std::string("Duration time must be less than number of slots")
            + std::to_string(Timetable::slotsPerDay) + "!"
        );
    }
}

bool Class::isIntervalValid()
{
    return (getStartTime() / Timetable::slotsPerDay)
        == ((getEndTime() - 1) / Timetable::slotsPerDay);
}

void Class::setRandomStartTime()
{
    do {
        setStartTime(std::rand() % (Timetable::numberOfSlots - getDurationTime()));
    } while (not isIntervalValid());
}

std::string Class::serialize() const
{
    std::stringstream ss;
    ss << std::to_string(getId()) << ",";
    ss << std::to_string(getDurationTime()) << ",";
    ss << std::to_string(getStartTime());
    return ss.str();
}

Class Class::deserialize(const std::string& serializedClass)
{
    std::stringstream ss{serializedClass};
    std::string token;

    std::getline(ss, token, ',');
    ID id = std::stoi(token);

    std::getline(ss, token, ',');
    Duration duration = std::stoi(token);

    std::getline(ss, token);
    Time startTime = std::stoi(token);

    Class clazz = Class{id, duration};
    clazz.setStartTime(startTime);
    return clazz;
}