#pragma once

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>

class Class {
public:
    using ID = uint16_t;
    using Time = uint8_t;
    using Duration = uint8_t;

    Class(const ID id, const Duration durationTime)
        : id { id }
        , durationTime { durationTime }
    {
        if (durationTime <= 0)
            throw std::invalid_argument("Duration time must be greater than 0!");
    }

    ID getId() const { return id; }
    Duration getDurationTime() const { return durationTime; }
    Time getStartingHour() const { return startingHour; }
    Time getEndingHour() const { return startingHour + durationTime; }
    void setStartingHour(const Time hour) { startingHour = hour; }

    friend std::ostream& operator<<(std::ostream& os, const Class& c)
    {
        os << "Class@" << &c;
        os << " { id: " << std::setw(3) << +c.getId();
        os << ", duration: " << std::setw(2) << +c.getDurationTime();
        return os << " }";
    }

private:
    ID id;
    Duration durationTime;
    Time startingHour { 0 };
};
