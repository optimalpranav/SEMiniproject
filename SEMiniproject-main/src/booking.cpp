#include "booking.h"
#include "validation.h"
#include "data_store.h"
#include "train_search.h"
#include <iostream>
#include <iomanip>

// ============================================================================
// Ticket Booking Implementation
// Maps to: FR-010 through FR-018
// ============================================================================

bool bookTicketFlow(std::vector<Train>& trains,
                     std::vector<Reservation>& reservations) {

    std::string trainId, passengerName, genderStr, dateStr, seatsStr;
    int age = 0, numSeats = 0;

    std::cout << std::endl;
    std::cout << "═══════ BOOK TICKET ═══════" << std::endl;

    // FR-010: Allow user to select a train
    std::cout << "Enter Train ID to book (e.g., T001): ";
    std::getline(std::cin, trainId);

    // Find the train
    Train* train = findTrainById(trains, trainId);
    if (train == nullptr) {
        std::cout << "[Error] Train not found. Please check the Train ID." << std::endl;
        return false;
    }

    // Display train details for confirmation
    displayTrainDetails(*train);

    // FR-013: Check seat availability
    std::cout << std::endl;
    std::cout << "Enter number of seats to book: ";
    std::getline(std::cin, seatsStr);
    try {
        numSeats = std::stoi(seatsStr);
    } catch (...) {
        std::cout << "[Error] Invalid number of seats." << std::endl;
        return false;
    }

    if (numSeats <= 0) {
        std::cout << "[Error] Number of seats must be at least 1." << std::endl;
        return false;
    }

    // FR-014: Reject booking when seat unavailable
    if (train->availableSeats < numSeats) {
        std::cout << "[Error] Not enough seats available. Available: "
                  << train->availableSeats << std::endl;
        return false;
    }

    // FR-011: Collect passenger details
    std::cout << "Enter passenger name: ";
    std::getline(std::cin, passengerName);

    // FR-012, FR-037: Validate passenger information
    if (!isValidPassengerName(passengerName)) {
        std::cout << "[Error] Invalid passenger name. Use alphabetic characters only." << std::endl;
        return false;
    }

    std::cout << "Enter passenger age: ";
    std::string ageStr;
    std::getline(std::cin, ageStr);
    try {
        age = std::stoi(ageStr);
    } catch (...) {
        std::cout << "[Error] Invalid age." << std::endl;
        return false;
    }

    if (!isValidAge(age)) {
        std::cout << "[Error] Age must be between 1 and 120." << std::endl;
        return false;
    }

    std::cout << "Enter gender (M/F/Other): ";
    std::getline(std::cin, genderStr);

    if (!isValidGender(genderStr)) {
        std::cout << "[Error] Invalid gender. Enter M, F, or Other." << std::endl;
        return false;
    }

    std::cout << "Enter journey date (DD-MM-YYYY): ";
    std::getline(std::cin, dateStr);

    if (!isValidDate(dateStr)) {
        std::cout << "[Error] Invalid date format. Use DD-MM-YYYY." << std::endl;
        return false;
    }

    // Create the reservation
    Reservation newRes;
    newRes.reservationId = generateReservationId(reservations);
    newRes.trainId = trainId;
    newRes.passengerName = passengerName;
    newRes.passengerAge = age;
    newRes.passengerGender = genderStr;
    newRes.journeyDate = dateStr;
    newRes.seatsBooked = numSeats;
    newRes.status = STATUS_CONFIRMED;

    // Update seat availability
    train->availableSeats -= numSeats;

    // FR-016: Store reservation
    reservations.push_back(newRes);
    bool resSaved = saveReservations(reservations);
    bool trainsSaved = saveTrains(trains);

    if (!resSaved || !trainsSaved) {
        // FR-018: Error message if booking cannot be completed
        std::cout << "[Error] Booking could not be saved. Please try again." << std::endl;
        return false;
    }

    // FR-015, FR-017: Display confirmation with unique reservation reference
    std::cout << std::endl;
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║          BOOKING SUCCESSFUL!                 ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Reservation ID  : " << std::setw(25) << std::left << newRes.reservationId << "║" << std::endl;
    std::cout << "║  Train           : " << std::setw(25) << std::left << (train->trainId + " - " + train->trainName) << "║" << std::endl;
    std::cout << "║  Passenger       : " << std::setw(25) << std::left << passengerName << "║" << std::endl;
    std::cout << "║  Journey Date    : " << std::setw(25) << std::left << dateStr << "║" << std::endl;
    std::cout << "║  Seats Booked    : " << std::setw(25) << std::left << numSeats << "║" << std::endl;
    std::cout << "║  Status          : " << std::setw(25) << std::left << "Confirmed" << "║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    std::cout << "Please save your Reservation ID: " << newRes.reservationId << std::endl;

    return true;
}
