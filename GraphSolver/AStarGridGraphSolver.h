/*
 * AStarGridGraphSolver.h
 *
 *  Created on: Apr 22, 2012
 *      Author: Joshua Newth (mrfurious@gmail.com)
 */

#ifndef ASTARGRIDGRAPHSOLVER_H_
#define ASTARGRIDGRAPHSOLVER_H_

#include "GridGraphSolver.h"
#include <list>

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
		PathNodeT *m_parent; //starts at 0, when = NUM_DIRS, the node can be popped
		GridNode *m_node;
		int m_H, m_G; //represents F=H+G, where H = heuristic, G = path cost
		PathNodeT(PathNodeT *parent, GridNode *n, int cost, int guess) : m_parent(parent), m_node(n), m_H(guess), m_G(cost) {}
		int getScore() { return m_G+m_H; }
	} PathNodeT;

	std::list<PathNodeT *> m_openList; //I know this could be a priority queue
	std::list<PathNodeT *> m_closedList;

	//just for graphing solution
	std::vector<PathNodeT *> m_path;


	int computeManhattanEstimate(GridNode *node);
	PathNodeT* popBestNode();

	void addToOpenList(PathNodeT *parent, GridNode *child);
	void computePath(PathNodeT *path);
	bool isOnClosedList(GridNode *node);
	PathNodeT* findOnList(const std::list<PathNodeT *>& list, GridNode *node);

	void drawPathSegment(GridNode *from, GridNode *to);
	void renderOpenAndClosedLists();
	void drawListNode(PathNodeT *p);
};

#endif /* ASTARGRIDGRAPHSOLVER_H_ */
