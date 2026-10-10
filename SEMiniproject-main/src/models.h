#ifndef MODELS_H
#define MODELS_H

#include <string>

// Maximum limits for data storage
const int MAX_TRAINS = 50;
const int MAX_RESERVATIONS = 200;
const int MAX_PASSENGERS = 200;

// Reservation status enumeration
enum ReservationStatus {
    STATUS_CONFIRMED = 1,
    STATUS_CANCELLED = 2
};

// Train information structure
// Maps to: FR-006, FR-007, FR-008, FR-009
struct Train {
    std::string trainId;        // Unique train identifier (e.g., "T001")
    std::string trainName;      // Name of the train
    std::string source;         // Source station
    std::string destination;    // Destination station
    std::string departureTime;  // Departure time (HH:MM format)
    std::string arrivalTime;    // Arrival time (HH:MM format)
    int totalSeats;             // Total seat capacity
    int availableSeats;         // Currently available seats
};

// Passenger information structure
// Maps to: FR-011, FR-012
struct Passenger {
    std::string name;           // Passenger full name
    int age;                    // Passenger age
    std::string gender;         // Gender (M/F/Other)
};

// Reservation record structure
// Maps to: FR-015, FR-016, FR-019-FR-023, BR-001, BR-002
struct Reservation {
    std::string reservationId;  // Unique reservation reference (e.g., "RES001")
    std::string trainId;        // Associated train ID
    std::string passengerName;  // Passenger name
    int passengerAge;           // Passenger age
    std::string passengerGender;// Passenger gender
    std::string journeyDate;    // Date of journey (DD-MM-YYYY)
    int seatsBooked;            // Number of seats booked
    ReservationStatus status;   // Current reservation status
};

// Result codes for cancellation operations
// Maps to: FR-031, FR-032, FR-034
enum CancelResult {
    CANCEL_SUCCESS = 0,
    CANCEL_NOT_FOUND = 1,
    CANCEL_ALREADY_CANCELLED = 2,
    CANCEL_USER_DECLINED = 3,
    CANCEL_INVALID_ID = 4,
    CANCEL_STORAGE_ERROR = 5
};

// Result codes for lookup operations
enum LookupResult {
    LOOKUP_SUCCESS = 0,
    LOOKUP_NOT_FOUND = 1,
    LOOKUP_INVALID_ID = 2
};

#endif // MODELS_H
