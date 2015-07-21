#include "../include/CSDistYieldComp.hh"

CSDistYieldComp::CSDistYieldComp()
{
    //ctor
}

CSDistYieldComp::~CSDistYieldComp()
{
    delete [] regEndPos;
    delete [] intScheme;
    delete [] enerVec;
    delete [] yieldVec;
}

void CSDistYieldComp::ExtractMCNPData(stringstream &stream, int &count)
{
    int intTemp;
    double temp;

    stream >> mtNum; count++;
    stream >> numRegs; count++;
    if(numRegs==0)
    {
        numRegs=1;
        regEndPos = new int[numRegs];
        intScheme = new int[numRegs];

        stream >> numIncEner; count++;
        regEndPos[0]=numIncEner;
        intScheme[0]=2;
    }
    else
    {
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
    }

    enerVec = new double[numIncEner];
    yieldVec = new double[numIncEner];

    for(int i=0; i<numIncEner; i++, count++)
    {
        stream >> temp;
        enerVec[i]=temp;
    }
    for(int i=0; i<numIncEner; i++, count++)
    {
        stream >> temp;
        yieldVec[i]=temp;
    }
}

//set up for Capture data
void CSDistYieldComp::WriteG4NDLCSData(stringstream &stream )
{
    double csNum;
    stream << std::setw(14) << std::right << numIncEner << std::setw(14) << std::right << numRegs << endl;

    int i,csIndex;
    for(i=0; i<numRegs; i++)
    {
        stream << regEndPos[i] << intScheme[i] << endl;
    }
    stream << '\n';
    stream.fill(' ');
    stream.precision(6);
    stream.setf(std::ios::scientific);

    for(int j=0; j<numIncEner; j++)
    {
        stream << std::setw(14) << std::right << enerVec[j]*1000000;
        for(i=csEnerStart-1; i<csSize+csEnerStart-1; i++)
        {
            if(enerCSVec[i]>enerVec[j])
            {
                break;
            }
        }
        i--;

        csIndex=i-csEnerStart+1;
        if(csIndex<0)
        {
            csIndex=0;
            i=csEnerStart-1;
        }

        csNum = (enerVec[i]-enerCSVec[i])*(csVec[csIndex+1]-csVec[csIndex])/(enerCSVec[i+1]-enerCSVec[i])+csVec[csIndex];
        stream << std::setw(14) << std::right << (yieldVec[j]*csNum);
        if(j%3==0)
            stream << '\n';
    }
}

void CSDistYieldComp::WriteG4NDLYieldData(stringstream &stream )
{
    stream << std::setw(14) << std::right << numIncEner << std::setw(14) << std::right << numRegs << endl;

    for(int i=0; i<numRegs; i++)
    {
        stream << regEndPos[i] << intScheme[i] << endl;
    }

    for(int i=0; i<numIncEner; i++)
    {
        stream << std::setw(14) << std::right << enerVec[i]*1000000;
        stream << std::setw(14) << std::right << yieldVec[i];
    }
}

string CSDistYieldComp::IdentifyYourSelf()
{
    return "CSDistYieldComp";
}

double CSDistYieldComp::Interpolate(double x)
{
    int i, low, reg=0, csIndex;
    double csNum1, csNum2;
    for(i=0; i<numIncEner-1; i++)
    {
        while((i>=regEndPos[reg])&&(numRegs-1>reg))
            reg++;
        if(enerVec[i]>x)
        {
            break;
        }
    }
    i--;
    if(i<0)
        i=0;

    for(low=csEnerStart-1; low<csSize+csEnerStart-2; low++)
    {
        if(enerCSVec[low]>enerVec[i])
        {
            break;
        }
    }

    low--;
    csIndex=low-csEnerStart+1;
    if(csIndex<0)
    {
        csIndex=0;
        i=csEnerStart-1;
    }

    if(csSize>1)
        csNum1 = max(0.,(enerVec[i]-enerCSVec[low])*(csVec[csIndex+1]-csVec[csIndex])/(enerCSVec[low+1]-enerCSVec[low])+csVec[csIndex]);
    else
        csNum1 = csVec[0];

    if(numIncEner>1)
    {
        for(low=csEnerStart-1; low<csSize+csEnerStart-2; low++)
        {
            if(enerCSVec[low]>enerVec[i+1])
            {
                break;
            }
        }
        low--;
        csIndex=low-csEnerStart+1;
        if(csIndex<0)
        {
            csIndex=0;
            i=csEnerStart-1;
        }

        if(csSize>1)
            csNum2 = max(0.,(enerVec[i+1]-enerCSVec[low])*(csVec[csIndex+1]-csVec[csIndex])/(enerCSVec[low+1]-enerCSVec[low])+csVec[csIndex]);
        else
            csNum2 = csVec[0];
    }
    else
    {
        csNum2 = csNum1;
    }

    if(numIncEner>1)
        return max(0.,CSDist::Interpolate(intScheme[reg], x, enerVec[i], enerVec[i+1], yieldVec[i]*csNum1, yieldVec[i+1]*csNum2));
    else
        return yieldVec[0]*csNum1;
}
