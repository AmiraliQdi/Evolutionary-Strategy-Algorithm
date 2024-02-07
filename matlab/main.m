% Main file


initializer = Initializer();

initializer.createRandomFloatMatrix();
disp('Initial Random Matrix:');
disp(initializer.pop_matrix);

info_matrix = [
    6, 5, 8, 9, 6, 7, 3, 6, 8;
    2, 3, 6, 7, 5, 9, 3, 4, 5
];

sorted_matrix = Evaluation(initializer.pop_matrix, info_matrix);
disp('Sorted Matrix:');
disp(sorted_matrix);



POP_COUNT = 30;
ANSWER_LEN = 9;
CHILD_COUNT = 5;
MAX_WEIGHT = 25.0;
disp('Reproduction Matrix:');
disp(initializer.pop_matrix);
reproduction_matrix = performReproduction(sorted_matrix, info_matrix, POP_COUNT, ANSWER_LEN, CHILD_COUNT, MAX_WEIGHT);



