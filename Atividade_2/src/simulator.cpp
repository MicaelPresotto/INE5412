#include "simulator.h"
#include "replacementAlgorithm.h"
#include <cstdio>
#include <vector>
#include <set>
#include <chrono>

void Simulator::run(std::size_t frames) {
    auto start_time = std::chrono::high_resolution_clock::now();
    std::size_t pfsFIFO = 0;
    std::size_t pfsLRU = 0;
    std::size_t pfsOPT = 0;

    std::vector<std::size_t> lines;
    std::size_t line;

    while (scanf("%zu", &line) != EOF) lines.push_back(line);

    FIFOAlgorithm fifo(frames);
    LRUAlgorithm lru(frames);
    OPTAlgorithm opt(frames, lines);

    for (std::size_t pageId : lines){
        pfsOPT += !opt.accessMemory(pageId);
        pfsFIFO += !fifo.accessMemory(pageId);
        pfsLRU += !lru.accessMemory(pageId);
    }

    printf("%lu quadros\n", frames);
    printf("%lu refs\n", lines.size());
    printf("FIFO: %lu PFs\n", pfsFIFO);
    printf("LRU: %lu PFs\n", pfsLRU);
    printf("OPT: %lu PFs\n", pfsOPT);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;
    printf("Tempo de execução: %f segundos\n", duration.count());
}   
