#include <systemc.h>
#include "Initializer.cpp"
#include "EvaluationModule.cpp"
#include "ReproductionModule.cpp"
#include "MutingModule.cpp"
#include "EvaluationWithChildModule.cpp"
#include "RepairModule.cpp"
#include "SelectBestModule.cpp"
#include"EndSelector.cpp"


int sc_main(int argc, char* argv[]) {

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int const info_values[2][9] = {
    {6, 5, 8, 9, 6, 7, 3, 6, 8},
    {2, 3, 6, 7, 5, 9, 3, 4, 5}
    };


    sc_signal<bool> start;
    //start = false;
    sc_clock clock("clock", 1, SC_NS);
    sc_signal<double> pop_matrix[POP_COUNT][ANSWER_LEN];
    sc_signal<int> info_matrix[2][ANSWER_LEN];
    sc_signal<double, SC_MANY_WRITERS> sorted_matrix[POP_COUNT][ANSWER_LEN];
    sc_signal<double> pop_with_child[POP_COUNT + (CHILD_COUNT * 2)][ANSWER_LEN];
    sc_signal<double> muted_pop[POP_COUNT + (CHILD_COUNT * 2)][ANSWER_LEN];
    sc_signal<double> sorted_muted_pop[POP_COUNT + (CHILD_COUNT * 2)][ANSWER_LEN];
    sc_signal<double> repaired_pop[POP_COUNT + (CHILD_COUNT * 2)][ANSWER_LEN];
    sc_signal<double> sorted_best[POP_COUNT][ANSWER_LEN];

    sc_signal<int> selector;

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 9; ++j) {
            info_matrix[i][j] = info_values[i][j];
        }
    }


    // Instantiate modules
    Initializer initializer("Initializer");
    initializer.start(start);
    for (int i = 0; i < POP_COUNT; i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            initializer.pop_matrix[i][j](pop_matrix[i][j]);
        }
    }

    EvaluationModule evaluator("EvaluationModule");
    evaluator.clock(clock);


    for (int i = 0; i < POP_COUNT; i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            evaluator.pop_matrix[i][j](pop_matrix[i][j]);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            evaluator.info_matrix[i][j](info_matrix[i][j]);
        }

    }
    for (int i = 0; i < POP_COUNT; i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            evaluator.sorted_matrix[i][j](sorted_matrix[i][j]);
        }
    }

    // Repproduction
    ReproductionModule reproduction("ReproductionModule");
    reproduction.clock(clock);
    for (int i = 0; i < POP_COUNT; i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            reproduction.pop_matrix_sorted[i][j](sorted_matrix[i][j]);
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            reproduction.info_matrix[i][j](info_matrix[i][j]);
        }
    }

    for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            reproduction.reproduction_matrix[i][j](pop_with_child[i][j]);
        }
    }

    // Mutation
    MutingModule mutation("MutationModule");
    mutation.clock(clock);
    for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            mutation.pop_matrix[i][j](pop_with_child[i][j]);
        }
    }


    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            mutation.info_matrix[i][j](info_matrix[i][j]);
        }
    }

    for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            mutation.muted_pop[i][j](muted_pop[i][j]);
        }
    }

    EvaluationWithChilesModule evaluator_wchild("EvaluationWithChildModule");
    evaluator_wchild.clock(clock);


    for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            evaluator_wchild.pop_matrix[i][j](muted_pop[i][j]);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            evaluator_wchild.info_matrix[i][j](info_matrix[i][j]);
        }
    }
    for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            evaluator_wchild.sorted_matrix[i][j](sorted_muted_pop[i][j]);
        }
    }

    RepairModule repair("RepairModule");
    repair.clock(clock);

    for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            repair.pop_matrix[i][j](sorted_muted_pop[i][j]);
        }
    }

    for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            repair.repaired_pop[i][j](repaired_pop[i][j]);
        }
    }

    SelectBestModule select("SelectBestModule");
    select.clock(clock);
    select.selector(selector);
    for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            select.pop_matrix[i][j](repaired_pop[i][j]);
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            select.info_matrix[i][j](info_matrix[i][j]);
        }

    }


    for (int i = 0; i < POP_COUNT; i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            select.sorted_best[i][j](sorted_best[i][j]);
        }
    }

    EndSelector end_selector("EndSelector");
    end_selector.clock(clock);

    for (int i = 0; i < POP_COUNT; i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            end_selector.best_answer[i][j](sorted_best[i][j]);
        }
    }

    end_selector.selector(selector);

    for (int i = 0; i < POP_COUNT; i++) {
        for (int j = 0; j < ANSWER_LEN; j++) {
            end_selector.loop_matrix[i][j](sorted_matrix[i][j]);
        }
    }


    // Initialize SystemC 
    sc_start(1, SC_NS);

    start.write(true);

    // Display the generated matrix from Initializer
    cout << "Matrix from Initializer:" << endl;
    for (int i = 0; i < POP_COUNT; ++i) {
        double value = 0;
        for (int j = 0; j < ANSWER_LEN; ++j) {
            cout << pop_matrix[i][j] << " ";
            value += pop_matrix[i][j] * info_matrix[0][j];
        }
        cout << " VALUE = " << value << endl;
    }


    // Display the sorted matrix from EvaluationModule
    cout << "\nSorted Matrix from EvaluationModule:" << endl;
    for (int i = 0; i < POP_COUNT; ++i) {
        double value = 0;
        for (int j = 0; j < ANSWER_LEN; ++j) {
            cout << sorted_matrix[i][j] << " ";
            value += sorted_matrix[i][j] * info_matrix[0][j];
        }
        cout << " VALUE = " << value << endl;
    }



    // Display the pop with childes
    cout << "\nPop with chileds Matrix from Reproduction:" << endl;
    for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); ++i) {
        double value = 0;
        for (int j = 0; j < ANSWER_LEN; ++j) {
            cout << pop_with_child[i][j] << " ";
            value += pop_with_child[i][j] * info_matrix[0][j];
        }
        cout << " VALUE = " << value << endl;
    }

    // Display the pop after mutation
    cout << "\nPop after mutaion:" << endl;
    for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); ++i) {
        double value = 0;
        for (int j = 0; j < ANSWER_LEN; ++j) {
            cout << muted_pop[i][j] << " ";
            value += muted_pop[i][j] * info_matrix[0][j];
        }
        cout << " VALUE = " << value << endl;
    }

    // Display the sorted pop after mutation
    cout << "\nSorted Pop after mutaion:" << endl;
    for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); ++i) {
        double value = 0;
        for (int j = 0; j < ANSWER_LEN; ++j) {
            cout << sorted_muted_pop[i][j] << " ";
            value += sorted_muted_pop[i][j] * info_matrix[0][j];
        }
        cout << " VALUE = " << value << endl;
    }

    // Display the sorted pop after mutation
    cout << "\nRepaired Pop after sort:" << endl;
    for (int i = 0; i < POP_COUNT + (CHILD_COUNT * 2); ++i) {
        double value = 0;
        for (int j = 0; j < ANSWER_LEN; ++j) {
            cout << repaired_pop[i][j] << " ";
            value += repaired_pop[i][j] * info_matrix[0][j];
        }
        cout << " VALUE = " << value << endl;
    }
    cout << endl;

    // Display the sorted pop after mutation
    cout << "\Best Pop after select:" << endl;
    for (int i = 0; i < POP_COUNT; ++i) {
        double value = 0;
        for (int j = 0; j < ANSWER_LEN; ++j) {
            cout << sorted_best[i][j] << " ";
            value += sorted_best[i][j] * info_matrix[0][j];
        }
        cout << " VALUE = " << value << endl;
    }


    return 0;
}
