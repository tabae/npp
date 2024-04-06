#include <iostream>
#include <vector>
#include <queue>
#include <numeric>
#include <algorithm>
#include <cassert>

template<typename T>
struct NPP {
    
    int m; // number_of_partitions;
    int n; // number_of_items;
    std::vector<T> w; // weight of items

    NPP(int m, int n) : m(m), n(n) { assert(m >= 1); assert(n >= 1); w.resize(n, 0); };
    
    NPP(int m, const std::vector<T>& w) : m(m), w(w), n(w.size()) { assert(m >= 1); assert(w.size() >= 1); };
    
    void set_weight(int i, T v) {
        w.at(i) = v;
    }
    
    /* 
        優先度付きキューを使って、貪欲に解く。
        重みが大きいアイテムから順番に見ていき、その時点で最も重みの総和が少ないブロックに割り当てる。
    */
    std::vector<std::vector<int>> solve_greedy() {
        std::vector<std::vector<int>> res(m);
        std::priority_queue<std::pair<T,int>, std::vector<std::pair<T,int>>, std::greater<std::pair<T,int>>> que;
        for(int i = 0; i < m; i++) que.push({0, i});
        std::vector<int> index(n);
        std::iota(index.begin(), index.end(), 0);
        std::sort(index.begin(), index.end(), [&](const int& l, const int r) { return w.at(l) > w.at(r); });
        for(int i = 0; i < n; i++) {
            auto [sum, id] = que.top();
            que.pop();
            sum += w.at(i);
            res.at(id).push_back(i);
            que.push({sum, id});
        }
        return res;
    }

    /*
        差分法を用いて解く。
        参考：https://scmopt.github.io/opt100/76npp.html
    */
    std::vector<std::vector<int>> solve_differencing() {
        std::vector<std::vector<int>> res(m);
        using S = std::tuple<T, std::vector<std::vector<int>>, std::vector<T>>; // (label, partition, sum)
        std::priority_queue<S> que;
        for(int i = 0; i < n; i++) {
            std::vector<std::vector<int>> part(m);
            std::vector<T> sum(m, 0);
            part.at(m-1).push_back(i);
            sum.at(m-1) = w.at(i);
            T label = sum.at(m-1) - sum.at(0);
            que.push({label, part, sum});
        }
        while(que.size() >= 2) {
            auto [label1, part1, sum1] = que.top(); que.pop();
            auto [label2, part2, sum2] = que.top(); que.pop();
            std::vector<std::pair<T,std::vector<int>>> tmp; 
            for(int p = 0; p < m; p++) {
                T sum;
                std::vector<int> part;
                for(const int i : part1.at(p)) part.push_back(i);
                for(const int i : part2.at(m-1-p)) part.push_back(i);
                sum = sum1.at(p) + sum2.at(m-1-p);
                tmp.push_back({sum, part});
            }
            std::sort(tmp.begin(), tmp.end());
            std::vector<T> sum(m);
            std::vector<std::vector<int>> part(m);
            for(int i = 0; i < m; i++) {
                sum.at(i) = tmp.at(i).first;
                part.at(i) = tmp.at(i).second;
            }
            T label = sum.at(m-1) - sum.at(0);
            que.push({label, part, sum});
        }
        auto [_label, part, _sum] = que.top(); que.pop();
        assert(que.empty());
        return part;
    }
};


int main() {

    std::vector<long long> items = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    NPP<long long> nnp(3, items);

    auto part_g = nnp.solve_greedy();
    std::cout << "solve_greedy" << std::endl;
    for(auto v : part_g) {
        long long sum = 0;
        for(int i = 0; i < v.size(); i++) {
            std::cout << items[v[i]] << (i == v.size()-1 ? " = " : " + ");
            sum += items[v[i]];
        }
        std::cout << sum << std::endl;
    }
    
    auto part_d = nnp.solve_differencing();
    std::cout << "solve_differencing" << std::endl;
    for(auto v : part_d) {
        long long sum = 0;
        for(int i = 0; i < v.size(); i++) {
            std::cout << items[v[i]] << (i == v.size()-1 ? " = " : " + ");
            sum += items[v[i]];
        }
        std::cout << sum << std::endl;
    }
}
