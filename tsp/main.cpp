// This code implements Guided Fast Local Search for the Traveling Salesman Problem
// In a nutshell, the Guided Local Search tries to attach some feature to each potential solution
// and to punish those bad features that are unlikely to produce optimal solution.
// While Fast Local Search divide the search space into a number of sub-space, and iterate through these
// sub-space sequentially. During the iteration, sub-spaces that are likely to produce optimal solution are activated, while
// sub-spaces that are unlikely to produce optimal solution are inactivated.
// For more information please refer to "Guided Local Search - Christos Voudouris and Edward P.K. Tsang"

#include<cstdio>
#include<vector>
#include<random>
#include<cmath>
#include<functional>
#include<limits>
#include<cassert>

using namespace std;
using DistanceMatrix = function<double(int, int)>;

struct Node
{
    double x, y;
};

struct Connection
{
    int in;
    int out;
};


struct Penalty
{
    Penalty(int node_count)
    {
        for(auto i = 1; i <= node_count; ++i)
        {
            penalty.push_back(vector<int>(i, 0));
        }
    }

    int & operator() (int i, int j)
    {
        if(j > i) swap(i, j);
        return penalty[i][j];
    }

    const int & operator() (int i, int j) const
    {
        if(j > i) swap(i, j);
        return penalty[i][j];
    }

    vector<vector<int>> penalty;
};

struct Activate
{
    Activate(int _size): bits(_size, 1), ones(_size) {}

    void set_1(int i)
    {
        ones += bits[i] == 0;
        bits[i] = 1;
    }

    void set_0(int i)
    {
        ones -= bits[i] == 1;
        bits[i] = 0;
    }

    int get(int i) const
    {
        return bits[i];
    }

    size_t size() const
    {
        return bits.size();
    }

    vector<int> bits;
    int ones;
};

void print_tour(const vector<Connection> & connection, FILE * f = stdout)
{
    auto node = 0;
    for(auto i = 0; i < connection.size(); ++i)
    {
        fprintf(f, "%d", node);
        fprintf(f, i + 1 == connection.size() ? "\n" : " ");
        node = connection[node].out;
    }
}

DistanceMatrix get_distance_matrix(const vector<Node> & node_vec)
{
    auto square = [](double x) { return x * x; };
    auto distance = [square](Node a, Node b) { return sqrt(square(a.x - b.x) + square(a.y - b.y)); };
    auto distance_matrix = [&node_vec, distance](int i, int j) { return distance(node_vec[i], node_vec[j]); };
    return distance_matrix;
}


vector<Connection> init_connection(int node_count, DistanceMatrix distance_matrix)
{
    vector<int> tour;
    for(auto i = 0; i < node_count; ++i)
    {
        tour.push_back(i);
    }

    for(auto i = 0; i + 1 < tour.size(); ++i)
    {
        auto min_distance = (numeric_limits<double>::max)();
        auto min_distance_node = -1;
        
        for(auto j = i + 1; j < tour.size(); ++j)
        {
            auto distance = distance_matrix(tour[i], tour[j]);
            if(min_distance > distance)
            {
                min_distance = distance;
                min_distance_node = j;
            }
        }
        swap(tour[i + 1], tour[min_distance_node]);
    }

    vector<Connection> connection(node_count);
    for (auto i = 0; i < tour.size(); ++i)
    {
        auto node = tour[i];
        auto next_node = tour[(i + 1) % tour.size()];
        connection[node].out = next_node;
        connection[next_node].in = node;
    }
    return connection;
}


// random sample from a vector
template<typename T>
T random_sample(const vector<T> & vec)
{
	assert(!vec.empty());

	static default_random_engine generator(time(nullptr));
	uniform_int_distribution<size_t> distribution(0, vec.size() - 1);
	auto random_index = distribution(generator);

	return vec[random_index];
}


// swap two edges, with four vertexes: t1, t2, t3, t4
// before swapping, the two edges are: t1 -> t2, t3 -> t4
// after swapping, the two edges are: t1 -> t3, t2 -> t4


tuple<int, int> select_t3_t4(int t1, int t2, const vector<Connection> & connection, DistanceMatrix distance_matrix, 
                            const Penalty & penalty, double lambda)
{
    auto max_gain = -(numeric_limits<double>::max)();
    auto t4_candidate = vector<int>();
    auto t2_out = connection[t2].out;
    for(auto i = 0; i < connection.size(); ++i)
    {
        auto t4 = i;
        auto t3 = connection[t4].in;

        if(t4 == t1 || t4 == t2 || t4 == t2_out) continue;

        auto d12 = distance_matrix(t1, t2);
        auto d34 = distance_matrix(t3, t4);
        auto d13 = distance_matrix(t1, t3);
        auto d24 = distance_matrix(t2, t4);

        auto p12 = penalty(t1, t2);
        auto p34 = penalty(t3, t4);
        auto p13 = penalty(t1, t3);
        auto p24 = penalty(t2, t4);

        auto gain = (d12 + lambda * p12) + (d34 + lambda * p34) - (d13 + lambda * p13) - (d24 + lambda * p24);

        if(max_gain < gain)
        {
            max_gain = gain;
            t4_candidate.clear();
            t4_candidate.push_back(t4);
        }
        else if(max_gain == gain)
        {
            t4_candidate.push_back(t4);
        }
    }

    if(max_gain > 1e-6)
    {
        auto t4 = random_sample(t4_candidate);
        auto t3 = connection[t4].in;

        return make_tuple(t3, t4);
    }

    return make_tuple(-1, -1);
}



void swap_edge(int t1, int t2, int t3, int t4, vector<Connection> & connection, DistanceMatrix distance_matrix, 
                const Penalty & penalty, double & distance, double & augmented_distance, double lambda)
{
    auto cur_node = t2;
    auto cur_node_out = connection[cur_node].out;

    while(cur_node != t3)
    {
        auto next_cur_node = cur_node_out;
        auto next_cur_node_out = connection[next_cur_node].out;


        connection[cur_node].in = cur_node_out;
        connection[cur_node_out].out = cur_node;

        cur_node = next_cur_node;
        cur_node_out = next_cur_node_out;
    }

    connection[t2].out = t4;
    connection[t4].in = t2;

    connection[t1].out = t3;
    connection[t3].in = t1;

    auto d12 = distance_matrix(t1, t2);
    auto d34 = distance_matrix(t3, t4);
    auto d13 = distance_matrix(t1, t3);
    auto d24 = distance_matrix(t2, t4);

    auto p12 = penalty(t1, t2);
    auto p34 = penalty(t3, t4);
    auto p13 = penalty(t1, t3);
    auto p24 = penalty(t2, t4);

    auto gain = (d12 + lambda * p12) + (d34 + lambda * p34) - (d13 + lambda * p13) - (d24 + lambda * p24);

    distance -= d12 + d34 - d13 - d24;
    augmented_distance -= gain;
}

void add_penalty(const vector<Connection> & connection, DistanceMatrix distance_matrix, Penalty & penalty, Activate & activate, 
                    double & augmented_distance, double lambda)
{
    auto max_util = -(numeric_limits<double>::max)();
    vector<int> max_util_node;

    for(auto i = 0; i < connection.size(); ++i)
    {
        auto i_out = connection[i].out;
        auto d = distance_matrix(i, i_out);
        auto p = (1 + penalty(i, i_out));
        auto util = d / (1 + p);

        if(max_util < util)
        {
            max_util = util;
            max_util_node.clear();
            max_util_node.push_back(i);
        }
        else if(max_util == util)
        {
            max_util_node.push_back(i);
        }
    }

    for(auto i : max_util_node)
    {
        auto i_out = connection[i].out;
        ++penalty(i, i_out);

        activate.set_1(i);
        activate.set_1(i_out);

        augmented_distance += lambda;
    }
}

double total_distance(const vector<Connection> & connection, DistanceMatrix distance_matrix)
{
    auto dis = 0.0;
    for(auto i = 0; i < connection.size(); ++i)
    {
        dis += distance_matrix(i, connection[i].out);
    }

    return dis;
}

double total_augmented_distance(const vector<Connection> & connection, DistanceMatrix distance_matrix, const Penalty & penalty, double lambda)
{
    auto augmented_dis = 0.0;
    for(auto i = 0; i < connection.size(); ++i)
    {
        auto i_out = connection[i].out;
        auto d = distance_matrix(i, i_out);
        auto p = penalty(i, i_out);
        augmented_dis += d + p * lambda;
    }

    return augmented_dis;
}


void save_result(const char * filename, double distance, const vector<Connection> & connection)
{
    auto f = fopen(filename, "w");

    fprintf(f, "%lf\n", distance);
    print_tour(connection, f);

    fclose(f);
}

vector<Node> load_node(const char * filename)
{
    auto f = fopen(filename, "r");

    int node_count;
    fscanf(f, "%d", &node_count);

    vector<Node> node_vec;
    for(auto i = 0; i < node_count; ++i)
    {
        Node p;
        fscanf(f, "%lf %lf", &p.x, &p.y);
        node_vec.push_back(p);
    }
    fclose(f);

    return node_vec;
}

double init_lambda(const vector<Connection> & connection, DistanceMatrix distance_matrix, double alpha)
{
    return alpha * total_distance(connection, distance_matrix) / connection.size();
}

vector<Connection> search(const vector<Connection> & connection, DistanceMatrix distance_matrix)
{
    auto step_limit = 1000000;

    auto penalty = Penalty(connection.size());
    auto alpha = 0.5;
    auto lambda = init_lambda(connection, distance_matrix, alpha);

    auto activate = Activate(connection.size());

    auto current_connection = connection;
    auto current_distance = total_distance(current_connection, distance_matrix);
    auto current_augmented_distance = total_augmented_distance(current_connection, distance_matrix, penalty, lambda);

    auto best_connection = current_connection;
    auto best_distance = current_distance;

    for (auto cur_step = 0; cur_step < step_limit; ++cur_step)
    {
        printf("[step %-8d] [current distance %lf] [current augmented distance %lf] [best distance %lf]\n", 
                cur_step + 1, current_distance, current_augmented_distance, best_distance);
        
        while(activate.ones > 0)
        {
            for(auto bit = 0; bit < activate.size(); ++bit)
            {
                if(!activate.get(bit)) continue;

                auto bit_in = current_connection[bit].in;
                auto bit_out = current_connection[bit].out;

                auto t1_t2_candidate = vector<tuple<int, int>>{ make_tuple(bit_in, bit), make_pair(bit, bit_out) };

                for(auto j = 0; j < t1_t2_candidate.size(); ++j)
                {
                    auto [t1, t2] = t1_t2_candidate[j];

                    auto [t3, t4] = select_t3_t4(t1, t2, current_connection, distance_matrix, penalty, lambda);

                    if(t3 == -1)
                    {
                        if(j == 1)
                        {
                            activate.set_0(bit);
                        }
                        continue;
                    }
                    

                    swap_edge(t1, t2, t3, t4, current_connection, distance_matrix, penalty, current_distance, current_augmented_distance, lambda);
                    
                    activate.set_1(t1);
                    activate.set_1(t2);
                    activate.set_1(t3);
                    activate.set_1(t4);

                    break;
                }

                if (best_distance > current_distance)
                {
                    best_connection = current_connection;
                    best_distance = current_distance;

                    save_result("cpp_output.txt", best_distance, best_connection);
                }
            }
        }
        add_penalty(current_connection, distance_matrix, penalty, activate, current_augmented_distance, lambda);
    }

    save_result("cpp_output.txt", best_distance, best_connection);
    return best_connection;
}




int main()
{
    auto node_vec = load_node("cpp_input.txt");
    // auto node_vec = load_node("data/tsp_51_1");
    // auto node_vec = load_node("data/tsp_100_3");
    // auto node_vec = load_node("data/tsp_200_2");
    // auto node_vec = load_node("data/tsp_574_1");
    // auto node_vec = load_node("data/tsp_1889_1");
    // auto node_vec = load_node("data/tsp_33810_1");

    auto distance_matrix = get_distance_matrix(node_vec);
    auto connection = init_connection(node_vec.size(), distance_matrix);

    auto best_connection = search(connection, distance_matrix);
    auto best_distance = total_distance(best_connection, distance_matrix);

    printf("best distance : %lf\n", best_distance);
    printf("best tour:\n");
    print_tour(best_connection);

    return 0;
}