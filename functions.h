double first_improvement_swap(Instance& inst);
double first_improvement_move(Instance& inst);
double first_improvement_trade(Instance& inst);
double first_improvement_trade_ungetted(Instance& inst);
double localSearch(Instance& inst, std::string method);
double call_localSearch(Instance& instance, std::string method);

double VNS(Instance& inst, int max_disturbance, bool verbose = false);

void initial_pop(Instance& inst, std::vector<Instance>& population, bool verbose = false);
void selection(std::vector<Instance>& population, std::vector<Instance>& mating_pool, bool verbose = false);
void crossover(std::vector<Instance>& mating_pool, std::vector<Instance>& next_gen, bool verbose = false);
void mutation(std::vector<Instance>& generation, bool verbose = false);
void validation(std::vector<Instance>& generation, bool verbose);
double geneticAlgorithm(Instance& instance, int num_generations = 10, bool verbose = false);
