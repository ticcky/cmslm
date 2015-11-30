//============================================================================
// Name        : cms_lm.cpp
// Author      : Lukas Zilka
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip>
#include <queue>
#include <algorithm>
#include <vector>
#include <tuple>
#include <sstream>
#include <fstream>
#include <deque>
#include <unordered_set>
#include <string>
#include <cmath>
#include "CountMinSketch.h"
#include "Vocab.h"

using namespace std;


class vector_hash
{
public:
    std::size_t operator()(const vector<int> &v) const
    {
        std::size_t x = 0;

        for (auto &i : v)
            x ^= std::hash<int>()(i);

        return x;
    }
};


struct greater1{
  bool operator()(const long& a,const long& b) const{
    return a>b;
  }
};


int main(int argc, const char** argv) {
	int ngram_order = 5;
	int ngram_ptr = 0;

	Vocab vocab;
	int unk_token_id = vocab.Add("<unk>");

	deque<int> ngram_queue;

	vector<CountMinSketch> cmsx(ngram_order, CountMinSketch(5, 100000000));
	vector<vector<vector<int>>> heaps(ngram_order);
	vector<unordered_set<vector<int>, vector_hash>> heap_contents(ngram_order);

	vector<int> keep_cnt = {65000, 700000, 300000, 60000, 10000};



	if(argc != 2) {
		cout << "Wrong number of arguments" << endl;
		return -1;
	}
	ifstream ifs(argv[1]);
	ifs.seekg(0, ifs.end);
	int file_length = ifs.tellg();
	ifs.seekg(0, ifs.beg);

	int total_tokens = 0;
	int total_lines = 0;
	string token;
	string line;
    while(getline(ifs, line)) {
    	if(total_lines % 10000 == 0) {
			cerr << "                                         ";
			cerr << "\rDone: " << fixed << setprecision(2) << ifs.tellg() * 100.0 / file_length << "%";
    	}

    	total_lines += 1;

    	stringstream line_stream("<s> " + line + " </s>");

    	ngram_queue.clear();

    	while(line_stream) {
			if(line_stream >> token) {
				total_tokens += 1;
				//cout << "Token: " << token << endl;
				ngram_queue.push_back(vocab.Add(token));
				if(ngram_queue.size() > ngram_order)
					ngram_queue.pop_front();

				//cout << "   " << vocab.Add(token) << endl;

				vector<int> ngram;
				int q_size = ngram_queue.size();
				for(int i = q_size - 1; i >= 0; i--) {
					ngram.push_back(ngram_queue[i]);

					CountMinSketch& curr_cms = cmsx[ngram.size() - 1];
					vector<vector<int>>& curr_heap = heaps[ngram.size() - 1];
					unordered_set<vector<int>, vector_hash>& curr_heap_content = heap_contents[ngram.size() - 1];
					int curr_thresh = keep_cnt[ngram.size() - 1];

					curr_cms.Inc(ngram);

					bool updated = false;
					bool already_in_heap = curr_heap_content.count(ngram) > 0;
					if(curr_heap.size() < curr_thresh) {
						if(!already_in_heap) {
							curr_heap.push_back(ngram);
							curr_heap_content.emplace(ngram);
							updated = true;
						}
					} else {
						auto curr_min_elm = curr_heap.back();
						auto curr_min_val = curr_cms.Get(curr_min_elm);
						auto curr_val = curr_cms.Get(ngram);

						if(curr_val > curr_min_val && !already_in_heap) {
							curr_heap_content.erase(curr_min_elm);
							curr_heap.pop_back();
							curr_heap.push_back(ngram);
							curr_heap_content.emplace(ngram);

							updated = true;
						}
					}

					if(updated) {
						push_heap(heaps[ngram.size() - 1].begin(), heaps[ngram.size() - 1].end());
					}
				}
			}
    	}
    }
    cerr << "\r";

    /*
    cout << endl;

    cout << "\\data\\" << endl;
    for(int i = 0; i < ngram_order; i++) {
    	cout << "ngram " << i << "=" << keep_cnt[i] << endl;
    }
    cout << endl;
	*/
    // cout << "TOTAL TOKENS: " << total_tokens << endl;
    for(int i = 0; i < ngram_order; i++) {
    	//cout << "\\" << i + 1 << "-grams:" << endl;

    	//cout << "NGRAM ORDER: " << i << endl;

    	double unk_mass = 0.0;

		while(heaps[i].size() > 0) {
			bool contains_unk = false;
			vector<int> curr_ngram = heaps[i].back();

			for(auto x : curr_ngram) {
				if(x == unk_token_id) {
					contains_unk = true;
					break;
				}
			}
			//double prob = cmsx[i].Get(curr_ngram);
			//prob = log10(prob) - log10(total_tokens);

			int cnt = cmsx[i].Get(curr_ngram);

			if(!contains_unk) {
				//cout << "\t";
				//cout << cmsx[i].Get(heaps[i].back());
				//cout << " : ";
				auto print_ngram = heaps[i].back();
				for(int i = print_ngram.size() - 1; i >= 0; i--) {
					int x = print_ngram[i];
					cout << vocab.Rev(x);
					if(i > 0)
						cout << " ";
					else
						cout << "\t";
				}

				cout << cnt;

				cout << endl;
			} else {
				//unk_mass += prob;
			}

			heaps[i].pop_back();

		}

		/*if(i == 0)
			cout << unk_mass << "\t" << "<unk>" << endl;
		cout << endl;*/
    }

    //cout << "\\end\\" << endl;

    /*
    //cout << cmsx.Get({1, 2, 3, 4, 5});



	auto ngram = make_tuple(0, 1, 2, 3);
	cout << get<0>(ngram) << endl;
	cout << sizeof(ngram) << endl;

	CountMinSketch<tuple<int, int, int>> cms3(5, 1000);
	cms3.Inc(make_tuple(0, 1, 2));
	cms3.Inc(make_tuple(0, 1, 2));
	cms3.Inc(make_tuple(0, 1, 2));
	cms3.Inc(make_tuple(0, 2, 2));
	cms3.Inc(make_tuple(0, 1, 3));

	cout << "CMS3:" << endl;
	cout << cms3.Get(make_tuple(0, 1, 2)) << endl;
	cout << cms3.Get(make_tuple(0, 2, 2)) << endl;
	cout << cms3.Get(make_tuple(0, 1, 3)) << endl;
	cout << cms3.Get(make_tuple(0, 1, 4)) << endl;



	vector<int> heap;
	heap.push_back(1);
	heap.push_back(10);
	heap.push_back(3);
	heap.push_back(5);
	heap.push_back(7);

	make_heap(heap.begin(), heap.end(), greater1());

	for(int i = 0; i < 5; i++) {
		pop_heap(heap.begin(), heap.end(), greater1());

		int min = heap.back();
		heap.pop_back();

		cout << "min:" << min << endl;
	}


	CountMinSketch<int> cms(5, 1000);
	cms.Inc(10);
	cms.Inc(10);
	cms.Inc(10);
	cms.Inc(20);
	cms.Inc(30);
	cms.Inc(10);

	cout << "CMS:" << endl;
	cout << cms.Get(10) << endl;
	cout << cms.Get(30) << endl;


	cout << "Vocab" << endl;
	cout << vocab.Add("ahoj") << endl;
	cout << vocab.Add("cau") << endl;
	cout << vocab.Add("nazdar") << endl;
	cout <<vocab.Add("ahoj") << endl;

	*/

	return 0;
}

