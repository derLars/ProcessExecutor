/*
 * ClassUnderTest.h
 *
 *  Created on: Jan 21, 2017
 *      Author: derlars
 */

#ifndef CLASSUNDERTEST_H_
#define CLASSUNDERTEST_H_

class ClassUnderTest {
public:
	~ClassUnderTest() {

	}
	ClassUnderTest() {

	}

	auto addNum(auto x, auto y) {
		return x + y;
	}

	auto subNum(auto x, auto y) {
		return x - y;
	}
};



#endif /* CLASSUNDERTEST_H_ */
