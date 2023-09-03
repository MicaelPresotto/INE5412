#ifndef _H_KERNEL
#define _H_KERNEL

#include "file_reader.h"
#include "process_factory.h"
#include "process.h"
#include "cpu.h"

class Kernel {

private:

    FileReader reader;
    ProcessFactory factory;
    std::vector<Process *> processes;
    std::vector<Process *> readyProcesses;
    std::vector<Process *> executedProcesses;
    CPU cpu;
    int clock;

public:
    Kernel() {};
    ~Kernel() {};

    void initialize() {
        // TODO: Verificar se os objetos retornados são desalocados
        std::cout << "Inicializando kernel...\n\n";
        
        std::cout << "Lendo arquivo...\n";
        std::vector<std::string> lines = reader.read("../entrada/entrada.txt");
        for (long unsigned int i = 0; i < lines.size(); i++) std::cout << lines[i] << std::endl;

        std::cout << std::endl;
        
        std::cout << "Criando processos...\n";
        processes = factory.createProcesses(lines);
        for (long unsigned int i = 0; i < processes.size(); i++) std::cout << *processes[i] << std::endl;

        std::cout << std::endl;

        updateReadyProcesses();
        clock = 0;
    }; // Criará os processos e etc

    int run() { 
        while (!processes.empty() || !readyProcesses.empty()) {
            std::cout << "Aqui" << std::endl;
            if (cpu.empty() && !readyProcesses.empty()) {
                // TODO: Arrumar para o escalonador
                Process *p = readyProcesses.at(0);
                readyProcesses.erase(readyProcesses.begin());
                int duration = p->getDuration() - p->getExecutedTime();

                cpu.loadProcess(p, duration);
            }

            cpu.execute(1);

            std::cout << clock << "-" << clock + 1 << ": " << cpu.getProcess() << std::endl;

            if (cpu.finishExecuting()) {
                Process *p = cpu.unloadProcess();

                if (p->getCurrentState() == PRONTO) readyProcesses.push_back(p);
                else executedProcesses.push_back(p);

                // TODO: Arrumar para o escalonador
                p = readyProcesses.at(0);
                readyProcesses.erase(readyProcesses.begin());
                int duration = p->getDuration() - p->getExecutedTime();

                cpu.loadProcess(p, duration);
            }

            updateReadyProcesses();
            clock++;
        }
        return 1;
    }; // Executa os processo
    
    void close() {
        std::cout << "Encerrando kernel...\n\n";
        for (long unsigned int i = 0; i < executedProcesses.size(); i++) delete executedProcesses[i];
    }; // Destrói tudo que foi criado

    void updateReadyProcesses() {
        for (auto itProcess = processes.begin(); itProcess != processes.end(); itProcess++) {
            if ((*itProcess)->getStart() == clock + 1) {
                readyProcesses.push_back(*itProcess);
                processes.erase(itProcess);
            }
        }
    }
};

#endif
