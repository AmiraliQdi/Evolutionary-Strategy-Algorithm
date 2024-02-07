#include <systemc.h>
#include "Variables.h"  
#include "random"

SC_MODULE(ReproductionModule) {
    sc_in<double> pop_matrix_sorted[POP_COUNT][ANSWER_LEN];
    sc_in<int> info_matrix[2][ANSWER_LEN];
    sc_in_clk clock;
    
    sc_out<double> reproduction_matrix[POP_COUNT + (CHILD_COUNT*2)][ANSWER_LEN];

    int generateRandomNumber() {
        
        std::random_device rd;
        std::mt19937 gen(rd());
        

        std::uniform_int_distribution<int> dis(0, 100);

        return dis(gen);
    }

    void performReproduction() {
        //cout << "REPRODUCTION MODULE,T" <<endl;
        double threshold = MAX_WEIGHT; 

        for (int i = 0; i < POP_COUNT; i++) {
            for (int j = 0; j < ANSWER_LEN; j++) {
                reproduction_matrix[i][j].write(pop_matrix_sorted[i][j].read());
            }
        }
        for (int c = 0; c < CHILD_COUNT; c++) {
            int selectedRows[2] = { -1, -1 };
            for (int i = 0; i < 2; ++i) {
                do {
                    selectedRows[i] = generateRandomNumber() % POP_COUNT;
                } while (evaluateRow(pop_matrix_sorted[selectedRows[i]], info_matrix[1]) >= threshold);
            }

            int cross_over_index = generateRandomNumber() % ANSWER_LEN;

            for (int i = 0; i < cross_over_index; i++) {
                reproduction_matrix[POP_COUNT + (2 * c)][i].write(pop_matrix_sorted[selectedRows[0]][i].read());
                reproduction_matrix[POP_COUNT + (2 * c) + 1][i].write(pop_matrix_sorted[selectedRows[1]][i].read());
            }

            for (int i = cross_over_index; i < ANSWER_LEN; i++) {
                reproduction_matrix[POP_COUNT + (c * 2)][i].write(pop_matrix_sorted[selectedRows[1]][i].read());
                reproduction_matrix[POP_COUNT + (c * 2) + 1][i].write(pop_matrix_sorted[selectedRows[0]][i].read());
            }
        }
    }

    double evaluateRow(sc_in<double> row[ANSWER_LEN],sc_in<int> info_row[ANSWER_LEN]) {
        double sum = 0.0;
        for (int i = 0; i < ANSWER_LEN; ++i) {
            sum += row[i] * info_row[i];
        }
        return sum;
    }

    SC_CTOR(ReproductionModule) {
        SC_METHOD(performReproduction);
        //sensitive << clock.pos();
        sensitive << pop_matrix_sorted[POP_COUNT - 1][ANSWER_LEN - 1];
    }
};
