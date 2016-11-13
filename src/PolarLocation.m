//
//  PolarLocation.m
//  Hertzpiral
//
//  Created by Mark Hertensteiner on 7/18/10.
//  Copyright 2010 Sphiral Productions. All rights reserved.
//

#import "PolarLocation.h"


@implementation PolarLocation

@synthesize radius, theta;

+(PolarLocation*) locationFromTheta: (float) thetaMake {
    PolarLocation* location = [[PolarLocation alloc] init];
    location.theta = thetaMake;
    while (location.theta > (2*pi)) location.theta -= (2*pi);
    while (location.theta < 0) location.theta += (2*pi);
    location.radius = pow(2,thetaMake/(2*pi));
    return location;
    
    
}

+(PolarLocation*) locationForCartesianPoint: (CGPoint) point {
//	float x=point.x - 512;
//    float y=-(point.y - 384);
    float x = point.x;
    float y = point.y;
 	PolarLocation* location = [[PolarLocation alloc] init];
	
	[location setRadius: sqrt(pow(x,2)+pow(y,2))];

	float theta;
	
	if (x < 0 && y >= 0) {
		theta=pi+atan(y/x);
	} 
	else {
		if (x<0 && y<0) {
			theta=pi+atan(y/x);
		} else {
			if (x>0 && y<0) {
				theta=(2*pi)+atan(y/x);
			} 
			else {
				theta=atan(y/x);
			}
		}
	}
	[location setTheta: (2*pi - theta)]; // theta + (pi/2)
//    [location setTheta: theta + (pi/2)];
    while (location.theta > (2*pi)) location.theta -= (2*pi);
    while (location.theta < 0) location.theta += (2*pi);
	return location;
}

-(CGPoint) cartesianFromPolar {
    CGPoint point;
    point.x = radius * cos(fmod(theta, (2 * pi)));
    point.y = radius * sin(fmod(theta, (2 * pi)));
//    point.y = point.y - 768;
 //   float tempTheta = -theta - 90;
    
    
    if ((theta < (pi/2) &&  theta > (-pi/2)) || (theta > (3 * pi / 2) && theta < (5 * pi / 2)))
        
//        (fmod(theta, (2 * pi)) < 0.25 || fmod(theta, (2 * pi)) > 0.75) 
        point.x = fabs( point.x);
    else
        point.x =  - fabs(point.x);
    
//    NSLog(@"Theta = %e", theta);
    if ((theta > 0.0 && theta <= pi) || (theta <= -pi && theta > (-2 * pi)) || (theta > (2*pi) && theta <= (3*pi)))
        //(fmod(theta, (2 * pi)) < 0.5)
        
        point.y = fabs(point.y);
    else
        point.y = - fabs(point.y);
    return point;
}

//+ (PolarLocation *) polarTransformPoint: (CGPoint) point1 plusPoint: (CGPoint) point2 {
//    PolarLocation * polar1 = [PolarLocation locationForCartesianPoint:point1];
//    PolarLocation * polar2 = [PolarLocation locationForCartesianPoint:point2];
//    polar2.radius -= polar1.radius;
//    polar2.theta -= polar1.theta;
//    return polar2;
//    
//}

@end
