#ifndef MODELS_H
#define MODELS_H
#include <string>
const int MAX_TRAINS = 50;
const int MAX_RESERVATIONS = 200;
const int MAX_PASSENGERS = 200;
enum ReservationStatus {
    STATUS_CONFIRMED = 1,
    STATUS_CANCELLED = 2
};
struct Train {
    std::string trainId;        
    std::string trainName;      
    std::string source;         
    std::string destination;    
    std::string departureTime;  
    std::string arrivalTime;    
    int totalSeats;             
    int availableSeats;         
};
struct Passenger {
    std::string name;           
    int age;                    
    std::string gender;         
};
struct Reservation {
    std::string reservationId;  
    std::string trainId;        
    std::string passengerName;  
    int passengerAge;           
    std::string passengerGender;
    std::string journeyDate;    
    int seatsBooked;            
    ReservationStatus status;   
};
enum CancelResult {
    CANCEL_SUCCESS = 0,
    CANCEL_NOT_FOUND = 1,
    CANCEL_ALREADY_CANCELLED = 2,
    CANCEL_USER_DECLINED = 3,
    CANCEL_INVALID_ID = 4,
    CANCEL_STORAGE_ERROR = 5
};
enum LookupResult {
    LOOKUP_SUCCESS = 0,
    LOOKUP_NOT_FOUND = 1,
    LOOKUP_INVALID_ID = 2
};
#endif 