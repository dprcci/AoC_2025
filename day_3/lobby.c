#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int find_max_joltage(const char * const bank, size_t bank_size){
    int left_digit = 0;
    int right_digit = 0;
    for(size_t i = 0; i < bank_size; i++){
        const char joltage_char = *(bank + i * sizeof(char));
        printf("%c",  joltage_char);
        // https://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
        int joltage_integer = ((int)joltage_char) - ((int)'0');
        
        if (joltage_integer > left_digit && i < (bank_size - 1)){
            left_digit = joltage_integer;
            right_digit = -1;
        } else if(joltage_integer > right_digit){
            right_digit = joltage_integer;
        }
    }
    return left_digit * 10 + right_digit;
}

int compute_joltage(const char * const battery_banks, size_t nb_battery_banks, size_t bank_size){
    int total_joltage = 0;
    // room for terminating character
    const size_t line_length = bank_size + 1;
    for (size_t i = 0; i < nb_battery_banks; i++){
        const char * current_bank = battery_banks + (line_length * i);
        total_joltage += find_max_joltage(current_bank, bank_size);
    }
    return total_joltage;
}

int read_input(const char* const filename, const char ** battery_banks, size_t * const  nb_battery_banks, size_t * const bank_size){
    FILE *file;
    
    size_t line_length = 0;
    size_t total_size;
    int num_lines;
    
    file = fopen(filename, "r");
    if (file == NULL) {
        return 1;
    }
    
    // assuming same line length for all lines
    int c;
    while ((c = fgetc(file)) != EOF && c != '\n') line_length++;
    
    fseek(file, 0, SEEK_END);
    total_size = ftell(file);
    
    if (line_length > 0) {
        num_lines = total_size / (line_length + 1);
    }

    // need room for null terminating character
    line_length += 1;
    
    char * file_content = malloc(num_lines * line_length * sizeof(char));
    if (battery_banks == NULL) {
        fclose(file);
        return 1;
    }
    
    fseek(file, 0, SEEK_SET);
    int idx = 0;
    // fgets read n - 1 chars. So need to account for + 1
    while (fgets(file_content + (idx++ * line_length * sizeof(char)), line_length + 1, file) != NULL);
    
    fclose(file);

    *battery_banks = file_content;
    *nb_battery_banks = num_lines;
    *bank_size = line_length - 1; // ignore the null terminating character
    return 0;
}

long long int integer_pow(int base, unsigned int exp) {
    if (exp == 0) {
        return 1;
    }

    long long int result = 1;
    for (unsigned int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

int find_next_bigger_idx(const int * const hash_map_entry, int hash_map_entry_size, int current_bank_idx){
    size_t low = 0;
    size_t high = hash_map_entry_size - 1;
    int best_canditate = -1;
    while (low <= high){        
        size_t middle = low + (high - low) / 2;
        int bank_idx = hash_map_entry[middle];
        if(bank_idx == current_bank_idx){
            return middle;
        }
        if (bank_idx > current_bank_idx){
            best_canditate = middle;
            high = middle - 1; 
        } else {
            low = middle + 1;
        }
    }
    return best_canditate;
}

long joltage_static_friction(const char * const bank, size_t bank_size, size_t nb_batteries){
    int * temp_result = calloc(nb_batteries, sizeof(int));
    if (temp_result == NULL){
        return -1;
    }

    size_t nb_batteries_found = 0;
    for (size_t i = 0; i < bank_size; i++){
        const char joltage_char = *(bank + i * sizeof(char));
        int joltage_integer = ((int)joltage_char) - ((int)'0');
        if (nb_batteries_found == nb_batteries && joltage_integer <= temp_result[nb_batteries_found-1]){
            continue;
        }
        size_t remaining_size = bank_size - i;
        size_t max_depth = remaining_size >= nb_batteries ? nb_batteries : remaining_size;
        size_t depth = nb_batteries - nb_batteries_found;
        while (nb_batteries_found > 0 && joltage_integer > temp_result[nb_batteries_found-1] && depth < max_depth){
            nb_batteries_found -= 1;
            depth += 1;
        }
        temp_result[nb_batteries_found++] = joltage_integer;
    }

    long result = 0;
     for(size_t i = 0; i < nb_batteries; i++){
        result += temp_result[i] * integer_pow(10, (nb_batteries - i) - 1);
    }
    free(temp_result);
    return result;
}


long compute_joltage_static_friction(const char * const battery_banks, size_t nb_battery_banks, size_t bank_size, size_t nb_batteries){
    long long unsigned total_joltage = 0;
    // room for terminating character
    const size_t line_length = bank_size + 1;
    for (size_t i = 0; i < nb_battery_banks; i++){
        const char * current_bank = battery_banks + (line_length * i);
        total_joltage += joltage_static_friction(current_bank, bank_size, nb_batteries);
    }
    return total_joltage;
}

//  if (joltage_integer > current_best_joltage){
//                 current_best_joltage = joltage_integer;
//                 current_start = bank_idx;
//                 if (current_best_joltage == 9) break;
//             } 


int main(int argc, char* argv[]) {
    const char *filename = (argc > 1) ? argv[1] : "example.txt";
    
    const char * battery_banks = NULL;
    size_t nb_battery_banks = 0;
    size_t bank_size = 0;
    
    int ok = read_input(filename, &battery_banks, &nb_battery_banks, &bank_size);
    if(ok != 0){
        return ok;
    }
    
    //int result = compute_joltage(battery_banks, nb_battery_banks, bank_size);
    //printf("total joltage: %d", result);

    long result = compute_joltage_static_friction(battery_banks, nb_battery_banks, bank_size, 12);
    printf("total joltage friction: %ld", result);

    free((void*)battery_banks);
}