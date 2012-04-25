/*
 * MyBFSGridGraphSolver.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: Joshua Newth (mrfurious@gmail.com)
 */

#include "MyBFSGridGraphSolver.h"
#include <assert.h>
#include <gl.h>
MyBFSGridGraphSolver::MyBFSGridGraphSolver(const GridGraphSolverCInfoT& info) {
	init(info);
	PathNodeT *p = new PathNodeT(NULL, m_start); //generally H is cost of parent node + 1
	m_start->setVisited(true);
	m_openList.push_front(p);
	incrementStepCount();
}

GridGraphSolver::SolveStateT MyBFSGridGraphSolver::solve() {
	while (STEPPING == step())
		;
	return m_state;
}

GridGraphSolver::SolveStateT MyBFSGridGraphSolver::step() {

	if (m_state == STEPPING) {
		//pop all elements in current list
		//if they are the finish node, add them, youre done
		if (m_openList.size() == 0) {
			m_state = UNSOLVED;
			return m_state;
		}

		std::list<PathNodeT *> children;
		while (m_openList.size()) {
			PathNodeT* next = m_openList.back();
			incrementStepCount();
			m_closedList.push_back(next); //add to closed list
			m_openList.pop_back(); //pop the just considered element
			if (next->m_node == m_finish) {
				m_state = SOLVED;
				computePath(next);
				return m_state;
			}
			for (int dir = 0; dir < GridNode::NUM_DIRS; dir++) {
				if (next->m_node->containsEdge(dir)
						&& !next->m_node->getNeighbor(dir)->isVisited()) {
					next->m_node->setVisited(true);
					children.push_back(
							new PathNodeT(next,
									next->m_node->getNeighbor(dir)));
				}
			}
		}
		m_openList = children; //new children list
	}
	return m_state;
}

void MyBFSGridGraphSolver::computePath(PathNodeT *path) {
	assert(m_state == SOLVED);
	//We can draw
	while (path != NULL) {
		m_path.push_back(path);
		path = path->m_parent;
	}
}

void MyBFSGridGraphSolver::render() {
	//open list iterator

	//draw open list as gray circle-with-tale
	renderOpenAndClosedLists();

	//draw path
	if (m_state == SOLVED) {
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

void MyBFSGridGraphSolver::drawPathSegment(GridNode *from, GridNode *to) {
	glBegin(GL_LINES);
	//offset to start of maze, then index over by rows, then add offset to center of cell
	glVertex2f(m_x + from->getCol() * m_cellWidth + m_offsetX,
			m_y + from->getRow() * m_cellHeight + m_offsetY);
	glVertex2f(m_x + to->getCol() * m_cellWidth + m_offsetX,
			m_y + to->getRow() * m_cellHeight + m_offsetY);
	glEnd();
}

void MyBFSGridGraphSolver::renderOpenAndClosedLists() {
	std::list<PathNodeT *>::iterator iter;

	glColor3f(0.5f, 0.5f, 0.5f);
	for (iter = m_closedList.begin(); iter != m_closedList.end(); iter++) {
		drawListNode(*iter);
		drawListNodeTail(*iter);

	}

	glColor3f(0.25f, 0.75f, 0.75f);
	for (iter = m_openList.begin(); iter != m_openList.end(); iter++) {
		drawListNode(*iter);
		drawListNodeTail(*iter);
	}
}

void MyBFSGridGraphSolver::drawListNode(PathNodeT *p) {
	drawCircle(p->m_node->getCol(), p->m_node->getRow(), m_cellWidth / 8);
}

void MyBFSGridGraphSolver::drawListNodeTail(PathNodeT *p) {

//	draw tail
	if (p && p->m_parent) {
		int deltaX = p->m_parent->m_node->getCol() - p->m_node->getCol();
		int deltaY = p->m_parent->m_node->getRow() - p->m_node->getRow();

		//center of circle, then offset to edge of circle
		float x = (m_x + p->m_node->getCol() * m_cellWidth + m_offsetX)
				+ deltaX * (m_cellWidth / 8);
		float y = (m_y + p->m_node->getRow() * m_cellHeight + m_offsetY)
				+ deltaY * (m_cellWidth / 8);

		glBegin(GL_LINES);
		glVertex2f(x, y); //starts line on edge of circle
		x += deltaX * (m_cellWidth / 6);
		y += deltaY * (m_cellWidth / 6);
		glVertex2f(x, y); //starts line on edge of circle
		glEnd();
	}
}

MyBFSGridGraphSolver::~MyBFSGridGraphSolver() {
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

