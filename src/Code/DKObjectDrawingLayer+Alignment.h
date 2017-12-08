///**********************************************************************************************************************************
///  DKObjectDrawingLayer+Alignment.h
///  DrawKit ©2005-2008 Apptree.net
///
///  Created by graham on 18/09/2006.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
///
///**********************************************************************************************************************************

#import "DKObjectDrawingLayer.h"

NS_ASSUME_NONNULL_BEGIN

@class DKGridLayer;

typedef NS_ENUM(NSInteger, DKAlignment) {
	kDKAlignmentLeftEdge				= 0,
	kDKAlignmentTopEdge					= 1,
	kDKAlignmentRightEdge				= 2,
	kDKAlignmentBottomEdge				= 3,
	kDKAlignmentVerticalCentre			= 4,
	kDKAlignmentHorizontalCentre		= 5,
	kDKAlignmentVerticalDistribution	= 6,
	kDKAlignmentHorizontalDistribution  = 7,
	kDKAlignmentVSpaceDistribution		= 8,
	kDKAlignmentHSpaceDistribution		= 9,
};

typedef NS_OPTIONS(NSUInteger, DKAlignmentAlign)
{
	kDKAlignmentAlignLeftEdge			= ( 1 << kDKAlignmentLeftEdge ),
	kDKAlignmentAlignTopEdge			= ( 1 << kDKAlignmentTopEdge ),
	kDKAlignmentAlignRightEdge			= ( 1 << kDKAlignmentRightEdge ),
	kDKAlignmentAlignBottomEdge			= ( 1 << kDKAlignmentBottomEdge ),
	kDKAlignmentAlignVerticalCentre		= ( 1 << kDKAlignmentVerticalCentre ),
	kDKAlignmentAlignHorizontalCentre	= ( 1 << kDKAlignmentHorizontalCentre ),
	kDKAlignmentAlignVDistribution		= ( 1 << kDKAlignmentVerticalDistribution ),
	kDKAlignmentAlignHDistribution		= ( 1 << kDKAlignmentHorizontalDistribution ),
	kDKAlignmentAlignVSpaceDistribution = ( 1 << kDKAlignmentVSpaceDistribution ),
	kDKAlignmentAlignHSpaceDistribution = ( 1 << kDKAlignmentHSpaceDistribution ),
	
	kDKAlignmentAlignNone				= 0,
	kDKAlignmentAlignColocate			= kDKAlignmentAlignVerticalCentre | kDKAlignmentAlignHorizontalCentre,
	kDKAlignmentAlignHorizontalMask		= kDKAlignmentAlignLeftEdge | kDKAlignmentAlignRightEdge | kDKAlignmentAlignHorizontalCentre | kDKAlignmentAlignHDistribution | kDKAlignmentAlignHSpaceDistribution,
	kDKAlignmentAlignVerticalMask		= kDKAlignmentAlignTopEdge | kDKAlignmentAlignBottomEdge | kDKAlignmentAlignVerticalCentre | kDKAlignmentAlignVDistribution | kDKAlignmentAlignVSpaceDistribution,
	kDKAlignmentAlignDistributionMask	= kDKAlignmentAlignVDistribution | kDKAlignmentAlignHDistribution | kDKAlignmentAlignVSpaceDistribution | kDKAlignmentAlignHSpaceDistribution
};

/*!
 
 This category implements object alignment features for \c DKObjectDrawingLayer
 
 */
@interface DKObjectDrawingLayer (Alignment)

//! setting the key object (used by alignment methods)
@property (assign, nullable) DKDrawableObject *keyObject;


- (void)		alignObjects:(NSArray<DKDrawableObject*>*) objects withAlignment:(DKAlignmentAlign) align;
- (void)		alignObjects:(NSArray<DKDrawableObject*>*) objects toMasterObject:(nullable id) object withAlignment:(DKAlignmentAlign) align;
- (void)		alignObjects:(NSArray<DKDrawableObject*>*) objects toLocation:(NSPoint) loc withAlignment:(DKAlignmentAlign) align;

- (void)		alignObjectEdges:(NSArray<DKDrawableObject*>*) objects toGrid:(DKGridLayer*) grid;
- (void)		alignObjectLocation:(NSArray<DKDrawableObject*>*) objects toGrid:(DKGridLayer*) grid;

- (CGFloat)		totalVerticalSpace:(NSArray<DKDrawableObject*>*) objects;
- (CGFloat)		totalHorizontalSpace:(NSArray<DKDrawableObject*>*) objects;

- (NSArray<DKDrawableObject*>*)	objectsSortedByVerticalPosition:(NSArray<DKDrawableObject*>*) objects;
- (NSArray<DKDrawableObject*>*)	objectsSortedByHorizontalPosition:(NSArray<DKDrawableObject*>*) objects;

- (BOOL)		distributeObjects:(NSArray<DKDrawableObject*>*) objects withAlignment:(DKAlignmentAlign) align;

- (NSUInteger)	alignmentMenuItemRequiredObjects:(id<NSValidatedUserInterfaceItem>) item;

// user actions:

- (IBAction)	alignLeftEdges:(nullable id) sender;
- (IBAction)	alignRightEdges:(nullable id) sender;
- (IBAction)	alignHorizontalCentres:(nullable id) sender;

- (IBAction)	alignTopEdges:(nullable id) sender;
- (IBAction)	alignBottomEdges:(nullable id) sender;
- (IBAction)	alignVerticalCentres:(nullable id) sender;

- (IBAction)	distributeVerticalCentres:(nullable id) sender;
- (IBAction)	distributeVerticalSpace:(nullable id) sender;

- (IBAction)	distributeHorizontalCentres:(nullable id) sender;
- (IBAction)	distributeHorizontalSpace:(nullable id) sender;

- (IBAction)	alignEdgesToGrid:(nullable id) sender;
- (IBAction)	alignLocationToGrid:(nullable id) sender;

- (IBAction)	assignKeyObject:(nullable id) sender;

@end

//! alignment helper function:
NSPoint		calculateAlignmentOffset( NSRect mr, NSRect sr, DKAlignmentAlign alignment );

static const DKAlignmentAlign kDKAlignmentDistributionMask API_DEPRECATED_WITH_REPLACEMENT("kDKAlignmentAlignDistributionMask", macosx(10.0, 10.9)) = kDKAlignmentAlignDistributionMask;
static const DKAlignmentAlign kDKAlignmentHorizontalAlignMask API_DEPRECATED_WITH_REPLACEMENT("kDKAlignmentAlignHorizontalMask", macosx(10.0, 10.9)) = kDKAlignmentAlignHorizontalMask;
static const DKAlignmentAlign kDKAlignmentVerticalAlignMask API_DEPRECATED_WITH_REPLACEMENT("kDKAlignmentAlignVerticalMask", macosx(10.0, 10.9)) = kDKAlignmentAlignVerticalMask;

NS_ASSUME_NONNULL_END
