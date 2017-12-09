///**********************************************************************************************************************************
///  DKStyle.h
///  DrawKit ©2005-2008 Apptree.net
///
///  Created by graham on 13/08/2006.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
///
///**********************************************************************************************************************************


#import "DKRastGroup.h"


@class	DKDrawableObject, DKUndoManager;

//! swatch types that can be passed to -styleSwatchWithSize:type:
typedef NS_ENUM(NSInteger, DKStyleSwatchType)
{
	kDKStyleSwatchAutomatic			= -1,
	kDKStyleSwatchRectanglePath		= 0,
	kDKStyleSwatchCurvePath			= 1
};


//! options that can be passed to -derivedStyleWithPasteboard:withOptions:
typedef NS_ENUM(NSInteger, DKDerivedStyleOptions)
{
	kDKDerivedStyleDefault			= 0,
	kDKDerivedStyleForPathHint		= 1,
	kDKDerivedStyleForShapeHint		= 2
};



#define STYLE_SWATCH_SIZE		NSMakeSize( 128.0, 128.0 )

// n.b. for style registry API, see DKStyleRegistry.h



@interface DKStyle : DKRastGroup <NSCoding, NSCopying, NSMutableCopying>
{
@private
	NSDictionary*			m_textAttributes;		// supports text additions
	NSUndoManager*			m_undoManagerRef;		// style's undo manager
	BOOL					m_shared;				// YES if the style is shared
	BOOL					m_locked;				// YES if style can't be edited
	id						m_renderClientRef;		// valid only while actually drawing
	NSString*				m_uniqueKey;			// unique key, set once for all time
	BOOL					m_mergeFlag;			// set to YES when a style is read in from a file and was saved in a registered state.
	NSTimeInterval			m_lastModTime;			// timestamp to determine when styles have been updated
	NSUInteger				m_clientCount;			// keeps count of the clients using the style
	NSMutableDictionary*	mSwatchCache;			// cache of swatches at various sizes previously requested
}

// basic standard styles:

+ (DKStyle*)			defaultStyle;			// very boring, black stroke and light gray fill
+ (DKStyle*)			defaultTrackStyle;		// grey stroke over wider black stroke, no fill

// easy construction of other simple styles:

+ (DKStyle*)			styleWithFillColour:(NSColor*) fc strokeColour:(NSColor*) sc;
+ (DKStyle*)			styleWithFillColour:(NSColor*) fc strokeColour:(NSColor*) sc strokeWidth:(CGFloat) sw;
+ (DKStyle*)			styleFromPasteboard:(NSPasteboard*) pb;

+ (NSArray*)			stylePasteboardTypes;
+ (BOOL)				canInitWithPasteboard:(NSPasteboard*) pb;

// pasted styles - separate non-persistent registry

+ (DKStyle*)			styleWithPasteboardName:(NSString*) name;
+ (void)				registerStyle:(DKStyle*) style withPasteboardName:(NSString*) pbname;

// default sharing flag

@property (class) BOOL stylesAreSharableByDefault;

// shadows:

+ (NSShadow*)			defaultShadow;
+ (BOOL)				setWillDrawShadows:(BOOL) drawShadows;
@property (class, readonly) BOOL willDrawShadows;

// performance options:

@property (class) BOOL shouldAntialias;
@property (class) BOOL shouldSubstitutePlaceholderStyle;

// updating & notifying clients:

- (void)				notifyClientsBeforeChange;
- (void)				notifyClientsAfterChange;
- (void)				styleWasAttached:(DKDrawableObject*) toObject;
- (void)				styleWillBeRemoved:(DKDrawableObject*) fromObject;
- (NSUInteger)			countOfClients;

// (text) attributes - basic support

- (void)				setTextAttributes:(NSDictionary*) attrs;
- (NSDictionary*)		textAttributes;
@property (readonly) BOOL hasTextAttributes;
- (void)				removeTextAttributes;

// shared and locked status:

@property (getter=isStyleSharable) BOOL styleSharable;
@property BOOL locked;

// registry info:

@property (readonly, getter=isStyleRegistered) BOOL styleRegistered;
- (NSArray*)			registryKeys;
- (NSString*)			uniqueKey;
- (void)				assignUniqueKey;
@property (readonly) BOOL requiresRemerge;
- (void)				clearRemergeFlag;
@property (readonly) NSTimeInterval lastModificationTimestamp;

- (BOOL)				isEqualToStyle:(DKStyle*) aStyle;

// undo:

- (void)				setUndoManager:(NSUndoManager*) undomanager;
- (NSUndoManager*)		undoManager;
- (void)				changeKeyPath:(NSString*) keypath ofObject:(id) object toValue:(id) value;

// stroke utilities:

- (void)				scaleStrokeWidthsBy:(CGFloat) scale withoutInformingClients:(BOOL) quiet;
- (CGFloat)				maxStrokeWidth;
- (CGFloat)				maxStrokeWidthDifference;
- (void)				applyStrokeAttributesToPath:(NSBezierPath*) path;
- (NSUInteger)			countOfStrokes;

// clipboard:

- (BOOL)				copyToPasteboard:(NSPasteboard*) pb;
- (DKStyle*)			derivedStyleWithPasteboard:(NSPasteboard*) pb;
- (DKStyle*)			derivedStyleWithPasteboard:(NSPasteboard*) pb withOptions:(DKDerivedStyleOptions) options;

// query methods:

@property (readonly) BOOL hasStroke;
@property (readonly) BOOL hasFill;
@property (readonly) BOOL hasHatch;
@property (readonly) BOOL hasTextAdornment;
@property (readonly, getter=isEmpty) BOOL empty;

// swatch images:

- (NSImage*)			styleSwatchWithSize:(NSSize) size type:(DKStyleSwatchType) type;
- (NSImage*)			standardStyleSwatch;
- (NSImage*)			image;
- (NSImage*)			imageToFitSize:(NSSize) aSize;
- (NSString*)			swatchCacheKeyForSize:(NSSize) size type:(DKStyleSwatchType) type;

// currently rendering client (may be queried by renderers)

- (id)					currentRenderClient;

// making derivative styles:

- (DKStyle*)			styleByMergingFromStyle:(DKStyle*) otherStyle;
- (DKStyle*)			styleByRemovingRenderersOfClass:(Class) aClass;
- (id)					clone;

@end

// pasteboard types:

extern NSPasteboardType	kDKStylePasteboardType;
extern NSPasteboardType	kDKStyleKeyPasteboardType;

// notifications:

extern NSString*		kDKStyleWillChangeNotification;
extern NSString*		kDKStyleDidChangeNotification;
extern NSString*		kDKStyleTextAttributesDidChangeNotification; 
extern NSString*		kDKStyleWasAttachedNotification;
extern NSString*		kDKStyleWillBeDetachedNotification;
extern NSString*		kDKStyleLockStateChangedNotification;
extern NSString*		kDKStyleSharableFlagChangedNotification;
extern NSString*		kDKStyleNameChangedNotification;

// preferences keys

extern NSString*		kDKStyleDisplayPerformance_no_anti_aliasing;
extern NSString*		kDKStyleDisplayPerformance_no_shadows;
extern NSString*		kDKStyleDisplayPerformance_substitute_styles;

