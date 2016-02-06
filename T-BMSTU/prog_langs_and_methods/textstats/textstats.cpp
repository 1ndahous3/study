#include "textstats.hpp"
#include <algorithm>

using namespace std;

void get_tokens(const string &s, const unordered_set<char> &delimiters,
		vector<string> &tokens) {
	char *chr = (char*) s.c_str();
	for (int i = 0, j = 0, k = 0; i < s.length(); i++, chr++) {
		if (delimiters.find(*chr) == delimiters.end() && *chr != ' '
				&& *chr != '\0') {
			j = i;
			while (delimiters.find(*chr) == delimiters.end() && *chr != ' '
					&& *chr != '\0') {
				i++;
				chr++;
			}
			k = i - 1;
			string elem = s.substr(j, k - j + 1);
			transform(elem.begin(), elem.end(), elem.begin(), ::tolower);
			tokens.push_back(elem);
		}
	}
}

void get_type_freq(const vector<string> &tokens, map<string, int> &freqdi) {
	for (string i : tokens)
		freqdi[i]++;
}

void get_types(const vector<string> &tokens, vector<string> &wtypes) {
	map<string, int> freqdi;
	get_type_freq(tokens, freqdi);
	for (pair<string, int> i : freqdi)
		wtypes.push_back(i.first);
	sort(wtypes.begin(), wtypes.end());
}

void get_x_length_words(const vector<string> &wtypes, int x,
		vector<string> &words) {
	for (string i : wtypes)
		if (i.length() >= x)
			words.push_back(i);
}

void get_x_freq_words(const map<string, int> &freqdi, int x,
		vector<string> &words) {
	for (pair<string, int> i : freqdi)
		if (i.second >= x)
			words.push_back(i.first);
}

void get_words_by_length_dict(const vector<string> &wtypes,
		map<int, vector<string> > &lengthdi) {
	for (string i : wtypes)
		lengthdi[i.length()].push_back(i);
}
