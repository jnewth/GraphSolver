/*
 * AStarGridGraphSolver.h
 *
 *  Created on: Apr 22, 2012
 *      Author: Joshua Newth (mrfurious@gmail.com)
 */

#ifndef ASTARGRIDGRAPHSOLVER_H_
#define ASTARGRIDGRAPHSOLVER_H_

#include "GridGraphSolver.h"
#include <queue>

class AStarGridGraphSolver: public GridGraphSolver {
public:
	AStarGridGraphSolver(const GridGraphSolverCInfoT& info);
	virtual ~AStarGridGraphSolver();
	virtual SolveStateT step();
	virtual SolveStateT solve();
	virtual void render();

protected:

	//My path node type
	typedef struct PathNodeT {
		GridNode *m_parent; //starts at 0, when = NUM_DIRS, the node can be popped
		GridNode *m_node;
		int m_score; //represents F=H+G
		PathNodeT(GridNode *parent, GridNode *n, int score) : m_parent(parent), m_node(n), m_score(score) {}
		const bool operator<(const PathNodeT& other) const { return (m_score > other.m_score);} //we are using a cost function
	} PathNodeT;

	std::priority_queue< PathNodeT *> m_openList;
	std::vector<PathNodeT *> m_closedList;

	//just for graphing solution
	std::vector<GridNode *> m_path;


	int computeScore(GridNode *node, int costOfParent);
	void addToOpenList(PathNodeT *parent, GridNode *child);
	void computePath(PathNodeT *path);
	bool isOnClosedList(GridNode *node);


	void drawPathSegment(GridNode *from, GridNode *to);
	void renderOpenList();
	void drawOpenNode(PathNodeT *p);

};

#endif /* ASTARGRIDGRAPHSOLVER_H_ */
