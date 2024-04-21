#pragma once

#include <cstdint>

class Class {
public:
    using id_type = uint16_t;
    using time_type = uint8_t;

    Class(const id_type id, const time_type durationTime)
        : id { id }
        , durationTime { durationTime }
    {
    }

    id_type getId() const { return id; }
    time_type getDurationTime() const { return durationTime; }
    time_type getStartingHour() const { return startingHour; }
    time_type getEndingHour() const { return startingHour + durationTime; }

    void setStartingHour(const time_type hour) { startingHour = hour; }

private:
    id_type id;
    time_type durationTime;
    time_type startingHour { 0 };
};
