// SAFE-GUARD:
// It is good practice to apply safe-guards to header files
// Safe-guard's ensures only 1 copy of the header file is used in the project build
// The macro name should be mirroring the file name with _ for spaces, dots, etc.
#ifndef CLINIC_H
#define CLINIC_H

//////////////////////////////////////
// Module macro's (usable by any file that includes this header)
//////////////////////////////////////

// Display formatting options
#define FMT_FORM 1
#define FMT_TABLE 2

// C Strings: array sizes
#define NAME_LEN 15
#define PHONE_DESC_LEN 4
#define PHONE_LEN 10
#define OPTION_LEN 1

#define START_HOUR 10
#define END_HOUR 14
#define MINUTE_INTERVAL 30
#define MIN_MONTH 1
#define MAX_MONTH 12
#define MIN_DAY 1
#define MAX_HOUR 23
#define MAX_MINUTE 59
#define MIN_TIME 0

//////////////////////////////////////
// Structures
//////////////////////////////////////

// Data type: Phone
struct Phone
{
    char description[PHONE_DESC_LEN + 1];
    char number[PHONE_LEN + 1];
};

// Data type: Patient
struct Patient
{
    int patientNumber;
    char name[NAME_LEN + 1];
    struct Phone phone;
};

// ------------------- MS#3 -------------------

// Data type: Time
struct Time
{
    int hour;
    int min;
};

// Data type: Date
struct Date
{
    int year;
    int month;
    int day;
};
// Data type: Appointment
struct Appointment
{
    int patientNumber;
    struct Date date;
    struct Time time;
};

// ClinicData type: Provided to student
struct ClinicData
{
    struct Patient *patients;
    int maxPatient;
    struct Appointment *appointments;
    int maxAppointments;
};

//////////////////////////////////////
// DISPLAY FUNCTIONS
//////////////////////////////////////

// Display's the patient table header (table format)
void displayPatientTableHeader(void);

// Displays a single patient record in FMT_FORM | FMT_TABLE format
void displayPatientData(const struct Patient *patient, int fmt);

// Display's appointment schedule headers (date-specific or all records)
void displayScheduleTableHeader(const struct Date *date, int isAllRecords);

// Display a single appointment record with patient info. in tabular format
void displayScheduleData(const struct Patient *patient,
                         const struct Appointment *appoint,
                         int includeDateField);

//////////////////////////////////////
// MENU & ITEM SELECTION FUNCTIONS
//////////////////////////////////////

// Menu: Main
void menuMain(struct ClinicData *data);

// Menu: Patient Management
void menuPatient(struct Patient patient[], int max);

// Menu: Patient edit
void menuPatientEdit(struct Patient *patient);

// Menu: Appointment Management
void menuAppointment(struct ClinicData *data);

// Display's all patient data in the FMT_FORM | FMT_TABLE format
void displayAllPatients(const struct Patient patient[], int max, int fmt);

// Search for a patient record based on patient number or phone number
void searchPatientData(const struct Patient patient[], int max);

// Add a new patient record to the patient array
void addPatient(struct Patient patient[], int max);

// Edit a patient record from the patient array
void editPatient(struct Patient patient[], int max);

// Remove a patient record from the patient array
void removePatient(struct Patient patient[], int max);

// View ALL scheduled appointments
void viewAllAppointments(const struct ClinicData *data);

// View appointment schedule for the user input date
void viewAppointmentSchedule(const struct ClinicData *data);

// Add an appointment record to the appointment array
void addAppointment(struct Appointment *app, const int maxAppointments, struct Patient *pt, int maxPatients);

// Remove an appointment record from the appointment array
void removeAppointment(struct Appointment *app, const int maxAppointments, struct Patient *pt, int maxPatients);

//////////////////////////////////////
// UTILITY FUNCTIONS
//////////////////////////////////////

// Search and display patient record by patient number (form)
void searchPatientByPatientNumber(const struct Patient patient[], int max);

// Search and display patient records by phone number (tabular)
void searchPatientByPhoneNumber(const struct Patient patient[], int max);

// Get the next highest patient number
int nextPatientNumber(const struct Patient patient[], int max);

// Find the patient array index by patient number (returns -1 if not found)
int findPatientIndexByPatientNum(int patientNumber,
                                 const struct Patient patient[], int max);

// Sort the data by date (using bubble sort)
void sortData(struct Appointment appoints[], int max);

// Check the next available slot for appointment
int nextAvailableSlot(struct Appointment appoints[], int max);

// Check if the appointment exists
int checkAppointment(int patientNumber, struct Date date, struct Appointment *app, int max);

//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Get user input for a new patient record
void inputPatient(struct Patient *patient);

// Get user input for phone contact information
void inputPhoneData(struct Phone *phone);

// Get user input for date information
void inputDate(struct Date *date);

// Get user input for time information
void inputTime(struct Time *time);

//////////////////////////////////////
// FILE FUNCTIONS
//////////////////////////////////////

// Import patient data from file into a Patient array (returns # of records read)
int importPatients(const char *datafile, struct Patient patients[], int max);

// Import appointment data from file into an Appointment array (returns # of records read)
int importAppointments(const char *datafile, struct Appointment appoints[], int max);

#endif // !CLINIC_H
