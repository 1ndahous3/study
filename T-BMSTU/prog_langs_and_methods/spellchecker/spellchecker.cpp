#include <iostream>
#include <algorithm>
#include <fstream>
#include <list>
#include <string>
#include <map>

using namespace std;

string correct(string &curr);
list<string> make_bigram(string word);

map<string, int> words;
map<string, list<string> > bigrams;

int main() {
	ifstream f("count_big.txt");
	while (!f.eof()) {
		pair<string, int> tmp;
		f >> tmp.first >> tmp.second;
		words[tmp.first] = tmp.second;
	}
	for (auto &i : words)
		bigrams[i.first] = make_bigram(i.first);
	do {
		string curr;
		cin >> curr;
		cout << correct(curr) << endl;
	} while (cin);
}

string correct(string &curr) {
	list<pair<float, string> > diff;
	list<string> curr_bigrams = make_bigram(curr);
	pair<float, string> elem = pair<float, string>(0, "");
	float diff_val = 0;
	for (auto &i : bigrams) {
		list<string> tmp_inter, tmp_union;
		set_intersection(i.second.begin(), i.second.end(), curr_bigrams.begin(),
				curr_bigrams.end(), inserter(tmp_inter, tmp_inter.begin()));
		set_union(i.second.begin(), i.second.end(), curr_bigrams.begin(),
				curr_bigrams.end(), inserter(tmp_union, tmp_union.begin()));
		float tmp_val = (float) tmp_inter.size() / (float) tmp_union.size();
		if (diff_val < tmp_val) {
			diff_val = tmp_val;
			elem = pair<float, string>(diff_val, i.first);
		} else if (diff_val == tmp_val)
			if (words[i.first] > words[elem.second])
				elem = pair<float, string>(diff_val, i.first);
	}
	return elem.second;
}

list<string> make_bigram(string word) {
	int length = word.length();
	list<string> res;
	if (length > 1)
		for (int i = 0; i < length - 1; i++)
			res.push_back(word.substr(i, 2));
	else
		res.push_back(word.substr(0, 1));
	res.sort();
	return res;
}
