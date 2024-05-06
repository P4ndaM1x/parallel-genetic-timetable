/**
 * @file Timetable.hpp
 * @brief Header file for the Timetable class.
 *
 * This file contains the declaration of the Timetable class, which represents a timetable for
 * classes. It includes the necessary headers, defines constants for the number of days and slots
 * per day, and declares member functions for adding, getting, updating, and printing classes in the
 * timetable. It also provides static functions for serializing and deserializing classes.
 *
 * @see Timetable.cpp
 */
#pragma once

#include "Class.hpp"

#include <string>
#include <vector>

class Class;

/**
 * @brief Manages classes in a timetable.
 */
class Timetable {
public:
    /**
     * @brief The number of days in the timetable.
     */
    static constexpr unsigned numberOfDays = 5;

    /**
     * @brief The number of slots per day in the timetable.
     */
    static constexpr unsigned slotsPerDay = 8;

    /**
     * @brief The total number of slots in the timetable.
     */
    static constexpr std::size_t numberOfSlots = numberOfDays * slotsPerDay;

    /**
     * @brief Container type for storing classes in the timetable.
     */
    using ClassContainer = std::vector<Class>;

    /**
     * @brief Serializes the classes to a string.
     * @param classes The classes to serialize.
     * @return The serialized string.
     */
    static std::string serializeClasses(const ClassContainer& classes);

    /**
     * @brief Deserializes the classes from a string.
     * @param serializedClasses The serialized string.
     * @return The deserialized classes.
     */
    static ClassContainer deserializeClasses(const std::string& serializedClasses);

    /**
     * @brief Constructs a Timetable object with the given classes.
     * @param classes The initial classes to add to the timetable.
     */
    Timetable(const ClassContainer& classes);

    /**
     * @brief Default constructor for Timetable.
     */
    Timetable() = default;

    /**
     * @brief Adds classes to the timetable.
     * @param classList The list of classes to add.
     */
    void addClasses(std::initializer_list<Class> classList);

    /**
     * @brief Returns the classes in the timetable.
     * @return The classes in the timetable.
     */
    ClassContainer getClasses() const { return classes; }

    /**
     * @brief Prints the classes in the timetable.
     */
    void printClasses() const;

    /**
     * @brief Updates the classes in the timetable with the given ones.
     * @param newClasses The new classes to update the timetable with.
     */
    void updateClasses(const ClassContainer& newClasses) { classes = newClasses; }

    /**
     * @brief Prints the timetable.
     */
    void print() const;

private:
    ClassContainer classes;
};
