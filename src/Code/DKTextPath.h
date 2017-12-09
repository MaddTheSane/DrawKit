//
//  DKTextPath.h
//  GCDrawKit
//
//  Created by graham on 25/11/2008.
//  Copyright 2008 Apptree.net. All rights reserved.
//

#import "DKDrawablePath.h"
#import "DKCommonTypes.h"



@class DKTextAdornment, DKDrawingView;


/*!
 
 Very similar to a DKTextShape but based on a path and defaulting to text-on-a-path rendering. Has virtually identical public API to DKTextShape.
 
 */
@interface DKTextPath : DKDrawablePath <NSCopying, NSCoding>
{
@private
	DKTextAdornment*		mTextAdornment;
	NSTextView*				mEditorRef;
	BOOL					mIsSettingStyle;
}

// convenience constructors:

+ (instancetype)			textPathWithString:(NSString*) str onPath:(NSBezierPath*) aPath;

// class defaults:

@property (class, copy) NSString *defaultTextString;
@property (class, readonly) Class textAdornmentClass;
+ (NSArray*)				pastableTextTypes;
+ (DKStyle*)				textPathDefaultStyle;

// the text:

- (void)					setText:(id) contents;
- (NSTextStorage*)			text;
- (NSString*)				string;

- (void)					pasteTextFromPasteboard:(NSPasteboard*) pb ignoreFormatting:(BOOL) fmt;
- (BOOL)					canPasteText:(NSPasteboard*) pb;

// conversion to path/shape with text path:

- (NSBezierPath*)			textPath;
- (NSArray*)				textPathGlyphs;
- (NSArray*)				textPathGlyphsUsedSize:(NSSize*) textSize;
- (DKDrawablePath*)			makePathWithText;
- (DKDrawableShape*)		makeShapeWithText;
- (DKShapeGroup*)			makeShapeGroupWithText;
- (DKStyle*)				styleWithTextAttributes;
- (DKStyle*)				syntheticStyle;

// text attributes - accesses the internal adornment object

- (NSDictionary*)			textAttributes;

@property (copy) NSFont *font;
@property CGFloat fontSize;
@property (copy) NSColor *textColour;

- (void)					scaleTextBy:(CGFloat) factor;

// paragraph style attributes:

@property DKVerticalTextAlignment verticalAlignment;
@property CGFloat verticalAlignmentProportion;
@property (copy) NSParagraphStyle *paragraphStyle;
@property (readonly) NSTextAlignment alignment;

@property DKTextLayoutMode layoutMode;

// editing the text:

- (void)					startEditingInView:(DKDrawingView*) view;
- (void)					endEditing;
@property (readonly, getter=isEditing) BOOL editing;

// the internal adornment object:

@property (copy) DKTextAdornment* textAdornment;

// user actions:

- (IBAction)				changeFont:(id) sender;
- (IBAction)				changeFontSize:(id) sender;
- (IBAction)				changeAttributes:(id) sender;
- (IBAction)				editText:(id) sender;

- (IBAction)				changeLayoutMode:(id) sender;

- (IBAction)				alignLeft:(id) sender;
- (IBAction)				alignRight:(id) sender;
- (IBAction)				alignCenter:(id) sender;
- (IBAction)				alignJustified:(id) sender;
- (IBAction)				underline:(id) sender;

- (IBAction)				loosenKerning:(id) sender;
- (IBAction)				tightenKerning:(id) sender;
- (IBAction)				turnOffKerning:(id)sender;
- (IBAction)				useStandardKerning:(id) sender;

- (IBAction)				lowerBaseline:(id) sender;
- (IBAction)				raiseBaseline:(id) sender;
- (IBAction)				superscript:(id) sender;
- (IBAction)				subscript:(id) sender;
- (IBAction)				unscript:(id) ssender;

- (IBAction)				verticalAlign:(id) sender;
- (IBAction)				convertToShape:(id) sender;
- (IBAction)				convertToShapeGroup:(id) sender;
- (IBAction)				convertToTextShape:(id) sender;
- (IBAction)				convertToPath:(id) sender;

- (IBAction)				paste:(id) sender;
- (IBAction)				capitalize:(id) sender;

- (IBAction)				takeTextAlignmentFromSender:(id) sender;
- (IBAction)				takeTextVerticalAlignmentFromSender:(id) sender;

@end

