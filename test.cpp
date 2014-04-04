#ifndef __PROGTEST__
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
#endif /* __PROGTEST__ */


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
    CCoord operator[](unsigned int i1)
    {
      if (i1<0) throw string("error");
      unsigned int i=i1;
      if (i>table.size()-1 /*|| i<0*/)     throw string("error");
      return table[i];
    }
    // op ()
    CGPS operator()( int a1,int b1)
    {
      if (a1<0 || b1<0) throw string ("error");
      unsigned int a=a1; unsigned int b=b1;
      if (a>table.size()-1 /*|| a<0 || b<0 */|| b>table.size()-1)     throw string("error");
      CGPS x0;
      for (unsigned int i=a;i<=b;i++)
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
            //cout<<str<<endl;
            //cout<<a<<endl;
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

    std::string gulp(std::istream &in)
{
    std::string ret;
    char buffer[4096];
    while (in.read(buffer, sizeof(buffer)))
        ret.append(buffer, sizeof(buffer));
    ret.append(buffer, in.gcount());
    return ret;
}

    friend std::istream& operator>>(std::istream & is, CGPS & rhs)
    {
    string str=rhs.gulp(is);
    vector<CCoord> v;
    bool test= rhs.testeur(str, v);
    if (test)
    {
      CGPS nouveau;
      for (unsigned int i=0;i<v.size();i++)
        nouveau . Add (v[i]);
      rhs=nouveau;
    }
    else
    {
      is.setstate(std::ios::failbit);
    }
    return is;
    }





    vector<CCoord> table;

 };



int main()
{
double d;
CGPS x0;
cout << x0;
 // []

x0 += "(50 6\'16.5\"N, 14 23\'20.25\" E)";
cout << x0;
 // [(50 6'16.5"N, 14 23'20.25"E)]

x0 . Add ( CCoord ( "(50.1003781N, 14.3925125E)" ) ) . Add ( CCoord ( 50.084202, 14.423357 ) );
cout << x0;
 // [(50 6'16.5"N, 14 23'20.25"E) > (50 6'1.361"N, 14 23'33.045"E) > (50 5'3.127"N, 14 25'24.085"E)]

d = x0 . Distance ();
 // d = 3373.886483

x0 += "(50.0843122N, 14.417463 E)";
cout << x0;
 // [(50 6'16.5"N, 14 23'20.25"E) > (50 6'1.361"N, 14 23'33.045"E) > (50 5'3.127"N, 14 25'24.085"E) > (50 5'3.524"N, 14 25'2.867"E)]

cout << x0;
 // [(50 6'16.5"N, 14 23'20.25"E) > (50 6'1.361"N, 14 23'33.045"E) > (50 5'3.127"N, 14 25'24.085"E) > (50 5'3.524"N, 14 25'2.867"E)]

//d = x0 . Distance ();
 // d = 3794.598230


CCoord tmp0 = x0 [ 1 ];
cout << tmp0;
 // (50 6'1.361"N, 14 23'33.045"E)

CGPS tmp1 = x0 ( 1, 2 );
cout << tmp1;
 // [(50 6'1.361"N, 14 23'33.045"E) > (50 5'3.127"N, 14 25'24.085"E)]


CGPS x1;
istringstream tmp2 ( "[(50 6\'16.5\"N, 14 23\'20.25\" E) > (51.5N, 0.0E) > (33.9S, 151.2E) > (37.42190N, 122.08405W) > (0.00N, 50.000W)]" );
tmp2 >> x1;
 // tmp2 . fail () = false


cout << x1;
 // [(50 6'16.5"N, 14 23'20.25"E) > (51 30'0"N, 0 0'0"E) > (33 54'0"S, 151 12'0"E) > (37 25'18.84"N, 122 5'2.58"W) > (0 0'0"N, 50 0'0"W)]

cout << x1;
 // [(50 6'16.5"N, 14 23'20.25"E) > (51 30'0"N, 0 0'0"E) > (33 54'0"S, 151 12'0"E) > (37 25'18.84"N, 122 5'2.58"W) > (0 0'0"N, 50 0'0"W)]

d = x1 . Distance ();
 // d = 38401662.588282

CGPS tmp3 = x1 ( 1, 3 );
cout << tmp3;
 // [(51 30'0"N, 0 0'0"E) > (33 54'0"S, 151 12'0"E) > (37 25'18.84"N, 122 5'2.58"W)]

istringstream tmp4 ( "[(51.5N, 0.0E) > (33.9S, 151.2E) > (37.42190N, 122.08405W) > (100.00N, 50.000W)]" );
tmp4 >> x1;
 // tmp4 . fail () = true
cout << x1;
 // [(50 6'16.5"N, 14 23'20.25"E) > (51 30'0"N, 0 0'0"E) > (33 54'0"S, 151 12'0"E) > (37 25'18.84"N, 122 5'2.58"W) > (0 0'0"N, 50 0'0"W)]

CCoord tmp5 = x1 [ 1 ];
cout << tmp5;
 // (51 30'0"N, 0 0'0"E)


//CCoord tmp6 = x1 [ -1 ];
 // exception thrown

//CCoord tmp7 = x1 [ 5 ];
 // exception thrown

CGPS tmp8 = x1 ( 2, 4 );
cout << tmp8;
 // [(33 54'0"S, 151 12'0"E) > (37 25'18.84"N, 122 5'2.58"W) > (0 0'0"N, 50 0'0"W)]

//CGPS tmp9 = x1 ( 2, 5 );
 // exception thrown

//CGPS tmp10 = x1 ( 3, 1 );
 // exception thrown

istringstream tmp11 ( "[(51.5N, 0.0E) > (33.9S, 151.2E) > (37.42190N, 122.08405W) > (80.00N, 50.000W)]" );
tmp11 >> x1;
cout<<"---------------"<<endl;
 // tmp11 . fail () = false
cout << x1;
 // [(51 30'0"N, 0 0'0"E) > (33 54'0"S, 151 12'0"E) > (37 25'18.84"N, 122 5'2.58"W) > (80 0'0"N, 50 0'0"W)]

cout << x1;
 // [(51 30'0"N, 0 0'0"E) > (33 54'0"S, 151 12'0"E) > (37 25'18.84"N, 122 5'2.58"W) > (80 0'0"N, 50 0'0"W)]
return 0;

}