#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>

// ============================================================================
// Input Validation Module
// Validates user inputs before processing operations.
// Maps to: FR-035, FR-036, FR-037, FR-038, FR-039
// Security: SEC-001, SEC-003, SEC-004
// Architecture: Validation/Domain Logic Layer (SAD Section 3.5)
// ============================================================================

// Validate reservation ID format
// - Must not be empty
// - Must start with "RES" followed by digits
// - Must be within reasonable length
// Returns: true if valid, false otherwise
bool isValidReservationId(const std::string& id);

// Validate passenger name
// - Must not be empty
// - Must contain only alphabetic characters and spaces
// Returns: true if valid, false otherwise
bool isValidPassengerName(const std::string& name);

// Validate passenger age
// - Must be between 1 and 120
// Returns: true if valid, false otherwise
bool isValidAge(int age);

// Validate journey date format (DD-MM-YYYY)
// Returns: true if format is valid, false otherwise
bool isValidDate(const std::string& date);

// Validate gender input
// Returns: true if valid (M, F, or Other), false otherwise
bool isValidGender(const std::string& gender);

// Validate station name
// - Must not be empty
// - Must contain only alphabetic characters and spaces
// Returns: true if valid, false otherwise
bool isValidStation(const std::string& station);

// Get a safe, sanitized version of a string for display in error messages
// Prevents SEC-003: no sensitive data in error messages
std::string sanitizeForDisplay(const std::string& input, int maxLength = 20);

#endif // VALIDATION_H
