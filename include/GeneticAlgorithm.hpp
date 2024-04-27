#pragma once

#include "Chromosome.hpp"
#include "Timetable.hpp"

#include <algorithm>
#include <random>
#include <vector>

class GeneticAlgorithm {

public:
    GeneticAlgorithm(Timetable& timetable, const unsigned populationSize, const unsigned numberOfGenerations, const double mutationRate);

    // entry point for the algorithm
    void run();
    std::vector<Chromosome> getPopulation();

private:
    // Initialize the population with random chromosomes
    void initialize();
    // Perform the whole iteration of the algorithm
    void evolve();
    // Calculate the fitness of the current population
    void fitness();
    // Randomly select a class and change its starting time
    void mutate();
    // Perform the crossover operation
    void crossover();

    Timetable& solution;
    const unsigned populationSize;
    const unsigned numberOfGenerations;
    const double mutationRate;

    std::vector<Chromosome> population;
};
