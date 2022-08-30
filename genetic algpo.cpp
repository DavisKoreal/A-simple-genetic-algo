#include <iostream>
#include <math.h>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>

class Solution {
public:
    
    double rank, x, y, z;

    Solution(double a, double b, double c, double d) {
        rank = a;
        x = b;
        y = c;
        z = d;
    }
    void fitness() {

        double ans = (6 * x + -y + std::pow(z, 200)) - 25;
        rank = (ans == 0) ? 9999 : std::abs(1 / ans);
    }
};

int main() {

    //initiating tools needed 
    std::random_device device;
    std::uniform_real_distribution<double> unif(-10000, 10000);
    std::vector<Solution> solutions;

    const int NUM = 1000000;

    //creating initial random Solutions and storing them in Solutions
    for (int i = 0; i < NUM; i++) {
        double initialrank = 0;
        Solution v(initialrank, unif(device), unif(device), unif(device));
        solutions.push_back(v);
    }


    int generation = 0;

    while (generation < 100) 
    {

        //running our fitness function for every solution 
        for (auto& s : solutions) {
            s.fitness();
        }

        //sort solutions by rank
        std::sort
        (
            solutions.begin(),
            solutions.end(),
            [](const auto& lhs, const auto& rhs)
            {
                return lhs.rank > rhs.rank;
            }
        );

        //print top solutions 
        std::for_each
        (
            solutions.begin(),
            solutions.begin() + 20,
            [](const auto& s) {
                std::cout << s.rank << " x : " << s.x << " y : " << s.y << " z : " << s.z << std::endl;
                //std::cout << s.rank << std::endl;
            }

        );

        std::cout << "..." << std::endl << std::endl;


        // copy the top 1000 solutions into the sample size

        const int SAMPLE_SIZE = 1000;
        std::vector<Solution> sample;
        std::copy
        (
            solutions.begin(),
            solutions.begin() + SAMPLE_SIZE,
            std::back_inserter(sample)
        );
        solutions.clear();

        //mutate the top solutions by %

        std::uniform_real_distribution<double> m(0.90, 1.10);
        std::for_each(
            sample.begin(),
            sample.end(),
            [&](auto& s) {
                s.x *= m(device);
                s.y *= m(device);
                s.z *= m(device);
            }
        );

        //crossing over 

        std::uniform_int_distribution<int> cross(0, SAMPLE_SIZE - 1);
        for (int i = 0; i < NUM; i++) {
            solutions.push_back(
                Solution(
                    0, 
                    sample[cross(device)].x, 
                    sample[cross(device)].y, 
                    sample[cross(device)].z
                )
            );
        }
    }
}