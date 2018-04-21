#include<cstdio>
#include<vector>
#include<cassert>
#include<algorithm>
#include<tuple>
using namespace std;

struct Item
{
	int index;
	int value;
	int weight;

	// high value density item will come first
	bool operator < (const Item & rhs) const
	{
		return static_cast<float>(value) / weight > static_cast<float>(rhs.value) / rhs.weight;
	}
};

// get the max value expectation from current capacity and current undecided item
float get_expectation(const vector<Item> & items, int capacity, int start)
{
	auto expectation = 0.0f;
	for(auto i = start; i < items.size(); ++i)
	{
		auto item = items[i];
		if(capacity >= item.weight)
		{
			expectation += item.value;
			capacity -= item.weight;
		}
		// if current capacity is not enough to carry the whole item, then put a fraction of it into the knapsack
		// and add the same fraction of its value to the expectation
		else
		{
			expectation += static_cast<float>(item.value) * capacity / item.weight;
			break;
		}
	}

	return expectation;
}

// find max value and the take/no-take choice for each item
pair<int, vector<int>> search(const vector<Item> & items,  int capacity)
{
	auto max_value = 0.0f;
	auto max_taken = vector<int>(items.size(), 0);

	// to prevent from stack-overflow, instead of using plain recursion here I maintain the stack myself
	// a stack element includes 5 parts:
	// value:         value accumulated so far
	// capacity:      left capacity
	// expectation:   upper bound of value that can get with the left capacity
	// taken:         current take/no-take choice of each item
	// pos:           next item to consider

	auto start_value = 0.0f;
	auto start_capacity = capacity;
	auto start_expectation = get_expectation(items, capacity, 0);
	auto start_taken = vector<int>(items.size(), 0);
	auto start_pos = 0;

	using StackElem = tuple<float, int, float, vector<int>, int>;
	vector<StackElem> stack;
	stack.push_back(make_tuple(start_value, start_capacity, start_expectation, start_taken, start_pos));
	while(!stack.empty())
	{
		auto top_elem = stack.back();
		stack.pop_back();

		auto cur_value = get<0>(top_elem);
		auto cur_capacity = get<1>(top_elem);
		auto cur_expectation = get<2>(top_elem);
		auto cur_taken = get<3>(top_elem);
		auto cur_pos = get<4>(top_elem);

		// if left capacity is not enough, then backtrack
		if(cur_capacity < 0) continue;
		
		// if current expectation is smaller than the best value, then backtrack
		if(cur_expectation <= max_value) continue;

		// if max value is smaller than current value, update max value and its item-take choices
		if(max_value < cur_value)
		{
			max_value = cur_value;
			max_taken = cur_taken;
		}

		// if next item to consider dose not exist, then backtrack
		if(cur_pos >= items.size()) continue;

		auto cur_item = items[cur_pos];
    
		// try not to take the next item
        auto notake_value = cur_value;
        auto notake_capacity = cur_capacity;
        auto notake_expectation = notake_value + get_expectation(items, notake_capacity, cur_pos + 1);
        auto notake_taken = cur_taken;
        
        stack.push_back(make_tuple(notake_value, notake_capacity, notake_expectation, notake_taken, cur_pos + 1));
    
		// try to take the next item
        auto take_value = cur_value + cur_item.value;
        auto take_capacity = cur_capacity - cur_item.weight;
        auto take_expectation = take_value + get_expectation(items, take_capacity, cur_pos + 1);
        auto take_taken = cur_taken;
        take_taken[cur_item.index] = 1;
        
        stack.push_back(make_tuple(take_value, take_capacity, take_expectation, take_taken, cur_pos + 1));
	}
	return make_pair(max_value, max_taken);
}

// print the content of a vector
void print_vec(const vector<int> & vec)
{
	for(auto i = 0; i < vec.size(); ++i)
	{
		printf("%d", vec[i]);
		if(i + 1 == vec.size()) printf("\n");
		else printf(" ");
	}
}

int main(int argc, char * argv[])
{
	// you can change this line to try different input
	// but when submiting, makesure you are reading from python_input.txt
	auto f = fopen("python_input.txt", "r");
	//auto f = fopen("data/ks_4_0", "r");
	//auto f = fopen("data/ks_200_0", "r");
	assert(f);

	int item_count, capacity;
	fscanf(f, "%d %d", &item_count, &capacity);
	vector<Item> items;
	for(auto i = 0; i < item_count; ++i)
	{
		int value;
		int weight;
		fscanf(f, "%d %d", &value, &weight);

		items.push_back(Item{ i, value, weight });
	}

	fclose(f);

	// sort the items, so that they are in value density decreasing order
	sort(items.begin(), items.end());

	auto result = search(items, capacity);
	auto value = result.first;
	auto taken = result.second;

	printf("maximum value %d\n", value);
	printf("taken vector:\n");
	print_vec(taken);

	// write result to cpp_output.txt, so that solver.py can read result from it
	auto ff = fopen("cpp_output.txt", "w");
	assert(ff);

	fprintf(ff, "%d\n", value);
	for(auto i = 0; i < taken.size(); ++i)
	{
		fprintf(ff, "%d", taken[i]);
		if(i + 1 == taken.size()) fprintf(ff, "\n");
		else fprintf(ff, " ");
	}
	fclose(ff);
	return 0;
}
