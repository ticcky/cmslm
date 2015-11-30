/*
 * Vocab.h
 *
 *  Created on: Nov 23, 2015
 *      Author: zilka
 */

#ifndef VOCAB_H_
#define VOCAB_H_

#include <unordered_map>

using namespace std;

class Vocab {
public:
	Vocab();
	virtual ~Vocab();
	int Add(string word);
	int Get(string word);
	string Rev(int word_id);
private:
	unordered_map<string, int> vocab;
	unordered_map<int, string> vocab_rev;
};

#endif /* VOCAB_H_ */
