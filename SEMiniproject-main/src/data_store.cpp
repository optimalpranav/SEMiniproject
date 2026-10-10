#include "data_store.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

// ============================================================================
// Data Store Implementation
// Uses pipe-delimited text files for simple, portable persistence.
// Architecture: Data Management Layer (SAD Section 3.5)
// ============================================================================

// --- Helper: Trim whitespace from both ends of a string ---
static std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return str.substr(start, end - start + 1);
}

// ============================================================================
// Train Data Operations
// ============================================================================

// File format: trainId|trainName|source|destination|departureTime|arrivalTime|totalSeats|availableSeats
std::vector<Train> loadTrains(const std::string& filepath) {
    std::vector<Train> trains;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "[DataStore] Warning: Could not open train data file: " << filepath << std::endl;
        return trains;
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments

        std::istringstream iss(line);
        Train train;
        std::string totalSeatsStr, availableSeatsStr;

        if (std::getline(iss, train.trainId, '|') &&
            std::getline(iss, train.trainName, '|') &&
            std::getline(iss, train.source, '|') &&
            std::getline(iss, train.destination, '|') &&
            std::getline(iss, train.departureTime, '|') &&
            std::getline(iss, train.arrivalTime, '|') &&
            std::getline(iss, totalSeatsStr, '|') &&
            std::getline(iss, availableSeatsStr)) {

            train.trainId = trim(train.trainId);
            train.trainName = trim(train.trainName);
            train.source = trim(train.source);
            train.destination = trim(train.destination);
            train.departureTime = trim(train.departureTime);
            train.arrivalTime = trim(train.arrivalTime);
            train.totalSeats = std::stoi(trim(totalSeatsStr));
            train.availableSeats = std::stoi(trim(availableSeatsStr));

            trains.push_back(train);
        }
    }

    file.close();
    return trains;
}

bool saveTrains(const std::vector<Train>& trains, const std::string& filepath) {
    std::ofstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "[DataStore] Error: Could not write to train data file: " << filepath << std::endl;
        return false;
    }

    file << "# Train Data: trainId|trainName|source|destination|departureTime|arrivalTime|totalSeats|availableSeats" << std::endl;

    for (const auto& train : trains) {
        file << train.trainId << "|"
             << train.trainName << "|"
             << train.source << "|"
             << train.destination << "|"
             << train.departureTime << "|"
             << train.arrivalTime << "|"
             << train.totalSeats << "|"
             << train.availableSeats << std::endl;
    }

    file.close();
    return true;
}

Train* findTrainById(std::vector<Train>& trains, const std::string& trainId) {
    for (auto& train : trains) {
        if (train.trainId == trainId) {
            return &train;
        }
    }
    return nullptr;
}

// ============================================================================
// Reservation Data Operations
// ============================================================================

// File format: reservationId|trainId|passengerName|passengerAge|passengerGender|journeyDate|seatsBooked|status
std::vector<Reservation> loadReservations(const std::string& filepath) {
    std::vector<Reservation> reservations;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        // Not an error on first run — file may not exist yet
        return reservations;
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        Reservation res;
        std::string ageStr, seatsStr, statusStr;

        if (std::getline(iss, res.reservationId, '|') &&
            std::getline(iss, res.trainId, '|') &&
            std::getline(iss, res.passengerName, '|') &&
            std::getline(iss, ageStr, '|') &&
            std::getline(iss, res.passengerGender, '|') &&
            std::getline(iss, res.journeyDate, '|') &&
            std::getline(iss, seatsStr, '|') &&
            std::getline(iss, statusStr)) {

            res.reservationId = trim(res.reservationId);
            res.trainId = trim(res.trainId);
            res.passengerName = trim(res.passengerName);
            res.passengerAge = std::stoi(trim(ageStr));
            res.passengerGender = trim(res.passengerGender);
            res.journeyDate = trim(res.journeyDate);
            res.seatsBooked = std::stoi(trim(seatsStr));
            res.status = static_cast<ReservationStatus>(std::stoi(trim(statusStr)));

            reservations.push_back(res);
        }
    }

    file.close();
    return reservations;
}

bool saveReservations(const std::vector<Reservation>& reservations, const std::string& filepath) {
    std::ofstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "[DataStore] Error: Could not write to reservation data file: " << filepath << std::endl;
        return false;
    }

    file << "# Reservation Data: reservationId|trainId|passengerName|passengerAge|passengerGender|journeyDate|seatsBooked|status" << std::endl;

    for (const auto& res : reservations) {
        file << res.reservationId << "|"
             << res.trainId << "|"
             << res.passengerName << "|"
             << res.passengerAge << "|"
             << res.passengerGender << "|"
             << res.journeyDate << "|"
             << res.seatsBooked << "|"
             << static_cast<int>(res.status) << std::endl;
    }

    file.close();
    return true;
}

Reservation* findReservationById(std::vector<Reservation>& reservations, const std::string& reservationId) {
    for (auto& res : reservations) {
        if (res.reservationId == reservationId) {
            return &res;
        }
    }
    return nullptr;
}

std::string generateReservationId(const std::vector<Reservation>& reservations) {
    int maxId = 0;
    for (const auto& res : reservations) {
        // Extract numeric part from "RESxxx"
        if (res.reservationId.length() > 3 && res.reservationId.substr(0, 3) == "RES") {
            try {
                int num = std::stoi(res.reservationId.substr(3));
                if (num > maxId) maxId = num;
            } catch (...) {
                // Skip malformed IDs
            }
        }
    }

    std::ostringstream oss;
    oss << "RES" << std::setfill('0') << std::setw(3) << (maxId + 1);
    return oss.str();
}
