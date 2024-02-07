classdef Initializer < handle
    properties
        pop_matrix
    end
    
    properties (Constant)
        POP_COUNT = 30;
        ANSWER_LEN = 9;
        CHILD_COUNT = 5;
    end
    
    methods
        function obj = Initializer()
            obj.pop_matrix = zeros(obj.POP_COUNT, obj.ANSWER_LEN);
            obj.createRandomFloatMatrix();
            obj.displayMatrix('Starting Random Matrix:');
        end
        
        function value = generateRandomFloatValue(obj)
            value = rand();
        end
        
        function createRandomFloatMatrix(obj)
            for i = 1:obj.POP_COUNT
                for j = 1:obj.ANSWER_LEN
                    obj.pop_matrix(i, j) = obj.generateRandomFloatValue();
                end
            end
        end
        
        function displayMatrix(obj, title)
            disp(title);
            disp(obj.pop_matrix);
            disp(' ');
        end
    end
end
