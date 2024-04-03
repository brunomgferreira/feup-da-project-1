#include <set>
#include "Data.h"

Data::Data() {}

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
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

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

        if(reservoir == "" | municipality == "" | code == "") continue;

        WaterReservoir* wr = new WaterReservoir(reservoir, municipality, id, code, maxDelivery);
        g.addVertex(code, VertexType::WaterReservoir);
        this->waterReservoirs.insert({code, wr});
    }
}

void Data::readFileStations(ifstream &file) {
    string line;
    getline(file,line);

    while(getline(file,line)) {
        // Remove carriage return characters if present
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        string code;
        double id;
        stringstream ss(line);
        ss >> id;
        ss.ignore();
        getline(ss, code,',');

        if(code == "") continue;

        PumpingStation* ps = new PumpingStation(id, code);
        g.addVertex(code, VertexType::PumpingStation);
        this->pumpingStations.insert({code, ps});
    }
}

void Data::readFileCities(ifstream &file) {
    string line;
    getline(file,line);

    while(getline(file,line)) {
        // Remove carriage return characters if present
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

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

        if(code == "" || city == "") continue;

        DeliverySite* ds = new DeliverySite(city, id, code, demand, population);
        g.addVertex(code, VertexType::DeliverySite);
        this->deliverySites.insert({code, ds});
    }
}

void Data::readFilePipes(ifstream &file) {
    string line;
    getline(file,line);

    while(getline(file,line)) {
        // Remove carriage return characters if present
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        string servicePointA, servicePointB;
        double capacity, direction;
        stringstream ss(line);
        getline(ss, servicePointA,',');
        getline(ss, servicePointB,',');
        ss >> capacity;
        ss.ignore();
        ss >> direction;
        ss.ignore();

        if(servicePointA == "" || servicePointB == "") continue;

        bool unidirectional = direction == 1 ? true : false;

        Pipe* pipe = new Pipe(servicePointA, servicePointB, capacity, unidirectional);
        string key = servicePointA+"-"+servicePointB;
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

    filesystem::path dir_path = filesystem::path(filesystem::current_path() / ".." / "output");
    // This way the folder is inside the cmake-build-debug folder: filesystem::path(filesystem::current_path() / "output);

    if (!filesystem::exists(dir_path))
        filesystem::create_directory(dir_path);

    ofstream outputFile(dir_path / "max_flow_data.csv");

    bool outputFileIsOpen = outputFile.is_open();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> All Cities Max Flow: " << endl;

    if(outputFileIsOpen) outputFile << "City,Code,Demand,Flow Value" << endl;

    cout << setw(24) << left << "City";
    cout << setw(10) << left << "Code";
    cout << setw(11) << left << "Demand";
    cout << setw(15) << left << "Flow Value" << endl << endl;

    double maxFlow = metrics.getMaxFlow();

    for(auto &pair : deliverySites) {
        const string code = pair.first;
        DeliverySite *ds = pair.second;

        string cityName = ds->getCity();
        double demand = ds->getDemand();
        double flow = g.findVertex(code)->getFlow();

        ostringstream ss;
        ss << fixed << setprecision(0) << demand;

        cout << setw(24) << left << cityName + ",";
        cout << setw(10) << left << code + ",";
        cout << setw(11) << left << ss.str() + ",";
        cout << setw(15) << left << fixed << setprecision(0) << flow << endl;

        if(outputFileIsOpen) outputFile << cityName << "," << code << "," << demand << "," << flow << endl;
    }
    cout << endl;
    cout << "Max Flow: " << fixed << setprecision(0) << maxFlow << " m3/s" << endl << endl;

    if(outputFileIsOpen) {
        outputFile.close();
        cout << ">> Output file is at: ./output/verify_water_supply.txt" << endl;
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

void Data::verifyWaterSupply() {

    filesystem::path dir_path = filesystem::path(filesystem::current_path() / ".." / "output");
    // This way the folder is inside the cmake-build-debug folder: filesystem::path(filesystem::current_path() / "output);

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

    cout << setw(24) << left << "City";
    cout << setw(10) << left << "Code";
    cout << setw(11) << left << "Deficit Value" << endl << endl;

    for(auto &pair : deliverySites) {
        const string code = pair.first;
        DeliverySite *ds = pair.second;

        string cityName = ds->getCity();
        double demand = ds->getDemand();
        double flow = g.findVertex(code)->getFlow();

        if (demand <= flow) continue;

        double difference = demand-flow;

        cout << setw(24) << left << fixed << setprecision(0) << cityName + ",";
        cout << setw(10) << left << fixed << setprecision(0) << code + ",";
        cout << setw(11) << left << fixed << setprecision(0) << difference << endl;

        if(outputFileIsOpen) outputFile << cityName << "," << code << "," << difference << endl;
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
        cout << ">> Output file is at: ./output/verify_water_supply.txt" << endl;
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

void Data::reservoirImpact(const string &code) {
    Graph *newGraph = g.copyGraph();

    newGraph->reservoirOutOfCommission(&waterReservoirs, &deliverySites, &code);

    auto it = waterReservoirs.find(code);

    WaterReservoir *wr = (*it).second;

    string name = wr->getName();
    double maxDelivery = wr->getMaxDelivery();

    double totalDemand = metrics.getTotalDemand();
    double maxFlow = metrics.getMaxFlow();
    double totalWaterSupplied = 0;

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Water Reservoir out of commission: " << endl;
    cout << "Code: " << code  << ", Name: " << name << ", Max Delivery: " << maxDelivery << endl << endl;
    cout << "> Cities lacking desired water rate level: " << endl;

    cout << setw(24) << left << "City";
    cout << setw(10) << left << "Code";
    cout << setw(11) << left << "Deficit Value" << endl << endl;

    for(auto &pair : deliverySites) {
        const string code = pair.first;
        DeliverySite *ds = pair.second;

        string cityName = ds->getCity();
        double demand = ds->getDemand();
        double flow = newGraph->findVertex(code)->getFlow();

        totalWaterSupplied+=flow;

        if (demand <= flow) continue;

        double difference = demand-flow;

        cout << setw(24) << left << fixed << setprecision(0) << cityName + ",";
        cout << setw(10) << left << fixed << setprecision(0) << code + ",";
        cout << setw(11) << left << fixed << setprecision(0) << difference << endl;
    }

    cout << endl;
    cout << "Total Demand: " << fixed << setprecision(0) << totalDemand << " m3/s" << endl;
    cout << "Max Flow: " << fixed << setprecision(0) << maxFlow << " m3/s" << endl;
    cout << "Total Water Supplied: " << fixed << setprecision(0) << totalWaterSupplied << " m3/s" << endl;

    if(totalDemand > totalWaterSupplied) {
        cout << "\033[31m";
        cout << "Without this reservoir the network cannot meet the water needs!" << endl << endl;
        cout << "\033[0m";
    }
    else {
        cout << "Without this reservoir the network can meet the water needs!" << endl << endl;
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

void Data::allReservoirsImpact() {
    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> All Reservoirs Impact: " << endl;
    cout << "Reservoir Code > (City Code, Deficit Value)" << endl << endl;

    Graph *newGraph = g.copyGraph();

    for(auto &pair : waterReservoirs) {
        string psCode = pair.first;

        newGraph->reservoirOutOfCommission(&waterReservoirs, &deliverySites, &psCode);

        cout << psCode << "\t >  ";

        for(auto &pair : deliverySites) {
            const string code = pair.first;
            DeliverySite *ds = pair.second;

            string cityName = ds->getCity();
            double demand = ds->getDemand();
            double flow = newGraph->findVertex(code)->getFlow();

            if (demand <= flow) continue;

            double difference = demand-flow;

            cout << "(" << code + ", " << fixed << setprecision(0) << difference << ")\t";
        }
        cout << endl;
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

void Data::notEssentialPumpingStations() {
    double maxFlow = metrics.getMaxFlow();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Not Essential Pumping Stations: " << endl;

    unsigned int count = 0;

    Graph *newGraph = g.copyGraph();

    for(auto &pair : pumpingStations) {
        string psCode = pair.first;

        newGraph->pumpingStationOutOfCommission(&waterReservoirs, &deliverySites, &psCode);

        // Get current max flow
        double totalWaterSupplied = newGraph->getTotalDemandAndMaxFlow(&deliverySites).second;

        if(totalWaterSupplied == maxFlow) {
            cout << setw(10) << "" << psCode << endl;
            count++;
        }
    }
    cout << endl;
    if(count == 0)
        cout << "All pumping stations are essential to " << endl
             << "maintain the current max flow!" << endl;
    else {
        cout << "Note: A pumping station is essential when it is " << endl
             << "necessary to maintain the current max flow." << endl;
    }
    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

void Data::pumpingStationImpact(const std::string &code) {

    double maxFlow = metrics.getMaxFlow();
    double totalDemand = metrics.getTotalDemand();

    Graph *newGraph = g.copyGraph();

    newGraph->pumpingStationOutOfCommission(&waterReservoirs, &deliverySites, &code);

    double totalWaterSupplied = 0;

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Pumping Station out of commission: " << endl;
    cout << "Code: " << code << endl << endl;
    cout << "> Cities lacking desired water rate level: " << endl;

    cout << setw(24) << left << "City";
    cout << setw(10) << left << "Code";
    cout << setw(11) << left << "Deficit Value" << endl << endl;

    for(auto &pair : deliverySites) {
        const string code = pair.first;
        DeliverySite *ds = pair.second;

        string cityName = ds->getCity();
        double demand = ds->getDemand();
        double flow = newGraph->findVertex(code)->getFlow();

        totalWaterSupplied += flow;

        if (demand <= flow) continue;

        double difference = demand-flow;

        cout << setw(24) << left << fixed << setprecision(0) << cityName + ",";
        cout << setw(10) << left << fixed << setprecision(0) << code + ",";
        cout << setw(11) << left << fixed << setprecision(0) << difference << endl;
    }

    cout << endl;
    cout << "Total Demand: " << fixed << setprecision(0) << totalDemand << " m3/s" << endl;
    cout << "Current Max Flow: " << fixed << setprecision(0) << maxFlow << " m3/s" << endl;
    cout << "Total Water Supplied: " << fixed << setprecision(0) << totalWaterSupplied << " m3/s" << endl;

    if(totalDemand > totalWaterSupplied) {
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
    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> All Pumping Stations Impact: " << endl;
    cout << "Pumping Station Code > (City Code, Deficit Value)" << endl << endl;

    Graph *newGraph = g.copyGraph();

    for(auto &pair : pumpingStations) {
        string psCode = pair.first;

        newGraph->pumpingStationOutOfCommission(&waterReservoirs, &deliverySites, &psCode);

        cout << psCode << "\t >  ";

        for(auto &pair : deliverySites) {
            const string code = pair.first;
            DeliverySite *ds = pair.second;

            string cityName = ds->getCity();
            double demand = ds->getDemand();
            double flow = newGraph->findVertex(code)->getFlow();

            if (demand <= flow) continue;

            double difference = demand-flow;

            cout << "(" << code + ", " << fixed << setprecision(0) << difference << ")\t";
        }
        cout << endl;
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

void Data::essentialPipelines() {
    unordered_map<string, set<string>> cityToEssentialPipelines;

    Graph *newGraph = g.copyGraph();

    for(auto &pair : pipes) {
        string pipeCode = pair.first;
        Pipe *pipeline = pair.second;

        string servicePointA = pipeline->getServicePointA();
        string servicePointB = pipeline->getServicePointB();
        bool unidirectional = pipeline->getUnidirectional();

        newGraph->pipelineOutOfCommission(&waterReservoirs, &deliverySites, &servicePointA, &servicePointB, unidirectional);

        for(auto &pair : deliverySites) {
            const string code = pair.first;
            DeliverySite *ds = pair.second;

            string cityName = ds->getCity();
            double vertexMaxFlow = g.findVertex(code)->getFlow();

            double vertexCurrentFlow = newGraph->findVertex(code)->getFlow();

            if(vertexCurrentFlow != vertexMaxFlow) {
                auto it = cityToEssentialPipelines.find(code);

                if(it == cityToEssentialPipelines.end()) {
                    cityToEssentialPipelines.insert({code, {pipeCode}});
                }
                else {
                    (*it).second.insert(pipeCode);
                }
            }
        }
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Essential Pipelines for each city: " << endl;
    cout << "(City Code, City Name) > (Pipeline Code)" << endl << endl;

    for(auto pair : cityToEssentialPipelines) {
        string code = pair.first;
        DeliverySite *ds = deliverySites.at(code);

        cout << "(" << code << "," << ds->getCity() << ")  >  ";

        for(string pipeCode : pair.second) {
            cout << "(" << pipeCode << ") ";
        }

        cout << endl;
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

void Data::pipelineImpact(const string &code) {

    auto it = pipes.find(code);
    if (it == pipes.end()) {
        size_t dashPos = code.find('-');
        it = pipes.find(code.substr(dashPos + 1)+"-"+code.substr(0, dashPos));
    }

    string pipeCode = (*it).first;
    Pipe *pipe = (*it).second;
    string servicePointA = pipe->getServicePointA();
    string servicePointB = pipe->getServicePointB();
    bool unidirectional = pipe->getUnidirectional();
    double capacity = pipe->getCapacity();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Pipeline Impact: " << endl;
    cout << "Code: " << code << endl;
    cout << "Capacity: " << capacity << endl;
    if(unidirectional) cout << "Unidirectional" << endl << endl;
    else cout << "Bidirectional" << endl << endl;

    cout << "> Cities lacking desired water rate level: " << endl;
    cout << "(Code, Name, Deficit Value)" << endl << endl;

    Graph *newGraph = g.copyGraph();

    newGraph->pipelineOutOfCommission(&waterReservoirs, &deliverySites, &servicePointA, &servicePointB, unidirectional);

    for(auto &pair : deliverySites) {
        const string dsCode = pair.first;
        DeliverySite *ds = pair.second;

        string cityName = ds->getCity();
        double demand = ds->getDemand();

        double vertexCurrentFlow = newGraph->findVertex(dsCode)->getFlow();

        double difference = demand - vertexCurrentFlow;

        if(difference > 0) cout << "(" << dsCode << "," << cityName << "," << difference << ")" << endl;
    }

    cout << endl;

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";

}

void Data::allPipelinesImpact() {

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> All Pipelines Impact: " << endl;
    cout << "(Pipeline Code) > (City Code, City Name, Deficit Value)" << endl << endl;

    Graph *newGraph = g.copyGraph();

    for(auto &pair : pipes) {
        string pipeCode = pair.first;
        Pipe *pipeline = pair.second;

        string servicePointA = pipeline->getServicePointA();
        string servicePointB = pipeline->getServicePointB();
        bool unidirectional = pipeline->getUnidirectional();

        newGraph->pipelineOutOfCommission(&waterReservoirs, &deliverySites, &servicePointA, &servicePointB, unidirectional);

        cout << "(" << pipeCode << ")  >  ";

        for(auto &pair : deliverySites) {
            const string code = pair.first;
            DeliverySite *ds = pair.second;

            string cityName = ds->getCity();
            double demand = ds->getDemand();

            double vertexCurrentFlow = newGraph->findVertex(code)->getFlow();

            double difference = demand - vertexCurrentFlow;

            if(difference > 0) cout << "(" << code << "," << cityName << "," << difference << ") ";
        }

        cout << endl;
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}