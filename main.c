#include <stdio.h>
#include <stdlib.h>

#ifndef _MSC_VER
    #define scanf_s scanf
#endif

#include "vehicle.h"

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * Function: mainMenu
 * Purpose: Display main menu for test so that user can select from the list of options the test to run
 */
int mainMenu() {
    int choice;

    printf("\nVehicle Management System\n");
    printf("-----------------------\n");

    printf("Select a test to run:\n");
    printf("1. Basic Garage Test (with user input)\n");
    printf("2. Bit Manipulation Test\n");
    printf("3. Boundary Values Test\n");
    printf("4. Error Handling Test\n");
    printf("5. Multiple Vehicles Test\n");
    printf("6. Run All Tests\n");
    printf("0. Exit Program\n");
    printf("Select an option (0-6): ");
    scanf_s("%d", &choice);

    // Clear input buffer
    clearInputBuffer();

    return choice;
}

/*
 * Function: testGarage
 * Purpose: Tests the basic functionality of the garage system
 */
void testGarage()
{
    //Number of vehicles to add to the garage
    int numVehicles;
    int removeIndex;
    char removeChoice;

    //Prompt the user to enter how many vehicles to store
    printf("Enter number of vehicles you would like to store: ");
    scanf_s("%d", &numVehicles);

    //Must clear the input stream from newline
    getc(stdin);

    //Create a new garage
    char ** garage = createGarage(numVehicles);

    //Display the contents of the garage
    displayGarage(garage, numVehicles);

    // Ask user if they want to remove a vehicle
    printf("\nWould you like to remove a vehicle? (yes/no): ");
    scanf_s("%c", &removeChoice, 1);
    clearInputBuffer();

    if (removeChoice == 'y' || removeChoice == 'Y') {
        printf("Enter the index of the vehicle to remove (1-%d): ", numVehicles);
        scanf_s("%d", &removeIndex);
        clearInputBuffer();

        if (removeIndex >= 1 && removeIndex <= numVehicles) {
            // Convert from 1-based to 0-based indexing
            garage = removeVehicle(garage, numVehicles--, removeIndex - 1);
            //Display the contents of the garage
            displayGarage(garage, numVehicles);
        } else {
            printf("Invalid vehicle index. No vehicle removed.\n");
        }
    }

    // Free all remaining vehicles and the garage
    for (int i = 0; i < numVehicles; i++) {
        free(garage[i]);
    }
    free(garage);

    printf("\nTest completed. All memory freed.\n");
    printf("Press Enter to continue...");
    getchar();
}

/*
 * Function to: test Bit Manipulation
 * Purpose: Tests bit manipulation operations for storing value and year
 */
void testBitManipulation() {
    printf("\n--- Bit Manipulation Test ---\n");

    unsigned int value = 5050;       // $5,050
    unsigned int year = 2003;        // 2003 model
    unsigned int packedData;

    // Put the value and year into one 32-bit integer
    packedData = (value << 11) | year;

    printf("Original value: $%u\n", value);
    printf("Original year: %u\n", year);
    printf("Packed data (hex): 0x%08X\n", packedData);

    // fetch value and year from the packed data
    unsigned int extractedValue = (packedData & 0xFFFFE000) >> 11;
    unsigned int extractedYear = packedData & 0x000007FF;

    printf("Extracted value: $%u\n", extractedValue);
    printf("Extracted year: %u\n", extractedYear);

    // show the bit pattern
    printf("Bit pattern: ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (packedData >> i) & 1);
        if (i == 11) printf(" | "); // Separator between value and year bits
    }
    printf("\n");

    printf("Bit manipulation test completed.\n");
    printf("Press Enter to continue...");
    getchar();
}

/*
 * Function to: test Boundary Values
 * Purpose: Tests boundary values for vehicle value and year
 */
void testBoundaryValues() {
    printf("\n--- Boundary Values Test ---\n");

    // Create a temporary garage with one vehicle
    char** garage = (char**)malloc(sizeof(char*));

    // Test maximum value
    printf("Testing maximum vehicle value...\n");
    unsigned int maxValue = 2097151; // 2^21 - 1
    unsigned int year = 2000;
    unsigned int packedData = (maxValue << 11) | year;

    // Manually create the vehicle
    char* vehicle = (char*)malloc(4 + 13); // 4 bytes + "Max Value Test" + null
    vehicle[0] = (packedData >> 24) & 0xFF;
    vehicle[1] = (packedData >> 16) & 0xFF;
    vehicle[2] = (packedData >> 8) & 0xFF;
    vehicle[3] = packedData & 0xFF;

    char* description = "Testing Maximum Value ";
    for (int i = 0; i <= 12; i++) {
        vehicle[4 + i] = description[i];
    }

    garage[0] = vehicle;

    // Display the vehicle
    displayVehicle(garage[0]);

    // Test maximum year
    printf("\nTesting maximum model year...\n");
    unsigned int value = 50000;
    unsigned int maxYear = 2047; // 2^11 - 1
    packedData = (value << 11) | maxYear;

    // Manually create the vehicle
    char* vehicle2 = (char*)malloc(4 + 12); // 4 bytes + "Max Year Test" + null
    vehicle2[0] = (packedData >> 24) & 0xFF;
    vehicle2[1] = (packedData >> 16) & 0xFF;
    vehicle2[2] = (packedData >> 8) & 0xFF;
    vehicle2[3] = packedData & 0xFF;

    description = "Max Year Test";
    for (int i = 0; i <= 11; i++) {
        vehicle2[4 + i] = description[i];
    }

    // Free the first vehicle and replace it
    free(garage[0]);
    garage[0] = vehicle2;

    // Display the vehicle
    displayVehicle(garage[0]);

    // Free memory
    free(garage[0]);
    free(garage);

    printf("Boundary values test completed.\n");
    printf("Press Enter to continue...");
    getchar();
}

/*
 * Function: testErrorHandling
 * Purpose: Tests error handling in vehicle management functions
 */
void testErrorHandling() {
    printf("\n--- Error Handling Test ---\n");

    // Test NULL vehicle pointer
    printf("Testing NULL vehicle pointer...\n");
    displayVehicle(NULL);

    // Test NULL garage pointer
    printf("\nTesting NULL garage pointer...\n");
    displayGarage(NULL, 5);

    // Test removing from NULL garage
    printf("\nTesting remove from NULL garage...\n");
    char** result = removeVehicle(NULL, 5, 2);
    if (result == NULL) {
        printf("Successfully returned NULL for NULL garage.\n");
    }

    // Test removing from out of bounds index
    printf("\nTesting remove with out of bounds index...\n");
    char** garage = (char**)malloc(sizeof(char*));

    // Create a vehicle with hardcoded values to avoid input
    unsigned int value = 10000;
    unsigned int year = 2020;
    unsigned int packedData = (value << 11) | year;

    char* vehicle = (char*)malloc(4 + 13); // 4 bytes + "Test Vehicle" + null
    vehicle[0] = (packedData >> 24) & 0xFF;
    vehicle[1] = (packedData >> 16) & 0xFF;
    vehicle[2] = (packedData >> 8) & 0xFF;
    vehicle[3] = packedData & 0xFF;

    char* description = "Test Vehicle";
    for (int i = 0; i <= 12; i++) {
        vehicle[4 + i] = description[i];
    }

    garage[0] = vehicle;

    printf("\nTrying to remove from index 5 (out of bounds)...\n");
    result = removeVehicle(garage, 1, 5);
    if (result == garage) {
        printf("Successfully returned previous garage for out of bounds index.\n");
    }

    // Free memory
    free(garage[0]);
    free(garage);

    printf("Error handling test completed.\n");
    printf("Press Enter to continue...");
    getchar();
}

/*
 * Function: testMultipleVehicles
 * Purpose: Tests garage operations with multiple vehicles
 */
void testMultipleVehicles() {
    printf("\n--- Multiple Vehicles Test ---\n");

    // Create a hardcoded garage with multiple vehicles
    int numVehicles = 3;
    int removeIndex;
    char removeChoice;

    char** garage = (char**)malloc(numVehicles * sizeof(char*));

    // Vehicle 1: Honda Civic, 2015, $12000
    unsigned int value1 = 12000;
    unsigned int year1 = 2015;
    unsigned int packedData1 = (value1 << 11) | year1;

    garage[0] = (char*)malloc(4 + 12); // 4 bytes + "Honda Civic" + null
    garage[0][0] = (packedData1 >> 24) & 0xFF;
    garage[0][1] = (packedData1 >> 16) & 0xFF;
    garage[0][2] = (packedData1 >> 8) & 0xFF;
    garage[0][3] = packedData1 & 0xFF;

    char* desc1 = "Honda Civic";
    for (int i = 0; i <= 11; i++) {
        garage[0][4 + i] = desc1[i];
    }

    // Vehicle 2: Toyota Camry, 2018, $18000
    unsigned int value2 = 18000;
    unsigned int year2 = 2018;
    unsigned int packedData2 = (value2 << 11) | year2;

    garage[1] = (char*)malloc(4 + 12); // 4 bytes + "Toyota Camry" + null
    garage[1][0] = (packedData2 >> 24) & 0xFF;
    garage[1][1] = (packedData2 >> 16) & 0xFF;
    garage[1][2] = (packedData2 >> 8) & 0xFF;
    garage[1][3] = packedData2 & 0xFF;

    char* desc2 = "Toyota Camry";
    for (int i = 0; i <= 11; i++) {
        garage[1][4 + i] = desc2[i];
    }

    // Vehicle 3: Ford F-150, 2020, $35000
    unsigned int value3 = 35000;
    unsigned int year3 = 2020;
    unsigned int packedData3 = (value3 << 11) | year3;

    garage[2] = (char*)malloc(4 + 10); // 4 bytes + "Ford F-150" + null
    garage[2][0] = (packedData3 >> 24) & 0xFF;
    garage[2][1] = (packedData3 >> 16) & 0xFF;
    garage[2][2] = (packedData3 >> 8) & 0xFF;
    garage[2][3] = packedData3 & 0xFF;

    char* desc3 = "Ford F-150";
    for (int i = 0; i <= 9; i++) {
        garage[2][4 + i] = desc3[i];
    }

    // Display previous garage
    printf("Previous contents in the garage:\n");
    displayGarage(garage, numVehicles);

    // Ask user if they want to remove a vehicle
    printf("\nWould you like to remove a vehicle? (yes/no): ");
    scanf_s("%c", &removeChoice, 1);
    clearInputBuffer();

    if (removeChoice == 'y' || removeChoice == 'Y') {
        printf("Enter the index of the vehicle to remove (1-%d): ", numVehicles);
        scanf_s("%d", &removeIndex);
        clearInputBuffer();

        if (removeIndex >= 1 && removeIndex <= numVehicles) {
            // Convert from 1-based to 0-based indexing
            printf("\nRemoving vehicle %d...\n", removeIndex);
            garage = removeVehicle(garage, numVehicles, removeIndex - 1);
            numVehicles--;

            // Display garage after removal
            printf("\nGarage after removal:\n");
            displayGarage(garage, numVehicles);
        } else {
            printf("Invalid vehicle index. No vehicle removed.\n");
        }
    }

    // Free memory
    for (int i = 0; i < numVehicles; i++) {
        free(garage[i]);
    }
    free(garage);

    printf("Multiple vehicles test completed.\n");
    printf("Press Enter to continue...");
    getchar();
}

/*
 * Function: runAllTests
 * Purpose: Runs all test functions
 */
void runAllTests() {
    printf("\n=== Running All Tests ===\n");

    testGarage();
    testBitManipulation();
    testBoundaryValues();
    testErrorHandling();
    testMultipleVehicles();

    printf("\n=== All Tests Completed ===\n");
    printf("Press Enter to continue...");
    getchar();
}

/*
 * Function: main
 * Purpose: Entry point for the program
 */
int main() {
    int choice;

    do {
        choice = mainMenu();

        switch (choice) {
            case 0:
                printf("Exiting program. Goodbye!\n");
                break;
            case 1:
                testGarage();
                break;
            case 2:
                testBitManipulation();
                break;
            case 3:
                testBoundaryValues();
                break;
            case 4:
                testErrorHandling();
                break;
            case 5:
                testMultipleVehicles();
                break;
            case 6:
                runAllTests();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();
        }
    } while (choice != 0);

    return 0;
}