///**********************************************************************************************************************************
///  DKStyle-Text.h
///  DrawKit ©2005-2008 Apptree.net
///
///  Created by graham on 21/09/2006.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
///
///**********************************************************************************************************************************

#import "DKStyle.h"


/*!
 
 This adds text attributes to the DKStyle object. A DKTextShape makes use of styles with attached text attributes to style
 the text it displays. Other objects that use text can make use of this as they wish.
 
 */
@interface DKStyle (TextAdditions)

@property (class, readonly, retain) DKStyle *defaultTextStyle;
+ (instancetype)		textStyleWithFont:(NSFont*) font;
+ (NSString*)			styleNameForFont:(NSFont*) font;

@property (copy) NSParagraphStyle *paragraphStyle;

@property NSTextAlignment alignment;

- (void)				changeTextAttribute:(NSString*) attribute toValue:(id) val;
- (NSString*)			actionNameForTextAttribute:(NSString*) attribute;

@property (copy) NSFont *font;
@property CGFloat fontSize;

@property (copy) NSColor *textColour;

@property NSUnderlineStyle underlined;
- (void)				toggleUnderlined;

- (void)				applyToText:(NSMutableAttributedString*) text;
- (void)				adoptFromText:(NSAttributedString*) text;

- (DKStyle*)			drawingStyleFromTextAttributes;

@end

