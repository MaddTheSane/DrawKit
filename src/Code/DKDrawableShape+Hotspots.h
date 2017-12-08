///**********************************************************************************************************************************
///  DKDrawableShape+Hotspots.h
///  DrawKit ©2005-2008 Apptree.net
///
///  Created by graham on 30/06/2007.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
///
///**********************************************************************************************************************************

#import "DKDrawableShape.h"


@class DKHotspot;
@protocol DKHotspotDelegate;


typedef NS_ENUM(NSInteger, DKHotspotState)
{
	kDKHotspotStateOff			= 0,
	kDKHotspotStateOn			= 1,
	kDKHotspotStateDisabled		= 2
};


@interface DKDrawableShape (Hotspots)

- (NSInteger)			addHotspot:(DKHotspot*) hspot;
- (void)				removeHotspot:(DKHotspot*) hspot;
- (void)				setHotspots:(NSArray*) spots;
- (NSArray*)			hotspots;

- (DKHotspot*)			hotspotForPartCode:(NSInteger) pc;
- (DKHotspot*)			hotspotUnderMouse:(NSPoint) mp;
- (NSPoint)				hotspotPointForPartcode:(NSInteger) pc;

- (NSRect)				hotspotRect:(DKHotspot*) hs;
- (void)				drawHotspotAtPoint:(NSPoint) hp inState:(DKHotspotState) state;
- (void)				drawHotspotsInState:(DKHotspotState) state;

@end


enum
{
	kDKHotspotBasePartcode		= 32768
};


#pragma mark -


/*!
 
 A HOTSPOT is an object attached to a shape to provide a direct user-interface for implementing custom actions, etc.
 
 Hotspots are clickable areas on a shape indicated by a special "knob" appearance. They can appear anywhere within the bounds. When clicked,
 they will be tracked and can do any useful thing they wish. The original purpose is to allow the direct manipulation of certain shape parameters
 such as radius of round corners, and so on, but the design is completely general-purpose.
 
 The action of a hotspot is handled by default by its delegate, though you could also subclass it and implement the action directly if you wish.
 
 The appearance of a hotspot is drawn by default by a method of DKKnob.
 
 */
@interface DKHotspot : NSObject <NSCoding, NSCopying>
{
	__unsafe_unretained DKDrawableShape*		m_owner;
	NSInteger			m_partcode;
	NSPoint				m_relLoc;
	__unsafe_unretained id<DKHotspotDelegate>	m_delegate;
}


- (id)					initHotspotWithOwner:(DKDrawableShape*) shape partcode:(NSInteger) pc delegate:(id) delegate;

@property (assign) DKDrawableShape *owner;
- (void)				setOwner:(DKDrawableShape*) shape;
- (void)				setOwner:(DKDrawableShape*) shape withPartcode:(NSInteger) pc;
- (DKDrawableShape*)	owner;

@property NSInteger partcode;

@property NSPoint relativeLocation;

- (void)				drawHotspotAtPoint:(NSPoint) p inState:(DKHotspotState) state;

@property (assign) id<DKHotspotDelegate> delegate;

- (void)				startMouseTracking:(NSEvent*) event inView:(NSView*) view;
- (void)				continueMouseTracking:(NSEvent*) event inView:(NSView*) view;
- (void)				endMouseTracking:(NSEvent*) event inView:(NSView*) view;

@end


#define		kDKDefaultHotspotSize		NSMakeSize( 6, 6 )

#pragma mark -

@protocol DKHotspotDelegate <NSObject>
@optional

 - (void)				hotspot:(DKHotspot*) hs willBeginTrackingWithEvent:(NSEvent*) event inView:(NSView*) view;
 - (void)				hotspot:(DKHotspot*) hs isTrackingWithEvent:(NSEvent*) event inView:(NSView*) view;
 - (void)				hotspot:(DKHotspot*) hs didEndTrackingWithEvent:(NSEvent*) event inView:(NSView*) view;


@end

