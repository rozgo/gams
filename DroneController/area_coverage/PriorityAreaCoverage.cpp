/*******************************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 ******************************************************************************/

/*******************************************************************************
 * PriorityAreaCoverage.cpp - Declares the structures and methods for the
 * algorithm to perform an priority area search
 ******************************************************************************/

#include "PriorityAreaCoverage.h"

#include "utilities/Position.h"
#include "utilities/CommonMadaraVariables.h"
#include "utilities/gps_utils.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime> 
#include <string>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <queue>
#include <set>
#include <utility>
#include <iomanip>

#include "QueueObject.h"

using std::cerr;
using std::endl;

using namespace SMASH::Utilities;
using namespace SMASH::AreaCoverage;

const std::string directions[] = {"stay", "up-left", "up", "up-right", "left", "right", "down-left", "down", "down-right"};
int globalSum = 0;
int globalStep = 0;
int globalAlgorithm = 0;
bool globalUseNeighbors = false;

std::vector<std::vector<std::pair<int, int> > > futureMoves;

// Constructor
PriorityAreaCoverage::PriorityAreaCoverage(Madara::Knowledge_Engine::Variables &variables, std::vector<Region> regions, Region searchRegion, double delta) : AreaCoverage(), m_variables(variables), m_searchRegion(searchRegion), m_delta(delta), m_regions(regions)
{
    m_turn = 1;
    m_variables = variables;
    m_variables.set("matrixIsSet", Madara::Knowledge_Record::Integer(0));
    std::cout << "Algorithm:" << variables.get("device.0.area_coverage_requested").to_string() << "\n";
}

double PriorityAreaCoverage::degreesToRadians(double degrees)
{
    return degrees * DEGREES_PER_METER;
}

// Destructor
PriorityAreaCoverage::~PriorityAreaCoverage() {}

// Initialize the area for the drone
Region* PriorityAreaCoverage::initialize(const Region& grid, int deviceIdx, 
    int numDrones)
{
    m_delta = degreesToRadians(m_delta);
    printf("Initializing priority area coverage algorithm.\n");
    m_cellToSearch = new Region(grid);

    m_delta = 0.5 * DEGREES_PER_METER;

    m_id = deviceIdx;

    // Calculate matrix size.		
//    m_width  = std::ceil( std::abs(grid.northWest.latitude - grid.southEast.latitude) / m_delta );
//    m_height = std::ceil( std::abs(grid.northWest.longitude - grid.southEast.longitude) / m_delta );
    m_width = 24;
    m_height = 18;
    m_ref_lat = grid.southEast.latitude;
    m_ref_long = grid.northWest.longitude;

    std::cout << "nw lat:  " << std::setprecision(8) << grid.northWest.latitude  << "\n";
    std::cout << "se lat:  " << std::setprecision(8) << grid.southEast.latitude  << "\n";
    std::cout << "nw long: " << std::setprecision(8) << grid.northWest.longitude << "\n";
    std::cout << "se long: " << std::setprecision(8) << grid.southEast.longitude << "\n";

    std::cout << "m_delta: " << m_delta << "\n";

    std::cout << "matrix width:  " << m_width  << "\n";
    std::cout << "matrix height: " << m_height << "\n";

    std::cout << "Algorithm:" << m_variables.get("device.0.area_coverage_requested").to_string() << "\n";


    // Create Search matrix
    m_matrix = std::vector<std::vector<int> > (m_height, std::vector<int>(m_width));
    init(m_matrix);
    print(m_matrix, -1, -1);
    solveMatrix(m_matrix, numDrones, m_id);

    // Wait - time to look at data
    sleep(5);
   
    return m_cellToSearch;
}

/**
 *  Initialized a matrix of size SIZE with random values from 0-9 priorities.
 **/
void PriorityAreaCoverage::init(std::vector<std::vector<int> > &matrix) 
{

  std::cout << "number of regions: " << m_regions.size() << std::endl;
  for (int j = 0; j < m_width; j++) 
  {
    for (int i = 0; i < m_height; i++) 
    {
/*      matrix[i][j] = 0;
      for (int r = 0; r < m_regions.size(); r++) {
        Region subRegion = m_regions[r];
//        std::cout << "Region priority: " << subRegion.priorityValue << std::endl;
        if( subRegion.contains(Position(xToLat(i), yToLong(j))) && subRegion.priorityValue > matrix[i][j] ) {
          std::cout << "changing priorty of [" << i << ", " <<  j << "] to " << subRegion.priorityValue << std::endl;
          matrix[i][j] = subRegion.priorityValue;
        }
      }
*/
      matrix[i][j] = i + j;
    }
  }
}

void PriorityAreaCoverage::solveMatrix(std::vector<std::vector<int> > &matrix, int numDrones, int myId)
{
    std::vector<QueueObject> dronePositions;

    for (int i = 0; i < numDrones; i++)
    {
    // initalize future move tracker
    std::vector<std::pair<int, int> > temp;
    temp.push_back(std::make_pair(0, 0));
    futureMoves.push_back(temp);

    // start initial positions
    std::cout << "Adding starting position for drone " << i << std::endl;
    dronePositions.push_back(QueueObject(i, 0, 0, 0));
    }

    // sort the positions by least moved
    std::make_heap (dronePositions.begin(),dronePositions.end());
    std::cout << "First on heap: " << dronePositions.front().id << std::endl;
    
    QueueObject temp = dronePositions.front();

    int turn = 0;
    // Loop through positions until matrix is searched
    while (sumMatrix(matrix) > 0) 
    {
        cout << "Calculating next move\n";
        std::pair<int, int> nextPosition = nextMove(matrix, temp.x, temp.y);
        futureMoves[turn % numDrones].push_back(std::make_pair(nextPosition.first, nextPosition.second));	
        turn++;
    }

    // Display future moves
    std::cout << "future moves:\n";
    for (int j = 0; j < numDrones; j++) {
      for (int i = 0; i < futureMoves[j].size(); i++) {
	std::cout << "[" << futureMoves[j][i].first << ", " << futureMoves[j][i].second << "]  ";
      }
      std::cout << "\n";
    }
}

/**
 *  Determines the next best move for a drone and doese the move. First, finds
 *  the next direction by finding the best neighbor and then executes a move in
 *  that direction.
 **/
std::pair<int, int> PriorityAreaCoverage::nextMove(std::vector<std::vector<int> > &matrix, int &x, int &y) {
 
  std::pair<int, int> nextPosition = distanceBasedSearch(matrix, x, y);
 
//  print(matrix, x, y);
//  std::cout << "\nMoving to " << nextPosition.first << ", " << nextPosition.second << std::endl;
//  sleep(2);
  
  matrix[nextPosition.first][nextPosition.second] = 0;

  return nextPosition;
}

double PriorityAreaCoverage::distance(int x1, int y1, int x2, int y2)
{
  return std::sqrt((double)std::pow(x2-x1,2.0) + (double)std::pow(y2-y1,2.0));
}

// Calculates the next location to move to, assuming we have reached our
// current target.
Position PriorityAreaCoverage::getNextTargetLocation()
{
    int x = 0, y = 0;
//    int nextDirection = decreasingOrder(m_matrix, x, y);
//    move(m_matrix, x, y, nextDirection);

//    m_targetLocation.latitude = (40.44108 + m_id * 0.00001);
//    m_targetLocation.longitude = (-79.947164 + m_id * 0.00001);      

//    std::string exs = vars.expand_statement(
//	"state_{.id} == ''"
//    );
//    Madara::Knowledge_Engine::Compiled_Expression ce = vars.compile(exs);
//    vars.evaluate(ce);


    m_targetLocation.latitude  = xToLat(futureMoves[m_id][m_turn].first);
    m_targetLocation.longitude = yToLong(futureMoves[m_id][m_turn].second);
    m_turn++;

    std::cout << "Moving to: [" << m_targetLocation.latitude << ", " << m_targetLocation.longitude << "]\n";
    
    return m_targetLocation;
}

double PriorityAreaCoverage::xToLat(int x) 
{
//    double startLat = 40.44108;
//    return startLat + (x * m_delta);    
    return m_ref_lat + (x * m_delta);
}

double PriorityAreaCoverage::yToLong(int y)
{
//    double startLong = -79.947164;
//    return startLong + (y * m_delta);	
    return m_ref_long + (y * m_delta);
}


// Query if algorithm has reached final target
// @return    false, default to algorithm never finishes
bool PriorityAreaCoverage::isTargetingFinalWaypoint()
{
    if (m_turn == futureMoves[m_id].size()) 
    {
      return true;
    }
    return false;
}

/**
 *  Prints a matrix as a square of numbers representing priorities and an X to
 *  make the locations of drones.
 **/
void PriorityAreaCoverage::print(std::vector<std::vector<int> > matrix, int x, int y) {
  
  for (int i = 0; i < m_height; i++) {
    for (int j = 0; j < m_width; j++) {
      if (j == x && i == y) {
        std:: cout << "X  ";
      }
      else {
        std::cout << matrix[i][j] << "  ";
      }
    }
    std::cout << std::endl;
  }
}

std::pair<int, int> PriorityAreaCoverage::distanceBasedSearch(std::vector<std::vector<int> > matrix, int x, int y) {

  int maxPriority = 0;
  double minDistance = 99999999999;
  std::pair<int, int> xy = std::make_pair(x, y);
  
  double max = distance(0,0,m_width,m_height);
//  cout << "w: " << m_width << " h: " << m_height << " dist: " << max << endl;
//  exit(2);

  for (int j = 0; j < m_width; j++) {
    for (int i = 0; i < m_height; i++) {
      if (matrix[i][j] > 0 && (matrix[i][j] * (1 - ( distance(x,y,i,j) / max ) ) >= maxPriority ) ) {
        maxPriority = matrix[i][j] * (1 - ( distance(x,y,i,j) / max ) );
        minDistance = distance(x,y,i,j);
        cout << "Distance: " << distance(x,y,i,j) << endl;
        xy.first = i;
        xy.second = j;
      }
if (matrix[i][j] > 0 && !(matrix[i][j] * (1 - ( distance(x,y,i,j / max) ) ) >= maxPriority ) ) {
  cout << "Say what?? " << matrix[i][j] << ", " << distance(x,y,i,j) << ", " << max <<  endl;
  cout << (matrix[i][j] * (1 - ( distance(x,y,i,j) / max ) ) ) << endl;
}
    }
  }
  return xy;
}

std::pair<int, int> PriorityAreaCoverage::stupidSearch(std::vector<std::vector<int> > matrix, int x, int y) {

  int maxPriority = 0;
  double minDistance = 99999999999;
  std::pair<int, int> xy = std::make_pair(x, y);

  for (int j = 0; j < m_width; j++) {
    for (int i = 0; i < m_height; i++) {
      if (matrix[i][j] > maxPriority || (matrix[i][j] == maxPriority && distance(x,y,i,j) < minDistance) ) {
        maxPriority = matrix[i][j];
        minDistance = distance(x,y,i,j);
        cout << "Distance: " << distance(x,y,i,j) << endl;
        xy.first = i;
        xy.second = j;
      }
    }
  }
  return xy;
}

std::pair<int, int> PriorityAreaCoverage::decreasingOrder(std::vector<std::vector<int> > matrix, int x, int y) {
//  int nextDirection;
  int max = 0;
  int origX = x;
  int origY = y;
  int testX = 0;
  int testY = 0;

  std::queue<std::pair<int, int> > toDo;
  std::set<std::pair<int, int> > queued;
  std::pair<int, int> xy;

  toDo.push( std::make_pair(x, y) );
  queued.insert( std::make_pair(x, y) );
  while ( !toDo.empty() ) {
    xy = toDo.front();
    x = xy.first;
    y = xy.second;
    toDo.pop();

//    std::cout << "processing " << x << ", " << y << std::endl;
    if (matrix[x][y] > max) {
      testX = x;
      testY = y;
      max = matrix[x][y];
//      std::cout << "new max: " << max << std::endl;
    }

//    if ( process( x, y ) != stop ) {
      for( int xp = -1; xp <= 1; ++xp ) {
        for( int yp = -1; yp <= 1; ++yp ) {
          if ( queued.find( std::make_pair(x+xp, y+yp) ) == queued.end() && inBounds(x+xp, y+yp) ) {
//            std::cout << "adding " << x+xp << ", " << y+yp << std::endl;
            toDo.push( std::make_pair(x+xp, y+yp) );
            queued.insert( std::make_pair(x+xp, y+yp) );
          }
        }
      }
  //  }
  }

  return std::make_pair(testX, testY);
}

/**
 *  Checks to see if an x and y position is inside the matrix.
 **/
bool PriorityAreaCoverage::inBounds(int x, int y) {
  if (x < 0 || y < 0 || x >= m_width || y >= m_height)
    return false;
  return true;
}

/**
 *  Calculates the total priority value of the unexplored squares in the matrix.
 **/
int PriorityAreaCoverage::sumMatrix(std::vector<std::vector<int> > &matrix) {
  int sum = 0;
  for (int j = 0; j < m_width; j++) {
    for (int i = 0; i < m_height; i++) {
      sum += matrix[i][j];
    }
  }
  return sum;
}

