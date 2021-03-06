#include "sortedMesh.H"

sortedMesh::sortedMesh(const pointField& thePoints,
						const faceList& theFaces,
						const labelList& theOwners,
						const labelList& theNeighbors,
						const polyBoundaryMesh& theBoundaries,
						double numberOfCells)
{
	std::cout << "constructing for sortedMesh class from polyMesh points,faces,owners,neighbours, and boundaries\n";
	std::cout << "thePoints.size() = " << thePoints.size() << "\n";
	std::cout << "theFaces.size() = " << theFaces.size() << "\n";
	std::cout << "theOwners.size() = " << theOwners.size() << "\n";
	std::cout << "theNeighbors.size() = " << theNeighbors.size() << "\n";
	std::cout << "theBoudaries.size() = " << theBoundaries.size() << "\n";
	
	/*
	std::cout << "thePoints[0][0] = " << thePoints[0][0] << "\n";
	std::cout << "thePoints[0][1] = " << thePoints[0][1] << "\n";
	std::cout << "thePoints[0][2] = " << thePoints[0][2] << "\n";
	std::cout << "theFaces[0][0] = " << theFaces[0][0] << "\n";
	std::cout << "theFaces[0][1] = " << theFaces[0][1] << "\n";
	std::cout << "theFaces[0][2] = " << theFaces[0][2] << "\n";
	std::cout << "theFaces[0][3] = " << theFaces[0][3] << "\n";
	std::cout << "theOwners[0] = " << theOwners[0] << "\n";
	std::cout << "theNeighbors[0] = " << theNeighbors[0] << "\n";
	std::cout << "theBoundaries[0][0][0] = " << theBoundaries[0][0][0] << "\n";
	*/
	
	for(double j = 0; j < numberOfCells; j++)
	{
		theCells.push_back(meshCell(j));
	}
	std::cout << "theCells.size() = " << theCells.size() << "\n";
	
	findCellFaces(thePoints,theFaces,theOwners,theNeighbors,theNeighbors.size());

	//findFaceLayers(theFaces,theOwners,theNeighbors.size());	//assumes that all boundary faces are held after all the internalField faces!!!
	// generateNewPointsList();
}

void sortedMesh::findCellFaces(const pointField& thePoints, const faceList& theFaces,
	const labelList& theOwners,	const labelList& theNeighbors, double nInternalFaces)
{
	std::cout << "finding owners and neighbors per cell\n";
	std::vector< std::vector<double> > ownersPerCell(theCells.size(),std::vector<double>(0,0));
	std::vector< std::vector<double> > neighborsPerCell(theCells.size(),std::vector<double>(0,0));
	for(double j = 0; j < nInternalFaces; j++)
	{
		ownersPerCell[theOwners[j]].push_back(j);
		neighborsPerCell[theNeighbors[j]].push_back(j);
	}
	
	// hmm, I did what I wanted, except it wasn't actually what I wanted.
	// the owners and neighbors do show which faces belong to a given cell,
	// BUT it means that only one of the two cells connected to the face are given?
	// wait, technically 6 isn't the max number anymore. the boundaries aren't included here
	// let's include the boundaries now
	for(double j = nInternalFaces; j < theOwners.size(); j++)
	{
		ownersPerCell[theOwners[j]].push_back(j);
	}
	//much better!
	
	for(double j = 0; j < theCells.size(); j++)
	{
		std::cout << "ownersPerCell[j].size() = " << ownersPerCell[j].size() << "\n";
		for(double i = 0; i < ownersPerCell[j].size(); i++)
		{
			std::cout << "ownersPerCell[j][i] = " << ownersPerCell[j][i] << "\n";
		}
		std::cout << "neighborsPerCell[j].size() = " << neighborsPerCell[j].size() << "\n";
		for(double i = 0; i < neighborsPerCell[j].size(); i++)
		{
			std::cout << "neighborsPerCell[j][i] = " << neighborsPerCell[j][i] << "\n";
		}
	}
	
	std::cout << "categorizing owners and faces of cell\n";
	for(double j = 0; j < theCells.size(); j++)
	{
		double nFacesPerCell = ownersPerCell[j].size() + neighborsPerCell[j].size();
		std::cout << "j = " << j << ", nFacesPerCell = " << nFacesPerCell << "\n";
		if(nFacesPerCell > 6)
		{
			// this cell has a bunch of refinement stuff going on!
		} else
		{
			double smallestX = 999999999999;
			double smallestY = 999999999999;
			double smallestZ = 999999999999;
			double smallestXface = -1;
			double smallestYface = -1;
			double smallestZface = -1;
			double biggestX = -999999999999;
			double biggestY = -999999999999;
			double biggestZ = -999999999999;
			double biggestXface = -1;
			double biggestYface = -1;
			double biggestZface = -1;
			for(double i = 0; i < ownersPerCell[j].size(); i++)
			{
				for(double kk = 0; kk < 4; kk++)	// 4 because there are 4 points in a face
				{
					// there are 3 parts to a point
					if(thePoints[theFaces[ownersPerCell[j][i]][kk]][0] < smallestX)
					{
						smallestX = thePoints[theFaces[ownersPerCell[j][i]][kk]][0];
						smallestXface = ownersPerCell[j][i];
					}
					if(thePoints[theFaces[ownersPerCell[j][i]][kk]][0] > biggestX)
					{
						biggestX = thePoints[theFaces[ownersPerCell[j][i]][kk]][0];
						biggestXface = ownersPerCell[j][i];
					}

					if(thePoints[theFaces[ownersPerCell[j][i]][kk]][1] < smallestY)
					{
						smallestY = thePoints[theFaces[ownersPerCell[j][i]][kk]][1];
						smallestYface = ownersPerCell[j][i];
					}
					if(thePoints[theFaces[ownersPerCell[j][i]][kk]][1] > biggestY)
					{
						biggestY = thePoints[theFaces[ownersPerCell[j][i]][kk]][1];
						biggestYface = ownersPerCell[j][i];
					}

					if(thePoints[theFaces[ownersPerCell[j][i]][kk]][2] < smallestZ)
					{
						smallestZ = thePoints[theFaces[ownersPerCell[j][i]][kk]][2];
						smallestZface = ownersPerCell[j][i];
					}
					if(thePoints[theFaces[ownersPerCell[j][i]][kk]][2] < biggestZ)
					{
						biggestZ = thePoints[theFaces[ownersPerCell[j][i]][kk]][2];
						biggestZface = ownersPerCell[j][i];
					}
				}
			}

			for(double i = 0; i < neighborsPerCell[j].size(); i++)
			{
				for(double kk = 0; kk < 4; kk++)	// 4 because there are 4 points in a face
				{
					// there are 3 parts to a point
					if(thePoints[theFaces[neighborsPerCell[j][i]][kk]][0] < smallestX)
					{
						smallestX = thePoints[theFaces[neighborsPerCell[j][i]][kk]][0];
						smallestXface = neighborsPerCell[j][i];
					}
					if(thePoints[theFaces[neighborsPerCell[j][i]][kk]][0] > biggestX)
					{
						biggestX = thePoints[theFaces[neighborsPerCell[j][i]][kk]][0];
						biggestXface = neighborsPerCell[j][i];
					}

					if(thePoints[theFaces[neighborsPerCell[j][i]][kk]][1] < smallestY)
					{
						smallestY = thePoints[theFaces[neighborsPerCell[j][i]][kk]][1];
						smallestYface = neighborsPerCell[j][i];
					}
					if(thePoints[theFaces[neighborsPerCell[j][i]][kk]][1] > biggestY)
					{
						biggestY = thePoints[theFaces[neighborsPerCell[j][i]][kk]][1];
						biggestYface = neighborsPerCell[j][i];
					}

					if(thePoints[theFaces[neighborsPerCell[j][i]][kk]][2] < smallestZ)
					{
						smallestZ = thePoints[theFaces[neighborsPerCell[j][i]][kk]][2];
						smallestZface = neighborsPerCell[j][i];
					}
					if(thePoints[theFaces[neighborsPerCell[j][i]][kk]][2] < biggestZ)
					{
						biggestZ = thePoints[theFaces[neighborsPerCell[j][i]][kk]][2];
						biggestZface = neighborsPerCell[j][i];
					}
				}
			}

			theCells[j].set_minZfaceID(smallestZface);
			theCells[j].set_maxZfaceID(biggestZface);
			theCells[j].set_westFaceID(smallestYface);
			theCells[j].set_eastFaceID(biggestYface);
			theCells[j].set_southFaceID(smallestXface);
			theCells[j].set_northFaceID(biggestXface);
			std::cout << "theCells[j].minZfaceID = " << theCells[j].minZface() << "\n";
			std::cout << "theCells[j].maxZfaceID = " << theCells[j].maxZface() << "\n";
			std::cout << "theCells[j].westFaceID = " << theCells[j].westFace() << "\n";
			std::cout << "theCells[j].eastFaceID = " << theCells[j].eastFace() << "\n";
			std::cout << "theCells[j].southFaceID = " << theCells[j].southFace() << "\n";
			std::cout << "theCells[j].northFaceID = " << theCells[j].northFace() << "\n";
		}
	}
}

void sortedMesh::findFaceLayers(const faceList& theFaces, 
	const labelList& theOwners, double nInternalFaces)
{
	std::cout << "running findFaceLayers in sortedMesh class\n";
	// find what faces belong to each type of layer. Another function will sort the faces in a given layer
	
	std::vector<double> xFaceLayers_unsorted;
	std::vector<double> yFaceLayers_unsorted;
	std::vector<double> zFaceLayers_unsorted;
	
	// this will sort a nonrefined mesh if it happens to be the right order,
	// BUT it looks like the refined mesh somehow has more than the usual 3 faces owned!
	// even up to 19 faces owned by one cell!
	// maybe deal with hit by looking up the neighbor for each of those faces and comparing
	// some kind of point between the current face and it's neighbor to see which way it is
	// oriented! Maybe that is the trick for all given faces, look up the neighbor for a given
	// face and check to see how it is different
	// hm, maybe not, since the neighbor is not a neighbor face, but a neighbor cell
	// unless we can use the cellIndex to obtain the minZ face, maxZ face, west east faces.
	
	// what if we compare the minZ of each cellIndex? If the cells share two minZ points
	// between owner and neighbor, then they are of the same zlayer. If the owner and neighbor
	// cell share two west or east points, then they are of the same xlayer or something
	
	// so need to go through each owner, or cell, 
	// and figure out which face is it's minZ (or maxZ), which face is its west (or east)
	// and which face is its south (or north), then look at the neighbor for that desired face
	// and figure out which faces of the neighbor are the minZ (or maxZ), the west (or east),
	// and the south (or north). If the two faces match, then we know that the two cells are
	// of the same given layer. Heck all we need to know if which faces of the owner are which
	// then we know that the neighbor cell is in that given layer with it.
	
	// but how to store it? okay we are starting with a single cell, we can know based 
	// off of that given cell to what layer to add it and those next to it. So we can know
	// from 3 to 6 cells around it. So if we throw the indices into some vectors, then we
	// need to keep track of whether they have already been assigned an x, y, and z layer
	// since each cell will belong to at least one of the given layers.
	// this would require checking through the vector if it already exists in there.
	// a better implementation would be a class, called meshCells or something like that
	// where a mesh cell has a given mesh number, the cell Index used by openfoam, and
	// a given layer number for each type of layer (x,y,z layers).
	// 		Then the first cell can be given a layer number of 0 for each type of layer
	// it gives a value of 0 or -1 or 1 for the correct layers depending on the cell direction
	// then check the next cell, which should be one of the neighbors if possible (I think
	// (this will be the case because of the way the lists are organized, if just loop cellI)
	// , and use information for the already assigned layer numbers around it
	// to assign the new layer numbers. At the end, renumber all layer numbers to start from 0.
	
	// going to be tricky dealing with the refined mesh, since the owners are screwed up
	// in theory it still works as long as I just use cell indices, but it has to go from one
	// cell to the next. Can't jump cell positions or this won't work at all
	
	// going to be easier to do this by simply using the owner and neighbor and other lists
	// using the mesh member functions tends to be only for cell centers or vectors,
	// not for points in corners
	
	
	
	double faceIndex = 0;
	double pastFaceIndex = 0;
	bool foundSingle = false;
	for(double j = 0; j < nInternalFaces-1; j++)
	{
		if(theOwners[j] == theOwners[j+1])
		{
			faceIndex++;
		} else
		{
			if(faceIndex == 2)
			{
				xFaceLayers_unsorted.push_back(j-2);
				yFaceLayers_unsorted.push_back(j-1);
				zFaceLayers_unsorted.push_back(j);
			} else if(faceIndex == 1)
			{
				if(pastFaceIndex == 2)
				{
					yFaceLayers_unsorted.push_back(j-1);
					zFaceLayers_unsorted.push_back(j);
				} else if(pastFaceIndex == 1)
				{
					if(foundSingle == false)
					{
						xFaceLayers_unsorted.push_back(j-1);
						zFaceLayers_unsorted.push_back(j);
					} else
					{
						xFaceLayers_unsorted.push_back(j-1);
						yFaceLayers_unsorted.push_back(j);
					}
				} else if(pastFaceIndex == 0)
				{
					xFaceLayers_unsorted.push_back(j-1);
					yFaceLayers_unsorted.push_back(j);
				}
			} else if(faceIndex == 0)
			{
				if(pastFaceIndex == 1)
				{
					if(foundSingle == false)
					{
						foundSingle = true;
						zFaceLayers_unsorted.push_back(j);
					} else
					{
						yFaceLayers_unsorted.push_back(j);
					}
				} else if(pastFaceIndex == 0)
				{
					xFaceLayers_unsorted.push_back(j);
					xFaceLayers_unsorted.push_back(j+1);
				}
			}
			pastFaceIndex = faceIndex;
			faceIndex = 0;
		}
	}
	
	std::cout << "finished separating out x, y, and z layers\n";
	std::cout << "xFaceLayers_unsorted.size() = " << xFaceLayers_unsorted.size() << "\n";
	std::cout << "yFaceLayers_unsorted.size() = " << yFaceLayers_unsorted.size() << "\n";
	std::cout << "zFaceLayers_unsorted.size() = " << zFaceLayers_unsorted.size() << "\n";


	std::cout << "xFaceLayers_unsorted:\n";
	for(double j = 0; j < xFaceLayers_unsorted.size(); j++)
	{
		std::cout << " (" << theFaces[xFaceLayers_unsorted[j]][0] << "," 
			<< theFaces[xFaceLayers_unsorted[j]][1] << ","
			<< theFaces[xFaceLayers_unsorted[j]][2] << ","
			<< theFaces[xFaceLayers_unsorted[j]][3] << ")";
	}
	std::cout << "\n";
	
	std::cout << "yFaceLayers_unsorted:\n";
	for(double j = 0; j < yFaceLayers_unsorted.size(); j++)
	{
		std::cout << " (" << theFaces[yFaceLayers_unsorted[j]][0] << ","
			<< theFaces[yFaceLayers_unsorted[j]][1] << ","
			<< theFaces[yFaceLayers_unsorted[j]][2] << ","
			<< theFaces[yFaceLayers_unsorted[j]][3] << ")";
	}
	std::cout << "\n";
	
	std::cout << "zFaceLayers_unsorted:\n";
	for(double j = 0; j < zFaceLayers_unsorted.size(); j++)
	{
		std::cout << " (" << theFaces[zFaceLayers_unsorted[j]][0] << ","
			<< theFaces[zFaceLayers_unsorted[j]][1] << ","
			<< theFaces[zFaceLayers_unsorted[j]][2] << ","
			<< theFaces[zFaceLayers_unsorted[j]][3] << ")";
	}
	std::cout << "\n";
	
}
