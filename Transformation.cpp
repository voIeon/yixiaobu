#include <cmath>

#include "Transformation.h"

#define WGS84_A     6378138.0
#define WGS84_B     6356752.314245

#define WGS84_f     1/298.257223565
#define WGS84_E2    0.0066943799900965025

#define DEGTORAD(a) (a)*0.0174532925
#define RADTODEG(a) (a)*57.295779513082
#define PI          3.1415926535898

//https://blog.51cto.com/fengyuzaitu/1892212
void Transformation::ECEFtoLL(CTVector3d& ecef, double& lat, double& lon)
{
    double      x, y, z;
    double      a, b, f, e, e1;
    double      Longitude;
    double      Latitude;
    double      Altitude;
    double      p, q;
    double      N;

    x = (double)ecef.x;
    y = (double)ecef.y;
    z = (double)ecef.z;

    a = (double)WGS84_A;
    b = (double)WGS84_B;
    f = (double)WGS84_f;

    e = sqrtl(((a * a) - (b * b)) / (a * a));
    e1 = sqrtl(((a * a) - (b * b)) / (b * b));

    p = sqrtl((x * x) + (y * y));
    q = atan2l((z * a), (p * b));

    Longitude = atan2l(y, x);
    Latitude = atan2l((z + (e1 * e1) * b * powl(sinl(q), 3)), (p - (e * e) * a * powl(cosl(q), 3)));

    N = a / sqrtl(1 - ((e * e) * powl(sinl(Latitude), 2)));

    Altitude = (p / cosl(Latitude)) - N;
    Longitude = Longitude * 180.0 / PI;
    Latitude = Latitude * 180.0 / PI;

    lat = (double)Latitude;
    lon = (double)Longitude;
    Altitude = (double)Altitude;
}

void Transformation::LLtoECEF(double lat, double lon, CTVector3d& ecef)
{
    // [1b] https://www.cnblogs.com/langzou/p/11388520.html
 /*   double alt = 0.0;
    double  N = WGS84_A / (sqrt(1 - WGS84_E2 * sin(lat) * sin(lat)));
    ecef.x = (N + alt) * cos(lat) * cos(lon);
    ecef.y = (N + alt) * cos(lat) * sin(lon);
    ecef.z = (N * (1 - WGS84_f) * (1 - WGS84_f) + alt) * sin(lat);*/
    // [1e]

    // [2b] https://blog.51cto.com/fengyuzaitu/1892212
    double a, b;
    a = (double)WGS84_A;
    b = (double)WGS84_B;
    double E = (a * a - b * b) / (a * a);;
    double COSLAT = cos(DEGTORAD(lat));
    double SINLAT = sin(DEGTORAD(lat));
    double COSLONG = cos(DEGTORAD(lon));
    double SINLONG = sin(DEGTORAD(lon));

    double N = a / (sqrt(1 - E * SINLAT * SINLAT));
    double NH = N + 0.0;//alt 0.0

    ecef.x = NH * COSLAT * COSLONG;
    ecef.y = NH * COSLAT * SINLONG;
    ecef.z = (b * b * N / (a * a) + 0.0) * SINLAT;//alt 0.0
    //[2e]
}
