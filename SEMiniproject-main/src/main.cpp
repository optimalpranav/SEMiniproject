// ============================================================================
// Railway Reservation System - Main Entry Point
// Team 12 | PES University | Software Engineering Mini-Project
//
// Menu-driven application as specified in SRS Section 3.1.1
// Architecture: Presentation Layer (SAD Section 3.5)
// ============================================================================

#include "models.h"
#include "data_store.h"
#include "train_search.h"
#include "booking.h"
#include "reservation.h"
#include "cancellation.h"
#include <iostream>
#include <string>
#include <limits>

// Display the main menu
// Maps to: SRS Section 3.1.1 - User Interface
void displayMenu() {
    std::cout << std::endl;
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║      RAILWAY RESERVATION SYSTEM              ║" << std::endl;
    std::cout << "║      Team 12 | PES University                ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════╣" << std::endl;
    std::cout << "║                                              ║" << std::endl;
    std::cout << "║   1. Search Train                            ║" << std::endl;
    std::cout << "║   2. View Train Details                      ║" << std::endl;
    std::cout << "║   3. Check Seat Availability                 ║" << std::endl;
    std::cout << "║   4. Book Ticket                             ║" << std::endl;
    std::cout << "║   5. View Reservation                        ║" << std::endl;
    std::cout << "║   6. Cancel Ticket                           ║" << std::endl;
    std::cout << "║   7. Exit                                    ║" << std::endl;
    std::cout << "║                                              ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter your choice (1-7): ";
}

int main() {
    // Load data from persistent storage
    std::vector<Train> trains = loadTrains();
    std::vector<Reservation> reservations = loadReservations();

    std::cout << std::endl;
    std::cout << "══════════════════════════════════════════════════" << std::endl;
    std::cout << "   Welcome to the Railway Reservation System!    " << std::endl;
    std::cout << "══════════════════════════════════════════════════" << std::endl;

    if (trains.empty()) {
        std::cout << std::endl;
        std::cout << "[Info] No train data loaded. Please ensure 'data/trains.dat' exists." << std::endl;
    } else {
        std::cout << "[Info] Loaded " << trains.size() << " train(s) and "
                  << reservations.size() << " reservation(s)." << std::endl;
    }

    // Main application loop
    // FR-039: Allow user to correct input without terminating the application
    bool running = true;
    while (running) {
        displayMenu();

        std::string choiceStr;
        std::getline(std::cin, choiceStr);

        // Handle EOF / input failure
        if (std::cin.eof() || std::cin.fail()) {
            std::cout << std::endl << "Input stream closed. Exiting." << std::endl;
            break;
        }

        // Parse choice
        int choice = 0;
        try {
            choice = std::stoi(choiceStr);
        } catch (...) {
            // FR-038: Meaningful error for invalid input
            std::cout << std::endl;
            std::cout << "[Error] Invalid choice. Please enter a number between 1 and 7." << std::endl;
            continue;
        }

        switch (choice) {
            case 1:
                // Search Train (FR-001 to FR-005)
                searchTrainFlow(trains);
                break;

            case 2: {
                // View Train Details (FR-006 to FR-009)
                std::string trainId;
                std::cout << std::endl;
                std::cout << "Enter Train ID to view details (e.g., T001): ";
                std::getline(std::cin, trainId);
                Train* train = findTrainById(trains, trainId);
                if (train) {
                    displayTrainDetails(*train);
                } else {
                    std::cout << "[Error] Train not found." << std::endl;
                }
                break;
            }

            case 3:
                // Check Seat Availability (FR-009, FR-013)
                checkSeatAvailabilityFlow(trains);
                break;

            case 4:
                // Book Ticket (FR-010 to FR-018)
                bookTicketFlow(trains, reservations);
                break;

            case 5:
                // View Reservation (FR-019 to FR-023)
                viewReservationFlow(reservations, trains);
                break;

            case 6:
                // Cancel Ticket (FR-024 to FR-034)
                cancelReservationFlow(reservations, trains);
                break;

            case 7:
                // Exit
                std::cout << std::endl;
                std::cout << "Thank you for using the Railway Reservation System!" << std::endl;
                std::cout << "Goodbye." << std::endl;
                running = false;
                break;

            default:
                std::cout << std::endl;
                std::cout << "[Error] Invalid choice. Please enter a number between 1 and 7." << std::endl;
                break;
        }
    }

    return 0;
}
