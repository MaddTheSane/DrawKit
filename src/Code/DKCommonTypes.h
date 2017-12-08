/*
 *  DKCommonTypes.h
///  DrawKit ©2005-2008 Apptree.net
 *
 *  Created by graham on 11/03/2008.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
 *
 */

//! functional types, as passed to drawKnobAtPoint:ofType:userInfo:
//! locked flag can be ORed in to pass signal the locked property - any other state info used by subclasses
//! should be passed in the userInfo.
typedef NS_OPTIONS(NSUInteger, DKKnobType)
{
	kDKInvalidKnobType				= 0,
	kDKControlPointKnobType			= 1,
	kDKOnPathKnobType				= 2,
	kDKBoundingRectKnobType			= 3,
	kDKRotationKnobType				= 4,
	kDKCentreTargetKnobType			= 5,
	kDKHotspotKnobType				= 6,
	kDKOffPathKnobType				= kDKControlPointKnobType,
	kDKMoreTextIndicatorKnobType	= 8,
	//--------------------------------------------
	kDKKnobIsDisabledFlag			= ( 1 << 16 ),
	kDKKnobIsInactiveFlag			= ( 1 << 17 ),
	kDKKnobIsSelectedFlag			= ( 1 << 18 ),
	//--------------------------------------------
	kDKKnobTypeMask					= 0xFFFF
};

//! an object that lays claim to own the knob class (e.g. DKLayer) needs to implement the following protocol:
@protocol DKKnobOwner <NSObject>

@property (readonly) CGFloat knobsWantDrawingScale;
@property (readonly) BOOL knobsWantDrawingActiveState;

@end


//! constants that can be passed to pasteboardTypesForOperation:  OR together to combine types
typedef NS_OPTIONS(NSUInteger, DKPasteboardOperationType)
{
	//! return the types that are written for a cut or copy operation
	kDKWritableTypesForCopy		= ( 1 << 0 ),
	//! return the types that are written for a drag operation (drag OUT)
	kDKWritableTypesForDrag		= ( 1 << 1 ),
	//! return the types that can be received by a paste operation
	kDKReadableTypesForPaste	= ( 1 << 2 ),
	//! return the types that can be received by a drag operation (drag IN)
	kDKReadableTypesForDrag		= ( 1 << 3 ),
	kDKAllReadableTypes			= kDKReadableTypesForDrag | kDKReadableTypesForPaste,
	kDKAllWritableTypes			= kDKWritableTypesForCopy | kDKWritableTypesForDrag,
	kDKAllDragTypes				= kDKReadableTypesForDrag | kDKWritableTypesForDrag,
	kDKAllCopyPasteTypes		= kDKReadableTypesForPaste | kDKWritableTypesForCopy,
	kDKAllPasteboardTypes		= 0xFF
};


//! text vertical alignment options
typedef NS_ENUM(NSInteger, DKVerticalTextAlignment)
{
	kDKTextShapeVerticalAlignmentTop			= 0,
	kDKTextShapeVerticalAlignmentCentre			= 1,
	kDKTextShapeVerticalAlignmentBottom			= 2,
	kDKTextShapeVerticalAlignmentProportional	= 3,
	kDKTextPathVerticalAlignmentCentredOnPath	= 4,
	kDKTextShapeAlignTextToPoint				= 27
};


// layout modes, used by DKTextShape, DKTextAdornment:

typedef NS_OPTIONS(NSUInteger, DKTextLayoutMode)
{
	//! simple text block ignores path shape (but can be clipped to it)
	kDKTextLayoutInBoundingRect				= 0,
	//! this usually results in "outside path"
	kDKTextLayoutAlongPath					= 1,
	//! will allow text inside circle for example, i.e. "inside path"
	kDKTextLayoutAlongReversedPath			= 2,
	//! flows the text by wrapping within the path's shape
	kDKTextLayoutFlowedInPath				= 3,
	//! positions a label centred on an object's centroid (requires external code)
	kDKTextLayoutAtCentroid					= 40,
	//! can be ORed in to only lay out the first line
	kDKTextLayoutFirstLineOnly				= 64
};

//! text capitalization, used by DKTextAdornment, DKTextShape, DKTextPath:
typedef NS_ENUM(NSInteger, DKTextCapitalization)
{
	//! no modification to the strings is performed
	kDKTextCapitalizationNone				= 0,
	//! text is made upper case
	kDKTextCapitalizationUppercase			= 1,
	//! text is made lower case
	kDKTextCapitalizationLowercase			= 2,
	//! first letter of each word in text is capitalized, otherwise lowercase
	kDKTextCapitalizationCapitalize			= 3
};

//! greeking, used by DKGreekingLayoutManager and DKTextAdornment
typedef NS_ENUM(NSInteger, DKGreeking)
{
	//! do not use greeking
	kDKGreekingNone							= 0,
	//! greek by filling line rects
	kDKGreekingByLineRectangle				= 1,
	//! greek by filling glyph rects
	kDKGreekingByGlyphRectangle				= 2
};

