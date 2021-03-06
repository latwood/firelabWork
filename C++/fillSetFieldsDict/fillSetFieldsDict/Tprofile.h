#ifndef TPROFILE_H
#define TPROFILE_H

#include <vector>
#include <sstream>

#include "Tvalue.h"

class Tprofile
{
public:

    /* This class was derived with a need to create Temperature profiles.
     * It would be a set of Tvalues where the class returns the set of Tvalues by type of Tprofile desired
     *
     * Eventually this needs to be changed to simply be profile so that it can be used to generate
     * profiles of differing types. Basically the same thing, just are they profiles of Tvalues or profiles
     * of uValues or profiles of pValues? Will make the program easier to work with in the future
     *
     * could be volVectorField vs volScalarField as the two types of Tprofiles, and it has a type that it is given
     * so the type would be U, p, k, epsilon. volVectorField would be U, the others are scalarFields
     * could combine into one profile class, but start out separate to make it easy I guess
     *
     * Should probably get rid of the default values idea since all the values will get set by a single profile
     */

    Tprofile();
    size_t size();
    Tvalue& operator[] (const double index);

    void generateTprofile(std::string singleValue);
    void generateLinearTprofile(std::vector<std::string> givenValues);
    void generateLinearTprofile(double lowTvalue,double highTvalue,double cellsInProfile);
    void generateLogTprofile(std::vector<std::string> givenValues);
    void generateLogTprofile(double lowTvalue,double highTvalue,double cellsInProfile);
    void generateTfullCellsProfile(double rows, double cols,std::vector<std::string> valuesForEachOriginalCell);

private:

    void cleanTprofile();

    std::vector<Tvalue> theTprofile;

};

#endif // TPROFILE_H
