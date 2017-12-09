//
//  DKKnob.h
///  DrawKit ©2005-2008 Apptree.net
//
//  Created by graham on 21/08/2006.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
//

#import <Cocoa/Cocoa.h>
#import "DKCommonTypes.h"


//! visual flags, used internally
typedef NS_OPTIONS(NSUInteger, DKKnobDrawingFlags)
{
	kDKKnobDrawsStroke				= ( 1 << 0 ),
	kDKKnobDrawsFill				= ( 1 << 1 )
};


@class DKHandle;



/*!
 
 simple class used to provide the drawing of knobs for object selection. You can override this and replace it (attached to any layer)
 to customise the appearance of the selection knobs for all drawn objects in that layer.
 
 The main method a drawable will call is drawKnobAtPoint:ofType:userInfo:
 
 The type (DKKnobType) is a functional description of the knob only - this class maps that functional description to a consistent appearance taking
 into account the basic type and a couple of generic state flags. Clients should generally avoid trying to do drawing themselves of knobs, but if they do,
 should use the lower level methods here to get consistent results.
 
 Subclasses may want to customise many aspects of a knob's appearance, and can override any suitable factored methods according to their needs. Customisations
 might include the shape of a knob, its colours, whether stroked or filled or both, etc.
 
 */
@interface DKKnob : NSObject <NSCoding, NSCopying>
{
@private
	id<DKKnobOwner>	m_ownerRef;					// the object that owns (and hence retains) this - typically a DKLayer
	NSSize			m_knobSize;					// the currently cached knob size
	CGFloat			mScaleRatio;				// ratio to zoom factor used to scale knob size (default = 0.3)
	NSColor*		mControlKnobColour;			// colour of square knobs
	NSColor*		mRotationKnobColour;		// colour of rotation knobs
	NSColor*		mControlOnPathPointColour;	// colour of on-path control points
	NSColor*		mControlOffPathPointColour;	// colour of off-path control points
	NSColor*		mControlBarColour;			// colour of control bars
	NSSize			mControlKnobSize;			// control knob size
	CGFloat			mControlBarWidth;			// control bar width
}

+ (DKKnob*)			standardKnobs;

// main high-level methods that will be called by clients

//! the object that owns (and hence retains) this - typically a \c DKLayer
@property (assign) id<DKKnobOwner>	owner;

- (void)			drawKnobAtPoint:(NSPoint) p ofType:(DKKnobType) knobType userInfo:(id) userInfo;
- (void)			drawKnobAtPoint:(NSPoint) p ofType:(DKKnobType) knobType angle:(CGFloat) radians userInfo:(id) userInfo;
- (void)			drawKnobAtPoint:(NSPoint) p ofType:(DKKnobType) knobType angle:(CGFloat) radians highlightColour:(NSColor*) aColour;

- (void)			drawControlBarFromPoint:(NSPoint) a toPoint:(NSPoint) b;
- (void)			drawControlBarWithKnobsFromPoint:(NSPoint) a toPoint:(NSPoint) b;
- (void)			drawControlBarWithKnobsFromPoint:(NSPoint) a ofType:(DKKnobType) typeA toPoint:(NSPoint) b ofType:(DKKnobType) typeB;
- (void)			drawRotationBarWithKnobsFromCentre:(NSPoint) centre toPoint:(NSPoint) p;
- (void)			drawPartcode:(NSInteger) code atPoint:(NSPoint) p fontSize:(CGFloat) fontSize;

- (BOOL)			hitTestPoint:(NSPoint) p inKnobAtPoint:(NSPoint) kp ofType:(DKKnobType) knobType userInfo:(id) userInfo;

//! colour of control bars
@property (retain) NSColor *controlBarColour;
//! control bar width
@property CGFloat controlBarWidth;

//! ratio to zoom factor used to scale knob size (default = 0.3)
@property CGFloat scalingRatio;

// low-level methods (mostly internal and overridable)

//! control knob size
@property NSSize controlKnobSize;

- (void)			setControlKnobSizeForViewScale:(CGFloat) scale;

// new model APIs

- (DKHandle*)		handleForType:(DKKnobType) knobType;
- (DKHandle*)		handleForType:(DKKnobType) knobType colour:(NSColor*) colour;
//! the size of the handle as it is actually drawn to hte screen, taking into account all the scale factors, etc.
@property (readonly) NSSize actualHandleSize;

@end

#pragma mark -


@interface DKKnob (Deprecated)

+ (void)			setControlKnobColour:(NSColor*) clr DEPRECATED_ATTRIBUTE;
+ (NSColor*)		controlKnobColour DEPRECATED_ATTRIBUTE;

+ (void)			setRotationKnobColour:(NSColor*) clr DEPRECATED_ATTRIBUTE;
+ (NSColor*)		rotationKnobColour DEPRECATED_ATTRIBUTE;

+ (void)			setControlOnPathPointColour:(NSColor*) clr DEPRECATED_ATTRIBUTE;
+ (NSColor*)		controlOnPathPointColour DEPRECATED_ATTRIBUTE;
+ (void)			setControlOffPathPointColour:(NSColor*) clr DEPRECATED_ATTRIBUTE;
+ (NSColor*)		controlOffPathPointColour DEPRECATED_ATTRIBUTE;

+ (void)			setControlBarColour:(NSColor*) clr DEPRECATED_ATTRIBUTE;
+ (NSColor*)		controlBarColour DEPRECATED_ATTRIBUTE;

+ (void)			setControlKnobSize:(NSSize) size DEPRECATED_ATTRIBUTE;
+ (NSSize)			controlKnobSize DEPRECATED_ATTRIBUTE;

+ (void)			setControlBarWidth:(CGFloat) width DEPRECATED_ATTRIBUTE;
+ (CGFloat)			controlBarWidth DEPRECATED_ATTRIBUTE;

+ (NSRect)			controlKnobRectAtPoint:(NSPoint) kp DEPRECATED_ATTRIBUTE;

- (NSColor*)		fillColourForKnobType:(DKKnobType) knobType DEPRECATED_ATTRIBUTE;
- (NSColor*)		strokeColourForKnobType:(DKKnobType) knobType DEPRECATED_ATTRIBUTE;
- (CGFloat)			strokeWidthForKnobType:(DKKnobType) knobType DEPRECATED_ATTRIBUTE;

// setting colours and sizes per-DKKnob instance

- (void)			setControlKnobColour:(NSColor*) clr DEPRECATED_ATTRIBUTE;
- (NSColor*)		controlKnobColour DEPRECATED_ATTRIBUTE;
- (void)			setRotationKnobColour:(NSColor*) clr DEPRECATED_ATTRIBUTE;
- (NSColor*)		rotationKnobColour DEPRECATED_ATTRIBUTE;

- (void)			setControlOnPathPointColour:(NSColor*) clr DEPRECATED_ATTRIBUTE;
- (NSColor*)		controlOnPathPointColour DEPRECATED_ATTRIBUTE;
- (void)			setControlOffPathPointColour:(NSColor*) clr DEPRECATED_ATTRIBUTE;
- (NSColor*)		controlOffPathPointColour DEPRECATED_ATTRIBUTE;

- (NSRect)			controlKnobRectAtPoint:(NSPoint) kp DEPRECATED_ATTRIBUTE;
- (NSRect)			controlKnobRectAtPoint:(NSPoint) kp ofType:(DKKnobType) knobType DEPRECATED_ATTRIBUTE;

- (NSBezierPath*)	knobPathAtPoint:(NSPoint) p ofType:(DKKnobType) knobType angle:(CGFloat) radians userInfo:(id) userInfo DEPRECATED_ATTRIBUTE;
- (void)			drawKnobPath:(NSBezierPath*) path ofType:(DKKnobType) knobType userInfo:(id) userInfo DEPRECATED_ATTRIBUTE;
- (DKKnobDrawingFlags) drawingFlagsForKnobType:(DKKnobType) knobType DEPRECATED_ATTRIBUTE;

@end


// keys in the userInfo that can be used to pass additional information to the knob drawing methods

extern NSString*	kDKKnobPreferredHighlightColour;		// references an NSColor
