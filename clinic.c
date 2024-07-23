#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>

// include the user library "core" so we can use those functions
#include "core.h"
// include the user library "clinic" where the function prototypes are declared
#include "clinic.h"

//////////////////////////////////////
// DISPLAY FUNCTIONS
//////////////////////////////////////

// Display's the patient table header (table format)
void displayPatientTableHeader(void)
{
    printf("Pat.# Name            Phone#\n"
           "----- --------------- --------------------\n");
}

// Displays a single patient record in FMT_FORM | FMT_TABLE format
void displayPatientData(const struct Patient *patient, int fmt)
{
    if (fmt == FMT_FORM)
    {
        printf("Name  : %s\n"
               "Number: %05d\n"
               "Phone : ",
               patient->name, patient->patientNumber);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
    else
    {
        printf("%05d %-15s ", patient->patientNumber,
               patient->name);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
}

// Display's appointment schedule headers (date-specific or all records)
void displayScheduleTableHeader(const struct Date *date, int isAllRecords)
{
    printf("Clinic Appointments for the Date: ");

    if (isAllRecords)
    {
        printf("<ALL>\n\n");
        printf("Date       Time  Pat.# Name            Phone#\n"
               "---------- ----- ----- --------------- --------------------\n");
    }
    else
    {
        printf("%04d-%02d-%02d\n\n", date->year, date->month, date->day);
        printf("Time  Pat.# Name            Phone#\n"
               "----- ----- --------------- --------------------\n");
    }
}

// Display a single appointment record with patient info. in tabular format
void displayScheduleData(const struct Patient *patient,
                         const struct Appointment *appoint,
                         int includeDateField)
{
    if (includeDateField)
    {
        printf("%04d-%02d-%02d ", appoint->date.year, appoint->date.month,
               appoint->date.day);
    }
    printf("%02d:%02d %05d %-15s ", appoint->time.hour, appoint->time.min,
           patient->patientNumber, patient->name);

    displayFormattedPhone(patient->phone.number);

    printf(" (%s)\n", patient->phone.description);
}

//////////////////////////////////////
// MENU & ITEM SELECTION FUNCTIONS
//////////////////////////////////////

// main menu
void menuMain(struct ClinicData *data)
{
    int selection;

    do
    {
        printf("Veterinary Clinic System\n"
               "=========================\n"
               "1) PATIENT     Management\n"
               "2) APPOINTMENT Management\n"
               "-------------------------\n"
               "0) Exit System\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');
        switch (selection)
        {
        case 0:
            printf("Are you sure you want to exit? (y|n): ");
            selection = !(inputCharOption("yn") == 'y');
            putchar('\n');
            if (!selection)
            {
                printf("Exiting system... Goodbye.\n\n");
            }
            break;
        case 1:
            menuPatient(data->patients, data->maxPatient);
            break;
        case 2:
            menuAppointment(data);
            break;
        }
    } while (selection);
}

// Menu: Patient Management
void menuPatient(struct Patient patient[], int max)
{
    int selection;

    do
    {
        printf("Patient Management\n"
               "=========================\n"
               "1) VIEW   Patient Data\n"
               "2) SEARCH Patients\n"
               "3) ADD    Patient\n"
               "4) EDIT   Patient\n"
               "5) REMOVE Patient\n"
               "-------------------------\n"
               "0) Previous menu\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 5);
        putchar('\n');
        switch (selection)
        {
        case 1:
            displayAllPatients(patient, max, FMT_TABLE);
            suspend();
            break;
        case 2:
            searchPatientData(patient, max);
            break;
        case 3:
            addPatient(patient, max);
            suspend();
            break;
        case 4:
            editPatient(patient, max);
            break;
        case 5:
            removePatient(patient, max);
            suspend();
            break;
        }
    } while (selection);
}

// Menu: Patient edit
void menuPatientEdit(struct Patient *patient)
{
    int selection;

    do
    {
        printf("Edit Patient (%05d)\n"
               "=========================\n"
               "1) NAME : %s\n"
               "2) PHONE: ",
               patient->patientNumber, patient->name);

        displayFormattedPhone(patient->phone.number);

        printf("\n"
               "-------------------------\n"
               "0) Previous menu\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');

        if (selection == 1)
        {
            printf("Name  : ");
            inputCString(patient->name, 1, NAME_LEN);
            putchar('\n');
            printf("Patient record updated!\n\n");
        }
        else if (selection == 2)
        {
            inputPhoneData(&patient->phone);
            printf("Patient record updated!\n\n");
        }

    } while (selection);
}

// Menu: Appointment Management
void menuAppointment(struct ClinicData *data)
{
    int selection;

    do
    {
        printf("Appointment Management\n"
               "==============================\n"
               "1) VIEW   ALL Appointments\n"
               "2) VIEW   Appointments by DATE\n"
               "3) ADD    Appointment\n"
               "4) REMOVE Appointment\n"
               "------------------------------\n"
               "0) Previous menu\n"
               "------------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 4);
        putchar('\n');
        switch (selection)
        {
        case 1:
            viewAllAppointments(data);
            suspend();
            break;
        case 2:
            viewAppointmentSchedule(data);
            suspend();
            break;
        case 3:
            addAppointment(data->appointments, data->maxAppointments,
                           data->patients, data->maxPatient);
            suspend();
            break;
        case 4:
            removeAppointment(data->appointments, data->maxAppointments,
                              data->patients, data->maxPatient);
            suspend();
            break;
        }
    } while (selection);
}

// Display's all patient data in the FMT_FORM | FMT_TABLE format
void displayAllPatients(const struct Patient patient[], int max, int fmt)
{
    int i;
    int eligibleRec = 0;

    if (fmt == FMT_TABLE)
    {
        displayPatientTableHeader();
    }

    for (i = 0; i < max; i++)
    {
        if (patient[i].patientNumber != 0)
        {
            displayPatientData(&patient[i], fmt);
            eligibleRec = 1;
        }
    }

    if (eligibleRec == 0)
    {
        printf("*** No records found ***\n");
    }
    putchar('\n');
}

// Search for a patient record based on patient number or phone number
void searchPatientData(const struct Patient patient[], int max)
{
    int selection;

    do
    {
        printf("Search Options\n"
               "==========================\n"
               "1) By patient number\n"
               "2) By phone number\n"
               "..........................\n"
               "0) Previous menu\n"
               "..........................\n"
               "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');
        switch (selection)
        {
        case 1:
            searchPatientByPatientNumber(patient, max);
            suspend();
            break;
        case 2:
            searchPatientByPhoneNumber(patient, max);
            suspend();
            break;
        }
    } while (selection);
}

// Add a new patient record to the patient array
void addPatient(struct Patient patient[], int max)
{
    int i = 0, place = 0;
    int space = 0;

    for (i = 0; i < max; i++)
    {
        if (patient[i].patientNumber == 0)
        {
            place = i;
            space = 1;
            break;
        }
    }

    if (space == 0)
    {
        printf("ERROR: Patient listing is FULL!\n\n");
    }
    else
    {
        patient[place].patientNumber = nextPatientNumber(patient, max);
        inputPatient(&patient[place]);
        printf("*** New patient record added ***\n\n");
    }
}

// Edit a patient record from the patient array
void editPatient(struct Patient patient[], int max)
{
    int patientNum;
    int findPatient = -1;
    printf("Enter the patient number: ");
    patientNum = inputIntPositive();
    findPatient = findPatientIndexByPatientNum(patientNum, patient, max);
    putchar('\n');
    if (findPatient == -1)
    {
        printf("ERROR: Patient record not found!\n\n");
    }
    else
    {
        menuPatientEdit(&patient[findPatient]);
    }
}

// Remove a patient record from the patient array
void removePatient(struct Patient patient[], int max)
{
    int patientNum;
    int findPatient = 0;
    int valid = 0;
    char removeProve, ch;
    char option[10] = {"yn"};
    printf("Enter the patient number: ");
    patientNum = inputIntPositive();
    printf("\n");

    findPatient = findPatientIndexByPatientNum(patientNum, patient, max);
    if (findPatient == -1)
    {
        printf("ERROR: Patient record not found!\n\n");
    }
    else
    {
        displayPatientData(&patient[findPatient], FMT_FORM);
        printf("\n");
        printf("Are you sure you want to remove this patient record? (y/n): ");

        do
        {
            scanf(" %c%c", &removeProve, &ch);

            if (ch == '\n')
            {
                if (removeProve == 'y' || removeProve == 'Y')
                {
                    patient[findPatient].patientNumber = 0;
                    patient[findPatient].name[0] = '\0';
                    patient[findPatient].phone.description[0] = '\0';
                    patient[findPatient].phone.number[0] = '\0';
                    valid++;
                    printf("Patient record has been removed!\n\n");
                }
                else if (removeProve == 'n' || removeProve == 'N')
                {
                    printf("Operation aborted.\n\n");
                    valid++;
                }
                else
                {
                    printf("ERROR: Character must be one of [%s]: ", option);
                    clearInputBuffer();
                }
            }
            else
            {
                printf("ERROR: Character must be one of [%s]: ", option);
                clearInputBuffer();
            }
        } while (valid == 0);
    }
}

// View ALL scheduled appointments
void viewAllAppointments(const struct ClinicData *data)
{
    int i, j;
    sortData(data->appointments, data->maxAppointments);
    displayScheduleTableHeader(NULL, 1);

    for (i = 0; i < data->maxAppointments; i++)
    {
        for (j = 0; j < data->maxPatient; j++)
        {
            if (data->appointments[i].patientNumber && data->patients[j].patientNumber)
            {
                if (data->appointments[i].patientNumber == data->patients[j].patientNumber)
                {
                    displayScheduleData(&data->patients[j], &data->appointments[i], 1);
                }
            }
        }
    }
    putchar('\n');
}

// View appointment schedule for the user input date
void viewAppointmentSchedule(const struct ClinicData *data)
{
    int i, j;
    struct Date date;
    inputDate(&date);
    printf("\n");
    sortData(data->appointments, data->maxAppointments);
    displayScheduleTableHeader(&date, 0);
    for (i = 0; i < data->maxAppointments; i++)
    {
        for (j = 0; j < data->maxAppointments; j++)
        {
            if (data->appointments[i].patientNumber == data->patients[j].patientNumber &&
                data->appointments[i].date.year == date.year &&
                data->appointments[i].date.month == date.month &&
                data->appointments[i].date.day == date.day)
            {
                displayScheduleData(&data->patients[j], &data->appointments[i], 0);
            }
        }
    }
    printf("\n");
}

// Add an appointment record to the appointment array
void addAppointment(struct Appointment *app, const int maxAppointments, struct Patient *pt, int maxPatients)
{
    int serPatientNum, findPatient = -1;
    int i, count = 0, slot = 0;
    struct Date date = {0};
    struct Time time = {0};

    printf("Patient Number: ");
    serPatientNum = inputIntPositive();
    findPatient = findPatientIndexByPatientNum(serPatientNum, pt, maxPatients);

    if (findPatient == -1)
    {
        printf("ERROR: Patient record not found!\n\n");
    }
    else
    {

        do
        {
            inputDate(&date);
            inputTime(&time);

            for (i = 0; i < maxAppointments; i++)
            {
                if (time.min == app[i].time.min &&
                    time.hour == app[i].time.hour &&
                    date.day == app[i].date.day &&
                    date.month == app[i].date.month &&
                    date.year == app[i].date.year)
                {
                    putchar('\n');
                    printf("ERROR: Appointment timeslot is not available!\n\n");
                    count++;
                    break;
                }
                else
                {
                    count = 0;
                }
            }
        } while (count != 0);

        if (count == 0)
        {
            while ((time.hour < START_HOUR || time.hour > END_HOUR) || (time.hour == END_HOUR && time.min > 0) || (time.min % MINUTE_INTERVAL != 0))
            {
                printf("ERROR: Time must be between %02d:00 and %02d:00 in %02d minute intervals.\n\n", START_HOUR, END_HOUR, MINUTE_INTERVAL);
                inputTime(&time);
            }

            slot = nextAvailableSlot(app, maxAppointments);
            app[slot].date = date;
            app[slot].time = time;
            app[slot].patientNumber = serPatientNum;
            putchar('\n');
            printf("*** Appointment scheduled! ***\n\n");
        }
    }
}

// Remove an appointment record from the appointment array
void removeAppointment(struct Appointment *app, const int maxAppointments, struct Patient *pt, int maxPatients)
{
    int serPatientNum, findPatient = -1;
    int findApp = -1;
    char removeProve, ch;
    int valid = 0;
    char option[10] = "yn";
    struct Date date = {0};

    printf("Patient Number: ");
    serPatientNum = inputIntPositive();
    findPatient = findPatientIndexByPatientNum(serPatientNum, pt, maxPatients);

    if (findPatient == -1)
    {
        printf("ERROR: Patient record not found!\n\n");
    }
    else
    {
        inputDate(&date);
        findApp = checkAppointment(serPatientNum, date, app, maxAppointments);

        if (findApp == -1)
        {
            printf("ERROR: No appointment for this date!\n\n");
        }
        else
        {
            putchar('\n');
            displayPatientData(&pt[findPatient], FMT_FORM);
            printf("Are you sure you want to remove this appointment (y,n): ");

            do
            {
                scanf(" %c%c", &removeProve, &ch);

                if (ch == '\n')
                {
                    if (removeProve == 'y' || removeProve == 'Y')
                    {
                        app[findApp].patientNumber = 0;
                        app[findApp].date.year = 0;
                        app[findApp].date.month = 0;
                        app[findApp].date.day = 0;
                        app[findApp].time.hour = 0;
                        app[findApp].time.min = 0;
                        valid++;
                        putchar('\n');
                        printf("Appointment record has been removed!\n\n");
                    }
                    else if (removeProve == 'n' || removeProve == 'N')
                    {
                        putchar('\n');
                        printf("Operation aborted.\n\n");
                        valid++;
                    }
                    else
                    {
                        printf("ERROR: Character must be one of [%s]: ", option);
                        clearInputBuffer();
                    }
                }
                else
                {
                    printf("ERROR: Character must be one of [%s]: ", option);
                    clearInputBuffer();
                }

            } while (valid == 0);
        }
    }
}

//////////////////////////////////////
// UTILITY FUNCTIONS
//////////////////////////////////////

// Search and display patient record by patient number (form)
void searchPatientByPatientNumber(const struct Patient patient[], int max)
{
    int serPatientNum;
    int findIndex = 0;
    printf("Search by patient number: ");
    serPatientNum = inputIntPositive();
    // clearInputBuffer();
    printf("\n");
    findIndex = findPatientIndexByPatientNum(serPatientNum, patient, max);
    if (findIndex == -1)
    {
        printf("*** No records found ***\n");
    }
    else
    {
        displayPatientData(&patient[findIndex], FMT_FORM);
    }

    putchar('\n');
}

// Search and display patient records by phone number (tabular)
void searchPatientByPhoneNumber(const struct Patient patient[], int max)
{
    char serPhoneNum[PHONE_LEN + 1] = {0};
    int i;
    int match = 0;
    printf("Search by phone number: ");
    inputCString(serPhoneNum, PHONE_LEN, PHONE_LEN);
    putchar('\n');
    displayPatientTableHeader();
    for (i = 0; i < max; i++)
    {
        if (strcmp(patient[i].phone.number, serPhoneNum) == 0)
        {
            displayPatientData(&patient[i], FMT_TABLE);
            match = 1;
        }
    }

    if (match == 0)
    {
        putchar('\n');
        printf("*** No records found ***\n");
    }
    putchar('\n');
}

// Get the next highest patient number
int nextPatientNumber(const struct Patient patient[], int max)
{
    int i;
    int patientNumLast = 0;
    int patientNumNext = 0;
    for (i = 0; i < max; i++)
    {
        if (patientNumLast < patient[i].patientNumber)
        {
            patientNumLast = patient[i].patientNumber;
        }
    }

    patientNumNext = patientNumLast + 1;
    return patientNumNext;
}

// Find the patient array index by patient number (returns -1 if not found)
int findPatientIndexByPatientNum(int patientNumber, const struct Patient patient[], int max)
{
    int i;

    for (i = 0; i < max; i++)
    {
        if (patientNumber == patient[i].patientNumber)
        {
            return i;
        }
    }
    return -1;
}

// Sort the data by date (using bubble sort)
void sortData(struct Appointment appoints[], int max)
{
    int i, j;
    struct Appointment temp;

    for (i = max - 1; i > 0; i--)
    {
        for (j = 0; j < i; j++)
        {
            if (appoints[j].date.year != appoints[j + 1].date.year)
            {
                if (appoints[j].date.year > appoints[j + 1].date.year)
                {
                    temp = appoints[j];
                    appoints[j] = appoints[j + 1];
                    appoints[j + 1] = temp;
                }
            }
            else if (appoints[j].date.month != appoints[j + 1].date.month)
            {
                if (appoints[j].date.month > appoints[j + 1].date.month)
                {
                    temp = appoints[j];
                    appoints[j] = appoints[j + 1];
                    appoints[j + 1] = temp;
                }
            }
            else if (appoints[j].date.day != appoints[j + 1].date.day)
            {
                if (appoints[j].date.day > appoints[j + 1].date.day)
                {
                    temp = appoints[j];
                    appoints[j] = appoints[j + 1];
                    appoints[j + 1] = temp;
                }
            }
            else if (appoints[j].time.hour != appoints[j + 1].time.hour)
            {
                if (appoints[j].time.hour > appoints[j + 1].time.hour)
                {
                    temp = appoints[j];
                    appoints[j] = appoints[j + 1];
                    appoints[j + 1] = temp;
                }
            }
            else if (appoints[j].time.min != appoints[j + 1].time.min)
            {
                if (appoints[j].time.min > appoints[j + 1].time.min)
                {
                    temp = appoints[j];
                    appoints[j] = appoints[j + 1];
                    appoints[j + 1] = temp;
                }
            }
        }
    }
}

// Check the next available slot for appointment
int nextAvailableSlot(struct Appointment appoints[], int max)
{
    int i;
    int slot = 0;
    for (i = 0; i < max; i++)
    {
        if (appoints[i].patientNumber < 1)
        {
            slot = i;
        }
    }
    return slot;
}

// Check if the appointment exists
int checkAppointment(int patientNumber, struct Date date, struct Appointment *app, int max)
{
    int i;

    for (i = 0; i < max; i++)
    {
        if (patientNumber == app[i].patientNumber && date.year == app[i].date.year && date.month == app[i].date.month && date.day == app[i].date.day)
        {
            return i;
        }
    }
    return -1;
}

//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Get user input for a new patient record
void inputPatient(struct Patient *patient)
{

    printf("Patient Data Input\n"
           "------------------\n");
    printf("Number: %05d\n", patient->patientNumber);
    printf("Name  : ");
    inputCString(patient->name, 1, NAME_LEN);
    putchar('\n');
    inputPhoneData(&patient->phone);
}

// Get user input for phone contact information
void inputPhoneData(struct Phone *phone)
{
    int selection;
    char des1[PHONE_DESC_LEN + 1] = {"CELL"};
    char des2[PHONE_DESC_LEN + 1] = {"HOME"};
    char des3[PHONE_DESC_LEN + 1] = {"WORK"};
    char des4[PHONE_DESC_LEN + 1] = {"TBD"};

    printf("Phone Information\n"
           "-----------------\n"
           "How will the patient like to be contacted?\n"
           "1. Cell\n"
           "2. Home\n"
           "3. Work\n"
           "4. TBD\n"
           "Selection: ");

    selection = inputIntRange(1, 4);
    printf("\n");
    if (selection == 1)
        strcpy(phone->description, des1);
    else if (selection == 2)
        strcpy(phone->description, des2);
    else if (selection == 3)
        strcpy(phone->description, des3);
    else if (selection == 4)
        strcpy(phone->description, des4);
    if (selection != 4)
    {

        printf("Contact: %s\n", phone->description);
        printf("Number : ");
        inputCString(phone->number, 10, 10);
        putchar('\n');
    }
    else
    {
        phone->number[0] = '\0';
    }
}

// Get user input for date information
void inputDate(struct Date *date)
{
    int lastDay = 31; // Default lastDay = 31 days

    printf("Year        : ");
    date->year = inputIntPositive();

    printf("Month (%d-%d): ", MIN_MONTH, MAX_MONTH);
    date->month = inputIntRange(MIN_MONTH, MAX_MONTH);

    if (date->month == 4 || date->month == 9 || date->month == 11)
    {
        lastDay = 30;
    }
    else if (date->month == 2)
    {
        if (date->year % 4 == 0)
        {
            lastDay = 29;
        }
        else
        {
            lastDay = 28;
        }
    }
    printf("Day (1-%d)  : ", lastDay);
    date->day = inputIntRange(MIN_DAY, lastDay);
}

// Get user input for time information
void inputTime(struct Time *time)
{
    printf("Hour (%d-%d)  : ", MIN_TIME, MAX_HOUR);
    time->hour = inputIntRange(MIN_TIME, MAX_HOUR);

    printf("Minute (%d-%d): ", MIN_TIME, MAX_MINUTE);
    time->min = inputIntRange(MIN_TIME, MAX_MINUTE);
}

//////////////////////////////////////
// FILE FUNCTIONS
//////////////////////////////////////

// Import patient data from file into a Patient array (returns # of records read)
int importPatients(const char *datafile, struct Patient patients[], int max)
{
    int i = 0;

    FILE *fp;
    fp = fopen(datafile, "r");

    if (fp != NULL)
    {
        for (i = 0; i < max; i++)
        {
            fscanf(fp, "%d|%[^|]|%[^|]|%[^\n]",
                   &patients[i].patientNumber,
                   patients[i].name,
                   patients[i].phone.description,
                   patients[i].phone.number);
        }
        fclose(fp);
    }
    else
    {
        putchar('\n');
        printf("Error: Fail to open the file\n");
    }

    return i;
}

// Import appointment data from file into an Appointment array (returns # of records read)
int importAppointments(const char *datafile, struct Appointment appoints[], int max)
{
    int i, count = 0;
    FILE *fp = NULL;
    fp = fopen(datafile, "r");

    if (fp != NULL)
    {
        for (i = 0; i < max; i++)
        {
            fscanf(fp, "%d,%d,%d,%d,%d,%d",
                   &appoints[i].patientNumber,
                   &appoints[i].date.year,
                   &appoints[i].date.month,
                   &appoints[i].date.day,
                   &appoints[i].time.hour,
                   &appoints[i].time.min);

            if (!feof(fp))
            {
                count++;
            }
        }
        fclose(fp);
    }

    return count;
}
