


Emmanouil Marantidis
------------------------
My Thesis from University !!
-------------------------------------------------------------------------------------------------

Title : 
Implementation of a Classifier System with a fixed number of rules, capable of chain rule activation (reasoning) for application to clustering problems.

The purpose of this work is to develop an example Classifier System with the ability to genetically evolve sets of rules (if then rules) that simulate a desired input-output behavior. 
A CS model will be developed that has a fixed set of rules, with "rule forces" and a Bucket Brigade Algorithm, which will be applied to a simple 
classification problem (Classification) The CS rule set will be genetically evolved using a Genetic Algorithm, which will use : 

1. Population of solutions (rules)
2.  Parent selection method (Roulette wheel)
3.  Multi-point Crossover operator (Multi-point Crossover)
4.  Mutation operator (Mutation)
5.  Elitism
6.  Variable termination criteria
7.   User-defined parameters In this work, the effectiveness of CS in application problems will be studied for various values of the parameters.

Overview
-------- 
Key Components in The Code
---------------------------

- Population of solutions (rules):
  - The std::vector<Rule> represents the population of rules, where each rule has a condition, action, and strength.

- Parent selection method (Roulette wheel):
  - The rouletteWheelSelection function implements the roulette wheel selection method to choose parents based on their strength.

- Multi-point Crossover operator (Multi-point Crossover):
  - The multiPointCrossover function performs a multi-point crossover between two parent rules to produce offspring.

- Mutation operator (Mutation):
  - The mutate function applies mutations to the condition and action parts of a rule with a specified mutation rate.

- Elitism:
  - The findEliteIndices and geneticAlgorithm functions ensure that the best-performing rules (elites) are carried over to the next generation without modification.

- Variable termination criteria:
  - The code terminates after a fixed number of generations (maxGenerations). 

- User-defined parameters:
  - Parameters such as populationSize, maxRules, maxGenerations, mutationRate, bidPercentage, rewardAmount, repeatCount, taxRate, negativeRewardFactor, strengthThreshold, and elitismRate are defined and can be adjusted by the user.


How The Code Maps to the Description
------------------------------------

- Classifier System (CS) with fixed set of rules:
  - The rules are stored in a fixed-size vector std::vector<Rule> rules.

- Genetic Evolution of Rule Sets:
  - Implemented through functions like geneticAlgorithm, mutate, multiPointCrossover, and rouletteWheelSelection.

- Bucket Brigade Algorithm:
  - The runBucketBrigade function implements the bucket brigade algorithm to evaluate and update the strength of rules based on their performance.

- Application to Classification Problem:
  - The training set std::vector<std::pair<std::string, std::string>> trainingSet and the function predictOutput are used to apply the evolved rules to a classification problem.

- User-defined Parameters:
  - The code allows for various user-defined parameters to be set and adjusted, impacting the behavior and effectiveness of the classifier system.


Detailed Mapping
----------------

	Initialization:
		std::vector<Rule> rules initialized in the main function.

	Rule Generation:
		generateRandomRule function generates initial rules.

	Selection:
		rouletteWheelSelection function selects parents based on rule strength.

	Crossover:
		multiPointCrossover function combines parts of two parent rules to produce offspring.

	Mutation:
		mutate function introduces random changes to rules.

	Elitism:
		findEliteIndices identifies the strongest rules to carry over.

	Rule Evaluation:
		runBucketBrigade evaluates rule performance and updates strengths using a bucket brigade algorithm.

	Training and Prediction:
		trainingSet provides input-output pairs for training.
		predictOutput uses the evolved rules to predict outputs for given inputs.

	User-defined Parameters:
		Parameters such as populationSize, maxRules, mutationRate, etc., are defined and can be adjusted.

In summary, the code is a good implementation of a classifier system with a genetic algorithm and a bucket brigade algorithm, fulfilling the requirements mentioned in the description. It includes all the necessary components for evolving rules, selecting parents, performing crossover and mutation, ensuring elitism, and applying the system to a classification problem.

Detailed Description of Each Function:
--------------------------------------

--> "struct Rule" <--
---------------------

	struct Rule {
    		std::string condition;
    		std::string action;
    		double strength;
    		Rule(const std::string& condition, const std::string& action, double strength)
        		: condition(condition), action(action), strength(strength) {}
	};

Purpose: 
- Represents a rule with a condition, action, and strength.

Components:
- condition: A string representing the condition part of the rule.
- action: A string representing the action part of the rule.
- strength: A double value representing the strength or effectiveness of the rule.
      
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--> "rnd()" <--
---------------

	double rnd() {
    		std::uniform_real_distribution<double> dist(0.0, 1.0);
    		return dist(rng);
	}

Purpose: 
- Generates a random double between 0.0 and 1.0 using a uniform distribution.

Usage: 
- Used in mutation and selection processes to introduce randomness.
    
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--> "mutateChar(char currentChar, std::mt19937& rng)" <--
---------------------------------------------------------

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

Purpose:
- Mutates a character by randomly selecting an alternative character.

Usage:
- Used in the mutate function to alter characters in the condition and action strings.
    
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--> "generateRandomRule(int conditionLength, int actionLength)" <--
-------------------------------------------------------------------

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

Purpose:
- Generates a random rule with a specified length for the condition and action.

Usage:
- Used to initialize the population of rules in the main function.
    
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--> "mutate(Rule& rule, double mutationRate, std::mt19937& rng)" <--
--------------------------------------------------------------------

		void mutate(Rule& rule, double mutationRate, std::mt19937& rng) {
    			for (auto& c : rule.condition) {
        			if (rnd() < mutationRate) {
            				c = mutateChar(c, rng);
        			}
    			}
    			for (auto& c : rule.action) {
        			if (rnd() < mutationRate) {
            				c = (c == '0') ? '1' : '0';
        			}
    			}
		}

Purpose:
- Mutates the condition and action parts of a rule based on a mutation rate.

Usage:
- Applied during the genetic algorithm's mutation step.
    
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--> "rouletteWheelSelection(const std::vector<Rule>& rules)" <--
----------------------------------------------------------------

		Rule rouletteWheelSelection(const std::vector<Rule>& rules) {
    			double totalStrength = std::accumulate(rules.begin(), rules.end(), 0.0,
        		[](double sum, const Rule& r) { return sum + r.strength; });
    				double value = rnd() * totalStrength;
    				double partialSum = 0.0;
    			for (const auto& rule : rules) {
        			partialSum += rule.strength;
        			if (partialSum > value || &rule == &rules.back()) {
            				return rule;
       		 		}
    			}
    			return rules.back();
		}

Purpose: 
- Selects a rule based on its strength using roulette wheel selection.

Usage: 
- Used during the selection step of the genetic algorithm.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--> "multiPointCrossover(Rule& parent1, Rule& parent2)" <--
-----------------------------------------------------------

		void multiPointCrossover(Rule& parent1, Rule& parent2) {
    			std::string genotype1 = parent1.condition + parent1.action;
    			std::string genotype2 = parent2.condition + parent2.action;
    			int point1 = rand() % genotype1.size();
    			int point2 = rand() % genotype1.size();
    			if (point1 > point2) {
        			std::swap(point1, point2);
    			}
    			for (int i = point1; i <= point2; ++i) {
        			std::swap(genotype1[i], genotype2[i]);
    			}
    			parent1.condition = genotype1.substr(0, parent1.condition.size());
    			parent1.action = genotype1.substr(parent1.condition.size());
   	 		parent2.condition = genotype2.substr(0, parent2.condition.size());
    			parent2.action = genotype2.substr(parent2.condition.size());
		}

Purpose: 
- Performs a multi-point crossover on two parent rules to create offspring.

Usage: 
- Used during the crossover step of the genetic algorithm.

-------------------------------------------------------------------------------------------------

--> "geneticAlgorithm(std::vector<Rule>& rules, double mutationRate, int generation, const std::vector<std::pair<std::string, std::string>>& trainingSet, double elitismRate)" <--
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	void geneticAlgorithm(std::vector<Rule>& rules, double mutationRate, int generation, const std::vector<std::pair<std::string, std::string>>& trainingSet, double elitismRate) {
    		int elitismCount = static_cast<int>(rules.size() * elitismRate);
    		std::vector<int> eliteIndices = findEliteIndices(rules, elitismCount);
    		std::vector<Rule> newGeneration;
    		newGeneration.reserve(rules.size());

    		for (int index : eliteIndices) {
        		if (index >= 0 && index < rules.size()) {
            			newGeneration.push_back(rules[index]);
        		}
    		}

    		while (newGeneration.size() < rules.size()) {
        		Rule parent1 = rouletteWheelSelection(rules);
        		Rule parent2 = rouletteWheelSelection(rules);
        		multiPointCrossover(parent1, parent2);

        		double averageStrength = (parent1.strength + parent2.strength) / 2.0;
        		parent1.strength = averageStrength;
        		parent2.strength = averageStrength;

        		mutate(parent1, mutationRate, rng);
        		mutate(parent2, mutationRate, rng);

        		newGeneration.push_back(parent1);
        		if (newGeneration.size() < rules.size()) {
            			newGeneration.push_back(parent2);
        		}
    		}

    		rules.swap(newGeneration);
	}

 
Purpose: 
- Runs the genetic algorithm for one generation, including selection, crossover, mutation, and elitism.
Usage:
- Called repeatedly in the main function to evolve the rules over multiple generations.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--> "generateRuleForSample(const std::string& input, double initialStrength, int actionLength)" <--
---------------------------------------------------------------------------------------------------


	Rule generateRuleForSample(const std::string& input, double initialStrength, int actionLength) {
    		std::string condition;
    		std::uniform_int_distribution<int> dist(0, 2);
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

Purpose: 
- Generates a new rule for a given input sample.
Usage:
- Used in runBucketBrigade to generate new rules when needed.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--> "findIndexOfLeastEffectiveRule(const std::vector<Rule>& rules)" <--
-----------------------------------------------------------------------

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

Purpose: 
- Finds the index of the rule with the lowest strength.
Usage:
- Used in runBucketBrigade to identify and replace the least effective rules.


----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--> "runBucketBrigade(std::vector<Rule>& rules, std::vector<std::pair<std::string, std::string>>& trainingSet, double bidPercentage, double rewardAmount, double negativeRewardFactor, int repeatCount, double taxRate, int maxRules)" <--
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


	bool runBucketBrigade(std::vector<Rule>& rules, std::vector<std::pair<std::string, std::string>>& trainingSet, double bidPercentage, double rewardAmount, double negativeRewardFactor, int repeatCount, double taxRate, int maxRules) {
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
                		std::sort(matchedRules.begin(), matchedRules.end(), [](const Rule* a, const Rule* b) {
                    			return a->strength > b->strength;
                		});

                		std::vector<Rule*> topTiedRules;
                		double topStrength = matchedRules.front()->strength;
                		std::copy_if(matchedRules.begin(), matchedRules.end(), std::back_inserter(topTiedRules), [topStrength](const Rule* rule) {
                    			return rule->strength == topStrength;
                		});

                		if (topTiedRules.size() > 1) {
                    			std::uniform_int_distribution<size_t> dist(0, topTiedRules.size() - 1);
                    			bestRule = topTiedRules[dist(rng)];
                		} else {
		                    	bestRule = matchedRules.front();
                		}
            		}

            		if (bestRule) {
                		double bidAmount = bestRule->strength * bidPercentage;
                		bestRule->strength -= bidAmount;

                		if (bestRule->action == expectedOutput) {
                    			bestRule->strength += rewardAmount;
                    			correctPredictions++;
                		} else {
                    			double negativeReward = bidAmount * negativeRewardFactor;
                    			bestRule->strength -= negativeReward;
                    			if (bestRule->strength < 0) bestRule->strength = 0;
                		}
            		} else if (rules.size() >= maxRules) {
                		int leastEffectiveRuleIndex = findIndexOfLeastEffectiveRule(rules);
                		if (leastEffectiveRuleIndex != -1) {
                    			rules[leastEffectiveRuleIndex] = generateRuleForSample(input, 10.0, 1);
                		}
            		} else {
                		rules.push_back(generateRuleForSample(input, 10.0, 1));
            		}
        	}

        	for (auto& rule : rules) {
            		rule.strength *= (1.0 - taxRate);
        	}
    	}
    	return true;
     	}


Purpose: 
- Implements the bucket brigade algorithm to evaluate and update the strength of rules based on their performance on the training set.
Usage:
- Called in the main function to perform rule evaluation and updating.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--> "predictOutput(const std::vector<Rule>& rules, const std::string& input)" <--
---------------------------------------------------------------------------------

	std::string predictOutput(const std::vector<Rule>& rules, const std::string& input) {
    		std::vector<const Rule*> matchedRules;

    		for (const auto& rule : rules) {
        		if (std::equal(rule.condition.begin(), rule.condition.end(), input.begin(),
            		[](char r, char i) { return r == '#' || r == i; })) {
           		 	matchedRules.push_back(&rule);
        		}
    		}

    		if (matchedRules.empty()) {
        		return "";
    		}

    		std::sort(matchedRules.begin(), matchedRules.end(), [](const Rule* a, const Rule* b) {
        		return a->strength > b->strength;
    		});

    		std::vector<const Rule*> topTiedRules;
    		double topStrength = matchedRules.front()->strength;
    		std::copy_if(matchedRules.begin(), matchedRules.end(), std::back_inserter(topTiedRules), [topStrength](const Rule* rule) {
        		return rule->strength == topStrength;
    		});

    		static std::mt19937 rng(std::random_device{}());
    		if (topTiedRules.size() > 1) {
        		std::uniform_int_distribution<size_t> dist(0, topTiedRules.size() - 1);
        		return topTiedRules[dist(rng)]->action;
    		}

    		return matchedRules.front()->action;
	}


Purpose: 
- Predicts the output for a given input based on the rules, considering the strength of matching rules.
Usage:
- Used to evaluate the accuracy of the evolved rules on the training set.


----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-->"main()"<--
--------------

    int main() {
    srand(static_cast<unsigned>(time(0)));

    int populationSize = 45, maxRules = 60, maxGenerations = 30000;
    double mutationRate = 0.10;
    double bidPercentage = 0.05;
    double rewardAmount = 2.0;
    int repeatCount = 20;
    double taxRate = 0.10;

    double negativeRewardFactor = 1.25;
    double strengthThreshold = 25;
    double elitismRate = 0.25;

    int conditionLength = 6;
    int actionLength = 1;

    // Set precision for floating-point numbers
    std::cout << std::fixed << std::setprecision(2);

    // Print the table header
    std::cout << std::setw(30) << "Parameter" << std::setw(15) << "Value" << std::endl;
    std::cout << std::string(45, '-') << std::endl;

    // Print the parameters in a tabular format
    std::cout << std::setw(30) << "Population Size" << std::setw(15) << populationSize << std::endl;
    std::cout << std::setw(30) << "Max Rules" << std::setw(15) << maxRules << std::endl;
    std::cout << std::setw(30) << "Max Generations" << std::setw(15) << maxGenerations << std::endl;
    std::cout << std::setw(30) << "Mutation Rate" << std::setw(15) << mutationRate << std::endl;
    std::cout << std::setw(30) << "Bid Percentage" << std::setw(15) << bidPercentage << std::endl;
    std::cout << std::setw(30) << "Reward Amount" << std::setw(15) << rewardAmount << std::endl;
    std::cout << std::setw(30) << "Repeat Count" << std::setw(15) << repeatCount << std::endl;
    std::cout << std::setw(30) << "Tax Rate" << std::setw(15) << taxRate << std::endl;
    std::cout << std::setw(30) << "Negative Reward Factor" << std::setw(15) << negativeRewardFactor << std::endl;
    std::cout << std::setw(30) << "Strength Threshold" << std::setw(15) << strengthThreshold << std::endl;
    std::cout << std::setw(30) << "Elitism Rate" << std::setw(15) << elitismRate << std::endl;
    std::cout << std::setw(30) << "Condition Length" << std::setw(15) << conditionLength << std::endl;
    std::cout << std::setw(30) << "Action Length" << std::setw(15) << actionLength << std::endl;

    std::cout << std::string(45, '-') << std::endl;
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

    std::vector<Rule> rules;
    rules.reserve(maxRules);
    for (int i = 0; i < populationSize; ++i) {
        rules.push_back(generateRandomRule(conditionLength, actionLength));
    }

    std::vector<double> accuracies(10);

    std::time_t now = std::time(nullptr);
    std::tm timeStruct = {};

    char buffer[80];
    localtime_s(&timeStruct, &now);
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", &timeStruct);
    std::string filename = "final_results_" + std::string(buffer) + ".txt";

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return 1;
    }

    // Set precision for floating-point numbers
    std::cout << std::fixed << std::setprecision(2);

    // Print the table header
    outFile << std::setw(30) << "Parameter" << std::setw(15) << "Value" << std::endl;
    outFile << std::string(45, '-') << std::endl;

    // Print the parameters in a tabular format
    outFile << std::setw(30) << "Population Size" << std::setw(15) << populationSize << std::endl;
    outFile << std::setw(30) << "Max Rules" << std::setw(15) << maxRules << std::endl;
    outFile << std::setw(30) << "Max Generations" << std::setw(15) << maxGenerations << std::endl;
    outFile << std::setw(30) << "Mutation Rate" << std::setw(15) << mutationRate << std::endl;
    outFile << std::setw(30) << "Bid Percentage" << std::setw(15) << bidPercentage << std::endl;
    outFile << std::setw(30) << "Reward Amount" << std::setw(15) << rewardAmount << std::endl;
    outFile << std::setw(30) << "Repeat Count" << std::setw(15) << repeatCount << std::endl;
    outFile << std::setw(30) << "Tax Rate" << std::setw(15) << taxRate << std::endl;
    outFile << std::setw(30) << "Negative Reward Factor" << std::setw(15) << negativeRewardFactor << std::endl;
    outFile << std::setw(30) << "Strength Threshold" << std::setw(15) << strengthThreshold << std::endl;
    outFile << std::setw(30) << "Elitism Rate" << std::setw(15) << elitismRate << std::endl;
    outFile << std::setw(30) << "Condition Length" << std::setw(15) << conditionLength << std::endl;
    outFile << std::setw(30) << "Action Length" << std::setw(15) << actionLength << std::endl;

    outFile << std::string(45, '-') << std::endl;

    for (int runcounter = 0; runcounter < 10; ++runcounter) {
        outFile << "Run : " << runcounter << "\n" << std::endl;
        for (int gen = 0; gen < maxGenerations; ++gen) {
            runBucketBrigade(rules, trainingSet, bidPercentage, rewardAmount, negativeRewardFactor, repeatCount, taxRate, maxRules);
            geneticAlgorithm(rules, mutationRate, gen, trainingSet, elitismRate);
        }

        std::vector<Rule> highStrengthRules;
        std::copy_if(rules.begin(), rules.end(), std::back_inserter(highStrengthRules), [strengthThreshold](const Rule& rule) {
            return rule.strength > strengthThreshold;
        });

        int correctPredictions = 0;
        for (const auto& ts : trainingSet) {
            if (predictOutput(rules, ts.first) == ts.second) {
                correctPredictions++;
            }
        }

        double accuracy = static_cast<double>(correctPredictions) / trainingSet.size() * 100.0;
        accuracies[runcounter] = accuracy;

        outFile << "Final High Strength Rules:\n";
        std::cout << "Final High Strength Rules:\n";
        for (const auto& rule : highStrengthRules) {
            outFile << "Condition: " << rule.condition << " Action: " << rule.action << " Strength: " << rule.strength << "\n" << std::endl;
            std::cout << "Condition: " << rule.condition << " Action: " << rule.action << " Strength: " << rule.strength << "\n";
        }

        outFile << "ALL RULES " << "\n" << std::endl;
        for (int i = 0; i < rules.size(); ++i) {
            outFile << "Rule " << i + 1 << ": Condition = " << rules[i].condition
                    << ", Action = " << rules[i].action
                    << ", Strength = " << rules[i].strength << "\n" << std::endl;
        }
        std::cout << "Final Accuracy: " << accuracy << "%\n";
        outFile << "\nFinal Accuracy: " << accuracy << "%\n" << std::endl;
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n";
    }

    double averageAccuracy = std::accumulate(accuracies.begin(), accuracies.end(), 0.0) / accuracies.size();
    double maxAccuracy = *std::max_element(accuracies.begin(), accuracies.end());
    outFile << "                   \n";
    outFile << "                   \n";
    outFile << "Average Accuracy: " << averageAccuracy << "%\n" << std::endl;
    outFile << "Maximum Accuracy: " << maxAccuracy << "%\n" << std::endl;

    outFile.close();
    if (outFile.fail()) {
        std::cerr << "Error occurred during file operation on: " << filename << "\n";
        return 1;
    }

    return 0;
    }

Purpose: 
- The main function initializes parameters, generates initial rules, runs the genetic algorithm over multiple generations, evaluates the results, and writes the final rules and accuracies to a file.

Steps:
- Initialize parameters and the training set.
- Generate the initial population of rules.
- Open a file to write results.
- Run the genetic algorithm and bucket brigade algorithm for a specified number of runs and generations.
- Evaluate the rules and write the results to the file.
