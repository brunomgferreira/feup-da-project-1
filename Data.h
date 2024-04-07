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
    string networkName;
    Graph g;
    GraphMetrics metrics;
public:
    /**
    * @brief Default constructor for the Data class.
    *
    * @details This default constructor creates a new instance of the Data class without
    * specifying any initial values. It initializes the Data object with default values.
    */
    Data();

    /**
     * @brief Retrieves the name of the network.
     *
     * @return The name of the network.
     */
    string getNetworkName() const;

    /**
     * @brief Reads data files containing information about reservoirs, stations, cities, and pipes.
     *
     * @details This function reads data files from the specified directory path and populates the network with the
     * information contained in these files. It identifies the appropriate files based on their names containing specific
     * substrings ('Reservoir', 'Stations', 'Cities', 'Pipes'). It then opens each file, reads its contents, and calls
     * respective functions to parse and process the data. After reading all necessary files, it calculates the maximum
     * flow in the network and metrics based on the provided data.
     *
     * @param dir_path The directory path containing the data files.
     *
     * @throw runtime_error if there are errors in opening or reading the files, or if multiple files of the same type
     * are found, or if any required file is missing.
     *
     * @complexity The time complexity of this function depends on the number of files in the directory and the complexity
     * of processing each file. Let n be the number of files in the directory:
     * - Iterating over the files and identifying the appropriate files each take O(n) time.
     * - Opening each file and reading its contents take O(1) time.
     * - Calling readFileReservoir, readFileStations, readFileCities, and readFilePipes each takes O(n) time in total,
     *   assuming the number of lines in each file is proportional to n.
     * - Calculating the maximum flow and metrics using Graph methods each takes O(V + E^2) time in the worst case.
     * Therefore, the overall time complexity is O(V + E^2).
     */
    void readFiles(const filesystem::path &dir_path);

    /**
     * @brief Reads water reservoir data from a file and populates the network.
     *
     * @details This function reads water reservoir data from the given file stream. Each line in the file is expected
     * to contain information about a water reservoir, including ID, name, municipality, code, and maximum delivery.
     * The function parses each line, creates WaterReservoir objects, and adds them to the network as vertices. It also
     * adds the water reservoirs to the waterReservoirs map.
     *
     * @param file The input file stream containing water reservoir data.
     *
     * @complexity The time complexity of this function depends on the number of lines in the file and the complexity
     * of parsing each line. Let n be the number of lines in the file:
     * - Parsing each line involves O(1) operations.
     * - Inserting a vertex into the graph and adding a water reservoir to the map each take O(1) time.
     * Therefore, the overall time complexity is O(n).
     */
    void readFileReservoir(ifstream &file);

    /**
     * @brief Reads pumping station data from a file and populates the network.
     *
     * @details This function reads pumping station data from the given file stream. Each line in the file is expected
     * to contain information about a pumping station, including ID and code. The function parses each line, creates
     * PumpingStation objects, and adds them to the network as vertices. It also adds the pumping stations to the
     * pumpingStations map.
     *
     * @param file The input file stream containing pumping station data.
     *
     * @complexity The time complexity of this function depends on the number of lines in the file and the complexity
     * of parsing each line. Let n be the number of lines in the file:
     * - Parsing each line involves O(1) operations.
     * - Inserting a vertex into the graph and adding a pumping station to the map each take O(1) time.
     * Therefore, the overall time complexity is O(n).
     */
    void readFileStations(ifstream &file);

    /**
     * @brief Reads city data from a file and populates the network.
     *
     * @details This function reads city data from the given file stream. Each line in the file is expected to contain
     * information about a city, including city name, code, ID, demand, and population. The function parses each line,
     * creates DeliverySite objects, and adds them to the network as vertices. It also adds the delivery sites to the
     * deliverySites map.
     *
     * @param file The input file stream containing city data.
     *
     * @complexity The time complexity of this function depends on the number of lines in the file and the complexity
     * of parsing each line. Let n be the number of lines in the file:
     * - Parsing each line involves O(1) operations.
     * - Inserting a vertex into the graph and adding a delivery site to the map each take O(1) time.
     * Therefore, the overall time complexity is O(n).
     */
    void readFileCities(ifstream &file);

    /**
     * @brief Reads pipe data from a file and populates the network.
     *
     * @details This function reads pipe data from the given file stream. Each line in the file is expected to contain
     * information about a pipe, including service points, capacity, and direction. The function parses each line,
     * creates Pipe objects, and adds them to the network. It also adds edges to the graph representing the pipes.
     *
     * @param file The input file stream containing pipe data.
     *
     * @complexity The time complexity of this function depends on the number of lines in the file and the complexity
     * of parsing each line. Let n be the number of lines in the file:
     * - Parsing each line involves O(1) operations.
     * - Inserting a pipe into the map and adding edges to the graph each take O(1) time.
     * Therefore, the overall time complexity is O(n).
     */
    void readFilePipes(ifstream &file);

    /**
     * @brief Checks if a delivery site exists in the network.
     *
     * @details This function checks if a delivery site with the specified code exists in the network by searching for
     * it in the map of delivery sites. If found, it returns true; otherwise, it returns false.
     *
     * @param code The code of the delivery site to be checked.
     *
     * @return True if the delivery site exists in the network, false otherwise.
     *
     * @complexity The time complexity of this function is O(1) in the average case when searching for a delivery site
     * directly in the map.
     */
    bool deliverySiteExists(const string &code);

    /**
     * @brief Checks if a water reservoir exists in the network.
     *
     * @details This function checks if a water reservoir with the specified code exists in the network by searching for
     * it in the map of water reservoirs. If found, it returns true; otherwise, it returns false.
     *
     * @param code The code of the water reservoir to be checked.
     *
     * @return True if the water reservoir exists in the network, false otherwise.
     *
     * @complexity The time complexity of this function is O(1) in the average case when searching for a water reservoir
     * directly in the map.
     */
    bool waterReservoirExists(const string &code);

    /**
     * @brief Checks if a pumping station exists in the network.
     *
     * @details This function checks if a pumping station with the specified code exists in the network by searching for
     * it in the map of pumping stations. If found, it returns true; otherwise, it returns false.
     *
     * @param code The code of the pumping station to be checked.
     *
     * @return True if the pumping station exists in the network, false otherwise.
     *
     * @complexity The time complexity of this function is O(1) in the average case when searching for a pumping station
     * directly in the map.
     */
    bool pumpingStationExists(const string &code);

    /**
     * @brief Checks if a pipeline exists in the network.
     *
     * @details This function checks if a pipeline with the specified code exists in the network. It first searches for
     * the pipeline in the map of pipelines. If found, it returns true. If not found, it checks if the pipeline code
     * represents a bidirectional pipeline by searching for its reverse code (if applicable) and verifies if it's not
     * unidirectional. If such a bidirectional pipeline exists, it returns true; otherwise, it returns false.
     *
     * @param code The code of the pipeline to be checked.
     *
     * @return True if the pipeline exists in the network, false otherwise.
     *
     * @complexity The time complexity of this function is O(1) in the average case when searching for a pipeline
     * directly in the map. In the worst case, when the pipeline code contains a dash ('-'), it performs two map
     * lookups, resulting in O(2) constant time complexity, which simplifies to O(1).
     */
    bool pipelineExists(const string &code);

    /**
     * @brief Displays the maximum flow for a specific city in the network.
     *
     * @details This function displays the maximum flow for a specific city in the network. It retrieves the demand and
     * flow values for the specified city and prints them to the console along with the city name and code.
     *
     * @param code The code of the city for which the maximum flow is to be displayed.
     *
     * @complexity The time complexity of this function is O(1), as it performs a constant number of operations
     * regardless of the size of the network.
     */
    void cityMaxFlow(const string &code);

    /**
     * @brief Calculates and displays the maximum flow for each city in the network.
     *
     * @details This function calculates and displays the maximum flow for each city in the network. It iterates over
     * all delivery sites, retrieves the demand and flow values for each city, and prints them to the console. If specified,
     * the results are also written to an output file. Additionally, it displays the overall maximum flow value for the network.
     *
     * @complexity The time complexity of this function is O(1).
     */
    void allCitiesMaxFlow();

    /**
     * @brief Verifies the water supply for each city in the network and identifies cities lacking the desired water rate level.
     *
     * @details This function checks the water supply for each city in the network by comparing the demand for water
     * with the actual flow of water to the city. It calculates the deficit value for each city where the demand exceeds
     * the flow and identifies cities lacking the desired water rate level. The results are displayed on the console
     * and written to an output file if specified. Additionally, it determines whether the network can meet the total
     * water needs based on the comparison between total demand and total water supplied.
     *
     * @complexity The time complexity of this function depends on the size of the network graph and the number of
     * delivery sites. It involves traversing the graph and performing calculations for each delivery site, resulting
     * in a time complexity proportional to the number of delivery sites. This means O(n) where n is the number of delivery sites.
     */
    void verifyWaterSupply();

    /**
     * @brief Performs a load optimization on the network to improve the distribution of water resources.
     *
     * @details This function optimizes the load distribution in the network by adjusting the flow of water through
     * different pipelines. It computes the initial metrics of the network and then applies optimization techniques
     * to achieve better load balancing. After optimization, it calculates the final metrics and compares them
     * with the initial metrics to evaluate the effectiveness of the optimization process. The function outputs
     * the results to the console, providing insights into the improvement achieved in load balancing and the
     * impact on the total maximum flow in the network.
     *
     * @complexity The time complexity of this function depends on the size of the network graph and the number of
     * delivery sites. It involves traversing the graph and performing calculations for each delivery site, resulting
     * in a time complexity proportional to the number of delivery sites. This means O(n) where n is the number of delivery sites.
     */
    void loadOptimization();

    /**
     * @brief Identifies water reservoirs that are not essential for maintaining the current maximum flow in the network.
     *
     * @details This function identifies water reservoirs that are not essential for maintaining the current maximum flow
     * in the network. It deactivates each reservoir individually and assesses its impact on the water flow to determine
     * whether it contributes to the maximum flow. Reservoirs that do not affect the maximum flow are considered non-essential.
     * The function outputs the results to both the console and a CSV file, providing insights into the significance of each
     * reservoir in the network's water distribution system and aiding in decision-making for resource allocation and system
     * optimization.
     *
     * @complexity The time complexity of this function depends on the size of the network and the number of delivery sites and reservoirs.
     * The biggest contributor to the overall time complexity is the execution of the Edmonds-Karp algorithm, which has a
     * time complexity of O(V * (E^2)). So it has an overall time complexity of O(n * V * (E^2)), where n is the number of reservoirs.
     * However, in practice, it performs efficiently, because it never executes the Edmonds-Karp from the beginning.
     */
    void notEssentialReservoirs();

    /**
     * @brief Determines the impact of deactivating a specific water reservoir on the water flow in the network.
     *
     * @details This function deactivates the specified water reservoir in the network and evaluates its impact on the water
     * flow. It computes the change in flow for each city served by the reservoir and outputs the results to both the console
     * and a CSV file. The function provides insights into how the deactivation of a particular reservoir affects the water
     * distribution system, helping in assessing the resilience of the network and planning for contingencies.
     *
     * @param code The code of the reservoir to be deactivated.
     *
     * @complexity The time complexity of this function depends on the size of the network and the number of delivery sites.
     * The biggest contributor to the overall time complexity is the execution of the Edmonds-Karp algorithm, which has a
     * time complexity of O(V * (E^2)). However, in practice, it performs efficiently, because it never executes the
     * Edmonds-Karp from the beginning.
     */
    void reservoirImpact(const string &code);

    /**
     * @brief Determines the impact of putting each reservoir out of commission on the water flow in the network.
     *
     * @details This function deactivates each reservoir in the network one by one and evaluates its impact on the water
     * flow. It computes the change in flow for each city served by the reservoirs and outputs the results to both the
     * console and a CSV file. The function provides insights into how the deactivation of reservoirs affects the water
     * distribution system, helping in assessing the resilience of the network and planning for contingencies.
     *
     * @complexity The time complexity of this function depends on the size of the network and the number of delivery sites and reservoirs.
     * The biggest contributor to the overall time complexity is the execution of the Edmonds-Karp algorithm, which has a
     * time complexity of O(V * (E^2)). So it has an overall time complexity of O(n * V * (E^2)), where n is the number of reservoirs.
     * However, in practice, it performs efficiently, because it never executes the Edmonds-Karp from the beginning.
     */
    void allReservoirsImpact();

    /**
     * @brief Identifies pumping stations that are not essential for maintaining the current maximum flow in the network.
     *
     * @details This function analyzes each pumping station in the network to determine if it is essential for maintaining
     * the current maximum flow. It deactivates each pumping station one by one and evaluates the total water supplied by
     * the network. If the total water supplied remains equal to the current maximum flow after deactivating a pumping
     * station, it indicates that the station is not essential for maintaining the current flow. The function outputs the
     * codes of the pumping stations that are not essential to a CSV file and to the console. Additionally, it provides a
     * note about the significance of essential pumping stations in maintaining the network's flow. This function helps in
     * identifying redundant pumping stations in the network, aiding in optimization and cost-saving measures.
     *
     * @complexity The time complexity of this function depends on the size of the network and the number of delivery sites and pumping stations.
     * The biggest contributor to the overall time complexity is the execution of the Edmonds-Karp algorithm, which has a
     * time complexity of O(V * (E^2)). So it has an overall time complexity of O(n * V * (E^2)), where n is the number of pumping stations. However, in practice, it performs efficiently, because it never executes the
     * Edmonds-Karp from the beginning.
     */
    void notEssentialPumpingStations();

    /**
     * @brief Identifies the impact of a specific pumping station being out of commission.
     *
     * @details This function simulates the impact of a given pumping station being out of commission by deactivating the station
     * and evaluating the resulting flow changes at each delivery site. It deactivates the specified pumping station, calculates
     * the new flow at each delivery site, and identifies any changes in flow compared to the original network. If there are changes
     * in flow, indicating the impact of the pumping station, the function outputs the details including the city code, city name,
     * demand, old flow, and new flow to the console. Additionally, it provides insights into the impact of the pumping station on
     * the network's ability to meet water demand and maintain the current maximum flow. The function also calculates and displays
     * the total demand, current maximum flow, and total water supplied by the network. It helps in understanding the importance
     * of each pumping station in maintaining the network's functionality.
     *
     * @param code The code of the pumping station to be analyzed for its impact on the network.
     *
     * @complexity The time complexity of this function depends on the size of the network and the number of delivery sites.
     * The biggest contributor to the overall time complexity is the execution of the Edmonds-Karp algorithm, which has a
     * time complexity of O(V * (E^2)). However, in practice, it performs efficiently, because it never executes the
     * Edmonds-Karp from the beginning.
     */
    void pumpingStationImpact(const string &code);

    /**
     * @brief Identifies the impact of all pumping stations on the network.
     *
     * @details This function simulates the impact of each pumping station on the network by temporarily deactivating each
     * pumping station and evaluating the resulting flow changes at delivery sites. For each pumping station, it deactivates
     * the station, calculates the new flow at each delivery site, and identifies any changes in flow compared to the original
     * network. If there are changes in flow, indicating the impact of the pumping station, the function outputs the details
     * including the pumping station code, city code, demand, old flow, and new flow to the console and optionally to a CSV file.
     * Additionally, it creates an output directory if it doesn't exist and saves the results in a CSV file. The function provides
     * insights into the impact of each pumping station on the water flow to each city in the network.
     *
     * @complexity The time complexity of this function depends on the size of the network and the number of delivery sites and pumping stations.
     * The biggest contributor to the overall time complexity is the execution of the Edmonds-Karp algorithm, which has a
     * time complexity of O(V * (E^2)). So it has an overall time complexity of O(n * V * (E^2)), where n is the number of pumping stations. However, in practice, it performs efficiently, because it never executes the
     * Edmonds-Karp from the beginning.
     */
    void allPumpingStationsImpact();

    /**
     * @brief Identifies essential pipelines for each city in the network.
     *
     * @details This function determines essential pipelines for each city by simulating the impact of deactivating each pipeline
     * in the network. It iterates through each pipeline, temporarily deactivates it, and evaluates the resulting flow changes at
     * delivery sites. If deactivating a pipeline causes a change in flow at a delivery site, indicating its essentiality, the
     * pipeline is marked as essential for that city. The function then outputs the essential pipelines for each city to the
     * console and optionally to a CSV file. Additionally, it creates an output directory if it doesn't exist and saves the
     * results in a CSV file. The function provides insights into which pipelines are essential for maintaining water flow to
     * each city in the network.
     *
     * @complexity The time complexity of this function depends on the size of the network and the number of delivery sites and pumping stations.
     * The biggest contributor to the overall time complexity is the execution of the Edmonds-Karp algorithm, which has a
     * time complexity of O(V * (E^2)). So it has an overall time complexity of O(V * (E^3)).
     * However, in practice, it performs efficiently, because it never executes the Edmonds-Karp from the beginning.
     */
    void essentialPipelines();

    /**
     * @brief Calculates the impact of a specific pipeline on the flow of delivery sites and network metrics.
     *
     * @details This function calculates the impact of a specific pipeline (identified by its code) on the flow of delivery sites
     * and various network metrics. It retrieves information about the pipeline from the network's data structure, such as its
     * service points, directionality, and capacity. Then, it temporarily takes the pipeline out of commission in a copy of the
     * network graph to simulate its impact. It iterates through each delivery site to determine the change in flow caused by
     * deactivating the pipeline and prints this information to the console. Additionally, it calculates and displays total demand,
     * current max flow, and total water supplied by other pipelines. It also provides insights into whether the network can meet
     * its water needs without the pipeline and whether the pipeline is essential to maintain the current max flow.
     *
     * @param code The code of the pipeline for which the impact is to be calculated.
     *
     * @complexity The time complexity of this function depends on the size of the network and the number of delivery sites.
     * The biggest contributor to the overall time complexity is the execution of the Edmonds-Karp algorithm, which has a
     * time complexity of O(V * (E^2)). However, in practice, it performs efficiently, because it never executes the
     * Edmonds-Karp from the beginning.
     */
    void pipelineImpact(const string &code);

    /**
     * @brief Calculates the impact of each pipeline on the flow of delivery sites and saves the results to a CSV file.
     *
     * @details This function calculates the impact of each pipeline on the flow of delivery sites in the network. It creates
     * a directory for the output file if it doesn't exist and opens a CSV file to write the results. Then, it iterates through
     * each pipeline in the network, temporarily taking each pipeline out of commission and recalculating the flow of delivery
     * sites. It prints the impact of each pipeline on the flow of each delivery site to the console and writes the results to
     * the CSV file. Finally, it closes the output file and displays the path to the file if it was successfully created.
     *
     * @complexity The time complexity of this function depends on the size of the network and the number of delivery sites and pumping stations.
     * The biggest contributor to the overall time complexity is the execution of the Edmonds-Karp algorithm, which has a
     * time complexity of O(V * (E^2)). So it has an overall time complexity of O(V * (E^3)).
     * However, in practice, it performs efficiently, because it never executes the Edmonds-Karp from the beginning.
     */
    void allPipelinesImpact();
};


#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_DATA_H
