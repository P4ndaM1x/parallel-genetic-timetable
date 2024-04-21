#pragma once

#include "Timetable.hpp"
#include "csv.hpp"
#include <algorithm>

class FileManager {
public:
    static void loadClasses(const std::string& filename, Timetable& timetable)
    {
        try {
            csv::CSVReader reader { filename };
            for (auto& row : reader)
                timetable.addClasses({ Class { row["id"].get<Class::id_type>(), row["duration"].get<Class::time_type>() } });

            auto classes = timetable.getClasses();
            validateUniqueIDs(classes);
            validateDurationTimes(classes);
        } catch (const std::exception& e) {
            std::cerr << "Exception while loading CSV file: " << e.what() << std::endl;
            throw;
        }
    }

private:
    static void validateUniqueIDs(const Timetable::data_type& localClasses)
    {
        auto classes = localClasses;
        auto it = std::unique(classes.begin(), classes.end(), [](auto a, auto b) { return a.getId() == b.getId(); });
        if (it != classes.end()) {
            throw std::invalid_argument("Duplicate class IDs found!");
        }
    }

    static void validateDurationTimes(const Timetable::data_type& localClasses)
    {
        auto classes = localClasses;
        auto found = std::any_of(classes.begin(), classes.end(), [](auto c) { return (c.getDurationTime() > Timetable::slotsPerDay); });
        if (found) {
            throw std::invalid_argument("Too long class duration found!");
        }
    }
};
