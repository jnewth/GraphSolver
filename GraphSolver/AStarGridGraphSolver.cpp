/*
 * AStarGridGraphSolver.cpp
 *
 *  Created on: Apr 22, 2012
 *      Author: Joshua Newth (mrfurious@gmail.com)
 */

#include "AStarGridGraphSolver.h"
#include <math.h>
#include <assert.h>
#include <gl.h>

AStarGridGraphSolver::AStarGridGraphSolver(const GridGraphSolverCInfoT& info) {
	init(info);
	PathNodeT *p = new PathNodeT(NULL, m_start, 0, computeManhattanEstimate(m_start)); //generally H is cost of parent node + 1
	m_openList.push_back(p);
	incrementStepCount();
}

AStarGridGraphSolver::~AStarGridGraphSolver() {
	//clean up anything in either open or closed lists
	std::list<PathNodeT *>::iterator iter;
	for (iter = m_openList.begin(); iter != m_openList.end(); iter++) {
		delete *iter;
	}
	m_openList.clear();

	for (iter = m_closedList.begin(); iter != m_closedList.end(); iter++) {
		delete *iter;
	}
	m_closedList.clear();
}

int AStarGridGraphSolver::computeManhattanEstimate(GridNode *node) {
	assert(node != NULL);
	//H = heuristic = we use Manhattan (technically inadmissible)
	int H = abs(node->getRow() - m_finish->getRow()) + abs(node->getCol() - m_finish->getCol());
	return H;
}

//just iterate through the list finding the best (lowest score node)
AStarGridGraphSolver::PathNodeT* AStarGridGraphSolver::popBestNode() {

	if (m_openList.size() == 0 ) { return NULL; }

	std::list<PathNodeT*>::iterator iter;
	std::list<PathNodeT*>::iterator best;
	int bestScore = INT32_MAX;
	for (iter = m_openList.begin(); iter != m_openList.end(); iter++) {
		if ((*iter)->getScore() < bestScore) {
			bestScore = (*iter)->getScore();
			best = iter;
		}
	}
	PathNodeT *bestPathNode = *best;
	m_openList.erase(best); //we are popping the best from our open list
	return bestPathNode;
}

GridGraphSolver::SolveStateT AStarGridGraphSolver::step() {
	if (m_state == STEPPING) {
		if (m_openList.size() == 0) {
			m_state = UNSOLVED;
			return m_state;
		}
		PathNodeT *p = popBestNode();
		incrementStepCount(); //every time we pop
		m_closedList.push_back(p); //always pop best and add to closed list
		if (p->m_node == m_finish) { //if it's the final, we are done
			m_state = SOLVED;
			computePath(p);
		}
		else {
			for (int dir = 0; dir < GridNode::NUM_DIRS; dir++) { //add children to node
				if (p->m_node->containsEdge(dir)) {
					GridNode *childNode = p->m_node->getNeighbor(dir);
					addToOpenList(p, childNode);
				}
			}
		}
	}
	return m_state;
}


void AStarGridGraphSolver::addToOpenList(PathNodeT *parent, GridNode *node) {
	//if child is on closed list, ignore
	//if child is already in open list, update its score, possibly updating parent
	if (node) {
		if (!isOnClosedList(node)) { //we ignore if they are on the closed list already
			PathNodeT *nodeOnList = findOnList(m_openList, node);
			if (nodeOnList) { //they are already on the open list, so update their score if necessary
				if (parent->m_G + 1 < nodeOnList->m_G) {
					nodeOnList->m_parent = parent; //better parent
					nodeOnList->m_G = parent->m_G + 1; //revised better score
				}
			}
			else {
				PathNodeT *child = new PathNodeT(parent, node, parent->m_G+1, computeManhattanEstimate(node)); //set parent
				m_openList.push_back(child);
			}
		}
	}
}

bool AStarGridGraphSolver::isOnClosedList(GridNode *node) {
	PathNodeT *p = findOnList(m_closedList, node);
	return (p != NULL);
}


AStarGridGraphSolver::PathNodeT* AStarGridGraphSolver::findOnList(const std::list<PathNodeT *>& myList, GridNode *node) {
	std::list<PathNodeT *>::const_iterator iter;
	for (iter = myList.begin(); iter != myList.end(); ++iter) {
		if ((*iter)->m_node == node) {
			return *iter;
		}
	}
	return NULL;
}

void AStarGridGraphSolver::computePath(PathNodeT *path) {
	assert(m_state == SOLVED);
	//We can draw
	while (path != NULL) {
		m_path.push_back(path);
		path = path->m_parent;
	}
}

GridGraphSolver::SolveStateT AStarGridGraphSolver::solve() {
	while(STEPPING == step());
	return m_state;
}

void AStarGridGraphSolver::render() {
	//open list iterator

	//draw open list as gray circle-with-tale
	renderOpenAndClosedLists();

	//draw path
	if (m_state == SOLVED)
	{
		glColor3f(0, 1, 0); //pure green
		std::vector<PathNodeT *>::iterator iter;
		PathNodeT* from = *m_path.begin();
		for (iter = m_path.begin(); iter != m_path.end(); iter++) {
			drawPathSegment(from->m_node, (*iter)->m_node);
			from = (*iter);
		}
	}
	renderStart();
	renderFinish();
}

void AStarGridGraphSolver::drawPathSegment(GridNode *from, GridNode *to) {
	glBegin(GL_LINES);
	//offset to start of maze, then index over by rows, then add offset to center of cell
	glVertex2f(m_x+ from->getCol()*m_cellWidth + m_offsetX, m_y+ from->getRow()*m_cellHeight + m_offsetY);
	glVertex2f(m_x+ to->getCol()*m_cellWidth + m_offsetX, m_y+ to->getRow()*m_cellHeight + m_offsetY);
	glEnd();
}

void AStarGridGraphSolver::renderOpenAndClosedLists() {
	std::list<PathNodeT *>::iterator iter;


	glColor3f(0.5f,0.5f,0.5f);
	for (iter = m_closedList.begin(); iter != m_closedList.end(); iter++) {
		drawListNode(*iter);
	}

	glColor3f(0.25f,0.75f,0.75f);
	for (iter = m_openList.begin(); iter != m_openList.end(); iter++) {
		drawListNode(*iter);
		//drawTail(*iter);
	}
}

void AStarGridGraphSolver::drawListNode(PathNodeT *p) {
	drawCircle(p->m_node->getRow(),p->m_node->getCol(), m_cellWidth/6);

	//draw tail

//	int deltaX = p->m_parent->m_node->getCol() - p->m_node->getCol();
//	int deltaY = p->m_parent->m_node->getRow() - p->m_node->getRow();
//
//	//center of circle, then offset to edge of circle
//	int x = (m_x+ p->m_node->getCol()*m_cellWidth + m_offsetX) + deltaX*(m_cellWidth/6);
//	int y = (m_y+ p->m_node->getRow()*m_cellHeight + m_offsetY) + deltaY*(m_cellWidth/6);
//
//	glBegin(GL_LINES);
//	glVertex2f(x , y ); //starts line on edge of circle
//	x += deltaX*(m_cellWidth/6);
//	y += deltaY*(m_cellWidth/6);
//	glVertex2f(x , y ); //starts line on edge of circle
//	glEnd();
	//}
}
