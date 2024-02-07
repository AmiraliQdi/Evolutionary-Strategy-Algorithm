#include <systemc.h>
#include "Variables.h"

SC_MODULE(EvaluationWithChilesModule) {

    sc_in<double> pop_matrix[POP_COUNT + (CHILD_COUNT * 2)][ANSWER_LEN];
    sc_in<int> info_matrix[2][ANSWER_LEN];
    sc_in_clk clock;

    sc_out<double> sorted_matrix[POP_COUNT + (CHILD_COUNT * 2)][ANSWER_LEN];

    void evaluateMatrices() {
        //cout << "EvaluationWithChild MODULE" << endl;
        std::vector<std::pair<double, int>> rowSumPairs;

        for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); ++i) {
            double sum = 0.0;
            for (int j = 0; j < ANSWER_LEN; ++j) {
                sum += pop_matrix[i][j] * info_matrix[0][j]; 
            }
            rowSumPairs.push_back(std::make_pair(sum, i));
        }

        std::sort(rowSumPairs.begin(), rowSumPairs.end(), std::greater<std::pair<double, int>>());

        for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); ++i) {
            int originalRowIndex = rowSumPairs[i].second;
            for (int j = 0; j < ANSWER_LEN; ++j) {
                sorted_matrix[i][j].write(pop_matrix[originalRowIndex][j]);
            }
        }
    }

    SC_CTOR(EvaluationWithChilesModule) {

        SC_METHOD(evaluateMatrices);
        //sensitive << clock.pos();
        sensitive << pop_matrix[POP_COUNT + (CHILD_COUNT * 2) - 1][ANSWER_LEN - 1];
    }
};
