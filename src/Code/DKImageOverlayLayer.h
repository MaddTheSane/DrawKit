//
//  DKImageOverlayLayer.h
///  DrawKit ©2005-2008 Apptree.net
//
//  Created by graham on 28/08/2006.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
//

#import "DKLayer.h"


//! coverage method flags - can be combined to give different effects
typedef NS_OPTIONS(NSUInteger, DKImageCoverageFlags)
{
	kDKDrawingImageCoverageNormal					= 0,
	kDKDrawingImageCoverageHorizontallyCentred		= 1,
	kDKDrawingImageCoverageHorizontallyStretched	= 2,
	kDKDrawingImageCoverageHorizontallyTiled		= 4,
	kDKDrawingImageCoverageVerticallyCentred		= 32,
	kDKDrawingImageCoverageVerticallyStretched		= 64,
	kDKDrawingImageCoverageVerticallyTiled			= 128,
};



/*!
 
 This layer type implements a single image overlay, for example for tracing a photograph in another layer. The coverage method
 sets whether the image is scaled, tiled or drawn only once in a particular position.
 
 */
@interface DKImageOverlayLayer : DKLayer <NSCoding>
{
	NSImage*				m_image;
	CGFloat					m_opacity;
	DKImageCoverageFlags	m_coverageMethod;
}

- (instancetype)			initWithImage:(NSImage*) image;
- (instancetype)			initWithContentsOfFile:(NSString*) imagefile;

@property (nonatomic, retain) NSImage *image;

@property (nonatomic) CGFloat opacity;

@property (nonatomic) DKImageCoverageFlags coverageMethod;

@property (readonly) NSRect imageDestinationRect;

@end



