/**
 * @file Chromosome.hpp
 * @brief Header file for the Chromosome class.
 * @see Chromosome.cpp
 */
#pragma once

#include "Class.hpp"
#include "Timetable.hpp"

#include <array>
#include <cstdint>
#include <limits>

/**
 * @brief Represents a solution to the timetabling problem.
 */
class Chromosome {
public:
    using TimeSlot = std::vector<Class::ID>;
    using TimeSlotContainer = std::array<TimeSlot, Timetable::numberOfSlots>;

    /**
     * @brief Constructs a Chromosome object.
     * @param classes The container of classes for the timetable.
     * @param randomizeStartTimes Flag indicating whether to randomize the start times of the
     * classes.
     */
    Chromosome(const Timetable::ClassContainer& classes, const bool randomizeStartTimes = true);

    /**
     * @brief Initializes the chromosome by randomly assigning classes to time slots.
     */
    void init();

    /**
     * @brief Mutates the chromosome by choosing random class and moving it to a random time slot.
     */
    void mutate();

    /**
     * @brief Calculates the error of the chromosome, which represents the fitness of the timetable.
     */
    void calculateError();

    /**
     * @brief Gets the previously calculated error of the chromosome.
     * @note Should be called after calculateError().
     * @return Number of conflicts in the timetable.
     */
    uint32_t getError() const;

    /**
     * @brief Prints the solution represented by the chromosome.
     */
    void printSolution() const;

    /**
     * @brief Gets the class with the specified ID.
     * @param id The ID of the class.
     * @return The class object.
     */
    Class getClass(const Class::ID id) const;

    /**
     * @brief Gets the container of classes for the timetable.
     * @return The container of classes.
     */
    Timetable::ClassContainer getClasses() const;

    /**
     * @brief Overloads the << operator to print the chromosome.
     * @param os The output stream.
     * @param c The chromosome object.
     * @return The output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const Chromosome& c)
    {
        Timetable::ClassContainer classes = c.getClasses();

        os << "Chromosome@" << &c << " {\n";
        for (unsigned i = 0; i < classes.size(); i++) {
            os << "\t" << std::to_string(i) << ": " << classes.at(i)
               << (i + 1 == classes.size() ? "\n}\n" : ",\n");
        }
        return os;
    }

private:
    bool isIntervalValid(Class::Time a, Class::Time b);
    void updateTimeSlotContainer();

    TimeSlotContainer timeSlots;
    Timetable::ClassContainer classes;
    uint32_t error{std::numeric_limits<uint32_t>::max()};
};
