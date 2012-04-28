/*
 * GridFileReader.h
 *
 *  Created on: Apr 8, 2012
 *      Author: Joshua Newth (mrfurious@gmail.com)
 */

#ifndef GRIDFILEREADER_H_
#define GRIDFILEREADER_H_

#include "GridGraph.h"

class GridFileReader {
public:
	typedef enum MazeTypeT {SIMPLE3, SNIGDHA10, ASTARTEST, MAZE10, MAZE2_10, MAZE3_10, MAZE4_10 };
	static const int NUM_MAZES = 8;
	static GridGraph* parseFile(const char *fname);
	static GridGraph* getMaze(int maze);

private:
	static GridGraph* snigdha10();
	static GridGraph* simple3();
	static GridGraph* testAStar();
    static GridGraph* maze10();
    static GridGraph* maze2_10();
    static GridGraph* maze3_10();
    static GridGraph* maze4_10();
};

#endif /* GRIDFILEREADER_H_ */
