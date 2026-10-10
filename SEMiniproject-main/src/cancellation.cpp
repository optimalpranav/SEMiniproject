#include "cancellation.h"
#include "reservation.h"
#include "validation.h"
#include "data_store.h"
#include <iostream>
#include <string>

// ============================================================================
// Ticket Cancellation Implementation
// Maps to: FR-024 through FR-034, BR-004, BR-005, BR-006
// Security: SEC-001, SEC-002, SEC-004, SEC-005
// Sequence: SAD Section 4.2 - Sequence Diagram 2
// ============================================================================

bool updateSeatAvailability(std::vector<Train>& trains,
                             const std::string& trainId,
                             int seatsToRelease) {
    // FR-030: Update seat availability after successful cancellation
    // BR-006: Maintain consistency between reservation status and seats
    Train* train = findTrainById(trains, trainId);

    if (train == nullptr) {
        std::cerr << "[Warning] Train not found for seat update: cannot restore seats." << std::endl;
        return false;
    }

    // Restore the released seats
    train->availableSeats += seatsToRelease;

    // Safety check: available seats should never exceed total seats
    if (train->availableSeats > train->totalSeats) {
        train->availableSeats = train->totalSeats;
    }

    return true;
}

CancelResult processCancellation(const std::string& reservationId,
                                  std::vector<Reservation>& reservations,
                                  std::vector<Train>& trains) {

    // SEC-001: Validate reservation identifier before operations
    if (!isValidReservationId(reservationId)) {
        return CANCEL_INVALID_ID;
    }

    // FR-026: Verify that the specified reservation exists
    Reservation* reservation = findReservationById(reservations, reservationId);

    if (reservation == nullptr) {
        // FR-031: Reject cancellation when reservation cannot be identified
        // SEC-002: No cancellation when reservation cannot be identified
        return CANCEL_NOT_FOUND;
    }

    // FR-032: Prevent already cancelled reservation from being cancelled again
    // BR-005: Cancelled reservation not treated as active
    if (reservation->status == STATUS_CANCELLED) {
        return CANCEL_ALREADY_CANCELLED;
    }

    // FR-029: Update reservation status after successful cancellation
    // BR-004: Status updated accordingly
    reservation->status = STATUS_CANCELLED;

    // FR-030: Update seat availability after cancellation
    // BR-006, NFR-003, SEC-005: Maintain consistency
    bool seatsUpdated = updateSeatAvailability(trains, reservation->trainId, reservation->seatsBooked);

    if (!seatsUpdated) {
        // Log warning but don't roll back — the reservation is still cancelled
        std::cerr << "[Warning] Seats could not be restored for train " << reservation->trainId << std::endl;
    }

    // Persist updated reservations and trains to data store
    bool resSaved = saveReservations(reservations);
    bool trainsSaved = saveTrains(trains);

    if (!resSaved || !trainsSaved) {
        // Storage error — data may be inconsistent
        std::cerr << "[Error] Failed to persist cancellation data." << std::endl;
        return CANCEL_STORAGE_ERROR;
    }

    return CANCEL_SUCCESS;
}

CancelResult cancelReservationFlow(std::vector<Reservation>& reservations,
                                    std::vector<Train>& trains) {

    std::string reservationId;

    // FR-024: Allow user to request cancellation
    std::cout << std::endl;
    std::cout << "═══════ CANCEL RESERVATION ═══════" << std::endl;

    // FR-025: Require reservation reference before processing
    std::cout << "Enter Reservation ID to cancel (e.g., RES001): ";
    std::getline(std::cin, reservationId);

    // Trim input
    size_t start = reservationId.find_first_not_of(" \t\r\n");
    size_t end = reservationId.find_last_not_of(" \t\r\n");
    if (start != std::string::npos) {
        reservationId = reservationId.substr(start, end - start + 1);
    } else {
        reservationId = "";
    }

    // Validate the reservation ID format
    if (!isValidReservationId(reservationId)) {
        // FR-038: Meaningful error message
        std::cout << std::endl;
        std::cout << "[Error] Invalid reservation ID format." << std::endl;
        std::cout << "        Reservation IDs follow the format: RESxxx (e.g., RES001)" << std::endl;
        return CANCEL_INVALID_ID;
    }

    // FR-026: Verify reservation exists
    Reservation* reservation = findReservationById(reservations, reservationId);

    if (reservation == nullptr) {
        // FR-031, FR-034: Reject and display error
        std::cout << std::endl;
        std::cout << "[Error] Reservation not found." << std::endl;
        std::cout << "        Please verify your reservation ID and try again." << std::endl;
        return CANCEL_NOT_FOUND;
    }

    // FR-032: Check if already cancelled
    if (reservation->status == STATUS_CANCELLED) {
        std::cout << std::endl;
        std::cout << "[Error] This reservation has already been cancelled." << std::endl;
        return CANCEL_ALREADY_CANCELLED;
    }

    // FR-027: Display reservation details before final cancellation confirmation
    displayReservation(*reservation, trains);

    // FR-028: Require user confirmation before completing cancellation
    std::cout << std::endl;
    std::string confirm;
    std::cout << "Are you sure you want to cancel this reservation? (Y/N): ";
    std::getline(std::cin, confirm);

    // Trim and normalize confirmation input
    start = confirm.find_first_not_of(" \t\r\n");
    end = confirm.find_last_not_of(" \t\r\n");
    if (start != std::string::npos) {
        confirm = confirm.substr(start, end - start + 1);
    } else {
        confirm = "";
    }

    if (confirm != "Y" && confirm != "y" && confirm != "yes" && confirm != "Yes" && confirm != "YES") {
        std::cout << std::endl;
        std::cout << "Cancellation aborted. Your reservation remains active." << std::endl;
        return CANCEL_USER_DECLINED;
    }

    // Perform the actual cancellation
    // FR-029: Update status
    reservation->status = STATUS_CANCELLED;

    // FR-030: Update seat availability
    bool seatsUpdated = updateSeatAvailability(trains, reservation->trainId, reservation->seatsBooked);

    // Persist changes
    bool resSaved = saveReservations(reservations);
    bool trainsSaved = saveTrains(trains);

    if (!resSaved || !trainsSaved) {
        std::cout << std::endl;
        std::cout << "[Error] Cancellation could not be saved. Please contact support." << std::endl;
        return CANCEL_STORAGE_ERROR;
    }

    // FR-033: Display cancellation confirmation
    std::cout << std::endl;
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║        CANCELLATION SUCCESSFUL               ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Reservation " << reservation->reservationId
              << " has been cancelled.       ║" << std::endl;
    if (seatsUpdated) {
        std::cout << "║  " << reservation->seatsBooked
                  << " seat(s) restored to train " << reservation->trainId
                  << ".         ║" << std::endl;
    }
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;

    return CANCEL_SUCCESS;
}
