#include <set>
#include "Data.h"

Data::Data() = default;

string Data::getNetworkName() const {
    return networkName;
}

void Data::readFiles(const filesystem::path &dir_path) {
    filesystem::path reservoirPath;
    filesystem::path stationsPath;
    filesystem::path citiesPath;
    filesystem::path pipesPath;

    try {
        for (const auto& entry : filesystem::directory_iterator(dir_path)) {

            if (entry.is_regular_file()) {
                string filename = entry.path().filename().string();

                if (filename.find("Reservoir") != string::npos) {
                    if (!reservoirPath.empty()) throw runtime_error("Error: Multiple Reservoir files found.");

                    reservoirPath = dir_path / filename;

                } else if (filename.find("Stations") != string::npos) {
                    if (!stationsPath.empty()) throw runtime_error("Error: Multiple Stations files found.");

                    stationsPath = dir_path / filename;

                } else if (filename.find("Cities") != string::npos) {
                    if (!citiesPath.empty()) throw runtime_error("Error: Multiple Cities files found.");

                    citiesPath = dir_path / filename;

                } else if (filename.find("Pipes") != string::npos) {
                    if (!pipesPath.empty()) throw runtime_error("Error: Multiple Pipes files found.");

                    pipesPath = dir_path / filename;
                }
            }
        }

        if(reservoirPath.empty() | stationsPath.empty() | citiesPath.empty() | pipesPath.empty()) throw runtime_error("Invalid input. Some file are missing in the given path.");

        ifstream reservoirFile(reservoirPath);
        ifstream stationsFile(stationsPath);
        ifstream citiesFile(citiesPath);
        ifstream pipesFile(pipesPath);

        if (!reservoirFile.is_open()) throw runtime_error("Error opening the reservoir file.");
        if (!stationsFile.is_open()) throw runtime_error("Error opening the stations file.");
        if (!citiesFile.is_open()) throw runtime_error("Error opening the cities file.");
        if (!pipesFile.is_open()) throw runtime_error("Error opening the pipes file.");

        readFileReservoir(reservoirFile);
        readFileStations(stationsFile);
        readFileCities(citiesFile);
        readFilePipes(pipesFile);

        networkName = dir_path.stem();

        g.maxFlow(&waterReservoirs, &deliverySites);
        metrics = g.calculateMetrics(&deliverySites);
    } catch (const exception& e) {
        throw;
    }
}

void Data::readFileReservoir(ifstream &file) {
    string line;
    getline(file,line);

    while(getline(file,line)) {
        // Remove carriage return characters if present
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());

        string reservoir, municipality, code;
        double id, maxDelivery;
        stringstream ss(line);
        getline(ss,reservoir,',');
        getline(ss,municipality,',');
        ss >> id;
        ss.ignore();
        getline(ss, code,',');
        ss >> maxDelivery;
        ss.ignore();

        if(reservoir.empty() | municipality.empty() | code.empty()) continue;

        auto* wr = new WaterReservoir(reservoir, municipality, id, code, maxDelivery);
        g.addVertex(code, VertexType::WaterReservoir);
        this->waterReservoirs.insert({code, wr});
    }
}

void Data::readFileStations(ifstream &file) {
    string line;
    getline(file,line);

    while(getline(file,line)) {
        // Remove carriage return characters if present
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());

        string code;
        double id;
        stringstream ss(line);
        ss >> id;
        ss.ignore();
        getline(ss, code,',');

        if(code.empty()) continue;

        auto* ps = new PumpingStation(id, code);
        g.addVertex(code, VertexType::PumpingStation);
        this->pumpingStations.insert({code, ps});
    }
}

void Data::readFileCities(ifstream &file) {
    string line;
    getline(file,line);

    while(getline(file,line)) {
        // Remove carriage return characters if present
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());

        string city, code;
        double id, demand, population;
        stringstream ss(line);
        getline(ss, city,',');
        ss >> id;
        ss.ignore();
        getline(ss, code,',');
        ss >> demand;
        ss.ignore();
        ss >> population;
        ss.ignore();

        if(code.empty() || city.empty()) continue;

        auto* ds = new DeliverySite(city, id, code, demand, population);
        g.addVertex(code, VertexType::DeliverySite);
        this->deliverySites.insert({code, ds});
    }
}

void Data::readFilePipes(ifstream &file) {
    string line;
    getline(file,line);

    while(getline(file,line)) {
        // Remove carriage return characters if present
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());

        string servicePointA, servicePointB;
        double capacity, direction;
        stringstream ss(line);
        getline(ss, servicePointA,',');
        getline(ss, servicePointB,',');
        ss >> capacity;
        ss.ignore();
        ss >> direction;
        ss.ignore();

        if(servicePointA.empty() || servicePointB.empty()) continue;

        bool unidirectional = direction == 1;

        Pipe* pipe = new Pipe(servicePointA, servicePointB, capacity, unidirectional);
        string key = servicePointA;
        key += "-";
        key += servicePointB;
        this->pipes.insert({key, pipe});

        if(unidirectional) g.addEdge(servicePointA, servicePointB, capacity);
        else g.addBidirectionalEdge(servicePointA, servicePointB, capacity);
    }
}

// Confirm Existence

bool Data::deliverySiteExists(const string &code) {
    auto it = deliverySites.find(code);
    if (it != deliverySites.end()) return true;
    return false;
}

bool Data::waterReservoirExists(const string &code) {
    auto it = waterReservoirs.find(code);
    if (it != waterReservoirs.end()) return true;
    return false;
}

bool Data::pumpingStationExists(const string &code) {
    auto it = pumpingStations.find(code);
    if (it != pumpingStations.end()) return true;
    return false;
}

bool Data::pipelineExists(const string &code) {
    auto it = pipes.find(code);
    if (it != pipes.end()) return true;
    size_t dashPos = code.find('-');
    if (dashPos != string::npos) {
        string serviceStationA = code.substr(0, dashPos);
        string serviceStationB = code.substr(dashPos + 1);

        it = pipes.find(serviceStationB+"-"+serviceStationA);
        if (it != pipes.end() && !(*it).second->getUnidirectional()) return true;
    }
    return false;
}


// Max Flow

void Data::cityMaxFlow(const string &code) {
    auto it = deliverySites.find(code);

    DeliverySite *ds = (*it).second;

    string cityName = ds->getCity();
    double demand = ds->getDemand();
    double flow = g.findVertex(code)->getFlow();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Specific City Max Flow: " << endl;
    cout << "City name: " << cityName << endl;
    cout << "Code: " << code << endl;
    cout << "Demand: " << fixed << setprecision(0) << demand << " m3/sec" << endl;
    cout << "Flow value: " << fixed << setprecision(0) << flow << " m3/sec" << endl;
    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

void Data::allCitiesMaxFlow() {
    filesystem::path dir_path = filesystem::path(filesystem::current_path() / ".." / "output" / networkName);

    if (!filesystem::exists(dir_path))
        filesystem::create_directory(dir_path);

    ofstream outputFile(dir_path / "max_flow.csv");

    bool outputFileIsOpen = outputFile.is_open();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> All Cities Max Flow: " << endl;

    if(outputFileIsOpen) outputFile << "City,Code,Demand,Flow Value" << endl;

    cout << setw(24) << left << "City" << " ";
    cout << setw(10) << left << "Code" << " ";
    cout << setw(11) << left << "Demand" << " ";
    cout << setw(15) << left << "Flow Value" << endl << endl;

    double maxFlow = metrics.getMaxFlow();

    for(auto &pair : deliverySites) {
        const string cityCode = pair.first;
        DeliverySite *ds = pair.second;

        string cityName = ds->getCity();
        double demand = ds->getDemand();
        double flow = g.findVertex(cityCode)->getFlow();

        cout << setw(24) << left << cityName << " ";
        cout << setw(10) << left << cityCode << " ";
        cout << setw(11) << left << fixed << setprecision(0) << demand << " ";
        cout << setw(15) << left << fixed << setprecision(0) << flow << endl;

        if(outputFileIsOpen) outputFile << cityName << "," << cityCode << "," << demand << "," << flow << endl;
    }
    cout << endl;
    cout << "Max Flow: " << fixed << setprecision(0) << maxFlow << " m3/s" << endl << endl;

    if(outputFileIsOpen) {
        outputFile.close();
        cout << ">> Output file is at: ./output/" << networkName << "/max_flow.csv" << endl;
    }
    else {
        cout << "\033[31m";
        cout << "There was an error creating/writing the output file." << endl;
        cout << "\033[0m";
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

// Verify Water Supply

void Data::verifyWaterSupply() {
    filesystem::path dir_path = filesystem::path(filesystem::current_path() / ".." / "output" / networkName);

    if (!filesystem::exists(dir_path))
        filesystem::create_directory(dir_path);

    ofstream outputFile(dir_path / "verify_water_supply.csv");

    bool outputFileIsOpen = outputFile.is_open();

    double totalDemand = metrics.getTotalDemand();
    double totalWaterSupplied = metrics.getMaxFlow();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Cities lacking desired water rate level: " << endl;

    if(outputFileIsOpen) outputFile << "City,Code,Deficit Value" << endl;

    cout << setw(24) << left << "City" << " ";
    cout << setw(10) << left << "Code" << " ";
    cout << setw(11) << left << "Deficit Value" << endl << endl;

    for(auto &pair : deliverySites) {
        const string cityCode = pair.first;
        DeliverySite *ds = pair.second;

        string cityName = ds->getCity();
        double demand = ds->getDemand();
        double flow = g.findVertex(cityCode)->getFlow();

        if (demand <= flow) continue;

        double difference = demand - flow;

        cout << setw(24) << left << fixed << setprecision(0) << cityName << " ";
        cout << setw(10) << left << fixed << setprecision(0) << cityCode << " ";
        cout << setw(11) << left << fixed << setprecision(0) << difference << endl;

        if(outputFileIsOpen) outputFile << cityName << "," << cityCode << "," << difference << endl;
    }

    cout << endl;
    cout << "Total Demand: " << fixed << setprecision(0) << totalDemand << " m3/s" << endl;
    cout << "Total Water Supplied: " << fixed << setprecision(0) << totalWaterSupplied << " m3/s" << endl;

    if(totalDemand>totalWaterSupplied) {
        cout << "\033[31m";
        cout << "The network cannot meet the water needs!" << endl << endl;
        cout << "\033[0m";
    }
    else {
        cout << "The network can meet the water needs!" << endl << endl;
    }

    if(outputFileIsOpen) {
        outputFile.close();
        cout << ">> Output file is at: ./output/" << networkName << "/verify_water_supply.csv" << endl;
    }
    else {
        cout << "\033[31m";
        cout << "There was an error creating/writing the output file." << endl;
        cout << "\033[0m";
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

// Load Optimization

void Data::loadOptimization() {
    Graph *newGraph = g.copyGraph();
    newGraph->optimizeLoad(&deliverySites);
    GraphMetrics finalMetrics = newGraph->calculateMetrics(&deliverySites);

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Load Optimization: " << endl;
    cout << "(initial metrics / final metrics) " << endl << endl;
    cout << "> Absolute: "<< endl;
    cout << "   Average:            " << setprecision(5) << metrics.getAbsoluteAverage() << " / " << finalMetrics.getAbsoluteAverage() << endl;
    cout << "   Max Difference:     " << setprecision(5) << metrics.getAbsoluteMaxDifference() << " / " << finalMetrics.getAbsoluteMaxDifference() << endl;
    cout << "   Variance:           " << setprecision(5) << metrics.getAbsoluteVariance() << " / " << finalMetrics.getAbsoluteVariance() << endl;
    cout << "   Standard deviation: " << setprecision(5) << metrics.getAbsoluteStandardDeviation() << " / " << finalMetrics.getAbsoluteStandardDeviation() << endl << endl;

    cout << "> Relative: " << endl;
    cout << "   Average:            " << fixed << setprecision(5) << metrics.getRelativeAverage() << " / " << finalMetrics.getRelativeAverage() << endl;
    cout << "   Max Difference:     " << fixed << setprecision(5) << metrics.getRelativeMaxDifference() << " / " << finalMetrics.getRelativeMaxDifference() << endl;
    cout << "   Variance:           " << fixed << setprecision(5) << metrics.getRelativeVariance() << " / " << finalMetrics.getRelativeVariance() << endl;
    cout << "   Standard deviation: " << fixed << setprecision(5) << metrics.getRelativeStandardDeviation() << " / " << finalMetrics.getRelativeStandardDeviation() << endl << endl;

    cout << "> Total Max Flow:      " << setprecision(0) << metrics.getMaxFlow() << " / " << finalMetrics.getMaxFlow() << endl;

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

// Resilience Functions

// Reservoir Impact

void Data::notEssentialReservoirs() {
    filesystem::path dir_path = filesystem::path(filesystem::current_path() / ".." / "output" / networkName);

    if (!filesystem::exists(dir_path))
        filesystem::create_directory(dir_path);

    ofstream outputFile(dir_path / "not_essential_reservoirs.csv");

    bool outputFileIsOpen = outputFile.is_open();

    double maxFlow = metrics.getMaxFlow();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Not Essential Reservoirs: " << endl;

    if(outputFileIsOpen) outputFile << "Reservoir Code" << endl;

    unsigned int numNotEssentialReservoirs = 0;

    Graph *newGraph = g.copyGraph();

    for(auto &pair : waterReservoirs) {
        string reservoirCode = pair.first;

        newGraph->stationOutOfCommission(&reservoirCode);

        // Get current max flow
        double totalWaterSupplied = newGraph->getTotalDemandAndMaxFlow(&deliverySites).second;

        if(totalWaterSupplied == maxFlow) {
            cout << setw(10) << "" << reservoirCode << endl;
            if(outputFileIsOpen) outputFile << reservoirCode << endl;
            numNotEssentialReservoirs++;
        }
    }
    cout << endl;
    if(numNotEssentialReservoirs == 0)
        cout << "All reservoirs are essential to " << endl
             << "maintain the current max flow!" << endl;
    else {
        cout << "Note: A reservoir is essential when it is " << endl
             << "necessary to maintain the current max flow." << endl;
    }

    cout << endl;

    if(outputFileIsOpen) {
        outputFile.close();
        cout << ">> Output file is at: ./output/" << networkName << "/not_essential_reservoirs.csv" << endl;
    }
    else {
        cout << "\033[31m";
        cout << "There was an error creating/writing the output file." << endl;
        cout << "\033[0m";
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

void Data::reservoirImpact(const string &code) {
    Graph *newGraph = g.copyGraph();

    newGraph->stationOutOfCommission(&code);

    auto it = waterReservoirs.find(code);

    WaterReservoir *wr = (*it).second;

    string reservoirName = wr->getName();
    double reservoirMaxDelivery = wr->getMaxDelivery();

    double totalDemand = metrics.getTotalDemand();
    double maxFlow = metrics.getMaxFlow();
    double totalWaterSupplied = 0;

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Water Reservoir out of commission: " << endl;
    cout << "Code: " << code << endl;
    cout << "Name: " << reservoirName << endl;
    cout << "Max Delivery: " << reservoirMaxDelivery << endl << endl;
    cout << "> Cities with affected water flow: " << endl;

    cout << setw(24) << left << "City" << " ";
    cout << setw(10) << left << "Code" << " ";
    cout << setw(10) << left << "Demand" << " ";
    cout << setw(10) << left << "Old Flow" << " ";
    cout << setw(10) << left << "New Flow" << endl << endl;

    for(auto &pair : deliverySites) {
        const string cityCode = pair.first;
        DeliverySite *ds = pair.second;

        string cityName = ds->getCity();
        double demand = ds->getDemand();
        double oldFlow = g.findVertex(cityCode)->getFlow();
        double newFlow = newGraph->findVertex(cityCode)->getFlow();

        totalWaterSupplied += newFlow;

        if (oldFlow == newFlow) continue;

        cout << setw(24) << left << fixed << setprecision(0) << cityName << " ";
        cout << setw(10) << left << fixed << setprecision(0) << cityCode << " ";
        cout << setw(10) << left << fixed << setprecision(0) << demand << " ";
        cout << setw(10) << left << fixed << setprecision(0) << oldFlow << " ";
        cout << setw(10) << left << fixed << setprecision(0) << newFlow << endl;
    }

    cout << endl;
    cout << "Total Demand: " << fixed << setprecision(0) << totalDemand << " m3/s" << endl;
    cout << "Max Flow: " << fixed << setprecision(0) << maxFlow << " m3/s" << endl;
    cout << "Total Water Supplied: " << fixed << setprecision(0) << totalWaterSupplied << " m3/s" << endl;

    if(totalWaterSupplied < totalDemand) {
        cout << "\033[31m";
        cout << "> Without this reservoir the network cannot meet the water needs!" << endl;
        cout << "\033[0m";
    }
    else {
        cout << "> Without this reservoir the network can meet the water needs!" << endl;
    }

    if(maxFlow == totalWaterSupplied) {
        cout << "> This reservoir is not essential to maintain the current max flow!" << endl;
    }
    else {
        cout << "\033[31m";
        cout << "> This reservoir is essential to maintain the current max flow!" << endl;
        cout << "\033[0m";
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

void Data::allReservoirsImpact() {
    filesystem::path dir_path = filesystem::path(filesystem::current_path() / ".." / "output" / networkName);

    if (!filesystem::exists(dir_path))
        filesystem::create_directory(dir_path);

    ofstream outputFile(dir_path / "reservoirs_impact.csv");

    bool outputFileIsOpen = outputFile.is_open();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> All Reservoirs Impact: " << endl;
    cout << "Reservoir Code > (City Code, Demand, Old Flow, New Flow)" << endl << endl;

    if(outputFileIsOpen) outputFile << "Reservoir Code,City Code,Demand,Old Flow,New Flow" << endl;

    Graph *newGraph = g.copyGraph();

    for(auto &pair : waterReservoirs) {
        string reservoirCode = pair.first;

        newGraph->stationOutOfCommission(&reservoirCode);

        cout << reservoirCode << "\t >  ";

        for(auto &dsPair : deliverySites) {
            const string cityCode = dsPair.first;
            DeliverySite *ds = dsPair.second;

            double demand = ds->getDemand();
            double oldFlow = g.findVertex(cityCode)->getFlow();
            double newFlow = newGraph->findVertex(cityCode)->getFlow();

            if (oldFlow == newFlow) continue;

            cout << "(" << cityCode + ", ";
            cout << fixed << setprecision(0) << demand << ", ";
            cout << fixed << setprecision(0) << oldFlow << ", ";
            cout << fixed << setprecision(0) << newFlow << ")   ";

            if(outputFileIsOpen) outputFile << reservoirCode << "," << cityCode << "," << demand << "," << oldFlow << "," << newFlow << endl;
        }
        cout << endl;
    }
    cout << endl;

    if(outputFileIsOpen) {
        outputFile.close();
        cout << ">> Output file is at: ./output/" << networkName << "/reservoirs_impact.csv" << endl;
    }
    else {
        cout << "\033[31m";
        cout << "There was an error creating/writing the output file." << endl;
        cout << "\033[0m";
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

// Pumping Station Impact

void Data::notEssentialPumpingStations() {
    filesystem::path dir_path = filesystem::path(filesystem::current_path() / ".." / "output" / networkName);

    if (!filesystem::exists(dir_path))
        filesystem::create_directory(dir_path);

    ofstream outputFile(dir_path / "not_essential_stations.csv");

    bool outputFileIsOpen = outputFile.is_open();

    double maxFlow = metrics.getMaxFlow();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Not Essential Pumping Stations: " << endl;

    if(outputFileIsOpen) outputFile << "Station Code" << endl;

    unsigned int numNotEssentialPumpingStations = 0;

    Graph *newGraph = g.copyGraph();

    for(auto &pair : pumpingStations) {
        string psCode = pair.first;

        newGraph->stationOutOfCommission(&psCode);

        // Get current max flow
        double totalWaterSupplied = newGraph->getTotalDemandAndMaxFlow(&deliverySites).second;

        if(totalWaterSupplied == maxFlow) {
            cout << setw(10) << "" << psCode << endl;
            if(outputFileIsOpen) outputFile << psCode << endl;
            numNotEssentialPumpingStations++;
        }
    }
    cout << endl;
    if(numNotEssentialPumpingStations == 0)
        cout << "All pumping stations are essential to " << endl
             << "maintain the current max flow!" << endl;
    else {
        cout << "Note: A pumping station is essential when it is " << endl
             << "necessary to maintain the current max flow." << endl;
    }

    cout << endl;

    if(outputFileIsOpen) {
        outputFile.close();
        cout << ">> Output file is at: ./output/" << networkName << "/not_essential_stations.csv" << endl;
    }
    else {
        cout << "\033[31m";
        cout << "There was an error creating/writing the output file." << endl;
        cout << "\033[0m";
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

void Data::pumpingStationImpact(const string &code) {
    double maxFlow = metrics.getMaxFlow();
    double totalDemand = metrics.getTotalDemand();

    Graph *newGraph = g.copyGraph();

    newGraph->stationOutOfCommission(&code);

    double totalWaterSupplied = 0;

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Pumping Station out of commission: " << endl;
    cout << "Code: " << code << endl << endl;
    cout << "> Cities with affected water flow: " << endl;

    cout << setw(24) << left << "City" << " ";
    cout << setw(10) << left << "Code" << " ";
    cout << setw(10) << left << "Demand" << " ";
    cout << setw(10) << left << "Old Flow" << " ";
    cout << setw(10) << left << "New Flow" << endl << endl;

    for(auto &pair : deliverySites) {
        const string cityCode = pair.first;
        DeliverySite *ds = pair.second;

        string cityName = ds->getCity();
        double demand = ds->getDemand();
        double oldFlow = g.findVertex(cityCode)->getFlow();
        double newFlow = newGraph->findVertex(cityCode)->getFlow();

        totalWaterSupplied += newFlow;

        if (oldFlow == newFlow) continue;

        cout << setw(24) << left << fixed << setprecision(0) << cityName << " ";
        cout << setw(10) << left << fixed << setprecision(0) << cityCode << " ";
        cout << setw(10) << left << fixed << setprecision(0) << demand << " ";
        cout << setw(10) << left << fixed << setprecision(0) << oldFlow << " ";
        cout << setw(10) << left << fixed << setprecision(0) << newFlow << endl;
    }

    cout << endl;
    cout << "Total Demand: " << fixed << setprecision(0) << totalDemand << " m3/s" << endl;
    cout << "Current Max Flow: " << fixed << setprecision(0) << maxFlow << " m3/s" << endl;
    cout << "Total Water Supplied: " << fixed << setprecision(0) << totalWaterSupplied << " m3/s" << endl;

    if(totalWaterSupplied < totalDemand) {
        cout << "\033[31m";
        cout << "> Without this pumping station the network cannot meet the water needs!" << endl;
        cout << "\033[0m";
    }
    else {
        cout << "> Without this pumping station the network can meet the water needs!" << endl;
    }

    if(maxFlow == totalWaterSupplied) {
        cout << "> This pumping station is not essential to maintain the current max flow!" << endl;
    }
    else {
        cout << "\033[31m";
        cout << "> This pumping station is essential to maintain the current max flow!" << endl;
        cout << "\033[0m";
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

void Data::allPumpingStationsImpact() {
    filesystem::path dir_path = filesystem::path(filesystem::current_path() / ".." / "output" / networkName);

    if (!filesystem::exists(dir_path))
        filesystem::create_directory(dir_path);

    ofstream outputFile(dir_path / "stations_impact.csv");

    bool outputFileIsOpen = outputFile.is_open();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> All Pumping Stations Impact: " << endl;
    cout << "Pumping Station Code > (City Code, Demand, Old Flow, New Flow)" << endl << endl;

    if(outputFileIsOpen) outputFile << "Station Code,City Code,Demand,Old Flow,New Flow" << endl;

    Graph *newGraph = g.copyGraph();

    for(auto &pair : pumpingStations) {
        string psCode = pair.first;

        newGraph->stationOutOfCommission(&psCode);

        cout << psCode << "\t >  ";

        for(auto &dsPair : deliverySites) {
            const string cityCode = dsPair.first;
            DeliverySite *ds = dsPair.second;

            double demand = ds->getDemand();
            double oldFlow = g.findVertex(cityCode)->getFlow();
            double newFlow = newGraph->findVertex(cityCode)->getFlow();

            if (oldFlow == newFlow) continue;

            cout << "(" << cityCode << ", ";
            cout << fixed << setprecision(0) << demand << ", ";
            cout << fixed << setprecision(0) << oldFlow << ", ";
            cout << fixed << setprecision(0) << newFlow << ")   ";

            if(outputFileIsOpen) outputFile << psCode << "," << cityCode << "," << demand << "," << oldFlow << "," << newFlow << endl;
        }
        cout << endl;
    }
    cout << endl;

    if(outputFileIsOpen) {
        outputFile.close();
        cout << ">> Output file is at: ./output/" << networkName << "/stations_impact.csv" << endl;
    }
    else {
        cout << "\033[31m";
        cout << "There was an error creating/writing the output file." << endl;
        cout << "\033[0m";
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

// Pipeline Impact

void Data::essentialPipelines() {
    unordered_map<string, set<string>> cityToEssentialPipelines;
    Graph *newGraph = g.copyGraph();

    for(auto &pair : pipes) {
        string pipelineCode = pair.first;
        Pipe *pipeline = pair.second;

        string servicePointA = pipeline->getServicePointA();
        string servicePointB = pipeline->getServicePointB();
        bool unidirectional = pipeline->getUnidirectional();

        newGraph->pipelineOutOfCommission(&servicePointA, &servicePointB, unidirectional);

        for(auto &dsPair : deliverySites) {
            const string cityCode = dsPair.first;

            double oldFlow = g.findVertex(cityCode)->getFlow();
            double newFlow = newGraph->findVertex(cityCode)->getFlow();

            if(oldFlow == newFlow) continue;

            auto it = cityToEssentialPipelines.find(cityCode);

            if(it == cityToEssentialPipelines.end()) {
                cityToEssentialPipelines.insert({cityCode, {pipelineCode}});
            }
            else {
                (*it).second.insert(pipelineCode);
            }
        }
    }

    filesystem::path dir_path = filesystem::path(filesystem::current_path() / ".." / "output" / networkName);

    if (!filesystem::exists(dir_path))
        filesystem::create_directory(dir_path);

    ofstream outputFile(dir_path / "cities_not_essential_pipelines.csv");

    bool outputFileIsOpen = outputFile.is_open();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Essential Pipelines for each city: " << endl;
    cout << "(City Code, City Name) > (Pipeline Code)" << endl << endl;

    if(outputFileIsOpen) outputFile << "City Code,City Name,Pipeline Code" << endl;

    for(const auto &pair : cityToEssentialPipelines) {
        string cityCode = pair.first;
        DeliverySite *ds = deliverySites.at(cityCode);

        string cityName = ds->getCity();

        cout << "(" << cityCode << ", " << cityName << ")  >  ";

        for(const string &pipelineCode : pair.second) {
            cout << "(" << pipelineCode << ") ";
            if(outputFileIsOpen) outputFile << cityCode << "," << cityName << "," << pipelineCode << endl;
        }

        cout << endl;
    }
    cout << endl;

    if(outputFileIsOpen) {
        outputFile.close();
        cout << ">> Output file is at: ./output/" << networkName << "/cities_not_essential_pipelines.csv" << endl;
    }
    else {
        cout << "\033[31m";
        cout << "There was an error creating/writing the output file." << endl;
        cout << "\033[0m";
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

void Data::pipelineImpact(const string &code) {
    double maxFlow = metrics.getMaxFlow();
    double totalDemand = metrics.getTotalDemand();

    auto it = pipes.find(code);
    if (it == pipes.end()) {
        size_t dashPos = code.find('-');
        it = pipes.find(code.substr(dashPos + 1)+"-"+code.substr(0, dashPos));
    }

    string pipelineCode = (*it).first;
    Pipe *pipeline = (*it).second;
    string servicePointA = pipeline->getServicePointA();
    string servicePointB = pipeline->getServicePointB();
    bool unidirectional = pipeline->getUnidirectional();
    double capacity = pipeline->getCapacity();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Pipeline Impact: " << endl;
    cout << "Code: " << code << endl;
    cout << "Capacity: " << capacity << endl;
    if(unidirectional) cout << "Unidirectional" << endl << endl;
    else cout << "Bidirectional" << endl << endl;

    cout << "> Cities with affected water flow: " << endl;
    cout << setw(24) << left << "City" << " ";
    cout << setw(10) << left << "Code" << " ";
    cout << setw(10) << left << "Demand" << " ";
    cout << setw(10) << left << "Old Flow" << " ";
    cout << setw(10) << left << "New Flow" << endl << endl;

    Graph *newGraph = g.copyGraph();

    newGraph->pipelineOutOfCommission(&servicePointA, &servicePointB, unidirectional);

    double totalWaterSupplied = 0;

    for(auto &dsPair : deliverySites) {
        const string cityCode = dsPair.first;
        DeliverySite *ds = dsPair.second;

        string cityName = ds->getCity();
        double demand = ds->getDemand();
        double oldFlow = g.findVertex(cityCode)->getFlow();
        double newFlow = newGraph->findVertex(cityCode)->getFlow();

        totalWaterSupplied += newFlow;

        if (oldFlow == newFlow) continue;

        cout << setw(24) << left << fixed << setprecision(0) << cityName << " ";
        cout << setw(10) << left << fixed << setprecision(0) << cityCode << " ";
        cout << setw(10) << left << fixed << setprecision(0) << demand << " ";
        cout << setw(10) << left << fixed << setprecision(0) << oldFlow << " ";
        cout << setw(10) << left << fixed << setprecision(0) << newFlow << endl;
    }

    cout << endl;
    cout << "Total Demand: " << fixed << setprecision(0) << totalDemand << " m3/s" << endl;
    cout << "Current Max Flow: " << fixed << setprecision(0) << maxFlow << " m3/s" << endl;
    cout << "Total Water Supplied: " << fixed << setprecision(0) << totalWaterSupplied << " m3/s" << endl;

    if(totalDemand > totalWaterSupplied) {
        cout << "\033[31m";
        cout << "> Without this pipeline the network cannot meet the water needs!" << endl;
        cout << "\033[0m";
    }
    else {
        cout << "> Without this pipeline the network can meet the water needs!" << endl;
    }

    if(maxFlow == totalWaterSupplied) {
        cout << "> This pipeline is not essential to maintain the current max flow!" << endl;
    }
    else {
        cout << "\033[31m";
        cout << "> This pipeline is essential to maintain the current max flow!" << endl;
        cout << "\033[0m";
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

void Data::allPipelinesImpact() {
    filesystem::path dir_path = filesystem::path(filesystem::current_path() / ".." / "output" / networkName);

    if (!filesystem::exists(dir_path))
        filesystem::create_directory(dir_path);

    ofstream outputFile(dir_path / "pipelines_impact.csv");

    bool outputFileIsOpen = outputFile.is_open();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> All Pipelines Impact: " << endl;
    cout << "(Pipeline Code) > (City Code, Demand, Old Flow, New Flow)" << endl << endl;

    if(outputFileIsOpen) outputFile << "Pipeline Code,City Code,Demand,Old Flow,New Flow" << endl;

    Graph *newGraph = g.copyGraph();

    for(auto &pair : pipes) {
        string pipelineCode = pair.first;
        Pipe *pipeline = pair.second;

        string servicePointA = pipeline->getServicePointA();
        string servicePointB = pipeline->getServicePointB();
        bool unidirectional = pipeline->getUnidirectional();

        newGraph->pipelineOutOfCommission(&servicePointA, &servicePointB, unidirectional);

        cout << "(" << pipelineCode << ")  >  ";

        for(auto &dsPair : deliverySites) {
            const string cityCode = dsPair.first;
            DeliverySite *ds = dsPair.second;

            double demand = ds->getDemand();
            double oldFlow = g.findVertex(cityCode)->getFlow();
            double newFlow = newGraph->findVertex(cityCode)->getFlow();

            if (oldFlow == newFlow) continue;

            cout << "(" << cityCode << ", "
                 << fixed << setprecision(0) << demand << ", "
                 << fixed << setprecision(0) << oldFlow << ", "
                 << fixed << setprecision(0) << newFlow << ")   ";

            if(outputFileIsOpen) outputFile << pipelineCode << "," << cityCode << "," << demand << "," << oldFlow << "," << newFlow << endl;
        }
        cout << endl;
    }
    cout << endl;

    if(outputFileIsOpen) {
        outputFile.close();
        cout << ">> Output file is at: ./output/" << networkName << "/pipelines_impact.csv" << endl;
    }
    else {
        cout << "\033[31m";
        cout << "There was an error creating/writing the output file." << endl;
        cout << "\033[0m";
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}