#include "Class.hpp"

Class::Class(const ID id, const Duration durationTime)
    : id { id }
    , durationTime { durationTime }
{
    if (durationTime <= 0) {
        throw std::invalid_argument("Duration time must be greater than 0!");
    }
    if (durationTime > Timetable::slotsPerDay) {
        throw std::invalid_argument(std::string("Duration time must be less than number of slots(") + std::to_string(Timetable::slotsPerDay) + "!");
    }
}