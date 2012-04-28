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
	typedef enum MazeTypeT {SIMPLE3, MAZE10, SNIGDHA10, ASTARTEST};
	static const int NUM_MAZES = 4;
	static GridGraph* parseFile(const char *fname);
	static GridGraph* getMaze(int maze);

private:
	static GridGraph* snigdha10();
	static GridGraph* simple3();
	static GridGraph* testAStar();
    static GridGraph* maze10();
    
};

#endif /* GRIDFILEREADER_H_ */
