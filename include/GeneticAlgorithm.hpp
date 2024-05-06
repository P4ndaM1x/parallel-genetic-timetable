/**
 * @file GeneticAlgorithm.hpp
 * @brief Header file for the GeneticAlgorithm class.
 * @see GeneticAlgorithm.cpp
 */
#pragma once

#include "Chromosome.hpp"
#include "Timetable.hpp"

#include <vector>

/**
 * @class GeneticAlgorithm
 * @brief Represents a genetic algorithm for solving timetabling problem.
 *
 * The GeneticAlgorithm class provides functionality to evolve a population of chromosomes
 * to find the best solution for a given timetabling problem. It uses genetic operators such
 * as mutation and crossover to generate new generations of chromosomes.
 */
class GeneticAlgorithm {

public:
    /**
     * @brief Container type for storing chromosomes in the population.
     */
    using ChromosomeContainer = std::vector<Chromosome>;

    /**
     * @brief Constructs a GeneticAlgorithm object.
     * @param timetable The timetable representing the problem to be solved.
     * @param populationSize The size of the population.
     * @param numberOfGenerations The number of generations to evolve.
     * @param mutationRate The rate of mutation.
     */
    GeneticAlgorithm(
        const Timetable& timetable,
        const unsigned populationSize,
        const unsigned numberOfGenerations,
        const double mutationRate
    );

    /**
     * @brief Entry point for running the genetic algorithm.
     */
    void run();

    /**
     * @brief Gets the current population.
     * @return The current population.
     */
    ChromosomeContainer& getPopulation();

    /**
     * @brief Sets the population to the given updated population.
     * @param updatedPopulation The updated population.
     */
    void setPopulation(const ChromosomeContainer& updatedPopulation);

    /**
     * @brief Sorts the population by error in ascending order.
     */
    void sortPopulationByError();

    /**
     * @brief Serializes the best chromosomes of the specified number from the population.
     * @param size The size of the population to serialize.
     * @return The serialized population.
     */
    std::string serializePopulationOfSize(const unsigned size);

    /**
     * @brief Gets the best chromosomes of the specified number from the population.
     * @param size The size of the best population to get.
     * @return The best population.
     */
    ChromosomeContainer getBestPopulationOfSize(const unsigned size);

    /**
     * @brief Deserializes a population from the given string.
     * @param serializedPopulation The serialized population.
     * @return The deserialized population.
     */
    static ChromosomeContainer deserializePopulation(const std::string& serializedPopulation);

    /**
     * @brief Serializes the given population.
     * @param population The population to serialize.
     * @return The serialized population.
     */
    static std::string serializePopulation(const ChromosomeContainer& population);

private:
    /**
     * @brief Initializes the genetic algorithm by creating a random initial population.
     */
    void initialize();

    /**
     * @brief Calculates the fitness of each chromosome in the population.
     */
    void fitness();

    /**
     * @brief Performs crossover between random pairs of chromosomes in the population.
     */
    void crossover();

    /**
     * @brief Tries to mutate each chromosome in the population with probability mutationRate.
     */
    void mutate();

    /**
     * @brief Performs the whole evolution of the population by applying genetic operators.
     */
    void evolve();

    /**
     * @brief Creates a new chromosome by combining two parent chromosomes.
     * @param a The first parent chromosome.
     * @param b The second parent chromosome.
     * @return The new chromosome.
     */
    Chromosome makeLove(const Chromosome& a, const Chromosome& b);

    /**
     * @brief Selects the best chromosomes to keep in the population.
     */
    void selectBest();

    Timetable solution;
    const unsigned populationSize;
    const unsigned numberOfGenerations;
    const double mutationRate;

    /**
     * @brief The percentage of the population to keep after selection.
     */
    const double percentToKeep{0.1};

    ChromosomeContainer population;
};
