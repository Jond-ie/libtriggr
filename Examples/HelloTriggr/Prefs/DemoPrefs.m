// A tweak's settings page with a "Configure via Triggr" row (libtriggr example).
#import <Preferences/PSListController.h>
#import <Preferences/PSSpecifier.h>
#import <Triggr.h>

@interface DemoRootListController : PSListController
@end

@implementation DemoRootListController
- (NSArray *)specifiers {
    if (!_specifiers) {
        PSSpecifier *group = [PSSpecifier groupSpecifierWithName:@"Triggr"];
        [group setProperty:TGTriggrAvailable() ? @"Pick what Demo Ping runs." : @"Install Triggr to assign actions." forKey:@"footerText"];
        PSSpecifier *row = [PSSpecifier preferenceSpecifierNamed:@"Configure via Triggr" target:self set:nil get:nil detail:nil cell:PSButtonCell edit:nil];
        row.buttonAction = @selector(configure);
        [row setProperty:@(TGTriggrAvailable()) forKey:@"enabled"];
        _specifiers = [@[group, row] mutableCopy];
    }
    return _specifiers;
}
- (void)configure {
    UIViewController *picker = [TGTriggr pickerForTrigger:@"com.example.hellotriggr.ping"];
    if (picker) [self.navigationController pushViewController:picker animated:YES];
}
@end
