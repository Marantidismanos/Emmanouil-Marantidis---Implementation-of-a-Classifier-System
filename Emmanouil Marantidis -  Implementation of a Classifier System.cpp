#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <random>
#include <map>
struct Rule {
	std::string condition;
	std::string action;
	double strength;
	Rule(const std::string& condition, const std::string& action, double strength)
		: condition(condition), action(action), strength(strength) {}
};

// Global random engine
std::mt19937 rng(std::random_device{}());

// Function using the global RNG
double rnd() {
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	return dist(rng);
}
char mutateChar(char currentChar, std::mt19937& rng) {
	static const std::map<char, std::vector<char>> alternatives = {
		{'0', {'1', '#'}},
		{'1', {'0', '#'}},
		{'#', {'0', '1'}}
	};
	std::uniform_int_distribution<int> dist(0, 1);
	auto it = alternatives.find(currentChar);
	if (it != alternatives.end()) {
		return it->second[dist(rng)];
	}
	return currentChar;
}
Rule generateRandomRule(int conditionLength, int actionLength) {
	std::string condition;
	std::string action;
	for (int i = 0; i < conditionLength; ++i) {
		condition += "01#"[rng() % 3];
	}
	for (int i = 0; i < actionLength; ++i) {
		action += "01"[rng() % 2];
	}
	double initialStrength = 10.0;

	return Rule(condition, action, initialStrength);
}

void mutate(Rule& rule, double mutationRate, std::mt19937& rng) {
	//std::cout << "Before mutation: Condition=" << rule.condition << ", Action=" << rule.action << std::endl;
	for (auto& c : rule.condition) {
		if (rnd() < mutationRate) {
			//char oldChar = c;
			c = mutateChar(c, rng);
			//std::cout << "Mutated '" << oldChar << "' to '" << c << "' in condition." << std::endl;
		}
	}
	for (auto& c : rule.action) {
		if (rnd() < mutationRate) {
			//char oldChar = c;
			c = (c == '0') ? '1' : '0';
			//std::cout << "Mutated '" << oldChar << "' to '" << c << "' in action." << std::endl;
		}
	}
	//std::cout << "After mutation: Condition=" << rule.condition << ", Action=" << rule.action << std::endl;
}

/*
Rule rouletteWheelSelection(const std::vector<Rule>& rules) {
	double totalStrength = std::accumulate(rules.begin(), rules.end(), 0.0,
		[](double sum, const Rule& r) { return sum + r.strength; });
	double value = rnd() * totalStrength;
	double partialSum = 0.0;
	for (const auto& rule : rules) {
		partialSum += rule.strength;
		if (partialSum >= value) {
			return rule;
		}
	}
	return rules.back();
}
*/
Rule rouletteWheelSelection(const std::vector<Rule>& rules) {
	double totalStrength = std::accumulate(rules.begin(), rules.end(), 0.0,
		[](double sum, const Rule& r) { return sum + r.strength; });
	double value = rnd() * totalStrength;
	double partialSum = 0.0;
	for (const auto& rule : rules) {
		partialSum += rule.strength;
		if (partialSum > value || &rule == &rules.back()) { // Ensure the last rule can be selected
			return rule;
		}
	}
	return rules.back(); // Fallback
}
/*
void multiPointCrossover(Rule& parent1, Rule& parent2) {
	int point1 = rand() % parent1.condition.size();
	int point2 = rand() % parent1.condition.size();
	while (point1 == point2) {
		point2 = rand() % parent1.condition.size();
	}
	if (point1 > point2) std::swap(point1, point2);

	for (int i = point1; i <= point2; ++i) {
		std::swap(parent1.condition[i], parent2.condition[i]);
	}
}
*/

void multiPointCrossover(Rule& parent1, Rule& parent2) {
	// Combine condition and action for crossover purposes
	std::string genotype1 = parent1.condition + parent1.action;
	std::string genotype2 = parent2.condition + parent2.action;

	int point1 = rand() % genotype1.size();
	int point2 = rand() % genotype1.size();
	if (point1 > point2) {
		std::swap(point1, point2); // Ensure point1 is less than point2
	}
	//std::cout << "Before crossover:\n";
	//std::cout << "Parent1: " << genotype1 << "\nParent2: " << genotype2 << std::endl;
	//std::cout << "Crossover points: " << point1 << ", " << point2 << std::endl;

	// Perform crossover between point1 and point2
	for (int i = point1; i <= point2; ++i) {
		std::swap(genotype1[i], genotype2[i]);
	}
	//std::cout << "After crossover:\n";
	//std::cout << "Parent1: " << genotype1 << "\nParent2: " << genotype2 << std::endl;


	// Split the genotypes back into condition and action
	parent1.condition = genotype1.substr(0, parent1.condition.size());
	parent1.action = genotype1.substr(parent1.condition.size());
	parent2.condition = genotype2.substr(0, parent2.condition.size());
	parent2.action = genotype2.substr(parent2.condition.size());
}


bool check_match(const std::string& input, const Rule& rule) {
	for (size_t i = 0; i < input.size(); ++i) {
		if (rule.condition[i] != '#' && rule.condition[i] != input[i]) {
			return false;
		}
	}
	return true;
}
double evaluateFitness(const Rule& rule, const std::vector<std::pair<std::string, std::string>>& trainingSet) {
	int correctPredictions = 0;
	for (const auto& vs : trainingSet) {
		if (check_match(vs.first, rule)) {  // Ensure we use the matching function to compare
			if (rule.action == vs.second) {
				correctPredictions++;
			}
		}
	}
	return  static_cast<double>(correctPredictions) / trainingSet.size();

}
// Function to find elite rules without sorting the entire population
std::vector<int> findEliteIndices(const std::vector<Rule>& rules, int elitismCount) {
	std::vector<int> eliteIndices(elitismCount, -1);
	std::vector<double> eliteStrengths(elitismCount, std::numeric_limits<double>::lowest());

	for (int i = 0; i < rules.size(); ++i) {
		for (int j = 0; j < elitismCount; ++j) {
			if (rules[i].strength > eliteStrengths[j]) {
				// Shift the lower elements down
				for (int k = elitismCount - 1; k > j; --k) {
					eliteStrengths[k] = eliteStrengths[k - 1];
					eliteIndices[k] = eliteIndices[k - 1];
				}
				// Place the new elite
				eliteStrengths[j] = rules[i].strength;
				eliteIndices[j] = i;
				break;
			}
		}
	}
	return eliteIndices;
}
void geneticAlgorithm(std::vector<Rule>& rules, double mutationRate, int generation, const std::vector<std::pair<std::string, std::string>>& trainingSet) {
	int elitismCount = static_cast<int>(rules.size() * 0.1); // For example, 10% elitism
	std::vector<int> eliteIndices = findEliteIndices(rules, elitismCount);
	std::vector<Rule> newGeneration;

	newGeneration.reserve(rules.size());  // Reserve space to avoid reallocations

	// Copy elite rules to the new generation
	for (int index : eliteIndices) {
		if (index >= 0 && index < rules.size()) {  // Additional bounds checking
			newGeneration.push_back(rules[index]);
		}
	}

	// Select, crossover, and mutate to create new offspring
	while (newGeneration.size() < rules.size()) {
		Rule parent1 = rouletteWheelSelection(rules);
		Rule parent2 = rouletteWheelSelection(rules);
		multiPointCrossover(parent1, parent2);

		// Calculate average strength of parents
		double averageStrength = (parent1.strength + parent2.strength) / 2.0;

		// Assign average strength to both offspring before mutation
		parent1.strength = averageStrength;
		parent2.strength = averageStrength;

		// Mutate and re-evaluate fitness based on new genotype
		mutate(parent1, mutationRate, rng);
		mutate(parent2, mutationRate, rng);
		parent1.strength = evaluateFitness(parent1, trainingSet);
		parent2.strength = evaluateFitness(parent2, trainingSet);

		// Add offspring to the new generation
		newGeneration.push_back(parent1);
		if (newGeneration.size() < rules.size()) {
			newGeneration.push_back(parent2);
		}
	}

	// Replace old generation with new generation
	rules.swap(newGeneration);
}


void shuffleTrainingSet(std::vector<std::pair<std::string, std::string>>& trainingSet) {
	static std::mt19937 rng(std::random_device{}());
	std::shuffle(trainingSet.begin(), trainingSet.end(), rng);
}
Rule generateRuleForSample(const std::string& input, double initialStrength, int actionLength) {
	std::string condition;
	std::uniform_int_distribution<int> dist(0, 2); // For '0', '1', or '#'
	for (char c : input) {
		int decision = dist(rng);
		condition += (decision < 2) ? c : '#';
	}

	std::string action;
	std::uniform_int_distribution<int> actionDist(0, 1);
	for (int i = 0; i < actionLength; ++i) {
		action += (actionDist(rng) == 0) ? '0' : '1';
	}

	return Rule(condition, action, initialStrength);
}


// Add a constant for the maximum number of rules allowed
//const int MAX_RULES = 100; // Example maximum

// Function to find the index of the least effective rule
int findIndexOfLeastEffectiveRule(const std::vector<Rule>& rules) {
	double minStrength = std::numeric_limits<double>::max();
	int index = -1;
	for (int i = 0; i < rules.size(); ++i) {
		if (rules[i].strength < minStrength) {
			minStrength = rules[i].strength;
			index = i;
		}
	}
	return index;
}
bool runBucketBrigade(std::vector<Rule>& rules, std::vector<std::pair<std::string, std::string>>& trainingSet, double bidPercentage, double rewardAmount, double  negativeRewardFactor, int repeatCount, double taxRate, int maxRules) {
	int correctPredictions = 0;

	while (repeatCount--) {
		shuffleTrainingSet(trainingSet);
		for (const auto& ts : trainingSet) {
			const std::string& input = ts.first;
			const std::string& expectedOutput = ts.second;

			std::vector<Rule*> matchedRules;
			for (auto& rule : rules) {
				if (check_match(input, rule)) {
					matchedRules.push_back(&rule);
				}
			}

			Rule* bestRule = nullptr;
			if (!matchedRules.empty()) {
				// Sort matchedRules by strength in descending order
				std::sort(matchedRules.begin(), matchedRules.end(), [](const Rule* a, const Rule* b) {
					return a->strength > b->strength;
					});

				// Handle ties for the top position
				std::vector<Rule*> topTiedRules;
				double topStrength = matchedRules.front()->strength;
				std::copy_if(matchedRules.begin(), matchedRules.end(), std::back_inserter(topTiedRules), [topStrength](const Rule* rule) {
					return rule->strength == topStrength;
					});

				if (topTiedRules.size() > 1) {
					// If there's a tie, randomly select one
					std::uniform_int_distribution<size_t> dist(0, topTiedRules.size() - 1);
					bestRule = topTiedRules[dist(rng)];
				}
				else {
					bestRule = matchedRules.front();
				}
			}

			if (bestRule) {
				double bidAmount = bestRule->strength * bidPercentage;
				bestRule->strength -= bidAmount;

				if (bestRule->action == expectedOutput) {
					bestRule->strength += rewardAmount;
					correctPredictions++;
				}
				else {
					/* double negativeReward = rewardAmount * 0.5;
					bestRule->strength -= negativeReward;
					if (bestRule->strength < 0) bestRule->strength = 0;*/

					// Apply negative reward if the prediction is incorrect
					double negativeReward = bidAmount * negativeRewardFactor; // Calculate negative reward
					bestRule->strength -= negativeReward; // Deduct negative reward
					if (bestRule->strength < 0) bestRule->strength = 0; // Ensure strength doesn't go below 0
				}
			}
			else if (rules.size() >= maxRules) {
				int leastEffectiveRuleIndex = findIndexOfLeastEffectiveRule(rules);
				if (leastEffectiveRuleIndex != -1) {
					// Replace the least effective rule with a new rule based on the unmatched input
					// generateRuleForSample only takes two parameters.
					rules[leastEffectiveRuleIndex] = generateRuleForSample(input, 10.0, 1);

				}
			}
			else {
				// Directly add a new rule if under the max limit
				rules.push_back(generateRuleForSample(input, 10.0, 1));
			}
		}

		// Apply a tax to all rules
		for (auto& rule : rules) {
			rule.strength *= (1.0 - taxRate);
		}
	}
	return true;
}

// Function to predict the output for a given input based on the rules, using strength and tie resolution
std::string predictOutput(const std::vector<Rule>& rules, const std::string& input) {
	std::vector<const Rule*> matchedRules;

	// Collect all rules that match the input
	for (const auto& rule : rules) {
		if (std::equal(rule.condition.begin(), rule.condition.end(), input.begin(),
			[](char r, char i) { return r == '#' || r == i; })) {
			matchedRules.push_back(&rule);
		}
	}

	if (matchedRules.empty()) {
		return ""; // Return an empty string if no rule matches
	}

	// Sort matched rules by strength in descending order
	std::sort(matchedRules.begin(), matchedRules.end(), [](const Rule* a, const Rule* b) {
		return a->strength > b->strength;
		});

	// Handle ties by selecting a rule randomly if there's a tie at the top
	std::vector<const Rule*> topTiedRules;
	double topStrength = matchedRules.front()->strength;
	std::copy_if(matchedRules.begin(), matchedRules.end(), std::back_inserter(topTiedRules), [topStrength](const Rule* rule) {
		return rule->strength == topStrength;
		});

	static std::mt19937 rng(std::random_device{}()); // Use a static rng to avoid frequent re-seeding
	if (topTiedRules.size() > 1) {
		std::uniform_int_distribution<size_t> dist(0, topTiedRules.size() - 1);
		return topTiedRules[dist(rng)]->action;
	}

	return matchedRules.front()->action; // Return the action of the rule with the highest strength
}

int main() {
	srand(static_cast<unsigned>(time(0)));

	// Number of rules
	int populationSize, maxRules, maxGenerations = 0;
	std::cout << "Give the Population size: ";
	std::cin >> populationSize;
	// Max number of rules 
	std::cout << "Enter the Maximum number of rules: ";
	std::cin >> maxRules;
	// Number of generations
	std::cout << "Give the number of Generations: ";
	std::cin >> maxGenerations;
	// Mutation rate
	double mutationRate;
	std::cout << "Give the mutationRate (e.g., 0.5): ";
	std::cin >> mutationRate;

	// Rule and action lengths
	int conditionLength = 6;
	int actionLength = 1;

	std::vector<std::pair<std::string, std::string>> trainingSet = {
	{"000000", "0"}, {"000001", "0"}, {"000010", "0"}, {"000011", "0"},
	{"000100", "0"}, {"000101", "0"}, {"000110", "0"}, {"000111", "0"},
	{"001000", "1"}, {"001001", "1"}, {"001010", "1"}, {"001011", "1"},
	{"001100", "1"}, {"001101", "1"}, {"001110", "1"}, {"001111", "1"},

	{"010000", "0"}, {"010001", "0"}, {"010010", "0"}, {"010011", "0"},
	{"010100", "1"}, {"010101", "1"}, {"010110", "1"}, {"010111", "1"},
	{"011000", "0"}, {"011001", "0"}, {"011010", "0"}, {"011011", "0"},
	{"011100", "1"}, {"011101", "1"}, {"011110", "1"}, {"011111", "1"},

	{"100000", "0"}, {"100001", "0"}, {"100010", "1"}, {"100011", "1"},
	{"100100", "0"}, {"100101", "0"}, {"100110", "1"}, {"100111", "1"},
	{"101000", "0"}, {"101001", "0"}, {"101010", "1"}, {"101011", "1"},
	{"101100", "0"}, {"101101", "0"}, {"101110", "1"}, {"101111", "1"},

	{"110000", "0"}, {"110001", "1"}, {"110010", "0"}, {"110011", "1"},
	{"110100", "0"}, {"110101", "1"}, {"110110", "0"}, {"110111", "1"},
	{"111000", "0"}, {"111001", "1"}, {"111010", "0"}, {"111011", "1"},
	{"111100", "0"}, {"111101", "1"}, {"111110", "0"}, {"111111", "1"},
	};


	// Initialize the rules.
	std::vector<Rule> rules;
	rules.reserve(maxRules);  // Reserve space to optimize memory allocations
	for (int i = 0; i < populationSize; ++i) {
		rules.push_back(generateRandomRule(conditionLength, actionLength));
	}

	for (const auto& rule : rules) {
		std::cout << "Condition: " << rule.condition << " Action: " << rule.action << " Strength: " << rule.strength << "\n";
	}

	double bidPercentage = 0.05;
	double rewardAmount = 2.0;
	int repeatCount = 10;
	double taxRate = 0.1;
	double negativeRewardFactor = 0.5; // Factor to determine the negative reward based on the bid amount

	for (int gen = 0; gen < maxGenerations; ++gen) {

		runBucketBrigade(rules, trainingSet, bidPercentage, rewardAmount, negativeRewardFactor, repeatCount, taxRate, maxRules);
		geneticAlgorithm(rules, mutationRate, gen, trainingSet);
		std::cout << "Generation " << gen << ":\n";
		for (int i = 0; i < rules.size(); ++i) {
			std::cout << i + 1 << ". Condition: " << rules[i].condition
				<< ", Action: " << rules[i].action
				<< ", Strength: " << rules[i].strength << "\n";
		}

		// Wait for the user to press a key to continue
		std::cout << "Press ENTER to continue to the next generation...\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	// Isolating high-strength rules
	std::vector<Rule> highStrengthRules;
	double strengthThreshold = 10.0;
	std::copy_if(rules.begin(), rules.end(), std::back_inserter(highStrengthRules), [strengthThreshold](const Rule& rule) {
		return rule.strength > strengthThreshold;
		});

	std::cout << "High Strength Rules:\n";
	for (const auto& rule : highStrengthRules) {
		std::cout << "Condition: " << rule.condition << " Action: " << rule.action << " Strength: " << rule.strength << "\n";
	}
	// Final accuracy calculation
	int correctPredictions = 0;
	for (const auto& ts : trainingSet) {
		if (predictOutput(rules, ts.first) == ts.second) {
			correctPredictions++;
		}
	}

	double accuracy = static_cast<double>(correctPredictions) / trainingSet.size() * 100.0;
	std::cout << "Accuracy: " << accuracy << "%" << std::endl;

	return 0;
}

