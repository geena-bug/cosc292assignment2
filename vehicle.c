/*
 * Vehicle Management System
 * Implementation by: Anyaso and Obey
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vehicle.h"

// To make the code compatible for both microsoft and mac users, Add compatibility for non-Microsoft compilers
#ifndef _MSC_VER
    #define scanf_s scanf
#endif

// Constants for bit operations
#define VALUE_MASK 0xFFFFE000  // Mask for the upper 21 bits (vehicle value)
#define YEAR_MASK 0x000007FF   // Mask for the lower 11 bits (model year)
#define VALUE_SHIFT 11         // Number of bits to shift for value
#define MAX_DESCRIPTION 100    // Maximum buffer size for description input

/*
 * Function to: createVehicle
 * Purpose: This meant to dynamically allocate a string to store a vehicle's information.
 *          Value and model year are stored in the first 4 bytes.
 *          The remaining memory is for the description of the vehicle.
 * Returns: a dynamically allocated block of memory containing the vehicle's information
 * By: Anyaso
 */
char* createVehicle() {
    char descriptionBuffer[MAX_DESCRIPTION];
    unsigned int value, year;
    unsigned int packedData;
    char* vehicle;
    int descriptionLength;

    // Get vehicle value from user
    printf("Input vehicle value (up to $2,097,151): $");
    scanf_s("%u", &value);

    // Validate vehicle value
    if (value > 2097151) { // 2^21 - 1
        printf("Error: Vehicle value exceeds required maximum value ($2,097,151)\n");
        // Clear input buffer
        while (getchar() != '\n');
        return NULL;
    }

    // Accept vehicle year from user
    printf("Input vehicle model year (up to 2047): ");
    scanf_s("%u", &year);

    // Validate model year
    if (year > 2047) { // 2^11 - 1
        printf("Error: Model year exceeds required maximum model (2047)\n");
        // Clear input buffer
        while (getchar() != '\n');
        return NULL;
    }

    // Clear the input buffer for the description
    while (getchar() != '\n');

    // Accept vehicle description from user
    printf("Input vehicle description: ");
    fgets(descriptionBuffer, MAX_DESCRIPTION, stdin);

    // Remove trailing newline if it exists
    descriptionLength = strlen(descriptionBuffer);
    if (descriptionBuffer[descriptionLength - 1] == '\n') {
        descriptionBuffer[descriptionLength - 1] = '\0';
        descriptionLength--;
    }

    // Allocate memory for the vehicle
    // 4 bytes for value/year + length of description + 1 for null terminator
    vehicle = (char*)malloc(4 + descriptionLength + 1);

    if (vehicle == NULL) {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }

    // Pack value and year into one integer using bit operations
    packedData = (value << VALUE_SHIFT) | year;

    // Save the packed data in the first 4 bytes
    vehicle[0] = (packedData >> 24) & 0xFF;
    vehicle[1] = (packedData >> 16) & 0xFF;
    vehicle[2] = (packedData >> 8) & 0xFF;
    vehicle[3] = packedData & 0xFF;

    // Copy the description to the rest of the allocated memory, this will be done manually
    for (int i = 0; i <= descriptionLength; i++) {
        vehicle[4 + i] = descriptionBuffer[i];
    }

    return vehicle;
}

/*
 * Function to: displayGarage
 * Purpose: This will display all the information stored for each vehicle in the garage.
 *          Calls displayVehicle for each vehicle in the garage.
 * Parameters: char** - pointer to the garage
 *             int - number of vehicles in the garage
 * By: Anyaso
 */
void displayGarage(char** garage, int numVehicles) {
    if (garage == NULL) {
        printf("Error: Garage pointer is NULL\n");
        return;
    }

    printf("\n--- Garage Contents (%d vehicles) ---\n", numVehicles);
    for (int i = 0; i < numVehicles; i++) {
        printf("Vehicle %d: ", i + 1);
        if (garage[i] == NULL) {
            printf("Empty\n");
        } else {
            displayVehicle(garage[i]);
        }
    }
    printf("--- End of Garage ---\n");
}

/*
 * Stub implementations of Obey's functions for testing purposes only.
 * These will be replaced with the actual implementations from your partner.
 */

// Stub for displayVehicle
void displayVehicle(char* vehicle) {
    if (vehicle == NULL) {
        printf("Error: Vehicle pointer is NULL\n");
        return;
    }

    // Extract the packed data from the first 4 bytes
    unsigned int packedData =
        ((unsigned int)(unsigned char)vehicle[0] << 24) |
        ((unsigned int)(unsigned char)vehicle[1] << 16) |
        ((unsigned int)(unsigned char)vehicle[2] << 8) |
        ((unsigned int)(unsigned char)vehicle[3]);

    // Extract the value (upper 21 bits)
    unsigned int value = (packedData & VALUE_MASK) >> VALUE_SHIFT;

    // Extract the year (lower 11 bits)
    unsigned int year = packedData & YEAR_MASK;

    // The description starts at the 5th byte
    char* description = vehicle + 4;

    printf("Vehicle: %s, Year: %u, Value: $%u\n", description, year, value);
}

// Stub for createGarage
char** createGarage(int numVehicles) {
    printf("Stub implementation: createGarage(%d)\n", numVehicles);

    if (numVehicles <= 0) {
        printf("Error: Number of vehicles must be positive\n");
        return NULL;
    }

    // Allocate an array of char pointers (garage)
    char** garage = (char**)malloc(numVehicles * sizeof(char*));

    if (garage == NULL) {
        printf("Error: Memory allocation for garage failed\n");
        return NULL;
    }

    // Create each vehicle and store in the garage
    printf("\n--- Creating a garage with %d vehicles ---\n", numVehicles);
    for (int i = 0; i < numVehicles; i++) {
        printf("\nEnter details for vehicle %d:\n", i + 1);
        garage[i] = createVehicle();

        // If vehicle creation failed, try again
        while (garage[i] == NULL) {
            printf("\nPlease try again for vehicle %d:\n", i + 1);
            garage[i] = createVehicle();
        }
    }

    return garage;
}

// Stub for removeVehicle
char** removeVehicle(char** garage, int numVehicles, int index) {
    printf("Stub implementation: removeVehicle(%p, %d, %d)\n",
           (void*)garage, numVehicles, index);

    if (garage == NULL) {
        printf("Error: Garage pointer is NULL\n");
        return NULL;
    }

    if (index < 0 || index >= numVehicles) {
        printf("Error: Vehicle index %d is out of bounds\n", index);
        return garage; // Return the original garage unchanged
    }

    // Create a new garage with one less space
    char** newGarage = (char**)malloc((numVehicles - 1) * sizeof(char*));

    if (newGarage == NULL) {
        printf("Error: Memory allocation for new garage failed\n");
        return garage; // Return the original garage unchanged
    }

    // Copy all vehicles except the one at the specified index
    int newIndex = 0;
    for (int i = 0; i < numVehicles; i++) {
        if (i == index) {
            // Free the vehicle being removed
            free(garage[i]);
        } else {
            // Copy the vehicle pointer to the new garage
            newGarage[newIndex++] = garage[i];
        }
    }

    // Free the old garage array (but not the vehicles within it, except the removed one)
    free(garage);

    printf("Vehicle at position %d has been removed.\n", index + 1);

    return newGarage;
}