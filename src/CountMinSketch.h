/*
 * CountMinSketch.h
 *
 *  Created on: Nov 23, 2015
 *      Author: zilka
 */

#ifndef COUNTMINSKETCH_H_
#define COUNTMINSKETCH_H_

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <climits>
#include <cstring>

#include "CityHash.h"



using namespace std;

class CountMinSketch {
public:
	CountMinSketch(int n_hash, int table_size) {
		table_ = new int[n_hash * table_size];
		memset(table_, 0, n_hash * table_size);

		n_hash_ = n_hash;
		table_size_ = table_size;
	}

	~CountMinSketch() {
		// TODO Auto-generated destructor stub
	}

	void Inc(vector<int>& data) {
		for(int i = 0; i < n_hash_; i++) {
			uint64 hashed_val = CityHash64WithSeed(reinterpret_cast<char*>(data.data()), data.size() * sizeof(int), i);
			//unsigned int hashed_val = hash_data(data.data(), data.size(), hash(i + 1));

			// cout << i * table_size_ + hashed_val % table_size_ << " ## " << n_hash_ * table_size_ << endl;
			table_[i * table_size_ + hashed_val % table_size_] += 1;
		}
	}

	int Get(vector<int>& data) {
		int min_val = INT_MAX;
		for(int i = 0; i < n_hash_; i++) {
			uint64 hashed_val = CityHash64WithSeed(reinterpret_cast<char*>(data.data()), data.size() * sizeof(int), i);
			//unsigned int hashed_val = hash_data(data.data(), data.size(), hash(i + 1));
			int new_val = table_[i * table_size_ + hashed_val % table_size_];

			if (new_val < min_val) {
				min_val = new_val;
			}
		}

		return min_val;
	}
private:
	int *table_;
	int n_hash_;
	int table_size_;

	unsigned int hash_data(const int * data, int size, int seed) {
		int res = seed;
		for(int i = 0; i < size; i++) {
			res ^= hash(data[i]);
		}

		return res;
	}

	unsigned int hash(unsigned int x) {
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x);
		return x;
	}
};

#endif /* COUNTMINSKETCH_H_ */
