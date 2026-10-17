// libtriggr — let any tweak plug into Triggr: add triggers, add actions, and show a
// "Configure via Triggr" picker in its own settings. 100% on-device.
//
// Link: weak-link libtriggr.dylib (/var/jb/usr/lib/libtriggr.dylib) or dlopen it, and check
// TGTriggrAvailable() first, so your tweak still loads without Triggr installed.
//
// Where code runs:
// - Triggers can be sent from ANY process (apps, daemons, SpringBoard). They reach SpringBoard,
//   which looks up the assignment and runs it.
// - Actions run inside the process that registered them. Register from a SpringBoard tweak
//   (most tweaks already have one). Out-of-SpringBoard actions: use an Extensions plist instead.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

#define TRIGGR_API_VERSION 1

// Places, as in Triggr's settings. A trigger or action can limit where it makes sense.
typedef NS_OPTIONS(NSUInteger, TGPlaces) {
    TGPlaceHomeScreen = 1 << 0,
    TGPlaceInApps     = 1 << 1,
    TGPlaceLockScreen = 1 << 2,
    TGPlaceAnywhere   = TGPlaceHomeScreen | TGPlaceInApps | TGPlaceLockScreen,
};

#pragma mark - Triggers (your tweak says "this happened")

@interface TGTriggerInfo : NSObject
// Unique, reverse-DNS: "com.yourname.tweak.something".
@property (nonatomic, copy) NSString *identifier;
@property (nonatomic, copy) NSString *title;          // "Double Tap Dock"
@property (nonatomic, copy) NSString *group;          // shown as its own group: "YourTweak"
@property (nonatomic, copy, nullable) NSString *footer; // explanation under the group
@property (nonatomic) TGPlaces places;                // default TGPlaceAnywhere
@end

#pragma mark - Actions (Triggr runs your code)

@interface TGActionInfo : NSObject
@property (nonatomic, copy) NSString *identifier;     // "com.yourname.tweak.toggle"
@property (nonatomic, copy) NSString *title;          // "Toggle YourTweak"
@property (nonatomic, copy) NSString *group;          // action picker category
@property (nonatomic, copy, nullable) NSString *symbolName; // SF Symbol for the picker row
@property (nonatomic) TGPlaces places;
@end

// What an action gets when it runs.
@interface TGActionContext : NSObject
@property (nonatomic, readonly, copy) NSString *triggerIdentifier; // what fired it
@property (nonatomic, readonly, copy, nullable) NSString *frontmostApp; // bundle id, nil on Home/Lock Screen
@property (nonatomic, readonly) BOOL locked;
@end

typedef void (^TGActionHandler)(TGActionContext *context);

#pragma mark - The library

@interface TGTriggr : NSObject

+ (instancetype)shared;

// Register once, early (e.g. in your %ctor). Re-registering the same identifier updates it.
- (void)registerTrigger:(TGTriggerInfo *)info;
- (void)registerAction:(TGActionInfo *)info handler:(TGActionHandler)handler;

// Fire a registered trigger. Returns YES if something is assigned to it here (and ran or
// will run) — use that to skip your tweak's default behaviour, Activator-style. Returns NO
// when nothing is assigned, so your tweak carries on as normal. Cheap when unassigned.
- (BOOL)sendTrigger:(NSString *)identifier;

// Is anything assigned to this trigger right now? (Skip expensive detection when not.)
- (BOOL)isTriggerAssigned:(NSString *)identifier;

// "Configure via Triggr": Triggr's action picker for one of your triggers, ready to push from your settings
// (a PreferenceLoader bundle in the Settings app). Choices save straight into Triggr. nil if Triggr isn't there.
+ (nullable UIViewController *)pickerForTrigger:(NSString *)identifier;

@end

#pragma mark - Availability

// YES when Triggr (with this API version or newer) is installed and loaded.
static inline BOOL TGTriggrAvailable(void) {
    Class c = NSClassFromString(@"TGTriggr");
    return c && [c respondsToSelector:@selector(shared)];
}

NS_ASSUME_NONNULL_END
