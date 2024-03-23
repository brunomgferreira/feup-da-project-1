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
        unsigned long id, maxDelivery;
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
        unsigned long id;
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
        unsigned long id, demand, population;
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
        unsigned long capacity, direction;
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


// Max Flow

void Data::cityMaxFlow(const string &code) {
    g.maxFlow(&waterReservoirs, &deliverySites);

    auto it = deliverySites.find(code);

    DeliverySite *ds = (*it).second;

    string cityName = ds->getCity();
    unsigned long demand = ds->getDemand();
    unsigned long flow = g.findVertex(code)->getFlow();

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Specific City Max Flow: " << endl;
    cout << "City name: " << cityName << endl;
    cout << "Code: " << code << endl;
    cout << "Demand: " << demand << " m3/sec" << endl;
    cout << "Flow value: " << flow << " m3/sec" << endl;
    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

void Data::allCitiesMaxFlow() {

    g.maxFlow(&waterReservoirs, &deliverySites);

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

    unsigned long maxFlow = 0;

    for(auto &pair : deliverySites) {
        const string code = pair.first;
        DeliverySite *ds = pair.second;

        string cityName = ds->getCity();
        unsigned long demand = ds->getDemand();
        unsigned long flow = g.findVertex(code)->getFlow();

        maxFlow += flow;

        cout << setw(24) << left << cityName + ",";
        cout << setw(10) << left << code + ",";
        cout << setw(11) << left << to_string(demand) + ",";
        cout << setw(15) << left << to_string(flow) << endl;

        if(outputFileIsOpen) outputFile << cityName << "," << code << "," << demand << "," << flow << endl;
    }
    cout << endl;
    cout << "Max Flow: " << to_string(maxFlow) << " m3/s" << endl << endl;

    if(outputFileIsOpen) cout << ">> Output file is at: ./output/max_flow_data.txt" << endl;
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

    g.maxFlow(&waterReservoirs, &deliverySites);

    unsigned long totalDemand = 0;
    unsigned long totalWaterSupplied = 0;

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
    cout << ">> Cities lacking desired water rate level: " << endl;

    cout << setw(24) << left << "City";
    cout << setw(10) << left << "Code";
    cout << setw(11) << left << "Deficit Value" << endl << endl;

    for(auto &pair : deliverySites) {
        const string code = pair.first;
        DeliverySite *ds = pair.second;

        string cityName = ds->getCity();
        unsigned long demand = ds->getDemand();
        unsigned long flow = g.findVertex(code)->getFlow();

        totalWaterSupplied += flow;
        totalDemand += demand;

        if (demand <= flow) continue;

        cout << setw(24) << left << cityName + ",";
        cout << setw(10) << left << code + ",";
        cout << setw(11) << left << to_string(demand-flow) << endl;
    }

    cout << endl;
    cout << "Total Demand: " << to_string(totalDemand) << " m3/s" << endl;
    cout << "Total Water Supplied: " << to_string(totalWaterSupplied) << " m3/s" << endl;

    if(totalDemand>totalWaterSupplied) {
        cout << "\033[31m";
        cout << "The network cannot meet the water needs!" << endl << endl;
        cout << "\033[0m";
    }
    else {
        cout << "The network can meet the water needs!" << endl << endl;
    }

    cout << "\033[32m";
    cout << "----------------------------------------------------" << endl;
    cout << "\033[0m";
}

