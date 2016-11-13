//
//  PolarLocation.h
//  Hertzpiral
//
//  Created by Mark Hertensteiner on 7/18/10.
//  Copyright 2010 Sphiral Productions. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <math.h>

#define pi M_PI

@interface PolarLocation : NSObject {
	float radius;
	float theta;
}

@property (nonatomic) float radius, theta;

+(PolarLocation*) locationFromTheta: (float) thetaMake;

+(PolarLocation*) locationForCartesianPoint: (CGPoint) point;

-(CGPoint) cartesianFromPolar;


@end
