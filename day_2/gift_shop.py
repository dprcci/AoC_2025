
EXAMPLE = "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124"

def gift_shop():
    with open("gift_shop.txt","r") as f:
        file_content = f.read()

    all_invalid_ids = []
    for f in file_content.split(','):
        lower, upper = f.strip().split('-')
        nb_digits_lower = len(lower)
        nb_digits_upper = len(upper)

        if nb_digits_lower == nb_digits_upper and nb_digits_lower % 2 != 0:
            continue

        fixed_start_len = 0
        for low, high in zip(lower, upper):
            if low != high:
                break
            fixed_start_len += 1

        if fixed_start_len >= (half := (nb_digits_upper // 2)):
            pattern = lower[:half]
            if int(lower) <= (res := int(2 * pattern)) <= int(upper):
                all_invalid_ids.append(res)
            continue
        
        if nb_digits_lower % 2 != 0:
            lower = str(10 ** nb_digits_lower)
        
        if nb_digits_upper % 2 != 0:
            upper = str(10 ** (nb_digits_upper-1) - 1)
        
        possible_pair_sizes = [size for size in range(nb_digits_lower, nb_digits_upper+1) if size % 2 == 0]
        for size in possible_pair_sizes:
            middle_idx = size // 2 
            fixed_start = lower[:fixed_start_len]
            
            lower_left_part = int(lower[:middle_idx])
            upper_left_part = int(upper[:middle_idx])
            
            results: list[str] = []
            for subset in range(int(str(lower_left_part)[fixed_start_len:]), int(str(upper_left_part)[fixed_start_len:])+1):
                if int(lower) <= int(2 * (fixed_start + str(subset))) <= int(upper):
                    results.append(int(2 * (fixed_start + str(subset))))

            all_invalid_ids.extend(results)
    return sum(all_invalid_ids)


def gift_shop_part_2():
    with open("gift_shop.txt","r") as f:
        file_content = f.read()

    all_invalid_ids = []
    for f in file_content.split(','):
        lower, upper = f.strip().split('-')
        all_invalid_ids += [*find_invalid(lower, upper)]
    
    return sum(all_invalid_ids)


def find_invalid_subrange(lower: str, upper: str):
        assert len(lower) == len(upper)
        
        invalid_subrange = set()
        digits_size = len(lower)
        if digits_size == 1:
            return set()
        
        pair_sizes = [i for i in range(1, (digits_size // 2) + 1) if digits_size % i == 0]
        for pair_size in pair_sizes:

            min_possible = int(lower[:pair_size])
            max_possible = int(upper[:pair_size])

            for num in range(int(min_possible), int(max_possible) + 1):
                sequence = str(num).zfill(pair_size)
                repeated = int(sequence * (digits_size // pair_size))
                
                if int(lower) <= repeated <= int(upper):
                    invalid_subrange.add(repeated)
    
        return invalid_subrange


def find_invalid(lower: str, upper: str):
    invalid_ids = set()
    
    nb_digits_lower = len(lower)
    nb_digits_upper = len(upper)

    if nb_digits_upper - nb_digits_lower > 1:
        for nb_digits_all in range(nb_digits_lower + 1, nb_digits_upper):
            low = 10**(nb_digits_all-1)
            upper = 10**(nb_digits_all) - 1
            invalid_ids |= find_invalid_subrange(low, upper)

    ranges = [(lower, upper)]    
    if nb_digits_lower != nb_digits_upper:
        lower_range = (lower, str((10**nb_digits_lower) - 1))
        upper_range = (str(10**(nb_digits_upper-1)), upper)
        ranges = [lower_range, upper_range]
    
    for low, up in ranges:
        invalid_ids |= find_invalid_subrange(low, up)
    
    #print("PAIR:", lower, upper, invalid_ids)
    return invalid_ids

if __name__ == '__main__':
    print("Invalid IDs Sum 1: ", gift_shop())
    print("Invalid IDs Sum 2: ", gift_shop_part_2())