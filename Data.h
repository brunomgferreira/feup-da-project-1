#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_DATA_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_DATA_H


using namespace std;

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <cmath>
#include "Graph.h"
#include "WaterReservoir.h"
#include "PumpingStation.h"
#include "DeliverySite.h"
#include "Pipe.h"

/**
 * @brief Class that saves all the program data.
 */

class Data {
private:
    unordered_map<string, WaterReservoir *> waterReservoirs;
    unordered_map<string, PumpingStation *> pumpingStations;
    unordered_map<string, DeliverySite *> deliverySites;
    unordered_map<string, Pipe *> pipes;
    Graph g;
    GraphMetrics metrics;
public:
    /**
    * @brief Default constructor for the Data class.
    *
    * @details This default constructor creates a new instance of the Data class without
    * specifying any initial values. It initializes the Data object with default values.
    * @details Time complexity: O(1)
    */
    Data();

    /**
     * @brief Reads files from the specified directory path
     *
     * @details This method reads files from the specified directory path and processes them.
     * Then for each file type a specific method is called to parse the data.
     * @param dir_path The directory path containing the files.
     * @throws runtime_error if multiple files of the same category are found or if there´s any
     * error reading or finding the files.
     */
    void readFiles(const filesystem::path &dir_path);

    /**
     * @brief Reads data from the specified input file stream and processes it as reservoir information.
     * @details This method reads data from the file stream and extracts reservoir information from each line
     * of the file, and then creates a WaterReservoir object and adds it to the graph.
     *
     * @param file The input file stream containing reservoir information.
     */
    void readFileReservoir(ifstream &file);

    /**
     * @brief Reads data from the specified input file stream and processes it as station information.
     * @details This method reads data from the file stream and extracts station information from each line
     * of the file, and then creates a PumpingStation object and adds it to the graph.
     * @param file The input file stream containing station information.
     */
    void readFileStations(ifstream &file);

    /**
     * @brief Reads data from the specified input file stream and processes it as city information.
     * @details This method reads data from the file stream and extracts city information from each line
     * of the file, and then creates a DeliverySite object and adds it to the graph.
     * @param file The input file stream containing city information.
     */
    void readFileCities(ifstream &file);

    /**
     * @brief Reads data from the specified input file stream and processes it as pipe information.
     * @details This method reads data from the file stream and extracts pipe information from each line
     * of the file, and then creates a Pipe object and adds it to the graph.
     * @param file The input file stream containing pipe information.
     */
    void readFilePipes(ifstream &file);

    /**
     * @brief Check if a delivery site with the given code exists.
     * @param code The code of the delivery site to check.
     * @return true if the delivery site exists, false otherwise.
     */
    bool deliverySiteExists(const string &code);

    /**
     * @brief Check if a water reservoir with the given code exists.
     * @param code The code of the water reservoir to check.
     * @return true if the water reservoir exists, false otherwise.
     */
    bool waterReservoirExists(const string &code);

    /**
     * @brief Check if a pumping station with the given code exists.
     * @param code The code of the pumping station to check.
     * @return true if the pumping station exists, false otherwise.
     */
    bool pumpingStationExists(const string &code);

    /**
     * @brief Check if a pipeline with the given code exists.
     * @param code The code of the pipeline to check.
     * @return true if the pipeline exists, false otherwise.
     */
    bool pipelineExists(const string &code);

    /**
     * @brief Displays information about the maximum flow for a specific city.
     * @param code The code of the city to check the maximum flow.
     */
    void cityMaxFlow(const string &code);

    /**
     * @brief Displays information about the maximum flow for all cities.
     */
    void allCitiesMaxFlow();

    /**
     * @brief Displays information about the water supply status.
     */
    void verifyWaterSupply();

    /**
     * @brief Loads the optimization of the network and displays information about initial and final values.
     */
    void loadOptimization();

    /**
     * @brief Simulates the impact of putting a water reservoir out of commission and displays the information.
     * @param code The code of the water reservoir to put out of commission.
     */
    void reservoirImpact(const string &code);

    /**
     * @brief Displays information about pumping stations that are not essential to maintain the current maximum flow.
     */
    void notEssentialPumpingStations();

    /**
     * @brief Simulates the impact of putting a pumping station out of commission and displays the information.
     * @param code The code of the pumping station to put out of commission.
     */
    void pumpingStationImpact(const string &code);

    /**
     * @brief Simulates the impact of putting each pumping station out of commission and displays the information.
     */
    void allPumpingStationsImpact();

    /**
     * @brief Displays information about pipelines that are essential to maintain the current maximum flow.
     */
    void essentialPipelines();

    /**
     * @brief Simulates the impact of putting a pipeline out of commission and displays the information.
     * @param code The code of the pipeline to put out of commission.
     */
    void pipelineImpact(const string &code);

    /**
     * @brief Simulates the impact of putting each pipeline out of commission and displays the information.
     */
    void allPipelinesImpact();
};


#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_DATA_H
