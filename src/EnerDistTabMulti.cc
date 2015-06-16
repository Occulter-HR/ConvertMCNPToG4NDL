#include "EnerDistTabMulti.hh"

EnerDistTabMulti::EnerDistTabMulti()
{
    //ctor
}

EnerDistTabMulti::~EnerDistTabMulti()
{
    if(regEndPos)
        delete [] regEndPos;
    if(intScheme)
        delete [] intScheme;
    if(incEner)
        delete [] incEner;

    for(int i=0; i<numIncEner; i++, count++)
    {
        if(tValues[i])
            delete [] tValues[i];
    }
    if(tValues)
        delete [] tValues;
}

void EnerDistTabMulti::ExtractMCNPData(stringstream stream, int &count)
{
    int numRegs, numIncEner, numOutEnerPerIn;
        int *regEndPos, *intScheme;
        double *incEner, **tValues

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
    tValues = new double *[numIncEner];

    for(int i=0; i<numIncEner; i++, count++)
    {
        stream >> temp;
        incEner[i]=temp;
    }

    stream >> numOutEnerPerIn; count++;
    for(int i=0; i<numIncEner; i++)
    {
        tValues[i] = new double [numOutEnerPerIn];
        for(int j=0; j<numOutEnerPerIn; j++, count++)
        {
            stream >> temp;
            tValues[i][j]=temp;
        }
    }
}

//For Fission
void EnerDistTabMulti::WriteG4NDLData(stringstream data)
{
//this is MCNP law 24
//there is no direct translation for this law in G4NDL but it can be made to fit theRepresentationType=1 with some assumptions
//use the average outgoing energy from each linear function (T*Ein) and assign them equal probability
//or to be more precise, create a fine grid of out-going energies, and a corresponding probability vector, then input the incoming energies boundaries
//for which the linear functions are valid, into each linear function, and add the probability of the lines to each of the out going probability elements whose
//coresponding out-going energy falls with the range of the linear function.
//then normalize the created out-going energy probability distribution

    double *outEner, *outProbDist, low, high, emax=-1, emin=-1, tempLow, tempHigh, sum=0.;

    //we ignore the given interpolation scheme since MCNP ignores it and uses a histogram scheme instead
    stream << std::setw(14) << std::right << numIncEner << std::setw(14) << std::right << 1 << '\n'
    stream << std::setw(14) << std::right << numIncEner;
    stream << std::setw(14) << std::right << 1 << '\n';

    for(int i=0; i<numIncEner; i++, count++)
    {
        stream << std::setw(14) << std::right << incEner[i]*1000000;
        stream << std::setw(14) << std::right << 2*numOutEnerPerIn;
        // assume linear interpolation
        stream << std::setw(14) << std::right << 1 << '\n';
        stream << std::setw(14) << std::right << 2*numOutEnerPerIn << std::setw(14) << std::right << 2 << '\n';

        outEner=new double [2*numPEnerPoints];
        outProbDist=new double [2*numPEnerPoints];
        outEnerLow=new double [2*numPEnerPoints];
        outEnerHigh=new double [2*numPEnerPoints];

        if(i==numIncEner-1)
        {
            low=incEner[i]-0.5*(incEner[i]-incEner[i-1]);
            high=incEner[i]+0.5*(incEner[i]-incEner[i-1]);
        }
        else
        {
            low=incEner[i]-0.5*(incEner[i+1]-incEner[i]);
            high=incEner[i]+0.5*(incEner[i+1]-incEner[i]);
        }

        for(int j=0; j<numOutEnerPerIn; j++)
        {
            tempLow=tValues[i][j]*(low);
            tempHigh=tValues[i][j]*(high)
            if((emin>tempLow)||(emin==-1))
            {
                emin=tempLow;
            }
            if(emax<tempHigh)
            {
                emax=tempHigh;
            }
        }

        for(int j=0; j<2*numOutEnerPerIn; j++)
        {
            outEner[j]=(emax-emin)*j/(2*numOutEnerPerIn)+(emax-emin)/(4*numOutEnerPerIn);
            outEnerLow[j]=(emax-emin)*j/(2*numOutEnerPerIn);
            outEnerHigh[j]=(emax-emin)*(j+1)/(2*numOutEnerPerIn);
            outProbDist[j]=0;
        }

        for(int j=0; j<numOutEnerPerIn; j++)
        {
            tempLow=tValues[i][j]*(low);
            tempHigh=tValues[i][j]*(high)
            for(int k=0; k<2*numOutEnerPerIn; k++)
            {
                if((outEnerLow[k]<tempLow)&&(outEnerHigh[k]>tempLow))
                {
                    outProbDist[k]+=outProb[j];
                    sum+=outProb[j];
                }
                if((outEnerLow[k]<tempHigh)&&(outEnerHigh[k]>tempHigh))
                {
                    outProbDist[k]+=outProb[j];
                    sum+=outProb[j];
                }
                if((tempLow<outEnerLow[k])&&(tempHigh>outEnerLow[k]))
                {
                    outProbDist[k]+=outProb[j];
                    sum+=outProb[j];
                }
                if((tempLow<outEnerHigh[k])&&(tempHigh>outEnerHigh[k]))
                {
                    outProbDist[k]+=outProb[j];
                    sum+=outProb[j];
                }
            }
        }

        for(int k=0; k<2*numOutEnerPerIn; k++)
        {
            outProbDist[k]/=sum;
        }
        sum=0.

        for(int j=0; j<2*numOutEnerPerIn; j++)
        {
            stream << std::setw(14) << std::right << outEner[j]*1000000;
            stream << std::setw(14) << std::right << outProbDist[j] << '\n';
        }
        delete[] outEner;
        delete[] outProbDist;
        delete[] outEnerLow;
        delete[] outEnerHigh;
    }

}
