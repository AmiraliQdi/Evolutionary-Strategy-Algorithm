function sorted_matrix = evaluateAndSortMatrix(pop_matrix, info_matrix)
    POP_COUNT = 30;
    CHILD_COUNT = 0; 
    ANSWER_LEN = 9;

    scores = zeros(POP_COUNT + (CHILD_COUNT * 2), 1);
    indices = (1:(POP_COUNT + (CHILD_COUNT * 2)))';

    for i = 1:(POP_COUNT + (CHILD_COUNT * 2))
        scores(i) = sum(pop_matrix(i, :) .* info_matrix(1, :));
    end

    score_pairs = [scores, indices];
    [~, sorted_order] = sort(score_pairs(:, 1), 'descend');
    sorted_matrix = pop_matrix(sorted_order, :);
end
