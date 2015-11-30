/*
 * Vocab.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: zilka
 */

#include "Vocab.h"

Vocab::Vocab() {
	// TODO Auto-generated constructor stub

}

Vocab::~Vocab() {
	// TODO Auto-generated destructor stub
}

int Vocab::Get(string word) {
	auto item = vocab.find(word);
	if(item != vocab.end()) {
		return item->second;
	} else {
		return -1;
	}
}

int Vocab::Add(string word) {
	auto item = vocab.find(word);
	if(item == vocab.end()) {
		vocab[word] = vocab.size();
		vocab_rev[vocab[word]] = word;
	}

	return vocab[word];
}

string Vocab::Rev(int word_id) {
	return vocab_rev[word_id];
}
