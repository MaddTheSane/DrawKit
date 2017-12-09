///**********************************************************************************************************************************
///  DKStroke.h
///  DrawKit ©2005-2008 Apptree.net
///
///  Created by graham on 09/11/2006.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
///
///**********************************************************************************************************************************

#import "DKRasterizer.h"


@class DKStrokeDash;


@interface DKStroke : DKRasterizer <NSCoding, NSCopying>
{
@private
	NSColor*			m_colour;
	DKStrokeDash*		m_dash;
	NSShadow*			m_shadow;
	NSLineCapStyle		m_cap;
	NSLineJoinStyle		m_join;
	CGFloat				m_mitreLimit;
	CGFloat				m_trimLength;
	CGFloat				mLateralOffset;
@protected
	CGFloat				m_width;
}

@property (class, readonly, retain) DKStroke *defaultStroke;
+ (instancetype)		strokeWithWidth:(CGFloat) width colour:(NSColor*) colour;

- (instancetype)		initWithWidth:(CGFloat) width colour:(NSColor*) colour;

@property (copy) NSColor *colour;

@property CGFloat width;
- (void)				scaleWidthBy:(CGFloat) scale;
- (CGFloat)				allowance;

@property (retain) DKStrokeDash *dash;
- (void)				setAutoDash;

@property  CGFloat lateralOffset;

@property (copy) NSShadow *shadow;

- (void)				strokeRect:(NSRect) rect;
- (void)				applyAttributesToPath:(NSBezierPath*) path;

@property NSLineCapStyle lineCapStyle;

@property NSLineJoinStyle lineJoinStyle;

@property CGFloat miterLimit;

@property CGFloat trimLength;

- (NSSize)				extraSpaceNeededIgnoringMitreLimit;

@end


/*

represents the stroke of a path, and can be added as an attribute of a DKStyle. Note that because a stroke
is an object, it's easy to stroke a path multiple times for special effects. A DKStyle will apply all
the strokes it is aware of in order when it is asked to stroke a path.

DKStyle can contains a list of strokes without limit.

*/
