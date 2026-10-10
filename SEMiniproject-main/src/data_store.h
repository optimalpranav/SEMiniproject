#ifndef DATA_STORE_H
#define DATA_STORE_H

#include "models.h"
#include <string>
#include <vector>

// ============================================================================
// Data Store Module
// Handles persistent storage of train and reservation data using text files.
// Maps to: SRS Section 2.1, 3.1.2
// Architecture: Data Management Layer (SAD Section 3.5)
// ============================================================================

// --- Train Data Operations ---

// Load all trains from the data file
// Returns: vector of Train structs
std::vector<Train> loadTrains(const std::string& filepath = "data/trains.dat");

// Save all trains to the data file
// Returns: true on success, false on I/O error
bool saveTrains(const std::vector<Train>& trains, const std::string& filepath = "data/trains.dat");

// Find a train by its ID
// Returns: pointer to Train if found, nullptr otherwise
Train* findTrainById(std::vector<Train>& trains, const std::string& trainId);

// --- Reservation Data Operations ---

// Load all reservations from the data file
// Returns: vector of Reservation structs
std::vector<Reservation> loadReservations(const std::string& filepath = "data/reservations.dat");

// Save all reservations to the data file
// Returns: true on success, false on I/O error
bool saveReservations(const std::vector<Reservation>& reservations, const std::string& filepath = "data/reservations.dat");

// Find a reservation by its ID
// Returns: pointer to Reservation if found, nullptr otherwise
Reservation* findReservationById(std::vector<Reservation>& reservations, const std::string& reservationId);

// Generate the next unique reservation ID
// Returns: string like "RES001", "RES002", etc.
std::string generateReservationId(const std::vector<Reservation>& reservations);

#endif // DATA_STORE_H
