/*Vehicle Maintenance Record System (VMRS). ; Keep a record of the maintenance performed on vehicles,
storing all the information in a binary file.
devCPP ver 5.11 /VSCode 1.103.0
24/10/2025
Jose Eduardo Meneses Hernández
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DATA_F "data/"

struct index_file
{
    char plate[10];
    long cost;
    long position;
};
struct date
{
    int day;
    int month;
    int year;
};
struct vehicle
{
    char owner[100];
    int model;
    char plate[10];
    char service[100];
    struct date date_service;
    long cost;
};

void input_and_format_plate(char *plate_output)
{
    char letters[100];
    char numbers[100];
    int valid;

    do
    {
        printf("\n   (3 uppercase letters): ");
        if (scanf(" %99s", letters) != 1)
        {
            valid = 0;
            while (getchar() != '\n')
                ;
        }
        else
        {
            while (getchar() != '\n')
                ;
            ;

            int len = strlen(letters);
            if (len != 3)
            {
                valid = 0;
                printf("   Error: You must enter exactly 3 letters. (Current: %d)\n", len);
            }
            else
            {
                valid = 1;
                for (int j = 0; j < 3; j++)
                {
                    if (!isupper(letters[j]))
                    {
                        valid = 0;
                        break;
                    }
                }
                if (!valid)
                {
                    printf("   Error: The 3 characters must be uppercase letters (Expl: ABC).\n");
                }
            }
        }
    } while (!valid);

    do
    {
        printf("   (3 numbers 0-9): ");
        if (scanf(" %99s", numbers) != 1)
        {
            valid = 0;
            while (getchar() != '\0')
                ;
            ;
        }
        else
        {
            while (getchar() != '\n')
                ;
            ;

            int len = strlen(numbers);
            if (len != 3)
            {
                valid = 0;
                printf("   Error: You must enter exactly 3 digits. (Current: %d)\n", len);
            }
            else
            {
                valid = 1;
                for (int j = 0; j < 3; j++)
                {
                    if (!isdigit(numbers[j]))
                    {
                        valid = 0;
                        break;
                    }
                }
                if (!valid)
                {
                    printf("   Error: The 3 characters must be digits (0-9) (Expl: 123).\n");
                }
            }
        }
    } while (!valid);

    snprintf(plate_output, 10, "%s-%s", letters, numbers);
}

int is_plate_duplicate(const char *plate)
{
    FILE *index = fopen(DATA_F "index.bin", "rb");
    if (!index)
    {
        return 0;
    }

    struct index_file idx;
    int duplicate = 0;

    while (fread(&idx, sizeof(struct index_file), 1, index) == 1)
    {
        if (strncmp(idx.plate, plate, sizeof(idx.plate)) == 0)
        {
            duplicate = 1;
            break;
        }
    }

    fclose(index);
    return duplicate;
}

void select_service(char *service_output)
{
    int selection;
    int valid = 0;

    printf("\n-- Select the Service Type --\n");
    printf("1. Paint\n");
    printf("2. Body Work\n");
    printf("3. Paint and Body Work\n");
    printf("4. Maintenance\n");

    do
    {
        printf("Enter your selection (1-4): ");
        if (scanf("%d", &selection) != 1)
        {
            printf("Error: Please enter a number.\n");
            while (getchar() != '\n')
                ;
        }
        else if (selection < 1 || selection > 4)
        {
            printf("Error: Invalid option. Please select a number between 1 and 4.\n");
        }
        else
        {
            valid = 1;
        }
    } while (!valid);

    switch (selection)
    {
    case 1:
        strcpy(service_output, "Paint");
        break;
    case 2:
        strcpy(service_output, "Body Work");
        break;
    case 3:
        strcpy(service_output, "Paint and Body Work");
        break;
    case 4:
        strcpy(service_output, "Maintenance");
        break;
    default:
        strcpy(service_output, "Unknown Service");
        break;
    }
    while (getchar() != '\n')
        ;
}

void fill(struct vehicle unit[], int n)
{
    FILE *data = fopen(DATA_F "data.bin", "ab");
    if (!data)
    {
        perror("Error opening data file");
        return;
    }

    FILE *index = fopen(DATA_F "index.bin", "ab");
    if (!index)
    {
        perror("Error opening index file");
        fclose(data);
        return;
    }

    for (int i = 0; i < n; i++)
    {
        printf("\n--- VEHICLE %d ---\n", i + 1);

        printf("Enter name of the owner: ");
        scanf(" %[^\n]", unit[i].owner);

        printf("Enter the vehicle model: ");
        while ((scanf("%d", &unit[i].model)) != 1 || unit[i].model < 1885 || unit[i].model > 2026)
        {
            printf("Enter a valid value (integer (1885-2026).\n");
            while (getchar() != '\n')
                ;
        }

        printf("Enter the plate (Format: LLL-NNN):\n");
        do
        {
            input_and_format_plate(unit[i].plate);

            if (is_plate_duplicate(unit[i].plate))
            {
                printf("Error: The plate '%s' is already registered. Please enter a unique plate.\n", unit[i].plate);
            }
        } while (is_plate_duplicate(unit[i].plate));

        printf("\nEnter the service: ");
        select_service(unit[i].service);

        printf("Enter the cost: ");
        while ((scanf("%ld", &unit[i].cost)) != 1 || unit[i].cost < 0)
        {
            printf("Enter a valid positive number.\n");
            while (getchar() != '\n')
                ;
        }

        printf("--Enter the date--\n");
        printf("Day: ");
        while ((scanf("%d", &unit[i].date_service.day)) != 1 || unit[i].date_service.day < 1 || unit[i].date_service.day > 31)
        {
            printf("Enter a valid day (1-31): ");
            while (getchar() != '\n')
                ;
        }

        printf("Month: ");
        while ((scanf("%d", &unit[i].date_service.month)) != 1 || unit[i].date_service.month < 1 || unit[i].date_service.month > 12)
        {
            printf("Enter a valid month (1-12): ");
            while (getchar() != '\n')
                ;
        }

        printf("Year: ");
        while ((scanf("%d", &unit[i].date_service.year)) != 1 || unit[i].date_service.year < 2025 || unit[i].date_service.year > 2026)
        {
            printf("Enter a valid year (2025-2026): ");
            while (getchar() != '\n')
                ;
        }

        long pos = ftell(data);
        fwrite(&unit[i], sizeof(struct vehicle), 1, data);

        struct index_file newIndex;
        strncpy(newIndex.plate, unit[i].plate, sizeof(newIndex.plate));
        newIndex.cost = unit[i].cost;
        newIndex.position = pos;

        fwrite(&newIndex, sizeof(struct index_file), 1, index);
    }

    fclose(data);
    fclose(index);
    printf("\nData successfully saved to data.bin.\n");
}

void showRecords(void)
{
    FILE *data = fopen(DATA_F "data.bin", "rb");
    if (!data)
    {
        perror("Error opening data file");
        return;
    }

    FILE *index = fopen(DATA_F "index.bin", "rb");
    if (!index)
    {
        perror("Error opening index file");
        fclose(data);
        return;
    }

    printf("\n----- VEHICLE RECORDS -----\n");

    struct vehicle temp;
    struct index_file idx;
    int i = 1;

    while (fread(&idx, sizeof(struct index_file), 1, index) == 1)
    {
        fseek(data, idx.position, SEEK_SET);

        if (fread(&temp, sizeof(struct vehicle), 1, data) != 1)
        {
            printf("Error reading data at position %ld\n", idx.position);
            continue;
        }

        printf("\n--- VEHICLE %d ---\n", i++);
        printf("Owner Name: %s\nvehicle model: %d\nPlate: %s\nService: %s\nCost: %ld\nService date: %02d/%02d/%04d\n",
               temp.owner, temp.model, temp.plate, temp.service, temp.cost,
               temp.date_service.day, temp.date_service.month, temp.date_service.year);
    }

    fclose(data);
    fclose(index);
}

void searchRecord(void)
{
    FILE *data = fopen(DATA_F "data.bin", "rb");
    if (!data)
    {
        perror("Error opening data file");
        return;
    }

    FILE *index = fopen(DATA_F "index.bin", "rb");
    if (!index)
    {
        perror("Error opening index file");
        fclose(data);
        return;
    }

    char plate[16];
    printf("\nEnter the plate to search the record: ");
    if (scanf(" %15[^\n]", plate) != 1)
    {
        printf("Invalid input.\n");
        fclose(data);
        fclose(index);
        return;
    }

    struct index_file indexSearch;
    int found = 0;

    while (fread(&indexSearch, sizeof(struct index_file), 1, index) == 1)
    {
        if (strcmp(indexSearch.plate, plate) == 0)
        {
            struct vehicle temp;
            if (fseek(data, indexSearch.position, SEEK_SET) != 0)
            {
                perror("fseek failed");
                break;
            }
            if (fread(&temp, sizeof(struct vehicle), 1, data) != 1)
            {
                perror("fread failed on data file");
                break;
            }

            printf("\n--- VEHICLE FOUND ---\n");
            printf("Owner Name: %s\nvehicle model: %d\nPlate: %s\nService: %s\nCost: %ld\nService date: %02d/%02d/%04d\n", temp.owner, temp.model, temp.plate, temp.service, temp.cost,
                   temp.date_service.day, temp.date_service.month, temp.date_service.year);

            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nPLATE NOT FOUND: verify the plate.\n");

    fclose(data);
    fclose(index);
}

void updateRecord(int count)
{
    FILE *data = fopen(DATA_F "data.bin", "rb+");
    if (!data)
    {
        perror("Error opening data file");
        return;
    }

    FILE *index = fopen(DATA_F "index.bin", "rb+");
    if (!index)
    {
        perror("Error opening index file");
        fclose(data);
        return;
    }

    int pos;
    printf("\nWhich record do you want to update? (1-%d)\n", count);
    while (scanf("%d", &pos) != 1 || pos < 1 || pos > count)
    {
        printf("Enter a valid record: (1-%d)\n", count);
        while (getchar() != '\n')
            ;
    }
    struct index_file idx;

    fseek(index, (pos - 1) * sizeof(struct index_file), SEEK_SET);
    fread(&idx, sizeof(struct index_file), 1, index);

    struct vehicle temp;
    fseek(data, idx.position, SEEK_SET);
    fread(&temp, sizeof(struct vehicle), 1, data);

    printf("\n--- CURRENT VEHICLE DATA ---\n");
    printf("Owner Name: %s\nvehicle model: %d\nPlate: %s\nService: %s\nCost: %ld\nService date: %02d/%02d/%04d\n", temp.owner, temp.model, temp.plate, temp.service, temp.cost,
           temp.date_service.day, temp.date_service.month, temp.date_service.year);

    fseek(index, idx.position, SEEK_SET);

    struct vehicle update;
    printf("\n--- VEHICLE %d ---\n", pos);

    printf("Enter name of the owner: ");
    scanf(" %[^\n]", update.owner);

    printf("Enter the vehicle model: ");
    while ((scanf("%d", &update.model)) != 1 || update.model < 1885)
    {
        printf("Enter a valid value (integer > 1885).\n");
        while (getchar() != '\n')
            ;
    }

    printf("Enter the plate (Format: LLL-NNN):\n");
    do
    {
        input_and_format_plate(update.plate);

        if (is_plate_duplicate(update.plate))
        {
            printf("Error: The plate '%s' is already registered. Please enter a unique plate.\n", update.plate);
        }
    } while (is_plate_duplicate(update.plate));

    printf("Enter the service: ");
    select_service(update.service);

    printf("Enter the cost: ");
    while ((scanf("%ld", &update.cost)) != 1 || update.cost < 0)
    {
        printf("Enter a valid positive number.\n");
        while (getchar() != '\n')
            ;
    }

    printf("--Enter the date--\n");
    printf("Day: ");
    while ((scanf("%d", &update.date_service.day)) != 1 || update.date_service.day < 1 || update.date_service.day > 31)
    {
        printf("Enter a valid day (1-31): ");
        while (getchar() != '\n')
            ;
    }

    printf("Month: ");
    while ((scanf("%d", &update.date_service.month)) != 1 || update.date_service.month < 1 || update.date_service.month > 12)
    {
        printf("Enter a valid month (1-12): ");
        while (getchar() != '\n')
            ;
    }

    printf("Year: ");
    while ((scanf("%d", &update.date_service.year)) != 1)
    {
        printf("Enter a valid year (positive integer): ");
        while (getchar() != '\n')
            ;
    }

    fseek(data, idx.position, SEEK_SET);
    fwrite(&update, sizeof(struct vehicle), 1, data);

    struct index_file newIndex;
    strcpy(newIndex.plate, update.plate);
    newIndex.cost = update.cost;
    newIndex.position = idx.position;

    fseek(index, (pos - 1) * sizeof(struct index_file), SEEK_SET);
    fwrite(&newIndex, sizeof(struct index_file), 1, index);

    fclose(data);
    fclose(index);
    printf("\nData successfully saved to data.bin.\n");
}

void deleteRecord(int count)
{
    FILE *data = fopen(DATA_F "data.bin", "rb");
    if (!data)
    {
        perror("Error opening data file");
        return;
    }

    FILE *index = fopen(DATA_F "index.bin", "rb");
    if (!index)
    {
        perror("Error opening index file");
        fclose(data);
        return;
    }

    int pos_to_delete;
    printf("\nWhich record do you want to delete? (1-%d)\n", count);
    while (scanf("%d", &pos_to_delete) != 1 || pos_to_delete < 1 || pos_to_delete > count)
    {
        printf("Enter a valid record: (1-%d)\n", count);
        while (getchar() != '\n')
            ;
    }

    struct index_file idx_to_delete;

    fseek(index, (pos_to_delete - 1) * sizeof(struct index_file), SEEK_SET);
    fread(&idx_to_delete, sizeof(struct index_file), 1, index);

    struct vehicle temp_vehicle;
    fseek(data, idx_to_delete.position, SEEK_SET);
    fread(&temp_vehicle, sizeof(struct vehicle), 1, data);

    printf("\n--- VEHICLE DATA TO DELETE ---\n");
    printf("Owner Name: %s\nVehicle model: %d\nPlate: %s\nService: %s\nCost: %ld\nService date: %02d/%02d/%04d\n",
           temp_vehicle.owner, temp_vehicle.model, temp_vehicle.plate, temp_vehicle.service, temp_vehicle.cost,
           temp_vehicle.date_service.day, temp_vehicle.date_service.month, temp_vehicle.date_service.year);

    printf("\nAre you sure you want to delete the record %d?\n (YES: 1. NO: 0)\n", pos_to_delete);
    int confirmation;
    while (scanf("%d", &confirmation) != 1 || confirmation > 1 || confirmation < 0)
    {
        printf("Enter a valid option: (0 or 1): ");
        while (getchar() != '\n')
            ;
    }

    if (confirmation == 0)
    {
        fclose(data);
        fclose(index);
        printf("\nOperation canceled\n");
        return;
    }

    if (confirmation == 1)
    {
        FILE *tempData = fopen(DATA_F "temp_data.bin", "wb");
        FILE *tempIndex = fopen(DATA_F "temp_index.bin", "wb");
        if (!tempData || !tempIndex)
        {
            perror("Error creating temporary files");
            fclose(data);
            fclose(index);
            return;
        }

        rewind(data);
        rewind(index);

        struct index_file currentIndex;
        int new_index_pos = 0;

        for (int i = 0; i < count; i++)
        {
            if (fread(&currentIndex, sizeof(struct index_file), 1, index) != 1)
            {
                break;
            }

            if (i != (pos_to_delete - 1))
            {
                struct vehicle current_vehicle;
                fseek(data, currentIndex.position, SEEK_SET);
                fread(&current_vehicle, sizeof(struct vehicle), 1, data);

                long new_data_pos = ftell(tempData);
                fwrite(&current_vehicle, sizeof(struct vehicle), 1, tempData);

                currentIndex.position = new_data_pos;
                fwrite(&currentIndex, sizeof(struct index_file), 1, tempIndex);

                new_index_pos++;
            }
        }

        fclose(data);
        fclose(index);
        fclose(tempData);
        fclose(tempIndex);

        remove(DATA_F "data.bin");
        remove(DATA_F "index.bin");
        rename(DATA_F "temp_data.bin", DATA_F "data.bin");
        rename(DATA_F "temp_index.bin", DATA_F "index.bin");

        printf("\nRecord %d deleted successfully. Total records now: %d\n", pos_to_delete, new_index_pos);
    }
}

void sortRecord(int count)
{
    FILE *index = fopen(DATA_F "index.bin", "r+b");
    if (!index)
    {
        perror("Error opening index file");
        return;
    }

    struct index_file records[2];

    printf("\nHow do you want to sort the records?\n");
    printf("1. By cost ascending\n");
    printf("2. By cost descending\n");

    int option;
    while (scanf("%d", &option) != 1 || option < 1 || option > 2)
    {
        printf("Enter a valid option (1–2): ");
        while (getchar() != '\n')
            ;
    }

    for (int i = 0; i < count - 1; i++)
    {
        int swapped = 0;

        for (int j = 0; j < count - i - 1; j++)
        {
            fseek(index, j * sizeof(struct index_file), SEEK_SET);
            fread(&records[0], sizeof(struct index_file), 1, index);

            fseek(index, (j + 1) * sizeof(struct index_file), SEEK_SET);
            fread(&records[1], sizeof(struct index_file), 1, index);

            int condition = 0;

            if (option == 1)
                condition = (records[0].cost > records[1].cost);

            if (option == 2)
                condition = (records[0].cost < records[1].cost);

            if (condition)
            {
                fseek(index, j * sizeof(struct index_file), SEEK_SET);
                fwrite(&records[1], sizeof(struct index_file), 1, index);

                fseek(index, (j + 1) * sizeof(struct index_file), SEEK_SET);
                fwrite(&records[0], sizeof(struct index_file), 1, index);

                swapped = 1;
            }
        }
        if (!swapped)
            break;
    }

    rewind(index);
    fclose(index);

    printf("\nRecords successfully sorted.\n");
}

int main()
{
    int n = 1;
    struct vehicle inform[n];
    struct index_file index[n];

    FILE *data = fopen(DATA_F "data.bin", "rb");
    if (!data)
    {
        perror("Error oppening file");
        return 0;
    }
    fseek(data, 0, SEEK_END);
    int size = ftell(data);
    fclose(data);
    int count = size / sizeof(struct vehicle);
    if (count == 0)
    {
        printf("----Welcome To Vehicle Maintenance Record System (VMRS)!----\nPress (ENTER) to make the test record ;).\n");
        while (getchar() != '\n')
            ;
        fill(inform, n);
    }

    int option;

    do
    {
        FILE *index = fopen(DATA_F "index.bin", "rb");
        if (!index)
        {
            perror("Error oppening file");
            fclose(data);
            return 0;
        }
        fseek(index, 0, SEEK_END);
        size = ftell(index);
        fclose(index);
        count = size / sizeof(struct index_file);

        if (count == 0)
        {
            printf("\n---- You don't have any record bro =| ----\nMake one to start again ;D\n Or [ctrl+c] to exit.\n");
            while (getchar() != '\n')
                ;
            fill(inform, n);
        }

        printf("\nSelect an option: \n ");
        printf("1 - Registers a new maintenance in the file.\n ");
        printf("2 - Displays all maintenance records.\n ");
        printf("3 - Searches for a vehicle by license plate\n ");
        printf("4 - Changes service details or cost.\n ");
        printf("5 - Deletes a specific maintenance record.\n ");
        printf("6 - Sorts records by cost.\n ");
        printf("0 - EXIT.\n");

        while ((scanf("%d", &option)) != 1 || option < 0 || option > 7)
        {
            printf("enter a valid value (0-7).\n");
            while (getchar() != '\n')
                ;
        }

        while (getchar() != '\n')
            ;

        switch (option)
        {
        case 1:
            printf("Enter the number of vehicles to record: ");
            while ((scanf("%d", &n)) != 1 || n < 1)
            {
                printf("Enter a valid positive number.\n");
                while (getchar() != '\n')
                    ;
            }
            fill(inform, n);
            break;
        case 2:
            showRecords();
            break;
        case 3:
            searchRecord();
            break;
        case 4:
            updateRecord(count);
            break;
        case 5:
            deleteRecord(count);
            break;
        case 6:
            sortRecord(count);
            break;
        default:
            break;
        }
    } while (option != 0);
    return 0;
}