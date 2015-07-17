/*
 Licensed to the Apache Software Foundation (ASF) under one
 or more contributor license agreements.  See the NOTICE file
 distributed with this work for additional information
 regarding copyright ownership.  The ASF licenses this file
 to you under the Apache License, Version 2.0 (the
 "License"); you may not use this file except in compliance
 with the License.  You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing,
 software distributed under the License is distributed on an
 "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 KIND, either express or implied.  See the License for the
 specific language governing permissions and limitations
 under the License.
 */

#import <Cordova/CDVPlugin.h>
#import <Cordova/CDVInvokedUrlCommand.h>
#import <Cordova/CDVScreenOrientationDelegate.h>
#import <Cordova/CDVWebViewDelegate.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>

@class CDVInAppBrowserViewController;

#pragma mark FLWebViewProvider
//
//  FLWebViewProvider.h
//  FLWebView
//
//  Created by Steve Richey on 11/21/14.
//  Copyright (c) 2014 Float Mobile Learning. Shared under an MIT license. See license.md for details.
//

//#import <Foundation/Foundation.h>

/*
 * This class defines methods that FLUIWebView and FLWKWebView should implement in
 * order to work within our ViewController.
 */
@protocol FLWebViewProvider <NSObject>

@property (nonatomic, strong) NSURLRequest *request;
@property (nonatomic, strong) NSURL *URL;
- (void) setDelegateViews: (id) delegateView;
- (void) loadRequest: (NSURLRequest *) request;
- (void) loadRequestFromString: (NSString *) urlNameAsString;
- (BOOL) canGoBack;
- (BOOL) canGoForward;

/*
 * UIWebView has stringByEvaluatingJavaScriptFromString, which is synchronous.
 * WKWebView has evaluateJavaScript, which is asynchronous.
 * Since it's far easier to implement the latter in UIWebView, we define it here and do that.
 */
- (void) evaluateJavaScript: (NSString *) javaScriptString completionHandler: (void (^)(id, NSError *)) completionHandler;

- (void) setScalesPageToFit: (BOOL) setPages;

/* These are the same for both! */
- (UIScrollView*) scrollView;
- (id)loadHTMLString:(NSString*) string baseURL:(NSURL*) url;
- (id)goBack;
- (id)goForward;

@end


@interface CDVInAppBrowser : CDVPlugin {
    BOOL _injectedIframeBridge;
}

@property (nonatomic, retain) CDVInAppBrowserViewController* inAppBrowserViewController;
@property (nonatomic, copy) NSString* callbackId;
@property (nonatomic, copy) NSRegularExpression *callbackIdPattern;

- (void)open:(CDVInvokedUrlCommand*)command;
- (void)close:(CDVInvokedUrlCommand*)command;
- (void)injectScriptCode:(CDVInvokedUrlCommand*)command;
- (void)show:(CDVInvokedUrlCommand*)command;

@end

@interface CDVInAppBrowserOptions : NSObject {}

@property (nonatomic, assign) BOOL location;
@property (nonatomic, assign) BOOL toolbar;
@property (nonatomic, copy) NSString* closebuttoncaption;
@property (nonatomic, copy) NSString* toolbarposition;
@property (nonatomic, assign) BOOL clearcache;
@property (nonatomic, assign) BOOL clearsessioncache;

@property (nonatomic, copy) NSString* presentationstyle;
@property (nonatomic, copy) NSString* transitionstyle;

@property (nonatomic, assign) BOOL enableviewportscale;
@property (nonatomic, assign) BOOL mediaplaybackrequiresuseraction;
@property (nonatomic, assign) BOOL allowinlinemediaplayback;
@property (nonatomic, assign) BOOL keyboarddisplayrequiresuseraction;
@property (nonatomic, assign) BOOL suppressesincrementalrendering;
@property (nonatomic, assign) BOOL hidden;
@property (nonatomic, assign) BOOL disallowoverscroll;

+ (CDVInAppBrowserOptions*)parseOptions:(NSString*)options;

@end

@interface CDVInAppBrowserViewController : UIViewController <UIWebViewDelegate, CDVScreenOrientationDelegate>{
@private
    NSString* _userAgent;
    NSString* _prevUserAgent;
    NSInteger _userAgentLockToken;
    CDVInAppBrowserOptions *_browserOptions;
    CDVWebViewDelegate* _webViewDelegate;
}

@property (nonatomic, strong) IBOutlet UIView <FLWebViewProvider>* webView;
@property (nonatomic, strong) IBOutlet UIBarButtonItem* closeButton;
@property (nonatomic, strong) IBOutlet UILabel* addressLabel;
@property (nonatomic, strong) IBOutlet UIBarButtonItem* backButton;
@property (nonatomic, strong) IBOutlet UIBarButtonItem* forwardButton;
@property (nonatomic, strong) IBOutlet UIActivityIndicatorView* spinner;
@property (nonatomic, strong) IBOutlet UIToolbar* toolbar;

@property (nonatomic, weak) id <CDVScreenOrientationDelegate> orientationDelegate;
@property (nonatomic, weak) CDVInAppBrowser* navigationDelegate;
@property (nonatomic) NSURL* currentURL;

- (void)close;
- (void)navigateTo:(NSURL*)url;
- (void)showLocationBar:(BOOL)show;
- (void)showToolBar:(BOOL)show : (NSString *) toolbarPosition;
- (void)setCloseButtonTitle:(NSString*)title;

- (id)initWithUserAgent:(NSString*)userAgent prevUserAgent:(NSString*)prevUserAgent browserOptions: (CDVInAppBrowserOptions*) browserOptions;

@end

@interface CDVInAppBrowserNavigationController : UINavigationController

@property (nonatomic, weak) id <CDVScreenOrientationDelegate> orientationDelegate;

@end

#pragma mark WKWebView integration



#pragma mark UIWebView+FLUIWebView
//
//  UIWebView+FLUIWebView.h
//  FLWebView
//
//  Created by Steve Richey on 11/21/14.
//  Copyright (c) 2014 Float Mobile Learning. Shared under an MIT license. See license.md for details.
//

//#import <UIKit/UIKit.h>
//#import "FLWebViewProvider.h"

/*
 * This category extends UIWebView and conforms to the FLWebViewProvider protocol.
 */
@interface UIWebView (FLUIWebView) <FLWebViewProvider>

/*
 * Shorthand for setting UIWebViewDelegate to a class.
 */
- (void) setDelegateViews: (id <UIWebViewDelegate>) delegateView;

@end


#pragma mark WKWebView+FLWKWebView
//
//  WKWebView+FLWKWebView.h
//  FLWebView
//
//  Created by Steve Richey on 11/21/14.
//  Copyright (c) 2014 Float Mobile Learning. Shared under an MIT license. See license.md for details.
//

//#import <WebKit/WebKit.h>
//#import "FLWebViewProvider.h"

/*
 * This category extends WKWebView and conforms to the FLWebViewProvider protocol.
 */
@interface WKWebView (FLWKWebView) <FLWebViewProvider>

/*
 * Shorthand for setting WKUIDelegate and WKNavigationDelegate to the same class.
 */
- (void) setDelegateViews: (id <WKNavigationDelegate, WKUIDelegate>) delegateView;

@end
