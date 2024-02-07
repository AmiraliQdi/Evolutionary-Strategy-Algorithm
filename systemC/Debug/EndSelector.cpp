#include <systemc.h>
#include "Variables.h"
#include "random"

SC_MODULE(EndSelector) {
    sc_in_clk clock;
    sc_in<double> best_answer[POP_COUNT][ANSWER_LEN];
    sc_in<int> selector;
    sc_out<double> loop_matrix[POP_COUNT][ANSWER_LEN];
    int vari;

    void select() {
        wait(2);
        if (selector.read() == 0) {
            cout << "iterations: " << vari << endl;
            for (int i = 0; i < POP_COUNT; i++) {
                for (int j = 0; j < ANSWER_LEN; j++) {
                    loop_matrix[i][j].write(best_answer[i][j].read());
                }
            }
        }
        else {
            cout << "ENDED" << endl;
        }
    }

    // Constructor
    SC_CTOR(EndSelector) {
        vari = rand();
        SC_THREAD(select);
        //sensitive << best_answer[POP_COUNT][ANSWER_LEN - 1];
        sensitive << clock;
    }
};
