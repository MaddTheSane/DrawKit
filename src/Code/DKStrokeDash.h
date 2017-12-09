//
//  DKStrokeDash.h
///  DrawKit ©2005-2008 Apptree.net
//
//  Created by graham on 10/09/2006.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
//

#import <Cocoa/Cocoa.h>


/*!
 This stores a particular dash pattern for stroking an NSBezierPath, and can be owned by a DKStroke.
 */
@interface DKStrokeDash : NSObject <NSCoding, NSCopying>
{
@private
	CGFloat		m_pattern[8];
	CGFloat		m_phase;
	NSUInteger	m_count;
	BOOL		m_scaleToLineWidth;
	BOOL		mEditing;
}

+ (DKStrokeDash*)	defaultDash;
+ (instancetype)	dashWithPattern:(CGFloat[]) dashes count:(NSInteger) count;
+ (instancetype)	dashWithName:(NSString*) name;
+ (void)			registerDash:(DKStrokeDash*) dash withName:(NSString*) name;
+ (NSArray*)		registeredDashes;

+ (DKStrokeDash*)	equallySpacedDashToFitSize:(NSSize) aSize dashLength:(CGFloat) len;

- (id)				initWithPattern:(CGFloat[]) dashes count:(NSInteger) count;
- (void)			setDashPattern:(CGFloat[]) dashes count:(NSInteger) count;
- (void)			getDashPattern:(CGFloat[]) dashes count:(NSInteger*) count;
@property (readonly) NSInteger count;
- (void)			setPhaseWithoutNotifying:(CGFloat) ph;
@property CGFloat phase;
@property (readonly)  CGFloat length;
- (CGFloat)			lengthAtIndex:(NSUInteger) indx;

@property BOOL scalesToLineWidth;

@property BOOL isBeingEdited;

- (void)			applyToPath:(NSBezierPath*) path;
- (void)			applyToPath:(NSBezierPath*) path withPhase:(CGFloat) phase;

- (NSImage*)		dashSwatchImageWithSize:(NSSize) size strokeWidth:(CGFloat) width;
- (NSImage*)		standardDashSwatchImage;

@end



@interface DKStrokeDash (Deprecated)

+ (void)			saveDefaults DEPRECATED_ATTRIBUTE;
+ (void)			loadDefaults DEPRECATED_ATTRIBUTE;

@end


#define			kDKStandardDashSwatchImageSize		(NSMakeSize( 80.0, 4.0 ))
#define			kDKStandardDashSwatchStrokeWidth	2.0
