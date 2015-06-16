#include "EnerDistMaxwellFisSpec.hh"

EnerDistMaxwellFisSpec::EnerDistMaxwellFisSpec()
{
    //ctor
}

EnerDistMaxwellFisSpec::~EnerDistMaxwellFisSpec()
{
    if(regEndPos)
        delete [] regEndPos;
    if(intScheme)
        delete [] intScheme;
    if(incEner)
        delete [] incEner;
    if(tValues)
        delete [] tValues;
}

void EnerDistMaxwellFisSpec::ExtractMCNPData(stringstream stream, int &count)
{
    int intTemp;
    double temp;

    stream >> numRegs; count++;
    regEndPos = new int[numRegs];
    intScheme = new int[numRegs];

    for(int i=0; i<numRegs; i++, count++)
    {
        stream >> intTemp;
        regEndPos[i]=intTemp;
    }
    for(int i=0; i<numRegs; i++, count++)
    {
        stream >> intTemp;
        intScheme[i]=intTemp;
    }

    stream >> numIncEner; count++;
    incEner = new double[numIncEner];
    tValues = new double[numIncEner];

    for(int i=0; i<numIncEner; i++, count++)
    {
        stream >> temp;
        incEner[i]=temp;
    }
    for(int i=0; i<numIncEner; i++, count++)
    {
        stream >> temp;
        tValues[i]=temp;
    }

    stream >> restrictEner; count++;
}

//For Fission
void EnerDistMaxwellFisSpec::WriteG4NDLData(stringstream data)
{
//this MCNP energy dist law 7
//Convert this to G4NDL theRepresentationType=7
//check the physics to make sure this is equivalent, appears to be the same except for the added coefficient in the MCNP version
// we ignore the collected restriction energy and assume it is taken care of in G4NDL

    stream << numIncEner << '\n';
    stream << numRegs << '\n';
    for(int i=0; i<numRegs; i++)
    {
        stream << regEndPos[i] << intScheme[i] << '\n';
    }

    for(int i=0; i<numIncEner; i++)
    {
        stream << incEner[i]*1000000 << tValues[i] << '\n';
    }
}
