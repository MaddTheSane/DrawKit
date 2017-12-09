///**********************************************************************************************************************************
///  DKStyle+SimpleAccess.h
///  DrawKit ©2005-2008 Apptree.net
///
///  Created by graham on 08/07/2008.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
///
///**********************************************************************************************************************************

#import "DKStyle.h"


@class DKStrokeDash, DKStroke, DKFill;



/*!
 
 This category on DKStyle provides some simple accessors if your app only has the most basic use of styles in mind, e.g. one solid fill and
 a single simple solid or dashed stroke.
 
 This operates on the topmost DKStroke/DKFill rasterizers in a style's list, and does not touch any others. By passing a colour of nil, the
 associated rasterizer is disabled. If a non-nil colour is passed, and there is no suitable rasterizer, one is created and added. If the
 rasterizer has to be created for both properies, the stroke will be placed in front of the fill.
 
 Note that this does not require or use and specially created style. It is recommended that if using these accessors, style sharing is
 turned off so that every object has its own style - then these accessors effectively operate on the graphic object's stroke and fill properties.
 
 The string setter sets or creates a DKTextAdornment component having the default text parameters and the string as its label.
 
 If the style is locked these do nothing.
 
 */
@interface DKStyle (SimpleAccess)

+ (instancetype)	styleWithDotDensity:(CGFloat) percent foreColour:(NSColor*) fore backColour:(NSColor*) back;

- (DKStroke*)		stroke;
- (DKFill*)			fill;

@property (copy) NSColor *fillColour;

@property (copy) NSColor *strokeColour;

@property CGFloat strokeWidth;

@property (copy) DKStrokeDash *strokeDash;

@property NSLineCapStyle strokeLineCapStyle;

@property NSLineJoinStyle strokeLineJoinStyle;

@property (copy) NSString *string;

@property (readonly) BOOL hasImageComponent;
@property (retain) NSImage *imageComponent;

@end

