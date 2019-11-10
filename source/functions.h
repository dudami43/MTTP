double first_improvement_shuffle(Instance& inst);
double first_improvement_swap(Instance& inst);
double first_improvement_move(Instance& inst);
double first_improvement_trade_ungotten(Instance& inst);
double localSearch(Instance& inst, std::string method);

double VNS(Instance& inst, int max_disturbance, int n_disturbe = 5, bool verbose = false);

void initial_pop(Instance& inst, std::vector<Instance>& population, int pop_size, bool verbose = false);
void selection(std::vector<Instance>& population, std::vector<Instance>& mating_pool, int alpha, int pop_size, bool verbose = false);
void crossover(std::vector<Instance>& mating_pool, std::vector<Instance>& next_gen, int alpha, int pop_size, bool verbose = false);
void mutation(std::vector<Instance>& generation, int beta, int pop_size, bool verbose = false);
void validation(std::vector<Instance>& generation, bool verbose);
double geneticAlgorithm(Instance& instance, int num_generations = 10, int pop_size = 20, double alpha = 0.8, double beta = 0.08, bool verbose = false);

void random_greedy(Instance& instance,  int ir_max, double size_rlc);
void random_greedy2(Instance& instance,  int ir_max, double size_rlc);
double grasp(Instance& instance, int imax = 20,  int ir_max = 2, double size_rlc = 0.1, bool verbose = false, int func = 1);
