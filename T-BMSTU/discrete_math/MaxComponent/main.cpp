#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;


class MaxComponent {
private:
	map<int, vector<int> *> *bridges = new map<int, vector<int> *>;
	vector<pair<int, set<int> *> *> *comps;

	void splitComponents() {
		comps = new vector<pair<int, set<int> *> *>();
		priority_queue<int> queue = priority_queue<int>();
		int br_counter = 0;
		set<int> *_set = new set<int>();
		_set->insert(0);
		bool visited[bridges->size()];
		for (bool &a : visited) {
			a = false;
		}

		for (int i = 0; i < bridges->size();) {
			if (!queue.empty()) {
				int foo = queue.top();
				queue.pop();
				_set->insert(foo);
				br_counter += checkVrtx(queue, visited, foo);
				visited[foo] = true;
				continue;
			}
			comps->push_back(new pair<int, set<int> *>(br_counter, _set));

			_set = new set<int>();
			_set->insert(i);
			br_counter = checkVrtx(queue, visited, i);
			visited[i++] = true;
		}
	}

	int checkVrtx(priority_queue<int> &queue, bool *visited, int j) {
		int bridges_count = 0;
		for (auto i: *bridges->at(j)) {
			if (!visited[i]) {
				queue.push(i);
				bridges_count++;
			}
		}

		return bridges_count;
	}

public:
	MaxComponent(map<int, vector<int> *> *_bridges) {
		bridges = _bridges;
		splitComponents();
	}

	~MaxComponent() {
		for (auto &_pair : *comps) {
			delete _pair->second;
			delete _pair;
		}
		for (auto &_pair : *bridges) {
			delete _pair.second;
		}
		delete comps;
		delete bridges;
	}

	string get_string() {

		stringstream ss;

		ss << "graph {\n";

		sort(comps->begin(), comps->end(), [](pair<int, set<int> *> *a, pair<int, set<int> *> *b) -> int {
			a->second->size() != b->second->size() ?
			b->second->size() - a->second->size() : a->first != b->first ?
												  b->first - a->first : *a->second->begin() - *b->second->begin();
		});

		pair<int, set<int>*> *red = *comps->begin();

		for (auto &_pair : *bridges) {
			int num = _pair.first;
			ss << "\t" << num;
			if (red->second->find(num) != red->second->end()) {
				ss << " [color = red]";
			}
			ss << "\n";
		}

		for (auto &_pair : *bridges) {
			int num1 = _pair.first;
			for (auto it = _pair.second->begin();  it != _pair.second->end(); ++it) {
				auto num2 = *it;
				if (num2 <= num1) {
					ss << "\t" << num2 << " -- " << num1;
					if (red->second->find(num1) != red->second->end()) {
						ss << " [color = red]";
					}
					ss << "\n";
				}
			}
		}

		ss << "}";

		return ss.str();
	}
};


int main() {
	int n_of_vrtxs, n_of_bridges;
	cin >> n_of_vrtxs >> n_of_bridges;
	map<int, vector<int> *> *bridges = new map<int, vector<int> *>();

	for (int i = 0; i < n_of_vrtxs; i++) {
		(*bridges)[i] = new vector<int>();
	}

	for (int i = 0; i < n_of_bridges; i++) {
		int foo, bar;
		cin >> foo >> bar;
		(*bridges)[foo]->push_back(bar);
		if (foo != bar) {
			(*bridges)[bar]->push_back(foo);
		}
	}

	MaxComponent comp(bridges);

	cout << comp.get_string() << endl;
	return 0;
}