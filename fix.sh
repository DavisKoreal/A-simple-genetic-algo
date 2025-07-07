#!/bin/bash

# Backup the original file
cp vectorized_genetic_algo.cpp vectorized_genetic_algo.cpp.fix_logging_corrected_final.bak

# Remove erroneous log_file.close() from Solution class, getRandomDouble, and crossDevice
sed -i '/log_file.close();/d' vectorized_genetic_algo.cpp

# Fix lambda in std::for_each to capture log_file and get_timestamp
sed -i '/std::for_each(/,/);/ s|[](const auto &s)|[&log_file, &get_timestamp](const auto &s)|' vectorized_genetic_algo.cpp

# Remove incorrect generation increment and logging in the initial population loop
sed -i '/for (int i = 0; i < NUM; i++)/,/int generation = 0;/ s|generation++;\n        log_file << get_timestamp() << " Completed generation " << generation << "\\n";||' vectorized_genetic_algo.cpp

# Remove incorrect generation increment and logging in the crossover loop
sed -i '/std::uniform_int_distribution<int> cross(0, SAMPLE_SIZE - 1);/,/solutions.push_back/ s|generation++;\n            log_file << get_timestamp() << " Completed generation " << generation << "\\n";||' vectorized_genetic_algo.cpp

# Remove extra opening brace after logging setup
sed -i '/auto get_timestamp = .*;/ {n; s/^{$//}' vectorized_genetic_algo.cpp

# Add generation increment and logging at the end of the while loop
sed -i '/solutions.push_back(Solution(0, crossDevice(cross, device, sample)));/a\            generation++;\n            log_file << get_timestamp() << " Completed generation " << generation << "\\n";' vectorized_genetic_algo.cpp

# Ensure log_file.close() is only at the end of main, before the closing brace
sed -i '/^    log_file.close();/d' vectorized_genetic_algo.cpp
sed -i '/^}/i\    log_file.close();' vectorized_genetic_algo.cpp

echo "Fixed logging and syntax issues in vectorized_genetic_algo.cpp"
echo "Backup created as vectorized_genetic_algo.cpp.fix_logging_corrected_final.bak"
echo "Recompile with: g++ vectorized_genetic_algo.cpp -o vect.exe"