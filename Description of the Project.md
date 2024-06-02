Overview

The code implements a genetic algorithm for rule-based learning, where rules are generated, mutated, selected, and evolved over multiple generations to solve a classification problem. The rules are represented by conditions and actions, and their strength indicates their effectiveness. The main steps include rule generation, selection, crossover, mutation, and evaluation.


Detailed Description of Each Function:
-------------------------------------------------------------------------------------------------

--> "struct Rule" <--

	struct Rule {
    		std::string condition;
    		std::string action;
    		double strength;
    		Rule(const std::string& condition, const std::string& action, double strength)
        		: condition(condition), action(action), strength(strength) {}
	};

Purpose: Represents a rule with a condition, action, and strength.

Components:
		
  condition: A string representing the condition part of the rule.
  
  action: A string representing the action part of the rule.
  
  strength: A double value representing the strength or effectiveness of the rule.
      
-------------------------------------------------------------------------------------------------

--> "rnd()" <--

	double rnd() {
    		std::uniform_real_distribution<double> dist(0.0, 1.0);
    		return dist(rng);
	}

Purpose: Generates a random double between 0.0 and 1.0 using a uniform distribution.

Usage: Used in mutation and selection processes to introduce randomness.
    
-------------------------------------------------------------------------------------------------

--> "mutateChar(char currentChar, std::mt19937& rng)" <--

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

Purpose: Mutates a character by randomly selecting an alternative character.

Usage: Used in the mutate function to alter characters in the condition and action strings.
    
-------------------------------------------------------------------------------------------------

--> "generateRandomRule(int conditionLength, int actionLength)" <--

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

Purpose: Generates a random rule with a specified length for the condition and action.

Usage: Used to initialize the population of rules in the main function.
    
-------------------------------------------------------------------------------------------------

--> "mutate(Rule& rule, double mutationRate, std::mt19937& rng)" <--

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

Purpose: Mutates the condition and action parts of a rule based on a mutation rate.

Usage: Applied during the genetic algorithm's mutation step.
    
-------------------------------------------------------------------------------------------------

--> "rouletteWheelSelection(const std::vector<Rule>& rules)" <--

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

Purpose: Selects a rule based on its strength using roulette wheel selection.

Usage: Used during the selection step of the genetic algorithm.

-------------------------------------------------------------------------------------------------

--> "multiPointCrossover(Rule& parent1, Rule& parent2)" <--

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

Purpose: Performs a multi-point crossover on two parent rules to create offspring.

Usage: Used during the crossover step of the genetic algorithm.

-------------------------------------------------------------------------------------------------
