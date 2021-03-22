#include "NodeGraph.h"
#include <raylib.h>

std::deque<NodeGraph::Node*> NodeGraph::findPath(Node* begin, Node* end)
{
	//Create a node pointer that points to the start node
	Node* start = begin;
	//Create a node pointer that points to the goal node
	Node* goal = end;

	//Check if the start or the goal pointer is null
	if (!start || !goal)
		//return an empty list
		return std::deque<Node*>();

	//Create a node pointer that will be act as an iterator for the graph
	Node* iterator = nullptr;

	//Create an open list
	std::deque<Node*> openList;
	//Create a closed list
	std::deque<Node*> closedList;

	//Add start to the open list
	openList.push_front(start);
	start->gScore = 0;

	//Loop while the open list is not empty
	while (openList.size() > 0)
	{
		//Sort the items in the open list by the g score
		openList = sortList(openList);

		//Set the iterator to be the first item in the open list
		iterator = openList.front();

		//Check if the iterator is pointing to the goal node
		if (iterator == goal)
			break;

		//Pop the first item off the open list
		openList.pop_front();

		//Add the first item to the closed list
		closedList.push_front(iterator);

		//Loop through all of the edges for the iterator
		for (int i = 0; i < iterator->connections.size(); i++)
		{
			//Create a node pointer to store the other end of the edge
			Node* other = nullptr;
			if (iterator->connections[i].target != iterator)
				other = iterator->connections[i].target;

			//Check if node at the end of the edge is in the closed list
			if (!isInList(closedList, other))
			{
				//Create an int and set it to be the g score of the iterator plus the cost of the edge
				float gScore = iterator->gScore + iterator->connections[i].cost;
				float hScore = (other->position - goal->position).getMagnitude();
				float fScore = gScore + hScore;
				//Check if the node at the end ofthe edge is in the open list
				if (!isInList(openList, other))
				{

					//Set the nodes g score to be the g score calculated earlier
					other->gScore = gScore;
					other->hScore = hScore;

					//Set the nodes previous to be the iterator
					other->previous = iterator;

					//Add the node to the open list
					openList.push_front(other);
				}
				//Otherwise if the g score is less than the node at the end of the edge's g score...
				else if (fScore < other->getFScore())
				{
					//Set its g score to be the g score calculated earlier
					other->gScore = gScore;
					other->hScore = hScore;

					//Set its previous to be the current node
					other->previous = iterator;
				}
			}
		}
	}

	// Reverse through previous nodes and store them in a vector to find the path
	std::deque<Node*> path;
	while (iterator != start)
	{
		path.push_front(iterator);
		iterator = iterator->previous;
	}
	return path;
}

void NodeGraph::drawGraph(Node* start)
{
	std::deque<Node*>* drawnList = new std::deque<Node*>();
	drawConnectedNodes(start, drawnList);
	delete drawnList;
}

void NodeGraph::drawNode(Node* node, int color)
{
	static char buffer[10];
	sprintf_s(buffer, "%.0f", node->gScore);

	//Draw the circle for the outline
	DrawCircle((int)node->position.x, (int)node->position.y, 16, GetColor(color));
	//Draw the inner circle
	DrawCircle((int)node->position.x, (int)node->position.y, 14, BLACK);
	//Draw the text
	DrawText(buffer, (int)node->position.x, (int)node->position.y, 24, RAYWHITE);
}

void NodeGraph::drawConnectedNodes(Node* node, std::deque<Node*>* drawnList)
{
	//drawNode(node);
	drawnList->push_back(node);

	//For each Edge in this node's connections
	for (Edge e : node->connections) {
		//Draw the Edge
		DrawLine((int)node->position.x, (int)node->position.y, (int)e.target->position.x, (int)e.target->position.y, WHITE);
		//Draw the cost
		MathLibrary::Vector2 costPos = { (node->position.x + e.target->position.x) / 2, (node->position.y + e.target->position.y) / 2 };
		static char buffer[10];
		sprintf_s(buffer, "%.0f", e.cost);
		DrawText(buffer, (int)costPos.x, (int)costPos.y, 16, RAYWHITE);
		//Draw the target node
		if (std::find(drawnList->begin(), drawnList->end(), e.target) == drawnList->end()) {
			drawConnectedNodes(e.target, drawnList);
		}
	}
}

std::deque<NodeGraph::Node*> NodeGraph::sortList(std::deque<Node*> list)
{
	for (int i = 0; i < list.size(); i++)
	{
		for (int j = list.size(); j > i; j--)
		{
			if (list[i]->getFScore() > list[j - 1]->getFScore())
			{
				Node* temp = list[i];
				list[i] = list[j - 1];
				list[j - 1] = temp;
			}
		}
	}
	return list;
}

bool NodeGraph::isInList(std::deque<Node*> list, Node* node)
{
	for (int i = 0; i < list.size(); i++)
		if (list[i] == node)
			return true;

	return false;
}
