#pragma once

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>

class Class {
public:
    using ID = uint32_t;
    using Duration = uint32_t;
    using Time = uint32_t;

    Class(const ID id, const Duration durationTime)
        : id { id }
        , durationTime { durationTime }
    {
        if (durationTime <= 0)
            throw std::invalid_argument("Duration time must be greater than 0!");
    }

    ID getId() const { return id; }
    Duration getDurationTime() const { return durationTime; }

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
};
