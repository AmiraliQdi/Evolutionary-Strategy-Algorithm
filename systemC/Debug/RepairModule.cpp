#include <systemc.h>
#include "Variables.h"

SC_MODULE(RepairModule) {
    sc_in_clk clock;
    sc_in<double> pop_matrix[POP_COUNT + (CHILD_COUNT * 2)][ANSWER_LEN];
    sc_out<double> repaired_pop[POP_COUNT + (CHILD_COUNT * 2)][ANSWER_LEN];


    void performRepair() {
        //cout << "REPAIR MODULE" << endl;
        for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); i++) {
            for (int j = 0; j < ANSWER_LEN; j++) {
                if (pop_matrix[i][j] >= 1.00) {
                    repaired_pop[i][j].write(1.00);
                    
                }
                else if (pop_matrix[i][j] < 0.00){
                    repaired_pop[i][j].write(0.00);
                }
                else {
                    repaired_pop[i][j].write(pop_matrix[i][j].read());
                }
            }
        }

    }

    // Constructor
    SC_CTOR(RepairModule) {
        
        SC_METHOD(performRepair);
        //sensitive << clock.pos();
        sensitive << pop_matrix[POP_COUNT + (CHILD_COUNT * 2) - 1][ANSWER_LEN - 1];
    }
};
