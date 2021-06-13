double getdistance(double lat_1, double long_1, double lat_2, double long_2)
{
    double R = 6373.0;
    double distance = 0;
    double d_lat = (lat_2 - lat_1) * M_PI / 180.0;
    double d_long = (long_2 - long_1) * M_PI / 180.0;
    double a = pow(sin(d_lat / 2), 2) + cos(lat_1 * M_PI / 180.0) * cos(lat_2 * M_PI / 180.0) * pow(sin(d_long / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    distance = R * c * 1000;
    // for validation:
    // printf("%.10f\n", a);
    // printf("%.10f\n", c);
    // printf("%.10f\n", distance);

    return distance;
}
