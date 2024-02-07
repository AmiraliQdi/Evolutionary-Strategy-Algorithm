#include <systemc.h>
#include "Variables.h"
#include <iostream>
#include <iterator>
#include <random>
#include "cmath"

SC_MODULE(MutingModule) {

    sc_in<double> pop_matrix[POP_COUNT + (CHILD_COUNT * 2)][ANSWER_LEN];
    sc_in<int> info_matrix[2][ANSWER_LEN];
    sc_in_clk clock;

    double standart_dev[POP_COUNT + (CHILD_COUNT * 2)][ANSWER_LEN];
    double rand_var;


    sc_out<double> muted_pop[POP_COUNT + (CHILD_COUNT * 2)][ANSWER_LEN];


    void performMuting() {

        const double mean = 0.0;
        const double stddev = 1;
        std::default_random_engine generator;
        std::normal_distribution<double> dist(mean, stddev);
        rand_var = dist(generator);


        for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); i++) {
            for (int j = 0; j < ANSWER_LEN; j++) {
                muted_pop[i][j].write(standart_dev[i][j] * dist(generator));
                standart_dev[i][j] *= exp(rand_var - dist(generator));
            }
        }
    }

    SC_CTOR(MutingModule) {
        const double mean = 0.0;
        const double stddev = 1;
        std::default_random_engine generator;
        std::normal_distribution<double> dist(mean, stddev);
        

        for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); i++) {
            for (int j = 0; j < ANSWER_LEN; j++) {
                standart_dev[i][j] = 1;
            }
        }

        SC_METHOD(performMuting);
        //sensitive << clock.pos();
        sensitive << pop_matrix[POP_COUNT + (CHILD_COUNT * 2) - 1][ANSWER_LEN - 1];
    }
};
