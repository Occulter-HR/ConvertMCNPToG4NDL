#ifndef AngDist32EqualPBin_HH
#define AngDist32EqualPBin_HH

#include "AngularDist.hh"

/*
Created By: Wesley Ford June 17, 2015

This class is responsible for the extraction of the energy independant 32 equal probability bin out-going angular distribution data from MCNP
and the writing of this data into the applicable G4NDL files.

To better understand the MCNP format that this class is built to extract from please refer to MCNP5 Manual Vol III
To better understand the G4NDL format that this class is built to write to, please refer to G4NDL Final State Decryption
*/

class AngDist32EqualPBin: public AngularDist
{
    public:
        AngDist32EqualPBin();
        virtual ~AngDist32EqualPBin();
        void ExtractMCNPData(stringstream &stream, int &count);
        void WriteG4NDLData(stringstream &stream);
        void SetPoint(stringstream &stream, int &count, double incNEner)
        {
            incNEnerVec.push_back(incNEner);
            double temp;

                //the angular distribution is represented by a probability density function where each of the 33 points, 32 bins, are spaced along the entire cosine of the scattering angle axis
            //such that the integral of the probability density with the cosine of the scattering angle inbetween them is kept constant, meaning that the bins are in a equilprobable arrangement
            // the first bin probably corresponds to a cosine value of -1 and and the last bin being some where around 1 depending on the spacing

            angVec.push_back(new double [33]);
            for(int k=0; k<33; k++, count++)
            {
                stream >> temp;
                angVec.back()[k]=temp;
            }
        }

    vector<double*> angVec;
    protected:
    private:
};

#endif // AngDist32EqualPBin_HH