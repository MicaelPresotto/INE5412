
#include <iostream>
#include <algorithm>

#include "process.h"
#include "kernel.h"


Kernel::Kernel() {}
Kernel::~Kernel() {}

void Kernel::initialize() {
    // TODO: Verificar se os objetos retornados são desalocados
    std::cout << "Inicializando kernel...\n\n";
    
    std::cout << "Lendo arquivo...\n";
    std::vector<std::string> lines = reader.read("entrada.txt");
    for (long unsigned int i = 0; i < lines.size(); i++) std::cout << lines[i] << std::endl;

    std::cout << std::endl;
    
    std::cout << "Criando processos...\n";
    processes = factory.createProcesses(lines);
    for (long unsigned int i = 0; i < processes.size(); i++) std::cout << *processes[i] << std::endl;

    std::cout << std::endl;

    clock = 0;
}

void Kernel::run() { 
    std::cout << "Executando os processos...\n\n";

    while (!processes.empty() || !readyProcesses.empty() || !cpu.empty()) {
        // Atualizando lista de processos
        updateReadyProcesses();

        // Se for a hora de trocar, troca o processo
        if (scheduler.isItTimeToSwitch(&cpu, readyProcesses)) {
            // Se a cpu estiver vazia, então descarrega processo
            if (!cpu.empty()) {
                Process *p = cpu.unloadProcess();
                if (p->getCurrentState() != TERMINADO) readyProcesses.push_back(p);
                else executedProcesses.push_back(p);
            }
            // Carrega o próximo processo na cpu
            Process *nextProcess = scheduler.getNextProcess(readyProcesses);
            if (nextProcess) cpu.loadProcess(nextProcess);
        }
        // Executando processo 
        cpu.execute(1);

        std::cout << clock << "-" << clock + 1 << ": " << std::endl;
        printProcesses(processes, "Esperando");
        printProcesses(readyProcesses, "Prontos");
        printProcesses(executedProcesses, "Finalizados");
        if (cpu.getProcess())
            std::cout << "Executando:\n" << "      " << *cpu.getProcess() << std::endl;

        // Contando mais um ciclo
        clock++;
    }
}

void Kernel::close() {
    std::cout << "Encerrando kernel...\n\n";
    for (long unsigned int i = 0; i < executedProcesses.size(); i++) delete executedProcesses[i];
}

void Kernel::updateReadyProcesses() {
    int c = clock;
    std::copy_if(processes.begin(), processes.end(), std::back_inserter(readyProcesses), [c](Process *p) {
        int result = p->getStart() == c;
        if (result) p->setCurrentState(PRONTO);
        return result;
    });

    auto newEnd = std::remove_if(processes.begin(), processes.end(), [c](Process *p) {
        return p->getStart() == c;
    });
    processes.resize(std::distance(processes.begin(), newEnd));

}

void printProcesses(std::vector<Process *> processes, std::string processesName) {
    std::cout << processesName << ":" << std::endl;
    for (auto p : processes) {
        std::cout << "      " << *p << std::endl;
    }
}