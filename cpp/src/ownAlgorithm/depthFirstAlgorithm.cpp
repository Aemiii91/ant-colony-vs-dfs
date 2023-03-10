#include "depthFirstAlgorithm.h"
#include <iostream>

vector<Node> DFSAlgorithm::SecondDraft() {
	vector<Node> currentPath, bestPath;
	vector<double> currentTime, bestTime;

	// Prerequisite variable assignment
	Node root = this->_graph.nodelist.front();
	Node currentNode = root;
	currentPath.emplace_back(root);
	currentTime.emplace_back(0);
	Node nextNode = GetClosestNode(currentPath, currentNode);
	Node secondNode = GetSecondBestNode(currentPath, &currentNode);

	bool travel = true;
	int iteratorCount = 0;
	double timeSpent = 0;
	while (travel) {
		while (TravelTime(&currentNode, &nextNode) +
					   TravelTime(&nextNode, &root) + timeSpent <=
				   this->_timeInterval &&
			   currentNode.GetEdgeListSize() > 1) {
			currentTime.emplace_back(TravelTime(&currentNode, &nextNode) +
									 timeSpent);
			timeSpent = currentTime.back();
			currentNode = GetClosestNode(currentPath, currentNode);
			currentPath.emplace_back(currentNode);
			nextNode = GetClosestNode(currentPath, currentNode);
		}

		if (currentPath.size() > bestPath.size()) {
			bestPath = currentPath;
			bestTime = currentTime;
		}

		currentPath.pop_back();
		currentNode = currentPath.back();
		currentTime.pop_back();
		timeSpent = currentTime.back();

		while (currentNode.childrensChecked == true) {
			if (currentNode.ID == root.ID &&
				currentNode.childrensChecked == true) {
				travel = false;
				break;
			}
			currentPath.pop_back();
			currentNode = currentPath.back();
			currentTime.pop_back();
			timeSpent = currentTime.back();
		}
		if (iteratorCount == 100) {
			travel = false;
		}

		if (!travel) {
			break;
		}

		secondNode = GetSecondBestNode(currentPath, &currentNode);
		if (TravelTime(&currentNode, &secondNode) +
					TravelTime(&secondNode, &root) + timeSpent <=
				this->_timeInterval &&
			currentNode.GetEdgeListSize() >= 1 &&
			!currentNode.childrensChecked) {
			currentPath.pop_back(); // Popping the currentNode from currentPath
									// and placing it in again with
									// childrensChecked bool set to true.
			currentNode.childrensChecked = true;
			currentPath.emplace_back(currentNode);
			currentTime.emplace_back(TravelTime(&currentNode, &secondNode) +
									 timeSpent);
			timeSpent = currentTime.back();
            cout << timeSpent << endl; //Apparently couting timeSpent fixes the SmallPathLimitedTime test in Release
			currentNode = GetSecondBestNode(currentPath, &currentNode);

			currentPath.emplace_back(currentNode);
			nextNode = GetClosestNode(currentPath, currentNode);
		} else {
			currentPath.pop_back();
			currentNode.childrensChecked = true;
			currentPath.emplace_back(currentNode);
		}
		iteratorCount++;
		cout << iteratorCount << endl;
	}
	bestTime.emplace_back(TravelTime(&bestPath.front(), &bestPath.back()) +
						  bestTime.back());
	bestPath.emplace_back(root);
	this->_path = bestPath;
	cout << "DFS Path cost: " << bestTime.back() << endl;
	return this->_path;
}

Node DFSAlgorithm::GetSecondBestNode(std::vector<Node> currentPath,
									 Node *currentNode) {
	double lowestCost = LONG_MAX;
	double secondLowestCost = LONG_MAX;
	Edge lowestEdge;
	Edge secondLowestEdge;
	Node res;

	for (auto edge : currentNode->edgeList) {
		if (edge.weight < lowestCost && !IsInPath(currentPath, edge.dist)) {
			secondLowestCost = lowestCost;
			secondLowestEdge = lowestEdge;
			lowestCost = edge.weight;
			lowestEdge = edge;
		} else if (edge.weight < secondLowestCost &&
				   !IsInPath(currentPath, edge.dist)) {
			secondLowestCost = edge.weight;
			secondLowestEdge = edge;
		}
	}
	res = GetNodeFromID(secondLowestEdge.dist);
	return res;
}

bool DFSAlgorithm::IsInPath(std::vector<Node> currentPath, int nextNode) {
	for (auto Node : currentPath) {
		if (nextNode == Node.ID) {
			return true;
		}
	}
	return false;
}

Node DFSAlgorithm::GetClosestNode(vector<Node> currentPath, Node currentNode) {
	double lowestCost = LONG_MAX;
	Node res;
	Edge lowestEdge;
	for (Edge edge : currentNode.edgeList) {
		if (edge.weight < lowestCost && !IsInPath(currentPath, edge.dist)) {
			lowestCost = edge.weight;
			lowestEdge = edge;
		}
	}
	res = GetNodeFromID(lowestEdge.dist);
	return res;
}
