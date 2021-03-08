#include <cmath>

#include "Transformation.h"


//[0] https://blog.51cto.com/fengyuzaitu/1892212
void Transformation::ECEFtoLL(CTVector3d& ecef, double& lat, double& lon)
{
    double      x, y, z;
    double      a, b, f, e, e1;
    double      longitude;
    double      latitude;
    double      altitude;
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

    longitude = atan2l(y, x);
    latitude = atan2l((z + (e1 * e1) * b * powl(sinl(q), 3)), (p - (e * e) * a * powl(cosl(q), 3)));

    N = a / sqrtl(1 - ((e * e) * powl(sinl(latitude), 2)));

    altitude = (p / cosl(latitude)) - N;

    lon = RADTODEG(longitude);
    lat = RADTODEG(latitude);

 //   altitude = (double)altitude;
	//[0]
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
    double cosLat = cos(DEGTORAD(lat));
    double sinLat = sin(DEGTORAD(lat));
    double cosLon = cos(DEGTORAD(lon));
    double sinLon = sin(DEGTORAD(lon));

    double N = a / (sqrt(1 - E * sinLat * sinLat));
    double NH = N + 0.0;//alt 0.0

    ecef.x = NH * cosLat * cosLon;
    ecef.y = NH * cosLat * sinLon;
    ecef.z = (b * b * N / (a * a) + 0.0) * sinLat;//alt 0.0
    //[2e]
}

void Transformation::ECEFtoENURotateMatrix(CTVector3d& ecef, double (&Rot)[9])
{
    double lon, lat, alt;
    ECEFtoLL(ecef, lat, lon);
    double lonRad = DEGTORAD(lon);
    double latRad = DEGTORAD(lat);
    Rot[0] = -sin(lonRad);
    Rot[1] = cos(lonRad);
    Rot[2] = 0;
    Rot[3] = -sin(latRad) * cos(lonRad);
    Rot[4] = -sin(latRad) * sin(lonRad);
    Rot[5] = cos(latRad);
    Rot[6] = cos(latRad) * cos(lonRad);
    Rot[7] = cos(latRad) * sin(lonRad);
    Rot[8] = sin(latRad);
}

void Transformation::Rotate01(double (&Rot)[9],CTVector3d& ecef0,CTVector3d& ecef1)
{
	//因为只用到在xy上的平移，暂时不使用z计算 (...)
	ecef1.x = Rot[0] * ecef0.x + Rot[1] * ecef0.y ;//+ ...;
	ecef1.y = Rot[3] * ecef0.x + Rot[4] * ecef0.y ;
	ecef1.z = Rot[6] * ecef0.x + Rot[7] * ecef0.y ;
}


void Transformation::Rotate10(double (&Rot)[9],CTVector3d& ecef1,CTVector3d& ecef0)
{
	ecef0.x = Rot[0] * ecef1.x + Rot[3] * ecef1.y;
	ecef0.y = Rot[1] * ecef1.x + Rot[4] * ecef1.y;
	ecef0.z = Rot[2] * ecef1.x + Rot[5] * ecef1.y;
}
