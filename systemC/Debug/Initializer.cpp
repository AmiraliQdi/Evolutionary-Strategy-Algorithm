#include <systemc.h>
#include "Variables.h"
#include "random"

SC_MODULE(Initializer) {
    sc_in<bool> start;
    bool doInit = true;
    std::default_random_engine generator;
    sc_out<double> pop_matrix[POP_COUNT][ANSWER_LEN];

    double generateRandomNumber() {
 
        std::uniform_real_distribution<double> dist(0, 1);

        return dist(generator);
      }


    void createRandomFloatMatrix() {
        if (doInit) {
            //cout << "INIT MODULE" << endl;
            for (int i = 0; i < POP_COUNT; ++i) {
                for (int j = 0; j < ANSWER_LEN; ++j) {
                    pop_matrix[i][j] = generateRandomNumber();
                }
            }
            doInit = false;
        }
    }

    SC_CTOR(Initializer) {
        SC_METHOD(createRandomFloatMatrix);
        sensitive << start;
    }
};
