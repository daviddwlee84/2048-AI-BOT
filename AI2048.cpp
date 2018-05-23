/**
 * @brief AI main program
 * 
 * @file AI2048.cpp
 * @author David Lee, Tom Chang
 * @date 2018-05-23
 */

#include "2048.h"
#include "AI2048.h"
using namespace std;

int main()
{
	// Initialization
	Game myGame;
	Grid CurrentGrid;

	int i = 0, j = 0;
	int score[TESTTIMES] = { 0 };
	int MaxScore = MIN;
	int ScoreSum = 0;
	int MaxTile = MIN;
	int CurrentMaxTile[TESTTIMES];
	int Depth = DEPTH;
	for (int Counter = 0; Counter < TESTTIMES; Counter++)
	{
		CurrentMaxTile[Counter] = MIN;
	}
	int PassCnt[STAGE_NUM] = { 0 };
	double StartTime = cpuTime();
	double EndTime;
	double EachStartTime[TESTTIMES];
	double EachEndTime[TESTTIMES];

	while (i < TESTTIMES)
	{
		EachStartTime[i] = cpuTime();
		while (!myGame.isGameOver())
		{
			myGame.printGrid(0, 0);
			myGame.getCurrentGrid(CurrentGrid);
			myGame.insertDirection(Search(CurrentGrid, &Depth));
			cout << "\n Depth = " << Depth << endl;
		}
		Depth = 4; // Initial depth

		FILE* ptr = fopen("result.txt", "w+");
		for (int Counter = 0; Counter < GRID_SIZE; Counter++)
		{
			if (MaxTile < CurrentGrid.operator[](Counter)) MaxTile = CurrentGrid.operator[](Counter);
			if (CurrentMaxTile[i] < CurrentGrid.operator[](Counter)) CurrentMaxTile[i] = CurrentGrid.operator[](Counter);
		}
		for (int Counter = 0; Counter < STAGE_NUM; Counter++)
		{
			if (CurrentMaxTile[i] >= (FIRST_STAGE << Counter))
				PassCnt[Counter]++;
		}

		score[i] = myGame.getScore();
		if (score[i] > MaxScore) MaxScore = score[i];

		fprintf(ptr, "Rounds: %d\n", i + 1);
		fprintf(ptr, "Highest Score: %d\n", MaxScore);
		for (int Counter = 0; Counter < i + 1; Counter++)
		{
			ScoreSum += score[Counter];
		}
		fprintf(ptr, "Everage Score: %d\n", ScoreSum / (i + 1));
		ScoreSum = 0;
		fprintf(ptr, "Max Tile: %d\n", MaxTile);
		for (int Counter = 0; Counter < STAGE_NUM; Counter++)
		{
			fprintf(ptr, "%d Rate: %f %%\n", (FIRST_STAGE << Counter), (double)PassCnt[Counter] * 100 / (i + 1));
		}
		EachEndTime[i] = EndTime = cpuTime();
		fprintf(ptr, "Time: %lf\n\n\n", EndTime - StartTime);
		fprintf(ptr, "===========================================\n\n\n");
		//cout << endl;
		//cout << "總分: " << myGame.getScore() << endl;

		for (int Counter = 0; Counter < i + 1; Counter++)
		{
			fprintf(ptr, "Rounds: %d\n", Counter + 1);
			fprintf(ptr, "Score: %d\n", score[Counter]);
			fprintf(ptr, "Max Tile: %d\n", CurrentMaxTile[Counter]);
			fprintf(ptr, "Time: %f\n\n", EachEndTime[Counter] - EachStartTime[Counter]);
		}
		fclose(ptr);

		myGame.reset();
		i++;
	}
	//myGame.~Game();

	return 0;
}

int Log2Value( int Value )
{
	int N = 0;
	while( Value > 0 )
	{
		Value /= 2;
		N++;
	}
	return N;
}

dir_e Search( Grid& Address, int* Depth )
{
	int MaxHeuristicValue = MIN; // Maximum heuristic value
	int HeuristicValue = 0; // Heuristic value
	dir_e BestMove = INVALID; // Best Direction
	dir_e Direction = INVALID; // Current Direction
	Grid CurrentGrid; // Current Grid
	double StartTime = cpuTime();
	double EndTime;
	int MaxTile = Address.getMaxTile();

	CurrentGrid.copy( Address ); // Copy to current grid

	// Search four direction and find maximum heuristic value and best move
	for (int Counter = 0; Counter < 4; Counter++)
	{
		switch (Counter)
		{
		case 0:
			Direction = LEFT;
			break;
		case 1:
			Direction = DOWN;
			break;
		case 2:
			Direction = RIGHT;
			break;
		case 3:
			Direction = UP;
			break;
		default:
			Direction = INVALID;
			cout << "INVALID";
			break;
		}
		// If the direction is able to move
		if (CurrentGrid.shift(Direction))
		{
			if (MaxTile < CurrentGrid.getMaxTile())
			{
				if (MaxTile >= 256) return Direction; // Force to combine if any tile is grater than 256
			}
		}
		CurrentGrid.copy(Address); // Recover current grid
	}
	for ( int Counter = 0; Counter < 4; Counter++ )
	{
		switch ( Counter )
		{
		case 0:
			Direction = LEFT;
			break;
		case 1:
			Direction = DOWN;
			break;
		case 2:
			Direction = RIGHT;
			break;
		case 3:
			Direction = UP;
			break;
		default:
			Direction = INVALID;
			cout << "INVALID";
			break;
		}

		// If the direction is able to move
		if ( CurrentGrid.shift( Direction ) )
		{
			HeuristicValue =  SearchAppreance( CurrentGrid, *Depth - 1 );
			
			// Update MaxHeuristicValue
			if ( HeuristicValue > MaxHeuristicValue )
			{
				MaxHeuristicValue = HeuristicValue;
				BestMove = Direction;
			}
		}
		CurrentGrid.copy( Address ); // Recover current grid
	}
	EndTime = cpuTime();
	if ((EndTime - StartTime) > 0.05) *Depth -= 1;
	if ((EndTime - StartTime) < 0.005) *Depth += 1;
	if (*Depth < 1) *Depth = 1;
	return BestMove; // Return direction
}

// Search for posible direction
int SearchDirection( Grid& Address, int Depth )
{
	int MaxHeuristicValue = MIN;
	dir_e Direction = INVALID;
	Grid CurrentGrid;

	// If the game is over or the depth reach expectation return heuristic value
	if ( isGameOver( Address ) || Depth < 0 )
	{
		return Evaluate( Address );
	}
	CurrentGrid.copy( Address );

	// Search four directions and find maximum heuristic value
	for ( int Counter = 0; Counter < 4; Counter++ )
	{
		switch ( Counter )
		{
		case 0:
			Direction = LEFT;
			break;
		case 1:
			Direction = DOWN;
			break;
		case 2:
			Direction = RIGHT;
			break;
		case 3:
			Direction = UP;
			break;
		default:
			Direction = INVALID;
			cout << "INVALID";
			break;
		}

		// If the direction is able to move
		if ( CurrentGrid.shift( Direction ) )
		{
			MaxHeuristicValue = max( MaxHeuristicValue, SearchAppreance( CurrentGrid, Depth - 1 ) );
		}
		CurrentGrid.copy( Address ); // Recover current grid
	}
	return MaxHeuristicValue;// Return maximum heuristic value
}

// Search for possible appearance
int SearchAppreance( Grid& Address, int Depth )
{
	Grid CurrentGrid;
	int HeuristicValue2 = 0;
	int HeuristicValue4 = 0;
	int HeuristicValue = 0;

	// If the game is over or the depth reach expectation return heuristic value
	if ( isGameOver( Address ) || Depth < 0 )
	{
		return Evaluate( Address );
	}

	CurrentGrid.copy( Address );
	// If no blank space to insert
	if ( CurrentGrid.getEmptyBlkNo() == 0 )
	{
		HeuristicValue = SearchDirection( CurrentGrid, Depth - 1 );
	}
	else
	{
		// Generate 4 and 2 tiles and sum heuristic value respectively
		for ( int Counter = 0; Counter < GRID_SIZE; Counter++ )
		{
			// If counter position is 0
			if ( CurrentGrid.operator[]( Counter ) == 0 )
			{
				CurrentGrid.setBlock( Counter, 2 ); // Insert tile 2
				HeuristicValue2 += SearchDirection( CurrentGrid, Depth - 1 ); // Sum the heuristic value of 2
				CurrentGrid.copy( Address ); // Recover current grid

				CurrentGrid.setBlock( Counter, 4 ); // Insert tile 4
				HeuristicValue4 += SearchDirection( CurrentGrid, Depth - 1 ); // Sum the heuristic value of 4
				CurrentGrid.copy( Address ); // Recover current grid
			}
		}
		HeuristicValue = ( ( ( HeuristicValue2 / CurrentGrid.getEmptyBlkNo() ) *  TILE_GEN_RATIO ) / ( TILE_GEN_RATIO + 1 ) ) + ( ( ( HeuristicValue4 / CurrentGrid.getEmptyBlkNo() ) * 1 ) / ( TILE_GEN_RATIO + 1 ) ); // 依照TILE_GEN_RATIO來計算評價值的期望值
		return HeuristicValue; // Return heuristic value
	}
}

// Check if game is over
bool isGameOver( Grid& Address )
{
	// Still have blank space
	if ( Address.getEmptyBlkNo() > 0 )
	{ 
		return FALSE;
	}
	// If adjacent position appear same number, it means it can be merge
	for ( int i = 0; i < GRID_LENGTH; i++ )
	{
		for ( int j = 0; j < GRID_LENGTH; j++ )
		{
			if ( ( i < GRID_LENGTH - 1 ) && Address.operator()( i, j ) == Address.operator()( i + 1, j ) )
			{
				return FALSE;
			}
			if ( ( j < GRID_LENGTH - 1 ) && Address.operator()( i, j ) == Address.operator()( i, j + 1 ) )
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

// Calculate heuristic value
int Evaluate( Grid& Address )
{
	int HeuristicValue = 0;
	HeuristicValue += Address.getEmptyBlkNo() * 128 * BLANK;
	HeuristicValue += Smoothness( Address ) * 128 * SMOOTH;
	HeuristicValue += Monotonicity( Address ) * 8 * MONO;
	HeuristicValue += Corner( Address ) * CORNER;

	return HeuristicValue;
}

int Smoothness( Grid& Address )
{
	int HeuristicValue = 0;
	int TempRow = 0;
	int TempCol = 0;
	for ( int i = 0; i < GRID_LENGTH; i++ )
	{
		for ( int j = 0; j < GRID_LENGTH; j++ )
		{
			if ( Address.operator()( i, j ) != 0 )
			{
				if ( TempRow != 0 && ( TempRow == Address.operator()( i, j ) ) )
				{
					HeuristicValue++;
					TempRow = 0;
				}
				else
				{
					TempRow = Address.operator()( i, j );
				}
			}
			if ( Address.operator()( j, i ) != 0 )
			{
				if ( TempCol != 0 && ( TempCol == Address.operator()( j, i ) ) )
				{
					HeuristicValue++;
					TempCol = 0;
				}
				else
				{
					TempCol = Address.operator()( j, i );
				}
			}
		}
		TempCol = 0;
		TempRow = 0;
	}
	return HeuristicValue;
}

int Monotonicity( Grid& Address )
{
	int HeuristicValue = 0;
	int HeurMax = MIN;
	int Last = MAX;
	int Current;
	int Case = 0;
	int MaxValue = MIN;
	int Index = 0;
	int IteratorList[8][3] = { { 0, GRID_LENGTH, 1 }, { 0, 1, GRID_LENGTH }, { GRID_SIZE - GRID_LENGTH, -GRID_LENGTH, 1 }, { GRID_SIZE - GRID_LENGTH, 1, -GRID_LENGTH }, { GRID_LENGTH - 1, GRID_LENGTH, -1 }, { GRID_LENGTH - 1, -1, GRID_LENGTH }, { GRID_SIZE - 1, -GRID_LENGTH, -1 }, { GRID_SIZE - 1, -1, -GRID_LENGTH } };

	// Find the maximum corner
	if ( Address.operator[]( 0 ) > MaxValue )
	{
		MaxValue = Address.operator[]( 0 );
		Case = 0;
	}
	if ( Address.operator[]( GRID_SIZE - GRID_LENGTH ) > MaxValue )
	{
		MaxValue = Address.operator[]( GRID_SIZE - GRID_LENGTH );
		Case = 2;
	}
	if ( Address.operator[]( GRID_LENGTH - 1 ) > MaxValue )
	{
		MaxValue = Address.operator[]( GRID_LENGTH - 1 );
		Case = 4;
	}
	if ( Address.operator[]( GRID_SIZE - 1 ) > MaxValue )
	{
		MaxValue = Address.operator[]( GRID_SIZE - 1 );
		Case = 6;
	}


	for ( int Counter = 0; Counter < 2; Counter++ )
	{
		Index = IteratorList[Case][0];
		for ( int CounterA = 0; CounterA < GRID_LENGTH; CounterA++ )
		{
			for ( int CounterB = 0; CounterB < GRID_LENGTH; CounterB++ )
			{
				Current = Address.operator[]( Index );
				if ((Current == 0) || (Last < Current) && (Last != MAX))
				{
					if (Counter == 0){
						HeurMax = HeuristicValue;
						break;
					}
					else if (HeurMax > HeuristicValue) return HeurMax;
					else return HeuristicValue;
				}
				if ( Last != MAX )
				{
					HeuristicValue += Last / (Log2Value(Last) - Log2Value(Current) + 1); // (((Log2Value(Last) - Log2Value(Current)) == 0) ? 4 : (Log2Value(Last) - Log2Value(Current)) )
				}
				if ( CounterB < GRID_LENGTH - 1 )
				{
					Index += IteratorList[Case][1];
				}
				else
				{
					IteratorList[Case][1] = -IteratorList[Case][1];
					Index += IteratorList[Case][2];
				}
				Last = Current;
			}
			if (Current == 0) break;
		}
		Case++;
		Last = MAX;
		HeuristicValue = 0;
	}
}

int Corner( Grid& Address )
{
	int Index;
	int CounterA;
	int CounterB;
	int way;
	int MaxValue = MIN;
	int MaxCurrent = MIN;
	int MinCurrent = MAX;
	int MinLast = MAX;
	int HeuristicValue = 0;
	int score[GRID_LENGTH] = { 0, 1600, 400, 48 };

	// Find maximum corner
	if ( Address.operator[]( 0 ) > MaxValue )
	{
		MaxValue = Address.operator[]( 0 );
		way = 0;
	}
	if ( Address.operator[]( GRID_LENGTH - 1 ) > MaxValue )
	{
		MaxValue = Address.operator[]( GRID_LENGTH - 1 );
		way = 1;
	}
	if ( Address.operator[]( GRID_SIZE - GRID_LENGTH ) > MaxValue )
	{
		MaxValue = Address.operator[]( GRID_SIZE - GRID_LENGTH );
		way = 2;
	}
	if ( Address.operator[]( GRID_SIZE - 1 ) > MaxValue )
	{
		MaxValue = Address.operator[]( GRID_SIZE - 1 );
		way = 3;
	}
	switch ( way )
	{
	case 0: // Upper left
		for ( CounterA = 0; CounterA < GRID_LENGTH; CounterA++ )
		{
			for ( CounterB = 0; CounterB < CounterA + 1; CounterB++ )
			{
				Index = CounterA + ( CounterB * ( GRID_LENGTH - 1 ) );
				if ( Address.operator[]( Index ) < MinCurrent ) MinCurrent = Address.operator[]( Index );
				if ( Address.operator[]( Index ) > MaxCurrent ) MaxCurrent = Address.operator[]( Index );
			}
			if ( MinLast >= MaxCurrent )	HeuristicValue += score[CounterA];
			MinLast = MinCurrent;
			MaxCurrent = MIN;
			MinCurrent = MAX;
		}
		MinLast = MAX;
		break;
	case 1:  // Upper right
		for ( CounterA = 0; CounterA < GRID_LENGTH; CounterA++ )
		{
			for ( CounterB = 0; CounterB < CounterA + 1; CounterB++ )
			{
				Index = ( 3 - CounterA ) + ( CounterB * ( GRID_LENGTH + 1 ) );
				if ( Address.operator[]( Index ) < MinCurrent ) MinCurrent = Address.operator[]( Index );
				if ( Address.operator[]( Index ) > MaxCurrent ) MaxCurrent = Address.operator[]( Index );
			}
			if ( MinLast >= MaxCurrent ) HeuristicValue += score[CounterA];
			MinLast = MinCurrent;
			MaxCurrent = MIN;
			MinCurrent = MAX;
		}
		MinLast = MAX;
		break;
	case 2:  // Lower left
		for ( CounterA = 0; CounterA < GRID_LENGTH; CounterA++ )
		{
			for ( CounterB = 0; CounterB < CounterA + 1; CounterB++ )
			{
				Index = ( 12 - 4 * CounterA ) + ( CounterB * ( GRID_LENGTH + 1 ) );
				if ( Address.operator[]( Index ) < MinCurrent ) MinCurrent = Address.operator[]( Index );
				if ( Address.operator[]( Index ) > MaxCurrent ) MaxCurrent = Address.operator[]( Index );
			}
			if ( MinLast >= MaxCurrent ) HeuristicValue += score[CounterA];
			MinLast = MinCurrent;
			MaxCurrent = MIN;
			MinCurrent = MAX;
		}
		MinLast = MAX;
		break;
	case 3:  // Lower right
		for ( CounterA = 0; CounterA < GRID_LENGTH; CounterA++ )
		{
			for ( CounterB = 0; CounterB < CounterA + 1; CounterB++ )
			{
				Index = ( 15 - 4 * CounterA ) + ( CounterB * ( GRID_LENGTH - 1 ) );
				if ( Address.operator[]( Index ) < MinCurrent ) MinCurrent = Address.operator[]( Index );
				if ( Address.operator[]( Index ) > MaxCurrent ) MaxCurrent = Address.operator[]( Index );
			}
			if ( MinLast >= MaxCurrent ) HeuristicValue += score[CounterA];
			MinLast = MinCurrent;
			MaxCurrent = MIN;
			MinCurrent = MAX;
		}
		MinLast = MAX;
		break;
	}
	return HeuristicValue;
}