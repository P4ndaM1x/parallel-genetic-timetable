#pragma once

#include "Timetable.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>

class Class {
public:
    using ID = uint32_t;
    using Duration = uint32_t;
    using Time = uint32_t;

    Class(const ID id, const Duration durationTime);

    ID getId() const { return id; }
    Duration getDurationTime() const { return durationTime; }
    Time getStartTime() const { return startTime; }
    Time getEndTime() const { return startTime + durationTime; }
    void setStartTime(const Time time) { startTime = time; }
    void setRandomStartTime();

    friend std::ostream& operator<<(std::ostream& os, const Class& c)
    {
        os << "Class@" << &c;
        os << " { id: " << std::setw(3) << +c.getId();
        os << ", duration: " << std::setw(2) << +c.getDurationTime();
        return os << " }";
    }

private:
    bool isIntervalValid();

    ID id;
    Duration durationTime;
    Time startTime{0};
};
