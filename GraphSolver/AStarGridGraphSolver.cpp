/*
 * AStarGridGraphSolver.cpp
 *
 *  Created on: Apr 22, 2012
 *      Author: Joshua Newth (mrfurious@gmail.com)
 */

#include "AStarGridGraphSolver.h"
#include <math.h>
#include <assert.h>

AStarGridGraphSolver::AStarGridGraphSolver(const GridGraphSolverCInfoT& info) {
	init(info);
	PathNodeT *p = new PathNodeT(NULL, m_start, computeScore(m_start, 0)); //generally H is cost of parent node + 1
	m_openList.push(p);
	incrementStepCount();
}

AStarGridGraphSolver::~AStarGridGraphSolver() {
	//clean up anything in either open or closed lists
}


int AStarGridGraphSolver::computeScore(GridNode *node, int costOfParent) {
	assert(node != NULL);
	//score = G + H
	//G = cost so far + step cost = cost to get to the preceding node + 1
	int G = costOfParent+1;
	//H = heuristic = we use Hamilton (technically inadmissible)
	int H = abs(node->getRow() - m_finish->getRow()) + abs(node->getCol() - m_finish->getCol());
	return (G+H);
}

GridGraphSolver::SolveStateT AStarGridGraphSolver::step() {
	if (m_state == STEPPING) {
		if (m_openList.size() == 0) {
			m_state = UNSOLVED;
		}
		PathNodeT *p = m_openList.top(); //get the best score in the list
		m_openList.pop(); //removes top element from queue
		if (p->m_node == m_finish) {
			m_state = SOLVED;
			m_closedList.push_back(p);
			computePath(p);
		}
		else {
			for (int dir = 0; dir < GridNode::NUM_DIRS; dir++) { //add children to node
				GridNode *childNode = p->m_node->getNeighbor(dir);
				addToOpenList(p, childNode);
			}
		}
	}
	return m_state;
}


void AStarGridGraphSolver::addToOpenList(PathNodeT *parent, GridNode *node) {
	//if child is on closed list, ignore
	//if child is already in open list, update its score, possibly updating parent
	if (node) {
		if (!isOnClosedList(node)) {
			PathNodeT *child = new PathNodeT(p, node, computeScore(child, parent->m_score)); //set parent
			updateScoreAndAdd(child);
		}
	}
}

bool AStarGridGraphSolver::isOnClosedList(GridNode *node) {
	//iterate through closed list
	std::vector<PathNodeT *>::iterator iter;
	for (iter = m_closedList.begin(); iter != m_closedList.end(); iter++) {
		if (iter->m_node == node) { return true; }
	}
	return false;
}

void AStarGridGraphSolver::computePath(PathNodeT *path) {
	assert(m_state == SOLVED);
	//We can draw
	while (path->m_node != NULL) {
		m_path.push_back(path->m_node);
		path = path->m_parent;
	}
}

GridGraphSolver::SolveStateT AStarGridGraphSolver::solve() {
	while(step() == STEPPING);
	return m_state;
}

void AStarGridGraphSolver::render() {
	//open list iterator

	//draw open list as gray circle-with-tale
	renderOpenList();

	//draw path
	{
		glColor3f(0, 1, 0); //pure green
		std::vector<GridNode *>::iterator iter;
		GridNode* from = m_start;
		for (iter = m_path.begin(); iter != m_path.end(); iter++) {
			drawPathSegment(from, (*iter));
			from = (*iter);
		}

		renderStart();
		renderFinish();
	}
}

void AStarGridGraphSolver::drawPathSegment(GridNode *from, GridNode *to) {
	glBegin(GL_LINES);
	//offset to start of maze, then index over by rows, then add offset to center of cell
	glVertex2f(m_x+ from->getCol()*m_cellWidth + m_offsetX, m_y+ from->getRow()*m_cellHeight + m_offsetY);
	glVertex2f(m_x+ to->getCol()*m_cellWidth + m_offsetX, m_y+ to->getRow()*m_cellHeight + m_offsetY);
	glEnd();
}

void AStarGridGraphSolver::renderOpenList() {
	//this is actually an annoying problem
	//the open list is an STL queue, which doesnt support iterating.
	//have to do something either expensive (like switching to vector)
	//or computational (like writing my own)
	//or terrible like popping everything, drawing it, and readding it later
	//do the terrible thing for now
	std::vector<PathNodeT *> tempStorage;
	//draw, then pop
	while (m_openList.size()) {
		PathNodeT *p = m_openList.top();
		m_openList.pop();
		tempStorage.push_back(p);
		drawOpenNode(p);
	}
	//then push back in to openlist
	std::vector<PathNodeT *>::iterator iter;
	for (iter= tempStorage.begin(); iter != tempStorage.end(); iter++) {
		m_openList.push(*iter); //push contents back in to open list
	}

}

void AStarGridGraphSolver::drawOpenNode(PathNodeT *p) {
	glColor3f(0.5f,0.5f,0.5f);
	drawCircle(p->m_node->getRow(),p->m_node->getCol(), m_cellWidth/6);
	//draw tail
	int deltaX = p->m_parent->getCol() - p->m_node->getCol();
	int deltaY = p->m_parent->getRow() - p->m_node->getRow();

	//center of circle, then offset to edge of circle
	int x = (m_x+ p->m_node->getCol()*m_cellWidth + m_offsetX) + deltaX*(m_cellWidth/6);
	int y = (m_y+ p->m_node->getRow()*m_cellHeight + m_offsetY) + deltaY*(m_cellWidth/6);

	glBegin(GL_LINES);
	glVertex2f(x , y ); //starts line on edge of circle
	x += deltaX*(m_cellWidth/6);
	y += deltaY*(m_cellWidth/6);
	glVertex2f(x , y ); //starts line on edge of circle
	glEnd();
}
