import sys
from functools import reduce

def compute_equation_results(equations, operators):
    operations = {
        "*": lambda a, b: a*b,
        "+": lambda a, b: a+b
    }

    total_sum = 0
    for column, equation in enumerate(equations):
        total_sum += reduce(operations[operators[column]], equation)
    
    return total_sum


def solve_equations(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()

    operators = lines[-1].strip().replace(' ', '')
    equations = [[None] * (len(lines) - 1) for _ in range(len(operators))]
    for idx, line in enumerate(lines[:-1]):
        column = 0
        number = []
        print(line)
        for char in line:
            if char != ' ' and char != '\n':
                number.append(char)
            elif number:
                equations[column][idx] = int(''.join(number))
                column += 1
                number.clear()

    assert len(operators) == len(equations)
    return compute_equation_results(equations, operators)


def solve_right_to_left(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    
    operators = lines[-1].strip().replace(' ', '')
    nb_cols = len(lines[-1])
    equations = [[] for _ in range(len(operators))]
    
    assert(all(len(l) == nb_cols for l in lines[:-1]))

    equation_nb = 0
    for col_ix in range(nb_cols):
        number = []
        for row_idx in range(len(lines) - 1):
            char = lines[row_idx][col_ix]
            if char != ' ' and char != '\n':
                number.append(char)
        if number:            
            equations[equation_nb].append(int(''.join(number)))
            number.clear()
        else:
            equation_nb += 1
    
    return compute_equation_results(equations, operators)


if __name__ == '__main__':
    filename = sys.argv[1] if len(sys.argv) > 1 else 'example.txt'
    result = solve_equations(filename)
    print(f'Found total problems answer: {result}')
    result = solve_right_to_left(filename)
    print(f'Found total problems answer right to left: {result}')