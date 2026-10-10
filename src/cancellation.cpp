#include "cancellation.h"

#include "reservation.h"

#include "validation.h"

#include "data_store.h"

#include <iostream>

#include <string>

bool updateSeatAvailability(std::vector<Train>& trains,

                             const std::string& trainId,

                             int seatsToRelease) {

    Train* train = findTrainById(trains, trainId);

    if (train == nullptr) {

        std::cerr << "[Warning] Train not found for seat update: cannot restore seats." << std::endl;

        return false;

    }

    train->availableSeats += seatsToRelease;

    if (train->availableSeats > train->totalSeats) {

        train->availableSeats = train->totalSeats;

    }

    return true;

}

CancelResult processCancellation(const std::string& reservationId,

                                  std::vector<Reservation>& reservations,

                                  std::vector<Train>& trains) {

    if (!isValidReservationId(reservationId)) {

        return CANCEL_INVALID_ID;

    }

    Reservation* reservation = findReservationById(reservations, reservationId);

    if (reservation == nullptr) {

        return CANCEL_NOT_FOUND;

    }

    if (reservation->status == STATUS_CANCELLED) {

        return CANCEL_ALREADY_CANCELLED;

    }

    reservation->status = STATUS_CANCELLED;

    bool seatsUpdated = updateSeatAvailability(trains, reservation->trainId, reservation->seatsBooked);

    if (!seatsUpdated) {

        std::cerr << "[Warning] Seats could not be restored for train " << reservation->trainId << std::endl;

    }

    bool resSaved = saveReservations(reservations);

    bool trainsSaved = saveTrains(trains);

    if (!resSaved || !trainsSaved) {

        std::cerr << "[Error] Failed to persist cancellation data." << std::endl;

        return CANCEL_STORAGE_ERROR;

    }

    return CANCEL_SUCCESS;

}

CancelResult cancelReservationFlow(std::vector<Reservation>& reservations,

                                    std::vector<Train>& trains) {

    std::string reservationId;

    std::cout << std::endl;

    std::cout << "═══════ CANCEL RESERVATION ═══════" << std::endl;

    std::cout << "Enter Reservation ID to cancel (e.g., RES001): ";

    std::getline(std::cin, reservationId);

    size_t start = reservationId.find_first_not_of(" \t\r\n");

    size_t end = reservationId.find_last_not_of(" \t\r\n");

    if (start != std::string::npos) {

        reservationId = reservationId.substr(start, end - start + 1);

    } else {

        reservationId = "";

    }

    if (!isValidReservationId(reservationId)) {

        std::cout << std::endl;

        std::cout << "[Error] Invalid reservation ID format." << std::endl;

        std::cout << "        Reservation IDs follow the format: RESxxx (e.g., RES001)" << std::endl;

        return CANCEL_INVALID_ID;

    }

    Reservation* reservation = findReservationById(reservations, reservationId);

    if (reservation == nullptr) {

        std::cout << std::endl;

        std::cout << "[Error] Reservation not found." << std::endl;

        std::cout << "        Please verify your reservation ID and try again." << std::endl;

        return CANCEL_NOT_FOUND;

    }

    if (reservation->status == STATUS_CANCELLED) {

        std::cout << std::endl;

        std::cout << "[Error] This reservation has already been cancelled." << std::endl;

        return CANCEL_ALREADY_CANCELLED;

    }

    displayReservation(*reservation, trains);

    std::cout << std::endl;

    std::string confirm;

    std::cout << "Are you sure you want to cancel this reservation? (Y/N): ";

    std::getline(std::cin, confirm);

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

    reservation->status = STATUS_CANCELLED;

    bool seatsUpdated = updateSeatAvailability(trains, reservation->trainId, reservation->seatsBooked);

    bool resSaved = saveReservations(reservations);

    bool trainsSaved = saveTrains(trains);

    if (!resSaved || !trainsSaved) {

        std::cout << std::endl;

        std::cout << "[Error] Cancellation could not be saved. Please contact support." << std::endl;

        return CANCEL_STORAGE_ERROR;

    }

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