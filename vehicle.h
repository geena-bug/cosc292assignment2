/*
 * Vehicle Management System Header File
 * Team: Anyaso and Obey
 */

#ifndef VEHICLE_H
#define VEHICLE_H

/*
 * Function: createVehicle
 * Purpose: Dynamically allocates a string to store a vehicle's information.
 *          Value and model year are stored in the first 4 bytes.
 *          The remaining memory is for the description of the vehicle.
 * Parameters: None
 * Returns: a dynamically allocated block of memory containing the vehicle's information
 * By: Anyaso
 */
char* createVehicle();

/*
 * Function: displayVehicle
 * Purpose: Display the information stored in a vehicle
 * Parameters: char* representing all information stored for a vehicle
 * Returns: Nothing
 * By: Obey
 */
void displayVehicle(char*);

/*
 * Function: createGarage
 * Purpose: Store multiple vehicles in an array. Dynamically allocates an array of char pointers.
 *          Calls the createVehicle method for each vehicle to be created.
 * Parameters: int - Number of vehicles to add to the array
 * Returns: a dynamically array of char*'s, each pointer representing a vehicle
 * By: Obey
 */
char** createGarage(int);

/*
 * Function: displayGarage
 * Purpose: Display all the information stored for each vehicle in the garage.
 *          Calls displayVehicle for each vehicle in the garage.
 * Parameters: char** - pointer to the garage
 *             int - number of vehicles in the garage
 * Returns: Nothing.
 * By: Anyaso
 */
void displayGarage(char**, int);

/*
 * Function: removeVehicle
 * Purpose: Remove a vehicle from the garage. Returns a new dynamically allocated garage with remaining cars.
 *          This function should free the garage passed in.
 * Parameters: char** - pointer to current garage
 *             int - number of vehicles in current garage
 *             int - index of the vehicle to remove (0 based)
 * Returns: a reference to the new dynamically allocated garage
 * By: Obey
 */
char** removeVehicle(char**, int, int);

/*
 * Function: testGarage
 * Purpose: Test the vehicle management functions
 * By: Both teammates
 */
void testGarage();
void testBitManipulation();
void testBoundaryValues();
void testErrorHandling();
void testMultipleVehicles();
void runAllTests();

#endif /* VEHICLE_H */