make all
valgrind --tool=memcheck --leak-check=yes --track-origins=yes -s ./bin/atividade_2 4 < ./entradas/vsim-gcc.txt