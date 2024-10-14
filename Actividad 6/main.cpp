#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <chrono>

struct TokenInfo {
    int repetitions;
    int fileCount;
};

void findRepeatedLetters(const std::string& word, std::unordered_map<std::string, TokenInfo>& dictionary) {
    if (!isalpha(word[0])) return; 

    dictionary[word].repetitions++;
}

void processFile(const std::string& filename, std::unordered_map<std::string, TokenInfo>& dictionary) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
        return;
    }

    std::set<std::string> uniqueTokens; 
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string word;

        while (stream >> word) {
            findRepeatedLetters(word, dictionary);
            uniqueTokens.insert(word);
        }
    }

    
    for (const auto& token : uniqueTokens) {
        dictionary[token].fileCount++;
    }

    file.close();
}

void writeOutput(const std::unordered_map<std::string, TokenInfo>& dictionary, const std::string& outputFilename) {
    std::ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de salida: " << outputFilename << std::endl;
        return;
    }

    outputFile << "Token;Repeticiones;# de archivos con ese token\n"; 
    for (const auto& pair : dictionary) {
        outputFile << pair.first << ";" << pair.second.repetitions << ";" << pair.second.fileCount << "\n";
    }

    outputFile.close();
}

int main() {
    std::unordered_map<std::string, TokenInfo> dictionary;

    std::cout << "Processing text files..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now(); 
    processFile("archivos_words.txt", dictionary);
    processFile("clases_words.txt", dictionary);

    auto end = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<double> elapsed = end - start;

    // Write output to file
    writeOutput(dictionary, "diccionario.txt");

    // Log execution time
    std::ofstream logFile("a6_matricula.txt");
    if (logFile.is_open()) {
        logFile << "Tiempo total de ejecución: " << elapsed.count() << " segundos\n";
        logFile.close();
    } else {
        std::cerr << "No se pudo abrir el archivo de log." << std::endl;
    }

    std::cout << "All tasks completed successfully." << std::endl;

    return 0;
}