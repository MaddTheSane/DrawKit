///**********************************************************************************************************************************
///  DKObjectDrawingLayer+Duplication.h
///  DrawKit ©2005-2008 Apptree.net
///
///  Created by graham on 22/06/2007.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
///
///**********************************************************************************************************************************

#import "DKObjectDrawingLayer.h"

@class DKDrawableObject;

@interface DKObjectDrawingLayer (Duplication)

- (NSArray<DKDrawableObject*>*)polarDuplicate:(NSArray<DKDrawableObject*>*) objectsToDuplicate
				centre:(NSPoint) centre
				numberOfCopies:(NSInteger) nCopies
				incrementAngle:(CGFloat) incRadians
				rotateCopies:(BOOL) rotCopies;
				
- (NSArray<DKDrawableObject*>*)linearDuplicate:(NSArray<DKDrawableObject*>*) objectsToDuplicate
				offset:(NSSize) offset
				numberOfCopies:(NSInteger) nCopies;
				
- (NSArray<DKDrawableObject*>*)autoPolarDuplicate:(DKDrawableObject*) object
				centre:(NSPoint) centre;
				
- (NSArray<DKDrawableObject*>*)concentricDuplicate:(NSArray<DKDrawableObject*>*) objectsToDuplicate
				centre:(NSPoint) centre
				numberOfCopies:(NSInteger) nCopies
				insetBy:(CGFloat) inset;
				

@end



/*

Some handy methods for implementing various kinds of object duplications.





*/
