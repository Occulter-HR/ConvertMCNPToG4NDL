#ifndef CSDIST_HH
#define CSDIST_HH

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include "zlib.h"
#include <dirent.h>
#include "include/ElementNames.hh"
#include <iomanip>

class CSDist
{
    public:
        CSDist();
        virtual ~CSDist();
        virtual void ExtractMCNPData(stringstream data)=0;
        virtual void WriteG4NDLYieldData(stringstream data)=0;
        virtual void WriteG4NDLCSData(stringstream data)=0;
        virtual void SetCSData(CSDist* nCSData)=0;
        virtual double* GetEnergyVec()=0;
        virtual double* GetCSVec()=0;
        virtual int GetCSSize()=0;
        virtual string IdentifyYourSelf()=0;
        virtual double Interpolate(double x)=0;

        double Interpolate(int aScheme, double x, double x1, double x2, double y1, double y2) const;
        double Histogram(double , double , double , double y1, double ) const;
        double LinearLinear(double x, double x1, double x2, double y1, double y2) const;
        double LinearLogarithmic(double x, double x1, double x2, double y1, double y2) const;
        double LogarithmicLinear(double x, double x1, double x2, double y1, double y2) const;
        double LogarithmicLogarithmic(double x, double x1, double x2, double y1, double y2) const;
        double Random(double , double , double , double y1, double y2) const;

    protected:
    private:
};

#endif // CSDIST_HH
