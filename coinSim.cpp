#include <iostream>
#include <array>
#include <algorithm>
#include <random>
#include <numeric>
#include <cstdlib>
#include <map>
#include <unordered_set>

#define COINS 1000 // Number of Coins
#define FLIPS 10   // Number of Flips per Coin
#define BERNOULLI_P 0.5 //Probablity of 1
#define RUNS 10000 // Number of Runs of the expirment
//#define PRINT_INT 100 // Interval to print Progress Indicator

/* results  of a single expirment from problem 1.10*/ 
struct exp_res{
	double first; //The first coint flipped
	double min; // First minimum found
	double rand; //A randomly chosen coin

	exp_res(double f, double m, double r):first(f),min(m),rand(r){}		
};

/*For a coin with p(heads) = p, flip N times, and return the relative frequency as a double*/
double gen_nu(const std::size_t N, const double p, std::mt19937 & gen){
	std::bernoulli_distribution d(p); 
                
        /* Using std<bool> for efficent packing*/
	std::vector<bool> samp(N);

        /* std::vector<bool> doesn't really work well with range based for
         * because the internal representation doesn't lend it self to that kind
         * of assignment. But iterators work just fine. 
         */
	for(auto i = samp.begin(); i != samp.end(); i++)
		*i = d(gen) ; //bernoulli_distribution returns a bool
        
        /* Compute sample mean with accumulator*/
	return (double) std::accumulate(samp.begin(), samp.end(), 0) / N;
}

/* for a constant number of coins flip each N times, and then compute the rel freq of the first, min (first occurance) and rand coin */
exp_res run_exp(){
	std::array<double, COINS> coin_flips;  
        
	std::random_device rd;
	std::mt19937 gen(rd()); //Supposedly using Mersenne twister is more "efficient"
        
	std::uniform_int_distribution<> dis(0, COINS-1);

	/* flip a 1000 fair coins 10 times each , then compute nu*/
	for(double &i : coin_flips)
		i = gen_nu(FLIPS, BERNOULLI_P, gen); // Generate sample mean for each COIN

	int rand_ind = dis(gen); //Pick a random coin (index)
	auto min_ele = std::min_element(std::begin(coin_flips), std::end(coin_flips)); //find the first min (as an iterator)

	return exp_res(coin_flips[0],*min_ele,coin_flips[rand_ind]);
}

int main(){

        /*We use a map to store the histogram. if you used the subscript operator
         * On a key that does not exist yet, that key gets created and initalized to zero
         */
	std::map<double, int> hist_first;
	std::map<double, int> hist_min;
	std::map<double, int> hist_rand;

	std::cout << "Starting experiment" << std::endl;
	/* run experiment some number of times*/
	for(int i = 0; i < RUNS; i++){ // change 100 to a #DEFINE
		exp_res res = run_exp();
		hist_first[res.first]++; //This will bump or add the value res.first to this map
		hist_min[res.min]++;
		hist_rand[res.rand]++;	
	}
	std::cout << "End experiment" << std::endl;

	std::unordered_set<double> keys_made;

        /*colllect all the keys we've seen (since we flip 10 times this will be factors of 10th)*/
	for(auto& p: hist_first)
		keys_made.insert(p.first);
	
	for(auto& p: hist_min)
		keys_made.insert(p.first);

	for(auto& p: hist_rand)
		keys_made.insert(p.first);

        /*For each key seen, output the the value from each histogram (will output 0 if never seen before)*/
	std::cout << "Results:" << std::endl;
	for(double k : keys_made)
		std::cout << k << "," << hist_first[k] << "," << hist_min[k] << "," << hist_rand[k] << std::endl;
}

/* OLD display method
	std::cout << "first{";
	for(auto& p: hist_first)
		std::cout << p.first << ':' << p.second << ' ';
	std::cout << "}\n";

	std::cout << "min{";
	for(auto& p: hist_min)
		std::cout << p.first << ':' << p.second << ' ';
	std::cout << "}\n";

	std::cout << "rand{";
	for(auto& p: hist_rand)
		std::cout << p.first << ':' << p.second << ' ';
	std::cout << "}\n";
*/
