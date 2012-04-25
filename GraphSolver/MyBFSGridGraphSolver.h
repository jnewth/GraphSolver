/*
 * MyBFSGridGraphSolver.h
 *
 *  Created on: Apr 25, 2012
 *      Author: Joshua Newth (mrfurious@gmail.com)
 */

#ifndef MYBFSGRIDGRAPHSOLVER_H_
#define MYBFSGRIDGRAPHSOLVER_H_

#include "GridGraphSolver.h"
#include <list>
class MyBFSGridGraphSolver: public GridGraphSolver {
public:
	MyBFSGridGraphSolver(const GridGraphSolverCInfoT& info);
	virtual ~MyBFSGridGraphSolver();

	virtual SolveStateT step();
	virtual SolveStateT solve();
	virtual void render();

protected:
	typedef enum {LIST_A, LIST_B};
	int currentList;
	//My path node type
	typedef struct PathNodeT {
		PathNodeT *m_parent; //starts at 0, when = NUM_DIRS, the node can be popped
		GridNode *m_node;
		PathNodeT(PathNodeT *parent, GridNode *n) : m_parent(parent), m_node(n) {}
	} PathNodeT;

	virtual void computePath(PathNodeT* end);

	std::list<PathNodeT *> m_openList; //we fill one while we pop the other
	std::list<PathNodeT *> m_closedList; //once they are popped we push them here

	//just for graphing solution
	std::vector<PathNodeT *> m_path;
	void drawPathSegment(GridNode *from, GridNode *to);
	void renderOpenAndClosedLists();
	void drawListNode(PathNodeT *p);
	void drawListNodeTail(PathNodeT *p);


};

#endif /* MYBFSGRIDGRAPHSOLVER_H_ */
