#include "train_search.h"
#include "validation.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// ============================================================================
// Train Search Implementation
// Maps to: FR-001 through FR-009
// ============================================================================

std::vector<Train> searchTrains(const std::vector<Train>& trains,
                                 const std::string& source,
                                 const std::string& destination) {
    std::vector<Train> results;

    // FR-003: Display trains matching search criteria
    for (const auto& train : trains) {
        // Case-insensitive comparison
        std::string trainSrc = train.source;
        std::string trainDst = train.destination;
        std::string searchSrc = source;
        std::string searchDst = destination;

        std::transform(trainSrc.begin(), trainSrc.end(), trainSrc.begin(), ::tolower);
        std::transform(trainDst.begin(), trainDst.end(), trainDst.begin(), ::tolower);
        std::transform(searchSrc.begin(), searchSrc.end(), searchSrc.begin(), ::tolower);
        std::transform(searchDst.begin(), searchDst.end(), searchDst.begin(), ::tolower);

        if (trainSrc == searchSrc && trainDst == searchDst) {
            results.push_back(train);
        }
    }

    return results;
}

void displayTrainList(const std::vector<Train>& matchingTrains) {
    if (matchingTrains.empty()) {
        // FR-004: No matching train
        std::cout << std::endl;
        std::cout << "[Info] No trains found for the specified route." << std::endl;
        return;
    }

    std::cout << std::endl;
    std::cout << "╔══════════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                         AVAILABLE TRAINS                                 ║" << std::endl;
    std::cout << "╠════════╦══════════════════╦════════════╦════════════╦════════╦════════════╣" << std::endl;
    std::cout << "║  ID    ║ Name             ║ Departure  ║ Arrival    ║ Seats  ║ Available  ║" << std::endl;
    std::cout << "╠════════╬══════════════════╬════════════╬════════════╬════════╬════════════╣" << std::endl;

    for (const auto& train : matchingTrains) {
        std::cout << "║ " << std::setw(6) << std::left << train.trainId
                  << "║ " << std::setw(17) << std::left << train.trainName
                  << "║ " << std::setw(11) << std::left << train.departureTime
                  << "║ " << std::setw(11) << std::left << train.arrivalTime
                  << "║ " << std::setw(7) << std::left << train.totalSeats
                  << "║ " << std::setw(11) << std::left << train.availableSeats
                  << "║" << std::endl;
    }

    std::cout << "╚════════╩══════════════════╩════════════╩════════════╩════════╩════════════╝" << std::endl;
}

void displayTrainDetails(const Train& train) {
    // FR-006, FR-007, FR-008, FR-009
    std::cout << std::endl;
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║             TRAIN DETAILS                    ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Train ID        : " << std::setw(25) << std::left << train.trainId << "║" << std::endl;
    std::cout << "║  Train Name      : " << std::setw(25) << std::left << train.trainName << "║" << std::endl;
    std::cout << "║  Source          : " << std::setw(25) << std::left << train.source << "║" << std::endl;
    std::cout << "║  Destination     : " << std::setw(25) << std::left << train.destination << "║" << std::endl;
    std::cout << "║  Departure       : " << std::setw(25) << std::left << train.departureTime << "║" << std::endl;
    std::cout << "║  Arrival         : " << std::setw(25) << std::left << train.arrivalTime << "║" << std::endl;
    std::cout << "║  Total Seats     : " << std::setw(25) << std::left << train.totalSeats << "║" << std::endl;
    std::cout << "║  Available Seats : " << std::setw(25) << std::left << train.availableSeats << "║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;
}

void searchTrainFlow(const std::vector<Train>& trains) {
    std::string source, destination;

    std::cout << std::endl;
    std::cout << "═══════ SEARCH TRAINS ═══════" << std::endl;

    // FR-001: Enter source station
    std::cout << "Enter source station: ";
    std::getline(std::cin, source);

    // FR-005: Reject incomplete input
    if (!isValidStation(source)) {
        std::cout << "[Error] Invalid source station name." << std::endl;
        return;
    }

    // FR-001: Enter destination station
    std::cout << "Enter destination station: ";
    std::getline(std::cin, destination);

    if (!isValidStation(destination)) {
        std::cout << "[Error] Invalid destination station name." << std::endl;
        return;
    }

    std::vector<Train> results = searchTrains(trains, source, destination);
    displayTrainList(results);
}

void checkSeatAvailabilityFlow(const std::vector<Train>& trains) {
    std::string trainId;

    std::cout << std::endl;
    std::cout << "═══════ CHECK SEAT AVAILABILITY ═══════" << std::endl;
    std::cout << "Enter Train ID (e.g., T001): ";
    std::getline(std::cin, trainId);

    for (const auto& train : trains) {
        if (train.trainId == trainId) {
            std::cout << std::endl;
            std::cout << "Train: " << train.trainName << " (" << train.trainId << ")" << std::endl;
            std::cout << "Available Seats: " << train.availableSeats << " / " << train.totalSeats << std::endl;
            return;
        }
    }

    std::cout << "[Error] Train not found. Please check the Train ID." << std::endl;
}
