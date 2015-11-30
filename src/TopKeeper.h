/*
 * TopKeeper.h
 *
 *  Created on: Nov 23, 2015
 *      Author: zilka
 */

#ifndef TOPKEEPER_H_
#define TOPKEEPER_H_

#include "CountMinSketch.h"

template<typename T>
class TopKeeper {
public:
	TopKeeper();
	virtual ~TopKeeper();
private:
	CountMinSketch cms;

};

#endif /* TOPKEEPER_H_ */
