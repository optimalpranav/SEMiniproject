#include "reservation.h"
#include "validation.h"
#include "data_store.h"
#include <iostream>
#include <iomanip>
#include <string>

// ============================================================================
// Reservation Lookup Implementation
// Maps to: FR-019, FR-020, FR-021, FR-022, FR-023
// Security: SEC-001, SEC-003
// ============================================================================

std::string getStatusString(ReservationStatus status) {
    switch (status) {
        case STATUS_CONFIRMED: return "Confirmed";
        case STATUS_CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

LookupResult lookupReservation(const std::string& reservationId,
                                std::vector<Reservation>& reservations,
                                Reservation& outReservation) {

    // SEC-001: Validate reservation identifier before operations
    if (!isValidReservationId(reservationId)) {
        return LOOKUP_INVALID_ID;
    }

    // FR-020: Search for reservation using the supplied reference
    Reservation* found = findReservationById(reservations, reservationId);

    if (found == nullptr) {
        // FR-022: Reservation not found
        return LOOKUP_NOT_FOUND;
    }

    // FR-021: Populate output with reservation details
    outReservation = *found;
    return LOOKUP_SUCCESS;
}

void displayReservation(const Reservation& reservation,
                        const std::vector<Train>& trains) {
    // FR-021: Display reservation details
    std::cout << std::endl;
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║          RESERVATION DETAILS                 ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Reservation ID  : " << std::setw(25) << std::left << reservation.reservationId << "║" << std::endl;

    // Resolve train name from train ID
    std::string trainDisplay = reservation.trainId;
    for (const auto& train : trains) {
        if (train.trainId == reservation.trainId) {
            trainDisplay = train.trainId + " - " + train.trainName;
            std::cout << "║  Train           : " << std::setw(25) << std::left << trainDisplay << "║" << std::endl;
            std::cout << "║  Route           : " << std::setw(25) << std::left
                      << (train.source + " → " + train.destination) << "║" << std::endl;
            std::cout << "║  Departure       : " << std::setw(25) << std::left << train.departureTime << "║" << std::endl;
            std::cout << "║  Arrival         : " << std::setw(25) << std::left << train.arrivalTime << "║" << std::endl;
            break;
        }
    }

    std::cout << "║  Passenger Name  : " << std::setw(25) << std::left << reservation.passengerName << "║" << std::endl;
    std::cout << "║  Age             : " << std::setw(25) << std::left << reservation.passengerAge << "║" << std::endl;
    std::cout << "║  Gender          : " << std::setw(25) << std::left << reservation.passengerGender << "║" << std::endl;
    std::cout << "║  Journey Date    : " << std::setw(25) << std::left << reservation.journeyDate << "║" << std::endl;
    std::cout << "║  Seats Booked    : " << std::setw(25) << std::left << reservation.seatsBooked << "║" << std::endl;

    // FR-023: Display current reservation status
    std::string statusStr = getStatusString(reservation.status);
    std::cout << "║  Status          : " << std::setw(25) << std::left << statusStr << "║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;
}

bool viewReservationFlow(std::vector<Reservation>& reservations,
                         const std::vector<Train>& trains) {
    std::string reservationId;

    // FR-019: Allow user to enter the reservation reference
    std::cout << std::endl;
    std::cout << "═══════ VIEW RESERVATION ═══════" << std::endl;
    std::cout << "Enter Reservation ID (e.g., RES001): ";
    std::getline(std::cin, reservationId);

    // Trim input
    size_t start = reservationId.find_first_not_of(" \t\r\n");
    size_t end = reservationId.find_last_not_of(" \t\r\n");
    if (start != std::string::npos) {
        reservationId = reservationId.substr(start, end - start + 1);
    } else {
        reservationId = "";
    }

    // Perform lookup
    Reservation result;
    LookupResult lookupResult = lookupReservation(reservationId, reservations, result);

    switch (lookupResult) {
        case LOOKUP_SUCCESS:
            displayReservation(result, trains);
            return true;

        case LOOKUP_INVALID_ID:
            // FR-038: Meaningful error message for invalid input
            // SEC-003: Don't expose sensitive data in error messages
            std::cout << std::endl;
            std::cout << "[Error] Invalid reservation ID format." << std::endl;
            std::cout << "        Reservation IDs follow the format: RESxxx (e.g., RES001)" << std::endl;
            return false;

        case LOOKUP_NOT_FOUND:
            // FR-022: Appropriate error message when reservation not found
            std::cout << std::endl;
            std::cout << "[Error] Reservation not found." << std::endl;
            std::cout << "        Please verify your reservation ID and try again." << std::endl;
            return false;

        default:
            std::cout << std::endl;
            std::cout << "[Error] An unexpected error occurred. Please try again." << std::endl;
            return false;
    }
}
