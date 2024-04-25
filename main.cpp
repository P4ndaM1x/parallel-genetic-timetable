#include "CLIManger.hpp"
#include "FileManager.hpp"

int main(int argc, char* argv[])
{
    CLI::Args::prepare(argc, argv);

    const auto testFilePath = CLI::Args::sampleDataDirPath / "test.csv";
    Timetable timetable;
    FileManager::loadClasses(testFilePath, timetable);
    timetable.printClasses();

    return 0;
}
