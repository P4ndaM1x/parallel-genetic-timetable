/**
 * @file Class.hpp
 * @brief Header file for the Class class.
 * @see Class.cpp
 */
#pragma once

#include "Timetable.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>

/**
 * @brief Represents a single class in a timetable.
 */
class Class {
public:
    using ID = uint32_t; /**< The type alias for the ID of the class. */
    using Duration = uint32_t; /**< The type alias for the duration time of the class. */
    using Time = uint32_t; /**< The type alias for the start and end time of the class. */

    /**
     * @brief Constructs a Class object with the given ID and duration time.
     * 
     * @param id The ID of the class.
     * @param durationTime The duration time of the class.
     */
    Class(const ID id, const Duration durationTime);

    /**
     * @brief Gets the ID of the class.
     * 
     * @return The ID of the class.
     */
    ID getId() const { return id; }

    /**
     * @brief Gets the duration time of the class.
     * 
     * @return The duration time of the class.
     */
    Duration getDurationTime() const { return durationTime; }

    /**
     * @brief Gets the start time of the class.
     * 
     * @return The start time of the class.
     */
    Time getStartTime() const { return startTime; }

    /**
     * @brief Gets the end time of the class.
     * 
     * @return The end time of the class.
     */
    Time getEndTime() const { return startTime + durationTime; }

    /**
     * @brief Sets the start time of the class.
     * 
     * @param time The start time to set.
     */
    void setStartTime(const Time time) { startTime = time; }

    /**
     * @brief Sets a random start time for the class.
     * 
     * This function sets a random start time for the class within the valid time slots of the timetable.
     * It uses the std::rand() function to generate a random number and sets the start time using the  result.
     * The start time is calculated as the modulo of the random number with the difference between the total number of slots in the timetable and the duration time of the class.
     * The function then checks if the interval is valid using the isIntervalValid() function.
     * If the interval is not valid, the process is repeated until a valid interval is found.
     * 
     * @note This function assumes that the Timetable::numberOfSlots and the duration time of the class have been properly set.
     */
    void setRandomStartTime();
    std::string serialize() const;

    static Class deserialize(const std::string& serializedString);

    /**
     * @brief Overloaded stream insertion operator for the Class class.
     * 
     * This function allows the Class object to be printed to an output stream.
     * The output format includes the object's address, id, duration, and start time.
     * 
     * @param os The output stream to write to.
     * @param c The Class object to be printed.
     * @return The modified output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const Class& c)
    {
        os << "Class@" << &c;
        os << " { id: " << std::setw(3) << +c.getId();
        os << ", duration: " << std::setw(2) << +c.getDurationTime();
        os << ", startTime: " << std::setw(2) << +c.getStartTime();
        return os << " }";
    }
    
    /**
     * @brief Serializes the class object to a string.
     * 
     * @return The serialized string.
     */
    std::string serialize() const;

    /**
     * @brief Deserializes the class object from a string.
     * 
     * @param serializedString The serialized string.
     * @return The deserialized class object.
     */
    static Class deserialize(const std::string& serializedString);

private:
    bool isIntervalValid();

    ID id; /**< The ID of the class. */
    Duration durationTime; /**< The duration time of the class. */
    Time startTime{0}; /**< The start time of the class. */
};
