#include <cstdio>
#include <cassert>
#include <utility>
#include <vector>
#include <limits>
#include <random>
#include <ctime>
#include <unordered_set>
#include <queue>

using namespace std;


// print color choice for every node
void print_color(const vector<int> & color)
{
	for(size_t i = 0; i < color.size(); ++i)
	{
		printf("%d", color[i]);
		if(i + 1 == color.size()) printf("\n");
		else printf(" ");
	}
	printf("\n");
}

// print neighbor connection of every node
void print_connection(const vector<vector<int>> & connection)
{
	for(auto & vec : connection)
	{
		print_color(vec);
	}
}

// random sample from a vector
int random_sample(const vector<int> & vec)
{
	assert(!vec.empty());

	static default_random_engine generator(time(nullptr));
	uniform_int_distribution<int> distribution(0, vec.size() - 1);
	auto random_index = distribution(generator);

	return vec[random_index];
}

// random sample from a range
int random_sample(int start, int end)
{
	assert(start <= end);

	static default_random_engine generator(time(nullptr));
	uniform_int_distribution<int> distribution(start, end);

	return distribution(generator);
}

// select next node to change color
int select_next_node(const vector<int> & violation, const vector<int> & color, unordered_set<int> & tabu_hash)
{
	auto max_violation = -1;
	vector<int> max_violation_node;

	for (size_t cur_node = 0; cur_node < violation.size(); ++cur_node)
	{
		// skip nodes with no violation
		if(violation[cur_node] == 0) continue;

		// skip nodes in tabu list
		if(tabu_hash.find(cur_node) != tabu_hash.end()) continue;

		// if violation is max violation, add the node to candidate list
		if (max_violation == violation[cur_node])
		{
			max_violation_node.push_back(cur_node);
		}
		// if violation is bigger than max violation, clear the candidate list and add the node
		else if (max_violation < violation[cur_node])
		{
			max_violation = violation[cur_node];
			max_violation_node.clear();
			max_violation_node.push_back(cur_node);
		}
	}

	if(max_violation == -1) return -1;

	// random sample a node from candidate list
	return random_sample(max_violation_node);
}


// change the color of a node
void change_color(int node, const vector<int> & node_neighbor, vector<int> & color, int total_color_count, vector<int> & violation, int & total_violation)
{
	// count the color distribution of neighbor nodes
	vector<int> color_count(total_color_count, 0);
	for (auto neighbor : node_neighbor)
	{
		auto neighbor_color = color[neighbor];
		++color_count[neighbor_color];
	}

	auto min_color_count = numeric_limits<int>::max();
	vector<int> min_color;

	// select color with least violation with neighbor
	for (auto cur_color = 0; cur_color < total_color_count; ++cur_color)
	{
		// skip its own color
		if(cur_color == color[node]) continue;

		// if the color violation is the min color violation, add the color to candidate list
		if(min_color_count == color_count[cur_color])
		{
			min_color.push_back(cur_color);
		}
		// if the color violation is smaller than the min color violation, clear the candidate list and add the color to it
		else if (min_color_count > color_count[cur_color])
		{
			min_color_count = color_count[cur_color];
			min_color.clear();
			min_color.push_back(cur_color);
		}
	}

	// must find at least one color
	assert(!min_color.empty());

	// random sample a color from the candidate list
	auto new_color = random_sample(min_color);

	// update violation for the node and its neighbor
	// update total violation
	for (auto neighbor : node_neighbor)
	{
		if(color[neighbor] == color[node])
		{
			--violation[neighbor];
			--violation[node];
			total_violation -= 2;
		}
		else if(color[neighbor] == new_color)
		{
			++violation[neighbor];
			++violation[node];
			total_violation += 2;
		}
	}

	color[node] = new_color;
}

// reinitialize color choice for every node
vector<int> reinitialize_color(const vector<int> & color, int total_color_count)
{
	vector<int> new_color(color.size());
	for(auto & c : new_color)
	{
		c = random_sample(0, total_color_count - 1);
	}

	return new_color;
}


// count violation for every node and count total violation
pair<vector<int>, int> init_violation(const vector<vector<int>> & connection, const vector<int> & color)
{
	vector<int> violation;
	int total_violation = 0;
	for (size_t cur_node = 0; cur_node < connection.size(); ++cur_node)
	{
		auto cur_violation = 0;
		for (auto neighbor : connection[cur_node])
		{
			cur_violation += color[cur_node] == color[neighbor];
		}
		violation.push_back(cur_violation);
		total_violation += cur_violation;
	}

	return make_pair(violation, total_violation);
}

// check feasibility of current number of color
pair<bool, int> is_feasible(const vector<vector<int>> & connection, vector<int> & color, int total_color_count, int tabu_limit)
{
	// maximum step to try
	// one step means change the color of a node
	auto STEP_LIMIT = 50000;
	auto step_count = 0;

	auto violation_and_total_violation = init_violation(connection, color);
	auto violation = violation_and_total_violation.first;
	auto total_violation = violation_and_total_violation.second;

	// tabu hash table and tabu queue, they contain same data
	// use hash table to accelerate retrieval, use queue to make the tabu list FIFO (First In First Out)
	unordered_set<int> tabu_hash;
	queue<int> tabu_queue;

	while (true)
	{
		// if no violation, then current setting is feasible
		if(total_violation == 0)
		{
			return make_pair(true, step_count);
		}

		// select next node to change color
		auto node = select_next_node(violation, color, tabu_hash);

		// if cannot select next node, maybe the tabu list is too long, then pop one element from the tabu list
		if(node == -1)
		{
			auto to_pop = tabu_queue.front();
			tabu_hash.erase(to_pop);
			tabu_queue.pop();
			continue;
		}
		
		// add the selected node to tabu list
		tabu_hash.insert(node);
		tabu_queue.push(node);
		// if the tabu list exceeds the limit, pop one element out
		if(tabu_hash.size() > tabu_limit)
		{
			auto to_pop = tabu_queue.front();
			tabu_hash.erase(to_pop);
			tabu_queue.pop();
		}

		// change color of the selected code
		change_color(node, connection[node], color, total_color_count, violation, total_violation);
		
		// increase step, if exceeds limit then stop searching
		++step_count;
		if(step_count >= STEP_LIMIT) break;
	}

	return make_pair(false, step_count);
}


// remove a color from current color choice
// it works like this, suppose there are total 10 colors from 0 ~ 9, the current color choice is:
// 7 1 4 2 5 9 0 3 6 8 5 9 0 4 
// if we want to remove color 5, for each color that is bigger than 5, we minus it by 1:
// 6 1 4 2 5 8 0 3 5 7 5 8 0 4
// then for each color equals to 5 we set it to a random color from 0 ~ 8:
// 6 1 4 2 3 8 0 3 1 7 4 8 0 4
vector<int> remove_color(const vector<int> & color, int total_color_count)
{
	static default_random_engine generator(time(nullptr));
	uniform_int_distribution<int> distribution(0, total_color_count - 2);

	uniform_int_distribution<int> distribution_for_remove(0, total_color_count - 1);
	auto to_remove = distribution_for_remove(generator);

	vector<int> new_color;
	for(auto c : color)
	{
		if(c == to_remove) new_color.push_back(distribution(generator));
		else if(c > to_remove) new_color.push_back(c - 1);
		else new_color.push_back(c);
	}

	return new_color;
}



// search the minimum color for a graph
// return the color choice of every node and the total number of color
pair<vector<int>, int> search(const vector<vector<int>> & connection)
{
	vector<int> color(connection.size());
	auto total_color_count = static_cast<int>(color.size());

	color = reinitialize_color(color, total_color_count);

	// set the length of tabu list to 1/10 of the number of nodes
	auto tabu_limit = max<int>(connection.size() / 10, 1);

	vector<int> feasible_color{-1};
	auto feasible_color_count = -1;

	for(auto cur_color_count = total_color_count; cur_color_count > 0; --cur_color_count)
	{
		// times to retry if did not find feasible solution in a given number of steps.
		auto RETRY_LIMIT = 100;
		auto retry_count = 0;
		while(true)
		{
			auto feasible_and_step_count = is_feasible(connection, color, cur_color_count, tabu_limit);
			auto feasible = feasible_and_step_count.first;
			auto step_count = feasible_and_step_count.second;

			if(feasible)
			{
				printf("%d color is feasible, tried %d step\n", cur_color_count, step_count);
				feasible_color = color;
				feasible_color_count = cur_color_count;

				color = remove_color(feasible_color, feasible_color_count);
				break;
			}

			++retry_count;
			if(retry_count >= RETRY_LIMIT)
			{
				 return make_pair(feasible_color, feasible_color_count);
			}

			printf("[Color %4d][Retry %5d] reinitializing color\n", cur_color_count, retry_count);
			color = remove_color(feasible_color, feasible_color_count);
		}
	}

	return make_pair(feasible_color, feasible_color_count);
}

int main()
{
	// you can change these lines to try different input files
	// when submit, make sure you input from python_input.txt
	auto f = fopen("python_input.txt", "r");
	//auto f = fopen("data/gc_1000_5", "r");
	//auto f = fopen("data/gc_250_9", "r");
	//auto f = fopen("data/gc_4_1", "r");
	assert(f);

	int node_count, edge_count;
	assert(fscanf(f, "%d %d", &node_count, &edge_count) == 2);
	printf("node : %d, edge : %d\n", node_count, edge_count);

	vector<vector<int>> connection(node_count);
	for (auto i = 0; i < edge_count; ++i)
	{
		int vs, ve;
		assert(fscanf(f, "%d %d", &vs, &ve) == 2);
		connection[vs].push_back(ve);
		connection[ve].push_back(vs);
	}
	printf("neighbor for each node:\n");
	print_connection(connection);
	printf("\n");

	fclose(f);

	auto result = search(connection);
	auto feasible_color = result.first;
	auto feasible_color_count = result.second;

	printf("final result:\n");
	printf("%d %d\n", feasible_color_count, 0);
	print_color(feasible_color);

	// write the output to cpp_output.txt
	// the solver.py will read result from this file
	auto ff = fopen("cpp_output.txt", "w");
	fprintf(ff, "%d\n", feasible_color_count);
	for(size_t i = 0; i < feasible_color.size(); ++i)
	{
		fprintf(ff, "%d", feasible_color[i]);
		if(i + 1 == feasible_color.size()) fprintf(ff, "\n");
		else fprintf(ff, " ");
	}

	return 0;
}
