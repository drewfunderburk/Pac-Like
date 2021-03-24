#include "Maze.h"

Maze::Maze(TileKey map[Maze::HEIGHT][Maze::WIDTH])
{
	//Create the player
	m_player = new Pac(
		WIDTH / 2 * TILE_SIZE + (TILE_SIZE / 2),
		HEIGHT / 2 * TILE_SIZE + (TILE_SIZE / 2),
		200
	);

	m_goal = new Goal(0, 0, Maze::TILE_SIZE - 2.5f, 0, 0x00FF00FF);
	//Generate the map
	generate(map);
	//Add the player to the scene
	addActor(m_player);
}

Maze::~Maze()
{
	delete m_player;
	for (int y = 0; y < m_size.y; y++) {
		for (int x = 0; x < m_size.x; x++) {
			delete m_grid[x][y].actor;
			delete m_grid[x][y].node;
		}
	}
}

void Maze::draw()
{
	Scene::draw();
	//NodeGraph::drawGraph(m_grid[0][0].node);
}

Maze::Tile Maze::getTile(MathLibrary::Vector2 position)
{
	int x = (int)(position.x / TILE_SIZE);
	int y = (int)(position.y / TILE_SIZE);
	if (x >= 0 && x < Maze::WIDTH && y >= 0 && y < Maze::HEIGHT)
		return m_grid[x][y];
	else
		return m_grid[0][0];
}

MathLibrary::Vector2 Maze::getPosition(Tile tile)
{
	return MathLibrary::Vector2{ tile.x * TILE_SIZE + (TILE_SIZE / 2.0f), tile.y * TILE_SIZE + (TILE_SIZE / 2.0f) };
}

Maze::Tile Maze::createTile(int x, int y, TileKey key)
{
	// Create a new tile at the given location
	Tile tile{ x, y };
	MathLibrary::Vector2 position = getPosition(tile);
	// Set the cost and actor of each tile
	switch (key) {
	case TileKey::OPEN:
		tile.cost = 1.0f;
		break;
	case TileKey::WALL:
		tile.cost = 100.0f;
		tile.actor = new Wall(position.x, position.y);
		addActor(tile.actor);
		break;
	case TileKey::GOAL:
		tile.cost = 0.0f;
		tile.actor = m_goal;
		m_goal->setWorldPostion(position);
		addActor(tile.actor);
		break;
	case TileKey::SPAWNER:
		tile.cost = 1.0f;
		tile.actor = new Spawner(position.x, position.y, (int)0xFF0000FF, m_goal);
		addActor(tile.actor);
		break;
	case TileKey::GHOST:
		tile.cost = 1.0f;
		Ghost* ghost = new Ghost(position.x, position.y, 100.0f, 0xFF6666FF, this);
		ghost->setTarget(m_goal);
		tile.actor = ghost;
		addActor(tile.actor);
		break;
	}
	return tile;
}

void Maze::generate(TileKey map[Maze::HEIGHT][Maze::WIDTH])
{
	for (int y = 0; y < m_size.y; y++) {
		for (int x = 0; x < m_size.x; x++) {
			// Create the tile
			Tile currentTile = createTile(x, y, map[y][x]);
			// Add node to graph
			currentTile.node = new NodeGraph::Node();
			currentTile.node->position = getPosition(currentTile);
			if (x > 0) { // west connection
				Tile other = m_grid[x - 1][y];
				currentTile.node->connections.push_back(NodeGraph::Edge{ other.node, other.cost });
				other.node->connections.push_back(NodeGraph::Edge{ currentTile.node, currentTile.cost });
			}
			if (y > 0) { // north connection
				Tile other = m_grid[x][y - 1];
				currentTile.node->connections.push_back(NodeGraph::Edge{ other.node, other.cost });
				other.node->connections.push_back(NodeGraph::Edge{ currentTile.node, currentTile.cost });
			}
			// Set the tile on the grid
			m_grid[x][y] = currentTile;
		}
	}
}
