//
//  DKDrawableShape+Utilities.h
//  GCDrawKit
//
//  Created by graham on 13/06/09.
//  Copyright 2009 Apptree.net. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "DKDrawableShape.h"

NS_ASSUME_NONNULL_BEGIN

//! utilities for calculating regions within a shape and drawing images allowing
//! for scale, rotation, etc.
@interface DKDrawableShape (Utilities)

- (NSBezierPath*)			pathWithRelativeRect:(NSRect) relRect;
- (NSBezierPath*)			pathWithRelativePosition:(NSPoint) relLoc finalSize:(NSSize) size;
- (nullable NSBezierPath*)	pathWithFinalSize:(NSSize) size offsetBy:(NSPoint) offset fromPartcode:(NSInteger) pc;
- (nullable NSBezierPath*)	path:(NSBezierPath*) inPath withFinalSize:(NSSize) size offsetBy:(NSPoint) offset fromPartcode:(NSInteger) pc;

- (NSPoint)					pointForRelativeLocation:(NSPoint) relLoc;


@end

NS_ASSUME_NONNULL_END
