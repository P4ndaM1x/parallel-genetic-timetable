#include "Timetable.hpp"

void Timetable::addClasses(std::initializer_list<Class> classList)
{
    for (const auto& c : classList) {
        classes.push_back(c);
    }
}
