#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <cassert>
#include <getopt.h>
#include <stdlib.h>

#ifdef USE_CHRONO
#include <chrono>
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::microseconds;
typedef std::chrono::high_resolution_clock hr_clock;
#else
#include "timer.h"
#endif

class InputException {
public:
    InputException(const std::string& msg) : msg_(msg){};
    const std::string& what() const {return msg_;}
private:
    const std::string msg_;
};


inline long maxSubSum1(const std::vector<long>& a);
inline long maxSubSum2(const std::vector<long>& a);
inline long maxSubSum3(const std::vector<long>& a);
inline long maxSubSum4(const std::vector<long>& a);
inline long maxSumRec(const std::vector<long>& a, long left, long right);

int main(int argc, char* argv[]) {
    const int default_algorithm = 1;
    const unsigned long default_number = 10000;
    const unsigned long minimum_number = 10;
    bool verbose = false;

    try {
        opterr = 0;
        int longopt_index;
        const char *optstring = "a:hn:v";
        struct option long_options[] = {
                {"algorithm", 1, nullptr, 'a'},
                {"help",      0, nullptr, 'h'},
                {"number",    1, nullptr, 'n'},
                {"verbose",   0, nullptr, 'v'},
                {nullptr, 0, nullptr, 0}
        };

        int algorithm = default_algorithm;
        unsigned long number = default_number;

        int c;
        while ((c = getopt_long(argc, argv, optstring, long_options,
                                &longopt_index)) != -1) {
            switch (c) {
                case 'a': {
                    std::istringstream is(optarg);
                    char leftover_char;
                    if (!(is >> algorithm) || is.get(leftover_char))
                        throw InputException("Invalid argument for algorithm");
                }
                    break;
                case 'n': {
                    std::istringstream is(optarg);
                    char leftover_char;
                    if (!(is >> number) || is.get(leftover_char))
                        throw InputException("Invalid argument for number");
                    }
                    break;
                case 'v':
                    verbose = true;
                    break;
                case 'h':
                default:
                    std::cerr <<
                              "Usage: subsequence [options]\n"
                              "Options:\n"
                              "-a, --algorithm   Choose algorithm (1-4)\n"
                              "                  Default: "
                              << default_algorithm << "\n"
                              "-h, --help        Shows this message\n"
                              "-n, --number      Number of items\n"
                              "                  Default: "
                              << default_number << "\n"
                              "-v, --verbose     Shows generated data\n";
                    exit(EXIT_FAILURE);
            }
        }

        if(algorithm<1 || algorithm>4) {
            std::ostringstream message;
            message << "Algorithm must be in the range of 1-4";
            throw InputException(message.str());
        }

        if(number<minimum_number) {
            std::ostringstream message;
            message << "Number must be at least " << minimum_number;
            throw InputException(message.str());
        }

        // constructing the random number range from the number of elements
        // ensures that at least some values repeat
        const long range = number/10;
        std::default_random_engine generator;
        std::uniform_int_distribution<long> uniform(-range, range);
        std::vector<long> data;
        data.reserve(number);
        for(unsigned long i=0; i<number; ++i) {
            const long x = uniform(generator);
            assert(x>=-range && x<=range);
            if(verbose) {
                std::cout << x << ' ';
            }
            data.push_back(x);
        }
        if(verbose) {
            std::cout << std::endl;
        }

        assert(algorithm>=1 && algorithm<=4);
        if(verbose) {
            std::cout << "Algorithm " << algorithm << std::endl;
        }
        long result = -1;
#ifdef USE_CHRONO
        if(verbose) {
            std::cout << "Using std::chrono" << std::endl;
        }
        const auto start = hr_clock::now();
#else
        Timer timer;
        timer.start();
#endif
        switch(algorithm){
            case 1:
                result = maxSubSum1(data);
                break;
            case 2:
                result = maxSubSum2(data);
                break;
            case 3:
                result = maxSubSum3(data);
                break;
            case 4:
                result = maxSubSum4(data);
                break;
            default:
                assert(false);
        }
#ifdef USE_CHRONO
        const auto stop = hr_clock::now();
        const auto micros = duration_cast<microseconds>(stop-start).count();
        const auto elapsed = static_cast<double>(micros);
#else
        timer.stop();
        const auto elapsed = timer.elapsed();
#endif
        assert(result >= 0);
        std::cout << "Result: " << result << '\n';
        std::cout << "Elapsed (us): " << elapsed << std::endl;
    }
    catch(std::exception& e) {
        std::cerr << "Standard exception caught: " << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
    catch(InputException& e) {
        std::cerr << "Invalid input: " << e.what() << '\n';
        std::cerr << "Use the --help option for usage information\n";
        exit(EXIT_FAILURE);
    }
    catch(...){
        std::cerr << "Unknown exception caught\n";
        exit(EXIT_FAILURE);
    }
    return 0;
}

long maxSubSum1(const std::vector<long>& a) {

	long maxSum = 0;

	for (size_t i = 0; i < a.size(); ++i )
		for(size_t j = i; j < a.size(); ++j) {
			long thisSum = 0;
			for (size_t k = i; k <=j; ++k)
				thisSum += a[k];
			if(thisSum > maxSum)
				maxSum = thisSum;
		}
	return maxSum;
}

long maxSubSum2(const std::vector<long>& a ) {

	long maxSum = 0;

	for (size_t i = 0; i < a.size(); ++i) {
		long thisSum = 0;
		for (size_t j = i; j < a.size(); ++j) {
			thisSum += a[j];
			if (thisSum > maxSum)
				maxSum = thisSum;
		}
	}
	return maxSum;
}

long max3( long a, long b, long c )
{
	    return a > b ? a > c ? a : c : b > c ? b : c;
}

long maxSubSum3(const std::vector<long>& a) {

	    return maxSumRec(a, 0, a.size() - 1);
}

long maxSubSum4(const std::vector<long>& a) {

	long maxSum = 0;
	long thisSum = 0;

	for(size_t j = 0; j < a.size(); ++j) {

		thisSum += a[j];

		if (thisSum > maxSum)
			    maxSum = thisSum;
		else if (thisSum < 0)
			    thisSum = 0;

	}
	return maxSum;
}

long maxSumRec(const std::vector<long>& a, long left, long right) {

	if(left == right)
		if(a[left] >0)
			    return a[left];
		else return 0;

	long center = (left + right) / 2;
	auto maxLeftSum = maxSumRec(a, left, center);
	auto maxRightSum = maxSumRec(a, center + 1, right);

	long maxLeftBorderSum = 0;
	long leftBorderSum = 0;

	for(long i = center; i > left; --i) {
		leftBorderSum += a[i-1];
		if(leftBorderSum > maxLeftBorderSum)
			    maxLeftBorderSum = leftBorderSum;
	}

	long maxRightBorderSum = 0;
	long rightBorderSum = 0;

	for(long j = center + 1; j < right; ++j) {
		rightBorderSum += a[j];
		if(rightBorderSum > maxRightBorderSum)
			    maxRightBorderSum = rightBorderSum;
	}

	return max3(maxLeftSum, maxRightSum, maxLeftBorderSum + maxRightBorderSum);

}
