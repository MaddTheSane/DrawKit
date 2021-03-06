///**********************************************************************************************************************************
///  DKObjectStorageProtocol.h
///  DrawKit ©2005-2008 Apptree.net
///
///  Created by graham on 03/01/2009.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
///
///**********************************************************************************************************************************

#import <Cocoa/Cocoa.h>

@protocol DKObjectStorage;


typedef NS_OPTIONS(NSUInteger, DKObjectStorageOptions)
{
	kDKReverseOrder				= ( 1 << 0 ),		//!< return objects in top to bottom order if set
	kDKIncludeInvisible			= ( 1 << 1 ),		//!< includes invisible objects
	kDKIgnoreUpdateRect			= ( 1 << 2 ),		//!< includes objects regardless of whether they are within the update region or not
	kDKZOrderMayBeRelaxed		= ( 1 << 3 )		//!< if set, the strict Z-ordering of objects may be relaxed if there is a performance benefit
};



@protocol DKStorableObject <NSObject, NSCoding, NSCopying>

- (id<DKObjectStorage>)		storage;
- (void)					setStorage:(id<DKObjectStorage>) storage;

@property (readwrite) NSUInteger index;

@property (readwrite, getter=isMarked) BOOL marked;

@property (readonly) BOOL visible;
@property (readonly) NSRect bounds;

@end



@protocol DKObjectStorage <NSObject>

// objects returned by these methods should be returned in bottom-to-top (drawing) Z-order unless the kDKZOrderMayBeRelaxed flag is set in which case
// the order can be arbitrary. Z-order and object index are synonymous

- (NSArray<id<DKStorableObject>>*)				objectsIntersectingRect:(NSRect) aRect inView:(NSView*) aView options:(DKObjectStorageOptions) options;
- (NSArray<id<DKStorableObject>>*)				objectsContainingPoint:(NSPoint) aPoint;
- (NSArray<id<DKStorableObject>>*)				objects;

// bulk load the storage e.g. when dearchiving

- (void)					setObjects:(NSArray<id<DKStorableObject>>*) objects;

// insertion and deletion is observable using KVO

@property (readonly) NSUInteger countOfObjects;
- (id<DKStorableObject>)	objectInObjectsAtIndex:(NSUInteger) indx;													// KVC/KVO compliant
- (NSArray<id<DKStorableObject>>*)				objectsAtIndexes:(NSIndexSet*) set;														// KVC/KVO compliant

- (void)					insertObject:(id<DKStorableObject>) obj inObjectsAtIndex:(NSUInteger) indx;				// KVC/KVO compliant
- (void)					removeObjectFromObjectsAtIndex:(NSUInteger) indx;											// KVC/KVO compliant
- (void)					replaceObjectInObjectsAtIndex:(NSUInteger) indx withObject:(id<DKStorableObject>) obj;	// KVC/KVO compliant
- (void)					insertObjects:(NSArray<id<DKStorableObject>>*) objs atIndexes:(NSIndexSet*) set;								// KVC/KVO compliant
- (void)					removeObjectsAtIndexes:(NSIndexSet*) set;												// KVC/KVO compliant

- (BOOL)					containsObject:(id<DKStorableObject>) object;
- (NSUInteger)				indexOfObject:(id<DKStorableObject>) object;
- (void)					moveObject:(id<DKStorableObject>) obj toIndex:(NSUInteger) indx;

// methods that may be used by spatially sensitive storage algorithms

- (void)					object:(id<DKStorableObject>) obj didChangeBoundsFrom:(NSRect) oldBounds;
- (void)					objectDidChangeVisibility:(id<DKStorableObject>) obj;
- (void)					setCanvasSize:(NSSize) size;

@optional
- (NSBezierPath*)			debugStorageDivisions;

@end



/*

This protocol is used by DKObjectStorage classes to implement a common object storage schema. The purpose is to allow object storage to swapped for more efficient
 algorithms tuned to end-user applications. Examples include simple linear storage (the default) and R-Tree storage, etc.

The storage object is required to own any number of objects and return them on demand based on point and rect-based queries. Such queries include drawing objects in
 a given update region, searching for objects in a given search area, and hit-testing objects for selective purposes. Objects also have a defined Z-order in the overall
 scene graph and this order must be maintained. For certain special purposes, the Z-order requirement can be relaxed which may lead to enhanced performance with some
 storage algorithms.
 
 DKObjectOwnerLayer owns a DKObjectStorage object and allows it to be replaced as needed.


*/


