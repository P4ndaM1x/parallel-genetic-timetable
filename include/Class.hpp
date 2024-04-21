#pragma once

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>

class Class {
public:
    using id_type = uint16_t;
    using time_type = uint8_t;

    Class(const id_type id, const time_type durationTime)
        : id { id }
        , durationTime { durationTime }
    {
        if (durationTime <= 0)
            throw std::invalid_argument("Duration time must be greater than 0!");
    }

    id_type getId() const { return id; }
    time_type getDurationTime() const { return durationTime; }
    time_type getStartingHour() const { return startingHour; }
    time_type getEndingHour() const { return startingHour + durationTime; }
    void setStartingHour(const time_type hour) { startingHour = hour; }

    friend std::ostream& operator<<(std::ostream& os, const Class& c)
    {
        os << "Class@" << &c;
        os << " { id: " << std::setw(3) << +c.getId();
        os << ", duration: " << std::setw(2) << +c.getDurationTime();
        return os << " }";
    }

private:
    id_type id;
    time_type durationTime;
    time_type startingHour { 0 };
};
