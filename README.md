# 数分割問題

N個の数を、M個のグループに分割し、それぞれのグループの数の総和がなるべく均しくなるようにしたい。

# 使い方

NNPクラスのインスタンスを作成する。

```cpp
// N個の数を定義する。ここではN=4。
std::vector<long long> w = {1, 2, 3, 4};

// 第一引数にグループの数M、第二引数にN個の数のリストを渡す。ここではM=2。
NNP<long long> nnp(2, w);
```

ソルバーを実行する。

戻り値は、長さMの`std::vector<int>`のリストで返される。それぞれのリストには、i番目のグループに所属する数の**インデックス**が含まれる。

```cpp
// 差分法で解く場合
std::vector<std::vector<int>> res = nnp.solve_differencing();

// 貪欲法で解く場合
std::vector<std::vector<int>> res = nnp.solve_greedy();
```

# 計算量

`solve_greedy`の場合、$O(max(NlogN, NlogM, MlogM))$。

`solve_differencing`の場合、$O(N M logN logM)$



