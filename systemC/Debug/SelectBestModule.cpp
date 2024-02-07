#include <systemc.h>
#include "Variables.h"

SC_MODULE(SelectBestModule) {
    sc_in_clk clock;
    sc_in<double> pop_matrix[POP_COUNT + (CHILD_COUNT * 2)][ANSWER_LEN];
    double sorted_pop[POP_COUNT + (CHILD_COUNT * 2)][ANSWER_LEN];
    sc_in<int> info_matrix[2][ANSWER_LEN];
    sc_out<double> sorted_best[POP_COUNT][ANSWER_LEN];
    sc_out<int> selector;

    double best_answer[ANSWER_LEN];
    double last_value;
    int end_counter = 0;

    void select_best() {
        //cout << "SELECT BEST MODULE" << endl;
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
                sorted_pop[i][j] = pop_matrix[originalRowIndex][j];
            }
        }

        for (int i = 0; i < POP_COUNT; ++i) {
            for (int j = 0; j < ANSWER_LEN; ++j) {
                sorted_best[i][j].write(sorted_pop[i][j]);
            }
        }
        double new_best_value = 0;
        for (int i = 0; i < ANSWER_LEN; i++) {
            best_answer[i] = sorted_pop[0][i];
            new_best_value += sorted_pop[0][i] * info_matrix[0][i];
        }

        for (int i = 0; i < ANSWER_LEN; i++) {
            cout << best_answer[i] << " ,";
        }
        cout << " BEST VALUE = " << new_best_value << endl;

        if (( abs(last_value - new_best_value) <= 0.0005) && last_value != 0) {
            end_counter += 1;
            cout << "SAME ANSWER" << endl;
        }
        else {
            last_value = new_best_value;
        }

        if (end_counter == 5) {
            selector.write(1);
            // TODO
        }
        else {
            selector.write(0);
        }
    }

    SC_CTOR(SelectBestModule) {
        last_value = 0;
        SC_METHOD(select_best);
        //sensitive << clock.pos();
        sensitive << pop_matrix[POP_COUNT + (CHILD_COUNT * 2) - 1][ANSWER_LEN - 1];
    }
};
