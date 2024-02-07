function reproduction_matrix = performReproduction(pop_matrix_sorted, info_matrix, POP_COUNT, ANSWER_LEN, CHILD_COUNT, MAX_WEIGHT)
   
    reproduction_matrix = zeros(POP_COUNT + (CHILD_COUNT*2), ANSWER_LEN);
    reproduction_matrix(1:POP_COUNT, :) = pop_matrix_sorted;
    for c = 1:CHILD_COUNT
        selectedRows = [-1, -1];
        for i = 1:2
            selectedRow = randi([1, POP_COUNT], 1, 1);
            weight = evaluateRow(pop_matrix_sorted(selectedRow, :), info_matrix(2, :));
            while weight >= MAX_WEIGHT
                selectedRow = randi([1, POP_COUNT], 1, 1);
                weight = evaluateRow(pop_matrix_sorted(selectedRow, :), info_matrix(2, :));
            end
            selectedRows(i) = selectedRow;
        end
        cross_over_index = randi([1, ANSWER_LEN], 1, 1);
        for i = 1:cross_over_index
            reproduction_matrix(POP_COUNT + (2 * (c - 1)) + 1, i) = pop_matrix_sorted(selectedRows(1), i);
            reproduction_matrix(POP_COUNT + (2 * (c - 1)) + 2, i) = pop_matrix_sorted(selectedRows(2), i);
        end
        for i = cross_over_index+1:ANSWER_LEN
            reproduction_matrix(POP_COUNT + (2 * (c - 1)) + 1, i) = pop_matrix_sorted(selectedRows(2), i);
            reproduction_matrix(POP_COUNT + (2 * (c - 1)) + 2, i) = pop_matrix_sorted(selectedRows(1), i);
        end
    end
end

function sum_value = evaluateRow(row, info_row)
    sum_value = sum(row .* info_row);
end
