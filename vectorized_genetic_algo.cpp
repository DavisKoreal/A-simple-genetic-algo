#include <iostream>
#include <math.h>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <ctime>

double true_value = 25;
double inferenceError = 10000000000000;
int vectorlenght = 15;
const int SAMPLE_SIZE = 1000;
double defaultforMaxAcc = 9999;
const int NUM = 10000;
const double allowederror = 0.005;
int numberofGenerations = 100;
std::vector<double> independent_variables = {};

class Solution
{
public:
    double rank;
    std::vector<double> coefficients;

    Solution(double a, std::vector<double> coeff)
    {
        rank = a;
        coefficients = coeff;
    }

    double runInference() const
    {
        double coefficientsSize = coefficients.size();
        double independentVarSize = independent_variables.size();
        if (((coefficientsSize == 0) || (independentVarSize == 0)) || (coefficientsSize != independentVarSize))
        {
            return inferenceError;
        }

        double result = 0;
        for (int i = 0; i < coefficientsSize; i++)
        {
            result += coefficients[i] * independent_variables[i];
        }

        return result;
    }

    void fitness()
    {
        double ans = runInference() - true_value;
        rank = (ans == 0) ? defaultforMaxAcc : std::abs(1 / ans);
    }

    void mutateTopSolutions(std::random_device &device, std::uniform_real_distribution<double> m)
    {
        for (int i = 0; i < vectorlenght; i++)
        {
            coefficients[i] *= m(device);
        }
    }

    void showCoefficients() const
    {
        std::cout << " [ ";
        for (int i = 0; i < vectorlenght; i++)
        {
            std::cout << coefficients[i] << ", ";
        }
        std::cout << " ]";
    }
};

double getRandomDouble()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(gen);
}

std::vector<double> crossDevice(std::uniform_int_distribution<int> cross, std::random_device &device, std::vector<Solution> sample)
{
    std::vector<double> coe = {};
    for (int i = 0; i < vectorlenght; i++)
    {
        coe.push_back(sample[cross(device)].coefficients[i]);
    }
    return coe;
}

int main()
{
    std::ofstream log_file("genetic_algo_log.txt", std::ios::app);
    std::ofstream results_file("genetic_algo_results.txt", std::ios::app);
    if (!log_file.is_open())
    {
        std::cerr << "Failed to open log file\n";
        return 1;
    }
    auto get_timestamp = []()
    {
        std::time_t now = std::time(nullptr);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return std::string(buf);
    };

    for (int i = 0; i < vectorlenght; i++)
    {
        independent_variables.push_back(getRandomDouble());
    }

    // initiating tools needed
    std::random_device device;
    std::uniform_real_distribution<double> unif(-10000, 10000);
    std::vector<Solution> solutions;

    // creating initial random Solutions and storing them in Solutions
    int generation = 0;
    for (int i = 0; i < NUM; i++)
    {
        double initialrank = 0;
        std::vector<double> coef = {};
        for (int i = 0; i < vectorlenght; i++)
        {
            coef.push_back(getRandomDouble());
        }
        Solution v(initialrank, coef);
        solutions.push_back(v);
    }

    log_file << get_timestamp() << " Starting generation " << generation << "\n";

    while (generation < numberofGenerations)
    {
        double error = 1 / solutions[0].rank;
        if (error < allowederror)
        {
            log_file << "\n --------------------- \n We are within reasonable error \n\n";
            break;
        }
        // running our fitness function for every solution
        for (auto &s : solutions)
        {
            s.fitness();
        }

        // sort solutions by rank
        std::sort(
            solutions.begin(),
            solutions.end(),
            [](const auto &lhs, const auto &rhs)
            {
                return lhs.rank > rhs.rank;
            });

        // print top solutions
        std::for_each(
            solutions.begin(),
            solutions.begin() + 20,
            [&log_file, &get_timestamp](const auto &s)
            {
                std::cout << s.rank << " ";
                s.showCoefficients();

                log_file << get_timestamp() << " solution error: " << 1 / s.rank << " coefficients: [ ";
                for (int i = 0; i < vectorlenght; i++)
                {
                    log_file << s.coefficients[i] << ", ";
                }
                log_file << "]\n";
            });

        std::cout << "..." << std::endl
                  << std::endl;

        // copy the top 1000 solutions into the sample size
        std::vector<Solution> sample;
        std::copy(
            solutions.begin(),
            solutions.begin() + SAMPLE_SIZE,
            std::back_inserter(sample));
        solutions.clear();

        // mutate the top solutions by %
        std::uniform_real_distribution<double> m(0.90, 1.10);
        log_file << get_timestamp() << " Mutating top solutions\n";
        std::for_each(
            sample.begin(),
            sample.end(),
            [&](auto &s)
            {
                s.mutateTopSolutions(device, m);
            });

        // crossing over
        std::uniform_int_distribution<int> cross(0, SAMPLE_SIZE - 1);
        log_file << get_timestamp() << " Performing crossover\n";
        for (int i = 0; i < NUM; i++)
        {
            solutions.push_back(Solution(0, crossDevice(cross, device, sample)));
        }

        generation++;
        log_file << get_timestamp() << " Completed generation " << generation << "\n";
    }

    log_file << "\n\n INDEPENDENT VARIABLES\n  [ ";
    for (int i = 0; i < vectorlenght; i++)
    {
        log_file << independent_variables[i] << " ";
    }
    log_file << " ]";

    results_file << get_timestamp() << "Final generation results are following: " << "\n";
    results_file << "Independent Variables: [ ";
    for (int i = 0; i < vectorlenght; i++)
    {
        results_file << independent_variables[i] << ", ";
    }
    results_file << "]\n";
    
    
    int recorded_solutions = 0;
    for (const auto &s : solutions)
    {
        if (recorded_solutions >= 20)
            break;
        double infValue = s.runInference();
        results_file << "Inference " << infValue << "Error: " << true_value - infValue << " Coefficients: [ ";
        for (int i = 0; i < vectorlenght; i++)
        {
            results_file << s.coefficients[i] << ", ";
        }
        results_file << "]\n";
        recorded_solutions += 1;
    }

    log_file.close();
    results_file.close();
    return 0;
}