#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;
#define pi 3.14159265358979323846
#define earthRadiusKm 6371000



class CCoord
 {
   public:
       CCoord ( double lat, double lon ) {this->lat=lat; this->lon=lon;this->data=conversion (lat,lon);}
       CCoord ( string entry ) { conversion(entry,lat,lon); this->data=conversion(lat,lon);}
       double lat,lon,deg1,min1,sec1,deg2,min2,seg2;
       string data;
       string conversion (double lat1,double lon1)
       {
          double lat=abs(lat1);
          double lon=abs(lon1);
          string n1,n2;
          if (lat1<0) n1="S"; else n1="N";
          if (lon1<0) n2="W"; else n2="E";
          double deg1=floor(lat);
          double minut1=floor((lat-deg1)*60);
          double sec1=floor(((lat-deg1)*60-minut1)*60*1000)/1000;
          double deg2=floor(lon);
          double minut2=floor((lon-deg2)*60);
          double sec2=floor(((lon-deg2)*60-minut2)*60*1000)/1000;
         stringstream s;
         s << "(" << deg1<< " " << minut1 << "'"<<sec1 <<'"'<< n1<<", "<< deg2<< " " << minut2 << "'"<<sec2 <<'"'<< n2<<")";
         return s.str();
       }
       double stringtodouble(string str)
       {
        std::istringstream stm;
        stm.str(str);
        double d;
        stm >>d;
        return d;
       }

       void conversion (string entry,double &lat1,double &lon1)
       {
          unsigned int i=1;
          string lat2="";
          double polat=-1;
          while (i<entry.length() &&((entry[i]<='9' && entry[i]>='0') || entry[i]=='.'))
            {
              lat2+=entry[i];
              i++;
            }
          while (i<entry.length() && (entry[i]!='S' && entry[i]!='N'))
            i++;
          if (entry[i]=='N') polat=1;
          while (i<entry.length() && !(entry[i]>='0' && entry[i]<='9'))
            i++;
          string lon2="";
          double polon=-1;
          while (i<entry.length() &&((entry[i]<='9' && entry[i]>='0') || entry[i]=='.'))
            {
              lon2+=entry[i];
              i++;
            }
          while (i<entry.length() && entry[i]!='E' && entry[i]!='W')
            i++;
          if (entry[i]=='E') polon=1; 
          lat1=(stringtodouble(lat2))*polat;
          lon1=(stringtodouble(lon2))*polon;
       }

    // output op <<
    friend ostream &operator<<( ostream &output, 
                                       const CCoord &t )
      { 
        output<<t.data;
         return output;            
      }
    // todo
 };


class CGPS
 {
   public:
    // default constructor
    CGPS ()
    {

    }
    // destructor
    ~CGPS()
    {
      
    }
    // Add ()
    CGPS & Add (CCoord entry)
    {
      table.push_back(entry);
      return *this;
    }

// This function converts decimal degrees to radians
double deg2rad(double deg) {
  return (deg * pi / 180);
};

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return (rad * 180 / pi);
};
    
    double distance(double lat1, double lon1, double lat2, double lon2) {
    double theta, dist;
    theta = lon1 - lon2;
    dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
    dist = acos(dist);
    dist = rad2deg(dist);
    dist = dist * 60 * 1.1515;

    dist *= 1.609344*1000;
    return (dist);
   }
    // Distance () 
   double Distance()
    {
      double d=0;
      for (unsigned int i=0;i<table.size()-1;i++)
      {
        //cout<<"i="<<i<<endl;
        //for(unsigned int j=i+1;j<table.size();j++)
        //{
        unsigned int j=i+1;
          double lat1=table[i].lat;
          double lon1=table[i].lon;
          double lat2=table[j].lat;
          double lon2=table[j].lon;
          d+= distance(lat1,lon1,lat2,lon2);
          //cout<<"distance" <<j <<" "<< i <<"est " <<distance(lat1,lon1,lat2,lon2)<<endl;
          
        //}
      }
        
        return d;
    }



   // op += 
    double stringtodouble(string str)
       {
        std::istringstream stm;
        stm.str(str);
        double d;
        stm >>d;
        return d;
       }
    void sequence(string entry,double &a, double &b)
    {
      string deg1="";
      unsigned int i=1;
      while (i<entry.length() && entry[i]<='9' && entry[i]>='0')
      {
        deg1+=entry[i];
        i++;
      }
      string minut1="";
      while (i<entry.length() && entry[i]==' ')
      {
        i++;
      }
      while (i<entry.length() && entry[i]<='9' && entry[i]>='0')
      {
        minut1+=entry[i];
        i++;
      }
      string sec1="";
      while (i<entry.length() && !(entry[i]<='9' && entry[i]>='0'))
      {
        i++;
      }
      while (i<entry.length() && ((entry[i]<='9' && entry[i]>='0')|| entry[i]=='.') )
      {
        sec1+=entry[i];
        i++;
      }
      while(i<entry.length() && entry[i]!=',')
        i++;
      int pos;
      if (entry[i-1]=='N') pos=1;
      else pos=-1;
      a=pos *(stringtodouble(deg1)+stringtodouble(minut1) /60 +stringtodouble(sec1)/60/60);
      i++;
      i++;
      string deg2="";
            while (i<entry.length() && entry[i]<='9' && entry[i]>='0')
      {
        deg2+=entry[i];
        i++;

      }
      string minut2="";
      while (i<entry.length() && entry[i]==' ')
      {
        i++;
      }
      while (i<entry.length() && entry[i]<='9' && entry[i]>='0')
      {
        minut2+=entry[i];
        i++;
      }
      string sec2="";
      while (i<entry.length() && !(entry[i]<='9' && entry[i]>='0'))
      {
        i++;
      }
      while (i<entry.length() && ((entry[i]<='9' && entry[i]>='0')|| entry[i]=='.') )
      {
        sec2+=entry[i];
        i++;
      }
      while(i<entry.length() && entry[i]!=',')
        i++;
      int pos2=1;
      if (entry[i-1]=='E') pos=1;
      else pos=-1;
      b=pos2 *(stringtodouble(deg2)+stringtodouble(minut2) /60 +stringtodouble(sec2)/60/60);
    }
    bool checksyntaxmin(string entry,double &a, double &b)
    {
      string deg1="";
      unsigned int i=1;
      while (i<entry.length() && entry[i]<='9' && entry[i]>='0')
      {
        deg1+=entry[i];
        i++;
      }
      string minut1="";
      while (i<entry.length() && entry[i]==' ')
      {
        i++;
      }
      while (i<entry.length() && entry[i]<='9' && entry[i]>='0')
      {
        minut1+=entry[i];
        i++;
      }
      string sec1="";
      while (i<entry.length() && !(entry[i]<='9' && entry[i]>='0'))
      {
        i++;
      }
      while (i<entry.length() && ((entry[i]<='9' && entry[i]>='0')|| entry[i]=='.') )
      {
        sec1+=entry[i];
        i++;
      }
      while(i<entry.length() && entry[i]!=',')
        i++;
      int pos;
      if (entry[i-1]=='N') pos=1;
      else pos=-1;
      a=pos *(stringtodouble(deg1)+stringtodouble(minut1) /60 +stringtodouble(sec1)/60/60);
      i++;
      i++;
      string deg2="";
            while (i<entry.length() && entry[i]<='9' && entry[i]>='0')
      {
        deg2+=entry[i];
        i++;

      }
      string minut2="";
      while (i<entry.length() && entry[i]==' ')
      {
        i++;
      }
      while (i<entry.length() && entry[i]<='9' && entry[i]>='0')
      {
        minut2+=entry[i];
        i++;
      }
      string sec2="";
      while (i<entry.length() && !(entry[i]<='9' && entry[i]>='0'))
      {
        i++;
      }
      while (i<entry.length() && ((entry[i]<='9' && entry[i]>='0')|| entry[i]=='.') )
      {
        sec2+=entry[i];
        i++;
      }
      while(i<entry.length() && entry[i]!=',')
        i++;
      int pos2=1;
      if (entry[i-1]=='E') pos=1;
      else pos=-1;
      b=pos2 *(stringtodouble(deg2)+stringtodouble(minut2) /60 +stringtodouble(sec2)/60/60);
      if (a>89 || b>179) return false;
          else return true;
    }
    void operator+=(string entry)
    {
      bool deci=false;
      for (unsigned i=0;i<entry.length() && !deci;i++)
      {
        if (entry[i]=='"')
          deci=true;
      }
      if (!deci)
      {
        table.push_back(CCoord(entry));
      }
      else
      {
        double a,b;
        sequence(entry,a,b);
        table.push_back(CCoord(a,b));
      }
    }
    // op []
    CCoord operator[](int i)
    {
      //if (i>table.size || i<0)  cerr <<"myDataFormatException";
      return table[i];
    }
    // op ()
    CGPS operator()(int a,int b)
    {
      CGPS x0;
      for (int i=a;i<=b;i++)
        x0.table.push_back(table[i]);
      return x0;
    }

    // op <<
    friend ostream &operator<<( ostream &output, 
                                       const CGPS &t )
      { 
        output<<"[";
         for (unsigned int i=0;i<t.table.size();i++ ) 
         {
          output << t.table[i].data;
          if (i+1<t.table.size())
            output <<" < ";
         }
        //output << t.table[t.table.size()-1].data;
        output <<']'<<endl;  
         return output;            
      }
    // op >>

    bool checksyntaxdec(string entry,double &lat1, double &lon1)
    {
          unsigned int i=1;
          string lat2="";
          double polat=-1;
          while (i<entry.length() &&((entry[i]<='9' && entry[i]>='0') || entry[i]=='.'))
            {
              lat2+=entry[i];
              i++;
            }
          while (i<entry.length() && (entry[i]!='S' && entry[i]!='N'))
            i++;
          if (entry[i]=='N') polat=1;
          while (i<entry.length() && !(entry[i]>='0' && entry[i]<='9'))
            i++;
          string lon2="";
          double polon=-1;
          while (i<entry.length() &&((entry[i]<='9' && entry[i]>='0') || entry[i]=='.'))
            {
              lon2+=entry[i];
              i++;
            }
          while (i<entry.length() && entry[i]!='E' && entry[i]!='W')
            i++;
          if (entry[i]=='E') polon=1;
          lat1=(stringtodouble(lat2))*polat;
          lon1=(stringtodouble(lon2))*polon;

          if (lat1>89 || lon1>179) return false;
          else return true;
    }

    bool deci(string entry)
    {
      unsigned int i=0;
      while (i<entry.length() && entry[i]!='"')
        i++;
      if (i>=entry.length()-1)
        return true;
      return false;
    }
    bool testeur (string entry, vector<CCoord> &v)
    {
      double a,b;
      string str="";
      bool cond=true;
      for (unsigned int i=0;i<entry.length();i++)
      {
        if (entry[i]=='(')
          str="";
        if (entry[i]==')')
        {
          str+=')';
          cout<<str<<endl;
          if (deci(str))
          {
            bool test=checksyntaxdec(str,a,b);
            cout<<str<<endl;
            cout<<a<<endl;
            if (test)
              v.push_back(CCoord(a,b));
            else
              cond=false;
          }
          else
          {
            bool test2=checksyntaxmin(str,a,b);
            if (test2)
              v.push_back(CCoord(a,b));
            else
              cond=false;
          }
        }
        else
        {
          str+=entry[i];
        }
      }
      return cond;
    }
    std::istream& operator >>(std::istream &is, Tablepoint &t)
    {
    //
    return is;
    }

    vector<CCoord> table;
 };



int main()
{
double a,b;
CGPS x0;
string test="[(50 6\'16.5\"N, 14 23\'20.25\"E)]";
vector<CCoord> v;
bool autre=x0.testeur(test,v);
cout<<v[0]<<endl;

return 0;

}